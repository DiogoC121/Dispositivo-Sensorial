/*============================================================================================================================*/  
/*
 * Bibliotecas e Macros
 *
 */ 

//#include <main.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/clock.h"
#include "mcc_generated_files/ext_int.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/traps.h"
#include "mcc_generated_files/tmr1.h"
#include "defines.h"
#include "mpu9250.h"
#include "nrf24l01.h"
#include "flags.h"

/*============================================================================================================================* /

 * Fun??es de Interrup??o
 * 
 * PRIORIDADE:
 * INT0 - External Interrupt NFR
 *      Priority: 1
 * 
 * INT1 - External Interrupt MPU  
 *      Priority: 2
 * 
 * T1 - Timer1
 *      Priority: 3
 * */
 

/*============================================================================================================================*/
/*
 * Protótipo das funções
 */

int16_t getADC(uint8_t sensor);
void sleep_mode();
void wake_up();
void tipo_interrupt(void);
int16_t NTC_To_Temperature(int16_t adc_value); 
int16_t porcentagem_bateria(int16_t bat);
void error(void);
int16_t strain_gauge_to_microstrain(uint16_t adc_value);
int16_t strain_to_force(int16_t microstrain);
void send_data(void);
void receive_data(void);
void tmr_change(void);


// Vari?veis globais

volatile bool flag_timer = false;
volatile bool flag_mpu = false;
volatile bool flag_nrf = false;
volatile bool dados_recebidos = false;  // Flag para dados recebidos
volatile int flag_erro = 0;

int16_t receivedCommand = 0;  // Comando recebido pelo NRF24L01+

int16_t temperatura = 0.0;       // Temperatura em ?C
int16_t tensao_bateria = 0.0;    // Tens?o da bateria em V
int16_t strain_gauge_x = 0.0;    // Leitura do strain gauge X
int16_t strain_gauge_y = 0.0;    // Leitura do strain gauge Y
int16_t strain_gauge_z = 0.0;    // Leitura do strain gauge Z

int16_t accel[3], gyro[3], mag[3], tempmpu;
int16_t temp_ntc, sbat_lvl;
int16_t sgx_deform, sgy_deform, sgz_deform;
int16_t sgx_force, sgy_force, sgz_force;

int16_t txBuffer[BUFFER_SIZE];
int16_t rxBuffer[1];

// Defina os valores de PR1 para as 5 velocidades
#define NUM_SPEEDS 5
const uint16_t PR1_VALUES[NUM_SPEEDS] = {
    0x7530,  // Velocidade 1 (original)
    0x3A98,  // Velocidade 2
    0x1D4C,  // Velocidade 3
    0x0EA6,  // Velocidade 4
    0x0753   // Velocidade 5
};

// Variável para armazenar a velocidade atual
static uint8_t current_speed = 0;

//const uint8_t addressnrf[5] = "00001";
/*============================================================================================================================

 * Fun??o Principal
 */

int main(void)
{
   SYSTEM_Initialize();  // Inicializa o PIC24FJ64GA002
   
   EX_INT1_InterruptEnable();
   EX_INT0_InterruptEnable();
   TMR1_Initialize();
   
   SSNRF_SetHigh(); 
   SSMPU_SetHigh();
   LED_SetHigh();  
   LED_SetHigh();
   
      
   SSNRF_SetHigh();
   MPU9250_Init();       // Inicializa o MPU9250
   __delay_ms(20);
   SSMPU_SetHigh();
   NRF24L01_Init();      // Inicializa o NRF24L01+uint8_t address[5] = {0x52, 0xA0, 0xC1, 0x71, 0x1E}; // Endere?o "node3"
   __delay_ms(20);


   LED_SetHigh();  
   NOUSE1_SetLow();
   NOUSE2_SetLow();
   NOUSE3_SetLow();
   NOUSE4_SetLow();
   ENABLE_SetLow();
   
   TMR1_Start();
   
   while(1)
   {
      tipo_interrupt();
      error();
      
      // Testes de SPI
       
      uint8_t status = NRF24L01_CheckStatus();
      if (status & 0x20) {
          dados_recebidos = true;               // Payload recebido (se estiver no modo RX)
      }
      if (status & 0x10) {
          flag_erro = 3;
          error();                             // M?ximo de retransmiss?es atingido
      }
      
      if(dados_recebidos == true){
          receive_data();
          receivedCommand = rxBuffer[0];
      }
      
      SSMPU_SetHigh();
      NRF24L01_WriteRegister(0xDC, 0xBA);
      SSNRF_SetHigh();
      MPU9250_WriteRegister(0xDA, 0xD0);
      
      SSMPU_SetHigh();
      SSNRF_SetHigh();
      
      // Testes de output 
      
      LED_SetHigh();
      __delay_ms(1000);  
      LED_SetLow();
      __delay_ms(1000);   
      
      //Envia os dados do MPU9250e do ADC pelo NRF24L01+
      //(dummy abaixo) 
      //MPU
      txBuffer[0]  = 0x00; // mpuAcelX;
      txBuffer[1]  = 0x01; // mpuAcelY;
      txBuffer[2]  = 0x02; // mpuAcelZ;
      txBuffer[3]  = 0x10; // mpuGiroX;
      txBuffer[4]  = 0x11; // mpuGiroY;
      txBuffer[5] = 0x12; // mpuGiroZ;
      txBuffer[6] = 0x20; // mpuMagX;
      txBuffer[7] = 0x21; // mpuMagY;
      txBuffer[8] = 0x22; // mpuMagZ;
      txBuffer[9] = 0x30; // mpuTemp;
      //ADC
      txBuffer[10] = 0x40; // ntcTemp;
      txBuffer[11] = 0x50; // batVolt;
      txBuffer[12] = 0x60; // extX;
      txBuffer[13] = 0x61; // extY;
      txBuffer[14] = 0x62; // extZ;
      txBuffer[15] = 0xFF; // erro;
      
      send_data();
      
      //-----------------------------------DAKI PRA BAIXO NEM RELA!
               
        if(flag_nrf){
            flag_nrf = false;       
        } 
        if(flag_mpu){
            flag_mpu = false;      
        } 
        if(flag_timer){
            flag_timer = false;
            TMR1_Start();       
        }

        //sleep_mode();
        dados_recebidos = false;
    }
return 1;
} // END MAIN

/*============================================================================================================================*/
/*
 * Fun??es do Microcotrolador
 */

// Fun??o de leitura das portas ADC
int16_t getADC(uint8_t sensor)
{     
    ENABLE_SetHigh();
    __delay_ms(10);
    AD1CHS = sensor;  // Seleciona o canal ADC
    AD1CON1bits.SAMP = 1;       // Inicia a amostragem
    __delay_us(10);  // Pequeno delay para amostragem
    AD1CON1bits.SAMP = 0;  // Inicia a convers?o
    while (!AD1CON1bits.DONE);  // Aguarda a convers?o
    __delay_ms(10);
    ENABLE_SetLow();
    return ADC1BUF0;            // Retorna o valor convertido
    
} /* end getADC */

/*============================================================================================================================*/
/*
 * Fun??es de Gerenciamento de Energia
 */

// Fun??o para deixar o dispositivo em modo SLEEP
void sleep_mode(void)
{
    MPU9250_Sleep();
    __delay_ms(50);
    NRF24L01_Sleep();
    __delay_ms(50);
    Sleep();    //sleep(1);     //CSS SLEEP
    
}/* end sleep_mode */

// Fun??o para tirar o dispositivo em modo SLEEP
void wake_up(void)
{
    MPU9250_Wake();
    __delay_ms(50);
    NRF24L01_Wake();
    __delay_ms(50);
    
}/* end wake_up */

/*============================================================================================================================*/
/*
 * Fun??es da L?gica de Funcionamento
 */

// Fun??o para tratar o tipo de interrup??o
void tipo_interrupt(void) 
{
    int i = 0;
    
    if(flag_nrf){
        i = 1;    
        wake_up();
        flag_nrf = false; // Interrup??o do NRF24L01+
    }else if(flag_mpu){
        i = 2;    
        wake_up();
        flag_mpu = false; // Interrup??o do MPU9250
    } else if(flag_timer){
        i = 3;    
        wake_up();
        flag_timer = false; // Interrup??o do Timer1
    }
    
    if(i == 1){
        // Ler os dados dos sensores
        MPU9250_ReadData(accel, gyro, mag, &tempmpu);  
        //MPU
        txBuffer[0]  = accel[0]; // mpuAcelX;
        txBuffer[1]  = accel[1]; // mpuAcelY;
        txBuffer[2]  = accel[2]; // mpuAcelZ;
        txBuffer[3]  = gyro[0]; // mpuGiroX;
        txBuffer[4]  = gyro[1]; // mpuGiroY;
        txBuffer[5]  = gyro[2]; // mpuGiroZ;
        txBuffer[6]  = mag[0]; // mpuMagX;
        txBuffer[7]  = mag[1]; // mpuMagY;
        txBuffer[8]  = mag[2]; // mpuMagZ;
        txBuffer[9]  = tempmpu; // mpuTemp;
        //ADC
        porcentagem_bateria(getADC(SBAT_CH));
        temp_ntc = NTC_To_Temperature(getADC(TEMP_CH));
        sbat_lvl = porcentagem_bateria(getADC(SBAT_CH));
        sgx_deform = strain_gauge_to_microstrain(getADC(SGX_CH));
        sgx_force = strain_to_force(sgx_deform);      
        sgy_force = strain_gauge_to_microstrain(getADC(SGY_CH));
        sgy_force = strain_to_force(sgy_force);
        sgz_deform = strain_gauge_to_microstrain(getADC(SGZ_CH));
        sgz_force = strain_to_force(sgz_deform);
        
        txBuffer[10] = temp_ntc; // ntcTemp;
        txBuffer[11] = sbat_lvl; // batVolt;
        txBuffer[12] = sgx_deform; // ext X deform;
        txBuffer[13] = sgy_deform; // ext Y deform;
        txBuffer[14] = sgz_deform; // ext Z deform;
        txBuffer[12] = sgx_force; // ext X force;
        txBuffer[13] = sgy_force; // ext Y force;
        txBuffer[14] = sgz_force; // ext Z force;
        
        receive_data();
        
        if(receivedCommand == 0x01){
          send_data();  
        }
        if(receivedCommand == 0x02){
          tmr_change(); 
        }
                
        
        dados_recebidos = false;
        
    }else if(i == 2){
        // Ler os dados dos sensores
        MPU9250_ReadData(accel, gyro, mag, &tempmpu);  
        //MPU
        txBuffer[0]  = accel[0]; // mpuAcelX;
        txBuffer[1]  = accel[1]; // mpuAcelY;
        txBuffer[2]  = accel[2]; // mpuAcelZ;
        txBuffer[3]  = gyro[0]; // mpuGiroX;
        txBuffer[4]  = gyro[1]; // mpuGiroY;
        txBuffer[5]  = gyro[2]; // mpuGiroZ;
        txBuffer[6]  = mag[0]; // mpuMagX;
        txBuffer[7]  = mag[1]; // mpuMagY;
        txBuffer[8]  = mag[2]; // mpuMagZ;
        txBuffer[9]  = tempmpu; // mpuTemp;
    }else if(i == 3){
        // Ler os dados dos sensores
        MPU9250_ReadData(accel, gyro, mag, &tempmpu);  
        //MPU
        txBuffer[0]  = accel[0]; // mpuAcelX;
        txBuffer[1]  = accel[1]; // mpuAcelY;
        txBuffer[2]  = accel[2]; // mpuAcelZ;
        txBuffer[3]  = gyro[0]; // mpuGiroX;
        txBuffer[4]  = gyro[1]; // mpuGiroY;
        txBuffer[5]  = gyro[2]; // mpuGiroZ;
        txBuffer[6]  = mag[0]; // mpuMagX;
        txBuffer[7]  = mag[1]; // mpuMagY;
        txBuffer[8]  = mag[2]; // mpuMagZ;
        txBuffer[9]  = tempmpu; // mpuTemp;
        //ADC
        porcentagem_bateria(getADC(SBAT_CH));
        temp_ntc = NTC_To_Temperature(getADC(TEMP_CH));
        sbat_lvl = porcentagem_bateria(getADC(SBAT_CH));
        sgx_deform = strain_gauge_to_microstrain(getADC(SGX_CH));
        sgx_force = strain_to_force(sgx_deform);      
        sgy_force = strain_gauge_to_microstrain(getADC(SGY_CH));
        sgy_force = strain_to_force(sgy_force);
        sgz_deform = strain_gauge_to_microstrain(getADC(SGZ_CH));
        sgz_force = strain_to_force(sgz_deform);

        txBuffer[10] = temp_ntc; // ntcTemp;
        txBuffer[11] = sbat_lvl; // batVolt;
        txBuffer[12] = sgx_deform; // ext X deform;
        txBuffer[13] = sgy_deform; // ext Y deform;
        txBuffer[14] = sgz_deform; // ext Z deform;
        txBuffer[12] = sgx_force; // ext X force;
        txBuffer[13] = sgy_force; // ext Y force;
        txBuffer[14] = sgz_force; // ext Z force;
        
        send_data();
        
        TMR1_Start();
    }
    
}
// Fun?ao que converte a leitura ADC do NTC 10K em ?C
int16_t NTC_To_Temperature(int16_t adc_value) 
{
    float Vout = (adc_value / 1023.0) * VIN;  // Tens?o no pino do NTC
    float R_ntc_actual = (R_ntc * Vout) / (VIN - Vout); // Resist?ncia do NTC
    float temperatura_kelvin = 1.0 / ((1.0 / T0) + (1.0 / BETA) * log(R_ntc_actual / R0));
    float temperatura_celsius = temperatura_kelvin - 273.15; // Converte para ?C

    return (int16_t)(temperatura_celsius * 100); // Multiplica por 100 para ponto fixo

}

// Fun??o para calcular a porcentagem da bateria
int16_t porcentagem_bateria(int16_t bat)
{
    float bateria = (bat / 1023.0) * VIN * 2; // Considerando divisor resistivo 2:1

    if (bateria >= 4.20) {
        return 100; // Bateria cheia
    } else if (bateria >= 4.00) {
        return (int)((bateria - 4.00) / (4.20 - 4.00) * 20) + 80;
    } else if (bateria >= 3.85) {
        return (int)((bateria - 3.85) / (4.00 - 3.85) * 20) + 60;
    } else if (bateria >= 3.75) {
        return (int)((bateria - 3.75) / (3.85 - 3.75) * 20) + 40;
    } else if (bateria >= 3.60) {
        return (int)((bateria - 3.60) / (3.75 - 3.60) * 20) + 20;
    } else if (bateria >= 3.50) {
        return (int)((bateria - 3.50) / (3.60 - 3.50) * 10) + 10;
    } else if (bateria >= 3.20) {
        return (int)((bateria - 3.20) / (3.50 - 3.20) * 10);
    } else {
        return 0; // Bateria cr?tica
    }
}

// Fun??o para exp?r ao usu?rio caso ocorra um erro
void error(void)
{   
    int i = 0;
    txBuffer[15] = 0;        
    if(flag_erro == 1){
        while(i < 10){
            LED_Toggle();
            __delay_ms(1000);        
            LED_Toggle();
            __delay_ms(1000);
            i++;
        }            
        txBuffer[15] = 1;           //-----------------------------------nrf n configurado.
    }
    if(flag_erro == 2){
            while(i < 10){
                LED_Toggle();
                __delay_ms(2000);        
                LED_Toggle();
                __delay_ms(2000);
                i++;
            }
            txBuffer[15] = 2;       //-----------------------------------mpu n configurado. 
    }
    if(flag_erro == 3){
        while(i < 10){
            LED_Toggle();
            __delay_ms(3000);        
            LED_Toggle();
            __delay_ms(3000);
            i++;
        }  
        txBuffer[15] = 3;          //-----------------------------------FAZER!! - n?o conectado ao outro nrf 
    }
    if(flag_erro == 4){
        while(i < 10){
            LED_Toggle();
            __delay_ms(4000);        
            LED_Toggle();
            __delay_ms(4000);
            i++;
        }
        txBuffer[15] = 4;          //-----------------------------------FAZER!! - interrup??es zoadas
    }
} /* end error */

// Converte leitura do ADC para deforma??o (??)
int16_t strain_gauge_to_microstrain(uint16_t adc_value) {
    float Vout = (adc_value / 1023.0) * VREF; // Tens?o de sa?da da ponte
    float strain = (4 * Vout) / (VEXC * GF);  // Deforma??o (strain)
    return (int16_t)(strain * 1e6);           // Retorna em ??
}

// Converte microstrain para for?a (Newton)
int16_t strain_to_force(int16_t microstrain) {
    float strain = microstrain / 1e6;  // Converte ?? para strain real
    float stress = E * strain;         // Calcula tens?o (stress)
    float force = stress * AREA;       // For?a aplicada (N)
    return (int16_t)force;             // Retorna for?a em Newtons
}

// ENVIA DADOS
void send_data(void) {
    NRF24L01_SetTXMode();
    NRF24L01_WritePayload((int16_t*)txBuffer, BUFFER_SIZE * sizeof(int16_t));
    while (!NRF24L01_tx_finished()); // Espera o fim da transmiss?o
    NRF24L01_SetRXMode(); // Volta para o modo RX
}

// RECEBE DADOS
void receive_data(void) {
    if (NRF24L01_rx_fifo_not_empty()) {
        NRF24L01_ReadPayload((int16_t*)rxBuffer[0], sizeof(rxBuffer));
        if ((rxBuffer[0] == 0x01)||(rxBuffer[0] == 0x02)) {
            dados_recebidos = true;
            receivedCommand = rxBuffer[0];
            rxBuffer[0] = 0;
        }
    }
}

void tmr_change(void) 
{
    // Incrementa a velocidade atual
    current_speed = (current_speed + 1) % NUM_SPEEDS;
    
    // Atualiza o valor de PR1 para a nova velocidade
    PR1 = PR1_VALUES[current_speed];
    
    // Reinicia o Timer 1
    TMR1 = 0x00;
    
    // Limpa a flag de interrupção do Timer 1
    IFS0bits.T1IF = false;
}
