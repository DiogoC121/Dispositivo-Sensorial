/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  PIC24FJ64GA002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

// ---------------------------------------------------------------------------------------------------------------------------------
/*============================================================================================================================*/  
/*
 * Bibliotecas e Macros
 */ 

//#include <main.h>

#include "defines.h"
#include "mpu9250.h"
#include "nrf24l01.h"

/*============================================================================================================================* /

 * Funções de Interrupção
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
 
// Variáveis globais

bool flag_timer = false;
bool flag_mpu = false;
bool flag_nrf = false;
bool enviar_dados = false;     // Flag para enviar dados
bool dados_recebidos = false;  // Flag para dados recebidos
uint8_t receivedCommand = 0;  // Comando recebido pelo NRF24L01+
int16_t accel[3], gyro[3], mag[3], temp;
float temperatura = 0.0;       // Temperatura em °C
float tensao_bateria = 0.0;    // Tensão da bateria em V
float strain_gauge_x = 0.0;    // Leitura do strain gauge X
float strain_gauge_y = 0.0;    // Leitura do strain gauge Y
float strain_gauge_z = 0.0;    // Leitura do strain gauge Z

/*============================================================================================================================

 * Função Principal
 */

int main(void)
{
   SYSTEM_Initialize();  // Inicializa o PIC24FJ64GA002
   
   EX_INT1_InterruptEnable();
   EX_INT0_InterruptEnable();
   
   MPU9250_Init();       // Inicializa o MPU9250
   NRF24L01_Init();      // Inicializa o NRF24L01+

   LED_SetHigh();  
   NOUSE1_SetLow();
   NOUSE2_SetLow();
   NOUSE3_SetLow();
   NOUSE4_SetLow();
   ENABLE_SetLow(); 

   /*
   //--Variáveis de dados do MPU
   
   uint8_t mpuAcelXH = 0x00;                                                                       // Acelerômetro Eixo X HIGH BYTE
   uint8_t mpuAcelXL = 0x00;                                                                       // Acelerômetro Eixo X LOW BYTE
   uint8_t mpuAcelYH = 0x00;                                                                       // Acelerômetro Eixo Y HIGH BYTE
   uint8_t mpuAcelYL = 0x00;                                                                       // Acelerômetro Eixo Y LOW BYTE
   uint8_t mpuAcelZH = 0x00;                                                                       // Acelerômetro Eixo Z HIGH BYTE
   uint8_t mpuAcelZL = 0x00;                                                                       // Acelerômetro Eixo Z LOW BYTE
   uint8_t mpuGiroXH = 0x00;                                                                       // Giroscópio   Eixo X HIGH BYTE
   uint8_t mpuGiroXL = 0x00;                                                                       // Giroscópio   Eixo X LOW BYTE
   uint8_t mpuGiroYH = 0x00;                                                                       // Giroscópio   Eixo Y HIGH BYTE
   uint8_t mpuGiroYL = 0x00;                                                                       // Giroscópio   Eixo Y LOW BYTE
   uint8_t mpuGiroZH = 0x00;                                                                       // Giroscópio   Eixo Z HIGH BYTE
   uint8_t mpuGiroZL = 0x00;                                                                       // Giroscópio   Eixo Z LOW BYTE
   uint8_t mpuTempH  = 0x00;                                                                       // Temperatura  MPU    HIGH BYTE
   uint8_t mpuTempL  = 0x00;                                                                       // Temperatura  MPU    LOW BYTE  
   uint8_t mpuMagXH  = 0x00;                                                                       // Magnetômetro Eixo X HIGH BYTE
   uint8_t mpuMagXL  = 0x00;                                                                       // Magnetômetro Eixo X LOW BYTE
   uint8_t mpuMagYH  = 0x00;                                                                       // Magnetômetro Eixo Y HIGH BYTE
   uint8_t mpuMagYL  = 0x00;                                                                       // Magnetômetro Eixo Y LOW BYTE
   uint8_t mpuMagZH  = 0x00;                                                                       // Magnetômetro Eixo Z HIGH BYTE
   uint8_t mpuMagZL  = 0x00;                                                                       // Magnetômetro Eixo Z LOW BYTE  
   
   //--Variáveis de dados do ADC
    
   uint16_t ntcTemp = 0x00;                                                                        // Temperatura NTC     saída do ADC
   uint16_t batVolt = 0x00;                                                                        // Tensão da Bateria   saída do ADC
   uint16_t extX    = 0x00;                                                                        // Extensômetro Eixo X saída do ADC
   uint16_t extY    = 0x00;                                                                        // Extensômetro Eixo Y saída do ADC
   uint16_t extZ    = 0x00;                                                                        // Extensômetro Eixo Z saída do ADC
   
   uint8_t ntcTempH = 0x00;                                                                        // Temperatura  NTC    HIGH BYTE
   uint8_t ntcTempL = 0x00;                                                                        // Temperatura  NTC    LOW BYTE    
   uint8_t batVoltH = 0x00;                                                                        // Tensão da Bateria   HIGH BYTE
   uint8_t batVoltL = 0x00;                                                                        // Tensão da Bateria   LOW BYTE    
   uint8_t extXH    = 0x00;                                                                        // Extensômetro Eixo X HIGH BYTE
   uint8_t extXL    = 0x00;                                                                        // Extensômetro Eixo X LOW BYTE    
   uint8_t extYH    = 0x00;                                                                        // Extensômetro Eixo Y HIGH BYTE
   uint8_t extYL    = 0x00;                                                                        // Extensômetro Eixo Y LOW BYTE    
   uint8_t extZH    = 0x00;                                                                        // Extensômetro Eixo Z HIGH BYTE
   uint8_t extZL    = 0x00;                                                                        // Extensômetro Eixo Z LOW BYTE
   
   //--Variáveis de envio/recebimento de dados;
    
   uint8_t txBuffer[30] = {0x00};                                                                  // Dados para envio pelo NRF24L01+
         
   float bateria = 0.0;                                                                            // Float para armazenar a tensão da bateria
   int bat_percentage = 50;
   float temperatura_ntc = 0.0;
   float vADC = 0.0;
   float resistencia = 0.0;
   
   float temperatura_mpu = 0.0;
   float aceleracao_x = 0.0;
   float aceleracao_y = 0.0;
   float aceleracao_z = 0.0;
   float giro_x = 0.0;
   float giro_y = 0.0;
   float giro_z = 0.0;
   float eletromag_x = 0.0;
   float eletromag_y = 0.0;
   float eletromag_z = 0.0;
   */
   
   while(1){ 
        LED_SetLow();  
        TMR1_Start();
        
        // Verifica erros no MPU9250
        if (MPU9250_CheckError()) 
        {
            error(2);
        }

        // Verifica erros no NRF24L01+
        if (NRF24L01_CheckStatus() & 0x10) 
        {
            // Tratar erro de retransmissão do NRF24L01+
            error(3);
        }

        // Envia dados se a flag estiver ativa
        if (enviar_dados) 
        {
            enviar_dados = false;

            // Prepara os dados para envio
            uint8_t dadosADC[20];
            memcpy(dadosADC, &temperatura, sizeof(float));
            memcpy(dadosADC + 4, &tensao_bateria, sizeof(float));
            memcpy(dadosADC + 8, &strain_gauge_x, sizeof(float));
            memcpy(dadosADC + 12, &strain_gauge_y, sizeof(float));
            memcpy(dadosADC + 16, &strain_gauge_z, sizeof(float));            
            // Envia os dados via NRF24L01+
            NRF24L01_SetTXMode();
            NRF24L01_WritePayload(dadosADC, sizeof(dadosADC));
            NRF24L01_StandbyMode();
            
            uint16_t dadosMPU[10]; // 3 (accel) + 3 (gyro) + 3 (mag) + 1 (temp) = 10 elements
            memcpy(dadosMPU, accel, sizeof(accel)); // Copy accel array (3 elements)
            memcpy(dadosMPU + 3, gyro, sizeof(gyro)); // Copy gyro array (3 elements)
            memcpy(dadosMPU + 6, mag, sizeof(mag)); // Copy mag array (3 elements)
            memcpy(dadosMPU + 9, &temp, sizeof(temp)); // Copy temp (1 element)
            // Envia os dados via NRF24L01+
            NRF24L01_SetTXMode();
            NRF24L01_WritePayload((uint8_t*)dadosMPU, sizeof(dadosMPU));
            NRF24L01_StandbyMode();
        }

        // Processa o comando recebido
        if (dados_recebidos) 
        {
            dados_recebidos = false;

            // Liga o LED se o comando for 1
            if (receivedCommand == 1) 
            {
                enviar_dados = true;
                LATAbits.LATA2 = 1;
            }
            // Desliga o LED se o comando for 2
            else if (receivedCommand == 2) 
            {
                ;
                    //TMR1 0; 
                //    TMR1 = 0x00;
                    //Period = 0.25 s; Frequency = 4000000 Hz; PR1 15625; 
                //    PR1 = 0x3D09;
                    //TCKPS 1:64; TON enabled; TSIDL disabled; TCS FOSC/2; TSYNC disabled; TGATE disabled; 
                //    T1CON = 0x8030; // 0x8030 = 1000 0000 0011 0000 (TON=1, TCKPS=11)
            }
            // Se o comando for 0 n faz nada
            else if (receivedCommand == 0) 
            {
                ;
            }
        }

        // Entra em modo sleep se não houver atividade
        if (!enviar_dados && !dados_recebidos) {
            sleep_mode();
        }
       
        //-----------------------------------DAKI PRA BAIXO NEM RELA!
               
        if(flag_nrf){
            
            flag_nrf = 0;       
        } 
        if(flag_mpu){
            flag_mpu = 0;      
        } 
        if(flag_timer){
            flag_timer = 0;
            TMR1_Start();       
        }
        
        sleep_mode();
    }
   return 1;
} // END MAIN

/*============================================================================================================================*/
/*
 * Funções do Microcotrolador
 */

// Função de leitura das portas ADC
uint16_t getADC(int sensor)
{   
   /*
   uint16_t conversion = 0;
   ENABLE_SetHigh();
   
   ADC1_ChannelSelect(sensor);
   ADC1_SoftwareTriggerEnable();
   __delay_ms(100);
   ADC1_SoftwareTriggerDisable();
   while(!ADC1_IsConversionComplete(sensor));
   conversion = ADC1_ConversionResultGet(sensor);
   __delay_ms(100);
   ENABLE_SetLow();
   return conversion;                                                                              // return conversion result
    
   */
    AD1CHSbits.CH0SA = sensor; // Seleciona o canal
    AD1CON1bits.SAMP = 1;       // Inicia a amostragem
    while (!AD1CON1bits.DONE);  // Aguarda a conversão
    return ADC1BUF0;            // Retorna o valor convertido
} /* end getADC */

/*============================================================================================================================*/
/*
 * Funções de Gerenciamento de Energia
 */

// Função para deixar o dispositivo em modo SLEEP
void sleep_mode(void)
{
    MPU9250_Sleep();
    __delay_ms(50);
    NRF24L01_Sleep();
    __delay_ms(50);
    Sleep();    //sleep(1);     //CSS SLEEP
    
}/* end sleep_mode */

// Função para tirar o dispositivo em modo SLEEP
void wake_up(void)
{
    MPU9250_Wake();
    __delay_ms(50);
    NRF24L01_Wake();
    __delay_ms(50);
    
}/* end wake_up */

/*============================================================================================================================*/
/*
 * Funções da Lógica de Funcionamento
 */

// Função para tratar o tipo de interrupção
void tipo_interrupt(int i) 
{
    wake_up();
    switch (i) {
        case 1: // le todos os sensores e envia via nrf
            flag_timer = 1; // Interrupção do Timer1
            dados_recebidos = 1;
            break;
        case 2: // le comando, se 1 envia os valores guardados via nrf, se 2 altera tempo do timer
            flag_nrf = 1; // Interrupção do NRF24L01+
            break;
        case 3: // le sensores do mpu
            flag_mpu = 1; // Interrupção do MPU9250
            break;
        default:
            break;
    }
}
// Funçao que converte a leitura ADC do NTC 10K em ºC
float NTC_To_Temperature(uint16_t adc_value) 
{
    float Vout = (adc_value / 1023.0) * 3.3;  // Tensão no pino do NTC
    float R_ntc_actual = (R_ntc * Vout) / (3.3 - Vout); // Resistência do NTC
    float temperatura_kelvin = 1.0 / ((1.0 / T0) + (1.0 / BETA) * log(R_ntc_actual / R0));
    return temperatura_kelvin - 273.15; // Converte para °C
}

// Função para calcular a porcentagem da bateria
int porcentagem_bateria(float bateria)
{
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
        return 0; // Bateria crítica
    }
}

// Função para expôr ao usuário caso ocorra um erro
void error(int erro)
{   
    int i = 0;
    switch (erro)
    {
        case 1:
            while(1){
                LED_Toggle();
                __delay_ms(1000);        
                LED_Toggle();
                __delay_ms(1000);
                i++;
            }
            //-----------------------------------FAZER!! - nrf n configurado.
            break;
        case 2:
            NRF24L01_ReadPayload(&erro, 1);
            while(1){
                LED_Toggle();
                __delay_ms(2000);        
                LED_Toggle();
                __delay_ms(2000);
                i++;
            }
            //-----------------------------------FAZER!! - mpu n configurado.           
            break;
        case 3:
            while(1){
                LED_Toggle();
                __delay_ms(3000);        
                LED_Toggle();
                __delay_ms(3000);
                i++;
            }
            //-----------------------------------FAZER!! - não conectado ao outro nrf  
            break;
        case 4:
            NRF24L01_ReadPayload(erro, 1);
            while(1){
                LED_Toggle();
                __delay_ms(2000);        
                LED_Toggle();
                __delay_ms(2000);
                i++;
            }
            //-----------------------------------FAZER!! - interrupções zoadas
            break;
        default:
            break;
    }
    while(i < 5){
        LED_Toggle();
        __delay_ms(500);        
        LED_Toggle();
        __delay_ms(500);
        i++;
    }
} /* end error */
