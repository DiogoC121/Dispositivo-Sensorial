/*============================================================================================================================*/  
/*
 * Bibliotecas e Macros
 *
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
int16_t receivedCommand = 0;  // Comando recebido pelo NRF24L01+
int16_t accel[3], gyro[3], mag[3], temp;
float temperatura = 0.0;       // Temperatura em °C
float tensao_bateria = 0.0;    // Tensão da bateria em V
float strain_gauge_x = 0.0;    // Leitura do strain gauge X
float strain_gauge_y = 0.0;    // Leitura do strain gauge Y
float strain_gauge_z = 0.0;    // Leitura do strain gauge Z

const uint8_t addressnrf[5] = "00001";
/*============================================================================================================================

 * Função Principal
 */

int main(void)
{
   SYSTEM_Initialize();  // Inicializa o PIC24FJ64GA002
   
   EX_INT1_InterruptEnable();
   EX_INT0_InterruptEnable();
   
   SSNRF_SetHigh();
   MPU9250_Init();       // Inicializa o MPU9250
   __delay_ms(20);
   SSMPU_SetHigh();
   NRF24L01_Init();      // Inicializa o NRF24L01+uint8_t address[5] = {0x52, 0xA0, 0xC1, 0x71, 0x1E}; // Endereço "node3"
   NRF24L01_SetAddress(addressnrf, 5);
   __delay_ms(20);


   LED_SetHigh();  
   NOUSE1_SetLow();
   NOUSE2_SetLow();
   NOUSE3_SetLow();
   NOUSE4_SetLow();
   ENABLE_SetLow();
   
   int16_t txBuffer[30];
   int16_t rxBuffer;
   
   while(1)
   {
      /*
      // Chamada das interrupÃ§Ãµes
      
      / *
      if(interrupt_active(INT_EXT1)){
         ext1_isr();
      }
      else if(interrupt_active(INT_EXT2)){
         ext2_isr();
      }
      * /
      
      // Leitura dos dados de temperatura, tensÃ£o de bateria e deformaÃ§Ã£o mecÃ¢nica do ADC e formataÃ§Ã£o dos dados para 2 bytes (High e Low) para ficar no mesmo formato do restante dos sensores
      
      ntcTemp  = getADC(0);                                                                        // LÃª valor ADC da temperatura do NTC
      ntcTempH = (uint8_t)(ntcTemp >> 8);                                                          // Transforma valor lido em 2 bytes
      ntcTempL = (uint8_t)ntcTemp;
      
      batVolt  = getADC(1);                                                                        // LÃª valor ADC da tensÃ£o da bateria     
      batVoltH = (uint8_t)(batVolt >> 8);                                                          // Transforma valor lido em 2 bytes
      batVoltL = (uint8_t)batVolt;
      
      extX  = getADC(2);                                                                           // LÃª valor ADC do extensÃ´metro eixo X
      extXH = (uint8_t)(extX >> 8);                                                                // Transforma valor lido em 2 bytes
      extXL = (uint8_t)extX;
      
      extY  = getADC(3);                                                                           // LÃª valor ADC do extensÃ´metro eixo Y
      extYH = (uint8_t)(extY >> 8);                                                                // Transforma valor lido em 2 bytes
      extYL = (uint8_t)extY;
            
      extZ  = getADC(4);                                                                           // LÃª valor ADC do extensÃ´metro eixo Z                                                                        // LÃª valor ADC da temperatura do NTC
      extZH = (uint8_t)(extZ >> 8);                                                                // Transforma valor lido em 2 bytes
      extZL = (uint8_t)extZ;
      
      // ManipulaÃ§Ã£o dos dados ADC
      
      bateria = (HVIN/1024) * (batVolt);
      
      if(bateria > (HVIN/2))
      {
         output_low(ENABLE);
      }
      else
      {
         output_high(ENABLE);
      }
      
      // Leitura dos sensores do MPU9250
      
      mpu9250_read_temp(&mpuTempH, &mpuTempL);                                                     // Leitura de Temperatura
      mpu9250_read_accel(&mpuAcelXH, &mpuAcelXL, &mpuAcelYH, &mpuAcelYL, &mpuAcelZH, &mpuAcelZL);  // Leitura do AcelerÃ´meto
      mpu9250_read_gyro(&mpuGiroXH, &mpuGiroXL, &mpuGiroYH, &mpuGiroYL, &mpuGiroZH, &mpuGiroZL);   // Leitura do GiroscÃ³pio
      //mpu9250_read_mag(&mpuMagXH, &mpuMagXL, &mpuMagYH, &mpuMagYL, &mpuMagZH, &mpuMagZL);          // Leitura do MagnetÃ´metro
      
      // ManipulaÃ§Ã£O dos dados lidos do MPU9250
      
      internTemp = ((float)((int16_t)((mpuTempH << 8) | mpuTempL)) / 333.87f) + 21.0f;
      */  
      // Testes de SPI
       
      uint8_t status = NRF24L01_CheckStatus();
      if (status & 0x20) {
          dados_recebidos = true;               // Payload recebido (se estiver no modo RX)
      }
      if (status & 0x10) {
          error(3);                             // Máximo de retransmissões atingido
      }
      
      if(dados_recebidos == true){
          NRF24L01_ReadPayload(&rxBuffer, 1);
          receivedCommand = rxBuffer;
      }
      
      SSMPU_SetHigh();
      NRF24L01_WriteRegister(0xDC, 0xBA);
      SSNRF_SetHigh();
      MPU9250_WriteRegister(0xDA, 0xD0);
      
      SSMPU_SetHigh();
      SSNRF_SetHigh();
      
      LATB = 0x0810;
      // Testes de output 
      
      LED_SetHigh();
      __delay_ms(200);  
      LED_SetLow();
      __delay_ms(200);   
      
      //Envia os dados do MPU9250 e do ADC pelo NRF24L01+
      txBuffer[0]  = 0x00; // mpuAcelXH;
      txBuffer[1]  = 0x01; // mpuAcelXL;
      txBuffer[2]  = 0x02; // mpuAcelYH;
      txBuffer[3]  = 0x03; // mpuAcelYL;
      txBuffer[4]  = 0x04; // mpuAcelZH;
      txBuffer[5]  = 0x05; // mpuAcelZL;
      txBuffer[6]  = 0x06; // mpuGiroXH;
      txBuffer[7]  = 0x07; // mpuGiroXL;
      txBuffer[8]  = 0x08; // mpuGiroYH;
      txBuffer[9]  = 0x09; // mpuGiroYL;
      txBuffer[10] = 0x0A; // mpuGiroZH;
      txBuffer[11] = 0x0B; // mpuGiroZL;
      txBuffer[12] = 0x0C; // mpuMagXH;
      txBuffer[13] = 0x0D; // mpuMagXL;
      txBuffer[14] = 0x0E; // mpuMagYH;
      txBuffer[15] = 0x0F; // mpuMagYL;
      txBuffer[16] = 0x10; // mpuMagZH;
      txBuffer[17] = 0x11; // mpuMagZL;
      txBuffer[18] = 0x12; // mpuTempH;
      txBuffer[19] = 0x13; // mpuTempL;
      
      txBuffer[20] = 0x14; // ntcTempH;
      txBuffer[21] = 0x15; // ntcTempL;
      txBuffer[22] = 0x16; // batVoltH;
      txBuffer[23] = 0x17; // batVoltL;
      txBuffer[24] = 0x18; // extXH;
      txBuffer[25] = 0x19; // extXL;
      txBuffer[26] = 0x1A; // extYH;
      txBuffer[27] = 0x1B; // extYL;
      txBuffer[28] = 0x1C; // extZH;
      txBuffer[29] = 0x1D; // extZL;
      
      //NRF24L01_WritePayload(txBuffer, 30);
      
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

        //sleep_mode();
        dados_recebidos = false;
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
    
    ENABLE_SetHigh();
    __delay_ms(50);
    AD1CHSbits.CH0SA = sensor; // Seleciona o canal
    AD1CON1bits.SAMP = 1;       // Inicia a amostragem
    while (!AD1CON1bits.DONE);  // Aguarda a conversão
    __delay_ms(50);
    ENABLE_SetLow();
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
void error(int16_t erro)
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
            NRF24L01_ReadPayload(&erro, 1);
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

uint16_t spi_xfer(uint16_t mensagem)
{
    uint16_t mensagemLida = 0;
    mensagemLida = SPI1_Exchange16bit(mensagem);
    return mensagemLida;
}
/*
// FunÃ§Ã£o de leitura das portas ADC
uint16_t getADC(int sensor)
{   
   / *sensor = 0 => TEMPERATURA
     sensor = 1 => BATERIA
     sensor = 2 => SG X
     sensor = 3 => SG Y
     sensor = 4 => SG Z
   * /
   
   uint16_t conversion = 0;
   
   output_high(ENABLE);
   __delay_us(100);   
   switch(sensor){
     case 0:
         set_adc_channel(TEMPSENS);
         __delay_us(10);   
         //while(!adc_done());   
         conversion = read_adc();
         break;
         
     case 1:
         set_adc_channel(SBAT);
         __delay_us(10);   
         //while(!adc_done());   
         conversion = read_adc();
         break;
         
     case 2:
         set_adc_channel(SGX);
         __delay_us(10);   
         //while(!adc_done());   
         conversion = read_adc();
         break;
         
     case 3:
         set_adc_channel(SGY);
         __delay_us(10);   
         //while(!adc_done());   
         conversion = read_adc();
         break;
         
     case 4:
         set_adc_channel(SGZ);
         __delay_us(10);   
         //while(!adc_done());   
         conversion = read_adc();
         break;
         
     default:
         conversion = 0;
         break;       
   }
   
   delay_us(100);   
   output_low(ENABLE);
   return conversion;                                                                              // return conversion result
} / * end getADC * /
*/