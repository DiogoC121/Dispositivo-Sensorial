/*
 * Arquivo principal do firmware para o Dispositivo Sensorial (SmartRock).
 * Autor: Diogo Correia da Silva
 */
#include <math.h>
#include "mcc_generated_files/system.h"
#include "defines.h"
#include "flags.h"
#include "mpu9250.h"
#include "nrf24l01.h"
#include "kalman.h"

#define NUM_VELOCIDADES 5       // Número de taxas de amostragem (períodos de timer) pré-configuradas

// --- Protótipos das Funções ---
void setup(void);
void ler_todos_sensores(void);
void aplicar_filtro_kalman(void);
void processar_dados_para_envio(void);
void enviar_dados(void);
void receber_dados(void);
void tratar_interrupcoes(void);
void alterar_periodo_timer(void);
void entrar_modo_repouso(void);
void acordar_dispositivo(void);
void tratar_erros(void);
int16_t ntc_para_temperatura(uint16_t adc_valor);
int16_t calcular_porcentagem_bateria(uint16_t adc_valor);
int16_t strain_gauge_para_microstrain(uint16_t adc_valor);
int16_t microstrain_para_forca(int16_t microstrain);
int16_t get_leitura_adc(uint8_t canal);

// --- Outras Variáveis Globais ---
volatile bool f_timer = false;
volatile bool f_mpu = false;
volatile bool f_nrf = false;
volatile bool dados_recebidos = false;
volatile int f_erro = 0;
int16_t txBuffer[BUFFER_SIZE];
int16_t rxBuffer[1];
KalmanState kalman_roll;
KalmanState kalman_pitch;
int16_t dados_acelerometro[3], dados_giroscopio[3], dados_magnetometro[3];
int16_t temperatura_mpu, temperatura_ntc, nivel_bateria_percentual;
int16_t sgx_forca, sgy_forca, sgz_forca;
float angulo_roll_filtrado, angulo_pitch_filtrado;
int16_t comando_recebido = 0;

// --- Configuração dos Períodos do Timer1 ---
// A taxa de amostragem pode ser alterada remotamente.
// O cálculo do período é: T = (PR1 + 1) / (F_OSC / 2 / PRESCALER)
// Com F_OSC = 4MHz e Prescaler = 1, a fórmula é: T = (PR1 + 1) / 2,000,000
const uint16_t PR1_VALORES[NUM_VELOCIDADES] = {
    30000,  // Velocidade 1 (0x7530): Período de ~15.0 ms
    15000,  // Velocidade 2 (0x3A98): Período de ~7.5 ms
    7500,   // Velocidade 3 (0x1D4C): Período de ~3.75 ms
    3750,   // Velocidade 4 (0x0EA6): Período de ~1.88 ms
    1875    // Velocidade 5 (0x0753): Período de ~0.94 ms
};
static uint8_t velocidade_atual = 0;

// --- Função Principal ---
int main(void) {
    setup();
    while (1) {
        tratar_erros();
        tratar_interrupcoes();
        entrar_modo_repouso();
    }
    return 1;
}

// --- Implementação das Funções ---
void setup(void) {
    SYSTEM_Initialize();
    SSNRF_SetHigh();
    SSMPU_SetHigh();
    LED_SetHigh();
    MPU9250_Init();
    __delay_ms(20);
    SSMPU_SetHigh();
    NRF24L01_Init();
    __delay_ms(20);
    kalman_init(&kalman_roll);
    kalman_init(&kalman_pitch);
    ENABLE_SetHigh();
    TMR1_Start();
    LED_SetLow();
}

void tratar_interrupcoes(void) {
    if (f_erro != 0) return;
    
    if (f_timer || f_mpu || f_nrf) {
        acordar_dispositivo();
    }

    if (f_timer) {
        f_timer = false;
        ler_todos_sensores();
        aplicar_filtro_kalman();
        processar_dados_para_envio();
        enviar_dados();
        TMR1_Start();
    }

    if (f_mpu) {
        f_mpu = false;
        MPU9250_ReadData(dados_acelerometro, dados_giroscopio, dados_magnetometro, &temperatura_mpu);
        aplicar_filtro_kalman();
    }

    if (f_nrf) {
        f_nrf = false;
        receber_dados();
        if (dados_recebidos) {
            dados_recebidos = false;
            if (comando_recebido == 0x01) {
                ler_todos_sensores();
                aplicar_filtro_kalman();
                processar_dados_para_envio();
                enviar_dados();
            } else if (comando_recebido == 0x02) {
                alterar_periodo_timer();
            }
        }
    }
}

void entrar_modo_repouso(void) {
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
    MPU9250_ReadData(dados_acelerometro, dados_giroscopio, dados_magnetometro, &temperatura_mpu);
    temperatura_ntc = ntc_para_temperatura(get_leitura_adc(TEMP_CH));
    nivel_bateria_percentual = calcular_porcentagem_bateria(get_leitura_adc(SBAT_CH));
    sgx_forca = microstrain_para_forca(strain_gauge_para_microstrain(get_leitura_adc(SGX_CH)));
    sgy_forca = microstrain_para_forca(strain_gauge_para_microstrain(get_leitura_adc(SGY_CH)));
    sgz_forca = microstrain_para_forca(strain_gauge_para_microstrain(get_leitura_adc(SGZ_CH)));
}

void aplicar_filtro_kalman(void) {
    #if (MPU_ACCEL_CONFIG_VAL == 0x00)
        const float ACCEL_SENSITIVITY = 16384.0f;
    #elif (MPU_ACCEL_CONFIG_VAL == 0x08)
        const float ACCEL_SENSITIVITY = 8192.0f;
    #elif (MPU_ACCEL_CONFIG_VAL == 0x10)
        const float ACCEL_SENSITIVITY = 4096.0f;
    #elif (MPU_ACCEL_CONFIG_VAL == 0x18)
        const float ACCEL_SENSITIVITY = 2048.0f;
    #else
        const float ACCEL_SENSITIVITY = 4096.0f;
    #endif

    #if (MPU_GYRO_CONFIG_VAL == 0x00)
        const float GYRO_SENSITIVITY = 131.0f;
    #elif (MPU_GYRO_CONFIG_VAL == 0x08)
        const float GYRO_SENSITIVITY = 65.5f;
    #elif (MPU_GYRO_CONFIG_VAL == 0x10)
        const float GYRO_SENSITIVITY = 32.8f;
    #elif (MPU_GYRO_CONFIG_VAL == 0x18)
        const float GYRO_SENSITIVITY = 16.4f;
    #else
        const float GYRO_SENSITIVITY = 65.5f;
    #endif

    float ax = (float)dados_acelerometro[0] / ACCEL_SENSITIVITY;
    float ay = (float)dados_acelerometro[1] / ACCEL_SENSITIVITY;
    float az = (float)dados_acelerometro[2] / ACCEL_SENSITIVITY;
    float gx = (float)dados_giroscopio[0] / GYRO_SENSITIVITY;
    float gy = (float)dados_giroscopio[1] / GYRO_SENSITIVITY;

    float angulo_roll_acel = atan2f(ay, az) * (180.0f / M_PI);
    float angulo_pitch_acel = atanf(-ax / sqrtf(ay * ay + az * az)) * (180.0f / M_PI);

    angulo_roll_filtrado = kalman_get_angle(&kalman_roll, angulo_roll_acel, gx, KALMAN_DT);
    angulo_pitch_filtrado = kalman_get_angle(&kalman_pitch, angulo_pitch_acel, gy, KALMAN_DT);
}

void processar_dados_para_envio(void) {
    txBuffer[0] = (int16_t)(angulo_roll_filtrado * 100);
    txBuffer[1] = (int16_t)(angulo_pitch_filtrado * 100);
    txBuffer[2] = dados_acelerometro[2]; 
    txBuffer[3] = dados_giroscopio[2];
    txBuffer[4] = 0;
    txBuffer[5] = 0;
    txBuffer[6] = dados_magnetometro[0];
    txBuffer[7] = dados_magnetometro[1];
    txBuffer[8] = dados_magnetometro[2];
    txBuffer[9] = temperatura_mpu;
    txBuffer[10] = temperatura_ntc;
    txBuffer[11] = nivel_bateria_percentual;
    txBuffer[12] = sgx_forca;
    txBuffer[13] = sgy_forca;
    txBuffer[14] = sgz_forca;
    txBuffer[15] = f_erro;
}

int16_t get_leitura_adc(uint8_t canal) {
    AD1CHS = canal;
    AD1CON1bits.SAMP = 1;
    __delay_us(20);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}

void enviar_dados(void) {
    NRF24L01_SetTXMode();
    NRF24L01_WritePayload((int16_t*)txBuffer, BUFFER_SIZE * sizeof(int16_t));
    
    uint8_t status;
    uint16_t timeout = 1000;
    while(timeout > 0){
        status = NRF24L01_CheckStatus();
        if((status & ((1 << 5) | (1 << 4)))){
            break;
        }
        __delay_us(10);
        timeout--;
    }
    
    if (status & (1 << 4)) {
        f_erro = 3;
        NRF24L01_flush_tx();
    }
    
    NRF24L01_ClearInterrupts();
    NRF24L01_SetRXMode();
}

void receber_dados(void) {
    if (NRF24L01_rx_fifo_not_empty()) {
        NRF24L01_ReadPayload((int16_t*)rxBuffer, sizeof(rxBuffer));
        comando_recebido = rxBuffer[0];
        dados_recebidos = true;
        NRF24L01_flush_rx();
        NRF24L01_ClearInterrupts();
    }
}

void alterar_periodo_timer(void) {
    velocidade_atual = (velocidade_atual + 1) % NUM_VELOCIDADES;
    TMR1_Period16BitSet(PR1_VALORES[velocidade_atual]);
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

int16_t ntc_para_temperatura(uint16_t adc_valor) {
    float Vout = (adc_valor / 1023.0f) * VREF_ADC;
    float R_ntc = (R_DIVISOR_NTC * Vout) / (VIN_MCU - Vout);
    float temp_k = 1.0f / ( (1.0f / T0_NTC) + (1.0f / BETA_NTC) * logf(R_ntc / R0_NTC) );
    return (int16_t)((temp_k - 273.15f) * 100);
}

int16_t calcular_porcentagem_bateria(uint16_t bat) {
    float tensao = (bat / 1023.0f) * VREF_ADC * FATOR_DIVISOR_BAT;
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