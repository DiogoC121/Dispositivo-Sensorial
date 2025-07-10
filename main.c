/*
 * Arquivo principal do firmware para o Dispositivo Sensorial (SmartRock).
 * Autor: Diogo Correia da Silva 150058641
 */
#include <math.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/tmr1.h"
#include "defines.h"
#include "flags.h"
#include "mpu9250.h"
#include "nrf24l01.h"
#include "kalman.h"

// --- Protótipos das Funções Locais ---
void setup(void);
void loop_principal(void);
void ler_todos_sensores(void);
void processar_dados_para_envio(void);
void enviar_dados(void);
void receber_comando(void);
void tratar_interrupcoes(void);
void reconfigurar_timmer(void);
void entrar_modo_baixo_consumo(void);
void acordar_dispositivo(void);
void tratar_erros(void);
int16_t get_leitura_adc(ADC1_CHANNEL canal);
int16_t ntc_para_temperatura(uint16_t adc_valor);
int16_t calcular_porcentagem_bateria(uint16_t adc_valor);
int16_t strain_gauge_para_microstrain(uint16_t adc_valor);
int16_t microstrain_para_forca(int16_t microstrain);


// --- Variáveis Globais ---
volatile bool f_timer = false;
volatile bool f_mpu = false;
volatile bool f_nrf = false;
volatile bool dados_recebidos = false;
volatile int f_erro = 0;

// Buffers para comunicação
int16_t txBuffer[BUFFER_SIZE];
uint8_t rxBuffer[1]; 

// Estados do Filtro de Kalman
KalmanState kalman_roll;
KalmanState kalman_pitch;

// Variáveis para armazenar dados dos sensores
int16_t dados_acelerometro[3], dados_giroscopio[3], dados_magnetometro[3];
int16_t temperatura_ntc;
int16_t nivel_bateria_percentual;
int16_t sgx_microstrain, sgy_microstrain, sgz_microstrain;
float angulo_roll_filtrado, angulo_pitch_filtrado;
uint8_t comando_recebido = 0;

// Períodos pré-configurados para o Timer1
const uint16_t PR1_VALORES[] = {
    30000, 15000, 7500, 3750, 1875
};
#define NUM_VELOCIDADES (sizeof(PR1_VALORES)/sizeof(uint16_t))
static uint8_t velocidade_atual_idx = 0;


// --- Função Principal ---
int main(void) {
    setup();
    loop_principal();
    return 1;
}

// --- Implementação das Funções ---

void setup(void) {
    uint16_t contador_tentativas = 0;

    SYSTEM_Initialize();
    
    SSNRF_SetHigh();
    SSMPU_SetHigh();
    LED_SetHigh();
    
    kalman_init(&kalman_roll);
    kalman_init(&kalman_pitch);

    INTERRUPT_GlobalEnable();
    
    do {
        NRF24L01_Init();
        if (NRF24L01_Check()) {
            break; 
        }
        contador_tentativas++;
        __delay_ms(10);
    } while (contador_tentativas < 100);

    if (contador_tentativas >= 100) {
        f_erro = ERRO_1_NRF_SETUP;
    }

    MPU9250_Init();
    
    ENABLE_SetHigh();
    TMR1_Start();
    LED_SetLow();
}

void loop_principal(void) {
    while (1) {
        if (f_erro != 0) {
            tratar_erros();
        }
        
        if (!f_timer && !f_mpu && !f_nrf) {
            entrar_modo_baixo_consumo();
        }
        
        tratar_interrupcoes();
    }
}

void tratar_interrupcoes(void) {
    if (f_timer) {
        f_timer = false;
        ler_todos_sensores();
        processar_dados_para_envio();
        enviar_dados();
        TMR1_Start();
    }

    if (f_mpu) {
        f_mpu = false;
        acordar_dispositivo();
        MPU9250_ReadData(dados_acelerometro, dados_giroscopio, dados_magnetometro, NULL);
        processar_dados_para_envio();
    }

    if (f_nrf) {
        f_nrf = false;
        acordar_dispositivo();
        receber_comando();
        
        if (dados_recebidos) {
            dados_recebidos = false;
            if (comando_recebido == CMD_ENVIAR_DADOS) {
                ler_todos_sensores();
                processar_dados_para_envio();
                enviar_dados();
            } else if (comando_recebido == CMD_RECONFIG_TIMMER) {
                reconfigurar_timmer();
            }
        }
    }
}

void entrar_modo_baixo_consumo(void) {
    NRF24L01_Sleep();
    MPU9250_Sleep();
    Sleep();
}

void acordar_dispositivo(void) {
    MPU9250_Wake();
    NRF24L01_Wake();
    __delay_ms(10);
}

void ler_todos_sensores(void) {
    MPU9250_ReadData(dados_acelerometro, dados_giroscopio, dados_magnetometro, NULL);
    temperatura_ntc = ntc_para_temperatura(get_leitura_adc(TEMPSENS));
    nivel_bateria_percentual = calcular_porcentagem_bateria(get_leitura_adc(SBAT));
    sgx_microstrain = strain_gauge_para_microstrain(get_leitura_adc(SGX));
    sgy_microstrain = strain_gauge_para_microstrain(get_leitura_adc(SGY));
    sgz_microstrain = strain_gauge_para_microstrain(get_leitura_adc(SGZ));
}

void processar_dados_para_envio(void) {
    float ax_g = (float)dados_acelerometro[0] / MPU_ACCEL_SENSITIVITY_4G;
    float ay_g = (float)dados_acelerometro[1] / MPU_ACCEL_SENSITIVITY_4G;
    float az_g = (float)dados_acelerometro[2] / MPU_ACCEL_SENSITIVITY_4G;
    float gx_dps = (float)dados_giroscopio[0] / MPU_GYRO_SENSITIVITY_500DPS;
    float gy_dps = (float)dados_giroscopio[1] / MPU_GYRO_SENSITIVITY_500DPS;

    float angulo_roll_acel = atan2f(ay_g, az_g) * (180.0f / M_PI);
    float angulo_pitch_acel = atanf(-ax_g / sqrtf(ay_g * ay_g + az_g * az_g)) * (180.0f / M_PI);

    angulo_roll_filtrado = kalman_get_angle(&kalman_roll, angulo_roll_acel, gx_dps, KALMAN_DT);
    angulo_pitch_filtrado = kalman_get_angle(&kalman_pitch, angulo_pitch_acel, gy_dps, KALMAN_DT);

    txBuffer[0] = (int16_t)(angulo_roll_filtrado * 100);
    txBuffer[1] = (int16_t)(angulo_pitch_filtrado * 100);
    txBuffer[2] = dados_giroscopio[2];
    txBuffer[3] = dados_magnetometro[0];
    txBuffer[4] = dados_magnetometro[1];
    txBuffer[5] = dados_magnetometro[2];
    txBuffer[6] = temperatura_ntc;
    txBuffer[7] = nivel_bateria_percentual;
    txBuffer[8] = sgx_microstrain;
    txBuffer[9] = sgy_microstrain;
    txBuffer[10] = sgz_microstrain;
    txBuffer[11] = f_erro;
}

void enviar_dados(void) {
    NRF24L01_SetTXMode();
    if (!NRF24L01_WritePayload((uint8_t*)txBuffer, BUFFER_SIZE * sizeof(int16_t))) {
        f_erro = ERRO_3_COMUNICACAO;
        NRF24L01_flush_tx();
    }
    NRF24L01_ClearInterrupts();
    NRF24L01_SetRXMode();
}

void receber_comando(void) {
    if (NRF24L01_rx_fifo_not_empty()) {
        NRF24L01_ReadPayload(rxBuffer, 1);
        comando_recebido = rxBuffer[0];
        dados_recebidos = true;
        NRF24L01_flush_rx();
        NRF24L01_ClearInterrupts();
    }
}

void reconfigurar_timmer(void) {
    velocidade_atual_idx = (velocidade_atual_idx + 1) % NUM_VELOCIDADES;
    TMR1_Period16BitSet(PR1_VALORES[velocidade_atual_idx]);
    TMR1_Counter16BitSet(0x00);
}

void tratar_erros(void) {
    if (f_erro == 0) return;
    for (int i = 0; i < (f_erro * 2); i++) {
        LED_Toggle();
        __delay_ms(250);
    }
    processar_dados_para_envio();
    enviar_dados();
    f_erro = 0;
}

int16_t get_leitura_adc(ADC1_CHANNEL canal) {
    ADC1_ChannelSelect(canal);
    ADC1_SoftwareTriggerEnable();
    __delay_us(20);
    ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(canal));
    return ADC1_ConversionResultGet(canal);
}

int16_t ntc_para_temperatura(uint16_t adc_valor) {
    float Vout = (adc_valor / 1023.0f) * VREF_ADC;
    float R_ntc = (R_DIVISOR_NTC * Vout) / (VIN_MCU - Vout);
    float temp_k = 1.0f / ( (1.0f / T0_NTC) + (1.0f / BETA_NTC) * logf(R_ntc / R0_NTC) );
    return (int16_t)((temp_k - 273.15f) * 100);
}

int16_t calcular_porcentagem_bateria(uint16_t adc_valor) {
    float tensao = (adc_valor / 1023.0f) * VREF_ADC * FATOR_DIVISOR_BAT;
    if (tensao >= 4.20f) return 100;
    if (tensao <= 3.20f) return 0;
    return (int16_t)(((tensao - 3.20f) / 1.0f) * 100);
}

int16_t strain_gauge_para_microstrain(uint16_t adc_valor) {
    float Vout_amp = (adc_valor / 1023.0f) * VREF_ADC;
    float Vdiff_ponte = (Vout_amp - VREF_AMP) / GANHO_AMP;
    float strain = (4.0f * Vdiff_ponte) / (VEXC_PONTE * GF_STRAIN);
    return (int16_t)(strain * 1e6f);
}

int16_t microstrain_para_forca(int16_t microstrain) {
    float strain = microstrain / 1e6f;
    float stress = E_MODULUS * strain;
    float force = stress * AREA_SECAO;
    return (int16_t)force;
}
