
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

/*
 * Arquivo.c para configuração do dispositivo sensor
 * Autor: Diogo Correia da Silva
 
 */

#include <xc.h> // include processor files - each processor file is guarded.  
// ---------------------------------------------------------------------------------------------------------------------------------
/*============================================================================================================================*/  
/*
 * Bibliotecas e Macros
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
//#include "main.h"
#include <math.h>
#include <string.h>  
#include <stdint.h>
#include <stdbool.h>

#define FCY 4000000UL
#include <libpic30.h>

#define VIN                         2.5     // Tensão de alimentação
#define HVIN                        1.25    // Metade da tensão de alimentação

#define R_ntc                       10000.0 // Resistor de 10kOhm no divisor de tensão
#define BETA                        3950.0  // Coeficiente Beta do termistor NTC 10k
#define T0                          298.15  // Temperatura de referência em Kelvin (25°C)
#define R0                          10000.0 // Resistência nominal do termistor a 25°C (10k?)

#define TEMP                        PIN_A0  // PINO - SENSOR ANALÓGICO DE TEMPERATURA NTC
#define SBAT                        PIN_A1  // PINO - SENSOR ANALÓGICO DA BATERIA 
#define SGZ                         PIN_B13 // PINO - SENSOR ANALÓGICO STRAIN GAUGE X
#define SGY                         PIN_B14 // PINO - SENSOR ANALÓGICO STRAIN GAUGE Y
#define SGX                         PIN_B15 // PINO - SENSOR ANALÓGICO STRAIN GAUGE Z

#define LED                         PIN_A2  // PINO - SAÍDA LED
#define ENABLE                      PIN_A4  // PINO - SAÍDA LED2
#define NOUSE1                      PIN_A3  // PINO - SAÍDA SEM USO
#define NOUSE2                      PIN_B8  // PINO - SAÍDA SEM USO
#define NOUSE3                      PIN_B9  // PINO - SAÍDA SEM USO
#define NOUSE4                      PIN_B10 // PINO - SAÍDA SEM USO

#define SCK                         PIN_B0  // PINO - CLOCK
#define SDO                         PIN_B1  // PINO - DATA OUT
#define SDI                         PIN_B2  // PINO - DATA IN
#define SSMPU                       PIN_B4  // PINO - CHIP SELECT - MPU
#define SSNRF                       PIN_B11 // PINO - CHIP SELECT - NRF
#define CENRF                       PIN_B12 // PINO - CHIP ENABLE - NRF

#define IRQMPU                      PIN_B3  // PINO - INTERRUPCAO EXTERNA 1 - MPU
#define IRQNRF                      PIN_B7  // PINO - INTERRUPCAO EXTERNA 2 - NRF

#define PGD                         PIN_B5  // PINO - DATA - PROGRAMACAO DO PIC
#define PGC                         PIN_B6  // PINO - CLOCK - PROGRAMACAO DO PIC

/*============================================================================================================================*/ 
/*
 * Mapa de registradores para Giroscopio e Acelerometro
 */ 
  
#define SELF_TEST_X_GYRO            0x00    // READ / WRITE   - Gyroscope Self-Test Registers - X 
#define SELF_TEST_Y_GYRO            0x01    // READ / WRITE   - Gyroscope Self-Test Registers - Y
#define SELF_TEST_Z_GYRO            0x02    // READ / WRITE   - Gyroscope Self-Test Registers - Z
#define SELF_TEST_X_ACCEL           0x0D    // READ / WRITE   - Accelerometer Self-Test Registers - X
#define SELF_TEST_Y_ACCEL           0x0E    // READ / WRITE   - Accelerometer Self-Test Registers - Y
#define SELF_TEST_Z_ACCEL           0x0F    // READ / WRITE   - Accelerometer Self-Test Registers - Z
#define XG_OFFSET_H                 0x13    // READ / WRITE   - Gyro Offset Registers - X High byte
#define XG_OFFSET_L                 0x14    // READ / WRITE   - Gyro Offset Registers - X Low byte
#define YG_OFFSET_H                 0x15    // READ / WRITE   - Gyro Offset Registers - Y High byte
#define YG_OFFSET_L                 0x16    // READ / WRITE   - Gyro Offset Registers - Y Low byte
#define ZG_OFFSET_H                 0x17    // READ / WRITE   - Gyro Offset Registers - Z High byte
#define ZG_OFFSET_L                 0x18    // READ / WRITE   - Gyro Offset Registers - Z Low byte
#define SMPLRT_DIV                  0x19    // READ / WRITE   - Sample Rate Divider
#define CONFIG                      0x1A    // READ / WRITE   - Configuration
#define GYRO_CONFIG                 0x1B    // READ / WRITE   - Gyroscope Configuration
#define ACCEL_CONFIG                0x1C    // READ / WRITE   - Accelerometer Configuration
#define ACCEL_CONFIG_2              0x1D    // READ / WRITE   - Accelerometer Configuration 2
#define LP_ACCEL_ODR                0x1E    // READ / WRITE   - Low Power Accelerometer ODR Control
#define WOM_THR                     0x1F    // READ / WRITE   - Wake-on Motion Threshold
#define FIFO_EN                     0x23    // READ / WRITE   - FIFO Enable
#define I2C_MST_CTRL                0x24    // READ / WRITE   - I2C Master Control
#define I2C_SLV0_ADDR               0x25    // READ / WRITE   - I2C Slave 1 Control - I2C_SLV0_ADDR
#define I2C_SLV0_REG                0x26    // READ / WRITE   - I2C Slave 1 Control - I2C_SLV0_REG
#define I2C_SLV0_CTRL               0x27    // READ / WRITE   - I2C Slave 1 Control - I2C_SLV0_CTRL
#define I2C_SLV1_ADDR               0x28    // READ / WRITE   - I2C Slave 1 Control - I2C_SLV1_ADDR
#define I2C_SLV1_REG                0x29    // READ / WRITE   - I2C Slave 1 Control - I2C_SLV1_REG
#define I2C_SLV1_CTRL               0x2A    // READ / WRITE   - I2C Slave 1 Control - I2C_SLV1_CTRL
#define I2C_SLV2_ADDR               0x2B    // READ / WRITE   - I2C Slave 2 Control - I2C_SLV2_ADDR
#define I2C_SLV2_REG                0x2C    // READ / WRITE   - I2C Slave 2 Control - I2C_SLV2_REG
#define I2C_SLV2_CTRL               0x2D    // READ / WRITE   - I2C Slave 2 Control - I2C_SLV2_CTRL
#define I2C_SLV3_ADDR               0x2E    // READ / WRITE   - I2C Slave 3 Control - I2C_SLV3_ADDR
#define I2C_SLV3_REG                0x2F    // READ / WRITE   - I2C Slave 3 Control - I2C_SLV3_REG
#define I2C_SLV3_CTRL               0x30    // READ / WRITE   - I2C Slave 3 Control - I2C_SLV3_CTRL
#define I2C_SLV4_ADDR               0x31    // READ / WRITE   - I2C Slave 4 Control - I2C_SLV4_ADDR
#define I2C_SLV4_REG                0x32    // READ / WRITE   - I2C Slave 4 Control - I2C_SLV4_REG
#define I2C_SLV4_DO                 0x33    // READ / WRITE   - I2C Slave 4 Control - I2C_SLV4_DO
#define I2C_SLV4_CTRL               0x34    // READ / WRITE   - I2C Slave 4 Control - I2C_SLV4_CTRL
#define I2C_SLV4_DI                 0x35    // READ           - I2C Slave 4 Control - I2C_SLV4_DI
#define I2C_MST_STATUS              0x36    // READ / CLEAR   - I2C Master Status
#define INT_PIN_CFG                 0x37    // READ / WRITE   - INT Pin / Bypass Enable Configuration
#define INT_ENABLE                  0x38    // READ / WRITE   - Interrupt Enable
#define INT_STATUS                  0x3A    // READ / CLEAR   - Interrupt Status
#define ACCEL_XOUT_H                0x3B    // READ           - Accelerometer Measurements - X High byte
#define ACCEL_XOUT_L                0x3C    // READ           - Accelerometer Measurements - X Low byte
#define ACCEL_YOUT_H                0x3D    // READ           - Accelerometer Measurements - Y High byte
#define ACCEL_YOUT_L                0x3E    // READ           - Accelerometer Measurements - Y Low byte
#define ACCEL_ZOUT_H                0x3F    // READ           - Accelerometer Measurements - Z High byte
#define ACCEL_ZOUT_L                0x40    // READ           - Accelerometer Measurements - Z Low byte
#define TEMP_OUT_H                  0x41    // READ           - Temperature Measurement - High byte
#define TEMP_OUT_L                  0x42    // READ           - Temperature Measurement - Low byte
#define GYRO_XOUT_H                 0x43    // READ           - Gyroscope Measurements - X High byte
#define GYRO_XOUT_L                 0x44    // READ           - Gyroscope Measurements - X Low byte
#define GYRO_YOUT_H                 0x45    // READ           - Gyroscope Measurements - Y High byte
#define GYRO_YOUT_L                 0x46    // READ           - Gyroscope Measurements - Y Low byte
#define GYRO_ZOUT_H                 0x47    // READ           - Gyroscope Measurements - Z High byte
#define GYRO_ZOUT_L                 0x48    // READ           - Gyroscope Measurements - Z Low byte
#define EXT_SENS_DATA_00            0x49    // READ           - External Sensor 0 Data
#define EXT_SENS_DATA_01            0x4A    // READ           - External Sensor 1 Data
#define EXT_SENS_DATA_02            0x4B    // READ           - External Sensor 2 Data
#define EXT_SENS_DATA_03            0x4C    // READ           - External Sensor 3 Data
#define EXT_SENS_DATA_04            0x4D    // READ           - External Sensor 4 Data
#define EXT_SENS_DATA_05            0x4E    // READ           - External Sensor 5 Data
#define EXT_SENS_DATA_06            0x4F    // READ           - External Sensor 6 Data
#define EXT_SENS_DATA_07            0x50    // READ           - External Sensor 7 Data
#define EXT_SENS_DATA_08            0x51    // READ           - External Sensor 8 Data
#define EXT_SENS_DATA_09            0x52    // READ           - External Sensor 9 Data
#define EXT_SENS_DATA_10            0x53    // READ           - External Sensor 10 Data
#define EXT_SENS_DATA_11            0x54    // READ           - External Sensor 11 Data
#define EXT_SENS_DATA_12            0x55    // READ           - External Sensor 12 Data
#define EXT_SENS_DATA_13            0x56    // READ           - External Sensor 13 Data
#define EXT_SENS_DATA_14            0x57    // READ           - External Sensor 14 Data
#define EXT_SENS_DATA_15            0x58    // READ           - External Sensor 15 Data
#define EXT_SENS_DATA_16            0x59    // READ           - External Sensor 16 Data
#define EXT_SENS_DATA_17            0x5A    // READ           - External Sensor 17 Data
#define EXT_SENS_DATA_18            0x5B    // READ           - External Sensor 18 Data
#define EXT_SENS_DATA_19            0x5C    // READ           - External Sensor 19 Data
#define EXT_SENS_DATA_20            0x5D    // READ           - External Sensor 20 Data
#define EXT_SENS_DATA_21            0x5E    // READ           - External Sensor 21 Data
#define EXT_SENS_DATA_22            0x5F    // READ           - External Sensor 22 Data
#define EXT_SENS_DATA_23            0x60    // READ           - External Sensor 23 Data
#define I2C_SLV0_DO                 0x63    // READ / WRITE   - I2C Slave 0 Data Out
#define I2C_SLV1_DO                 0x64    // READ / WRITE   - I2C Slave 1 Data Out
#define I2C_SLV2_DO                 0x65    // READ / WRITE   - I2C Slave 2 Data Out
#define I2C_SLV3_DO                 0x66    // READ / WRITE   - I2C Slave 3 Data Out
#define I2C_MST_DELAY_CTRL          0x67    // READ / WRITE   - I2C Master Delay Control
#define SIGNAL_PATH_RESET           0x68    // READ / WRITE   - Signal Path Reset
#define MOT_DETECT_CTRL             0x69    // READ / WRITE   - Accelerometer Interrupt Control 
#define USER_CTRL                   0x6A    // READ / WRITE   - User Control
#define PWR_MGMT_1                  0x6B    // READ / WRITE   - Power Management 1
#define PWR_MGMT_2                  0x6C    // READ / WRITE   - Power Management 2
#define FIFO_COUNTH                 0x72    // READ           - FIFO Count Registers - High bits
#define FIFO_COUNTL                 0x73    // READ           - FIFO Count Registers - Low bits
#define FIFO_R_W                    0x74    // READ / WRITE   - FIFO Read Write
#define WHO_AM_I                    0x75    // READ           - Who Am I
#define XA_OFFSET_H                 0x77    // READ / WRITE   - Accelerometer Offset Registers - Upper bits of the X accelerometer offset cancellation.
#define XA_OFFSET_L                 0x78    // READ / WRITE   - Accelerometer Offset Registers - Lower bits of the X accelerometer offset cancellation.
#define YA_OFFSET_H                 0x7A    // READ / WRITE   - Accelerometer Offset Registers - Upper bits of the Y accelerometer offset cancellation.
#define YA_OFFSET_L                 0x7B    // READ / WRITE   - Accelerometer Offset Registers - Lower bits of the Y accelerometer offset cancellation.
#define ZA_OFFSET_H                 0x7D    // READ / WRITE   - Accelerometer Offset Registers - Upper bits of the Z accelerometer offset cancellation.
#define ZA_OFFSET_L                 0x7E    // READ / WRITE   - Accelerometer Offset Registers - Lower bits of the Z accelerometer offset cancellation.
  
/*============================================================================================================================*/ 
/*
 * Mapa de registradores para Magnetometro
 */ 
   
#define WIA                         0x00    // READ           - Device ID 
#define INFO                        0x01    // READ           - Information
#define ST1                         0x02    // READ           - Status 1           - Data status
#define HXL                         0x03    // READ           - Measurement data   - X-axis data Low byte
#define HXH                         0x04    // READ           - Measurement data   - X-axis data High byte
#define HYL                         0x05    // READ           - Measurement data   - Y-axis data Low byte
#define HYH                         0x06    // READ           - Measurement data   - Y-axis data High byte
#define HZL                         0x07    // READ           - Measurement data   - Z-axis data Low byte
#define HZH                         0x08    // READ           - Measurement data   - Z-axis data High byte
#define ST2                         0x09    // READ           - Status 2           - Data status
#define CNTL                        0x0A    // READ / WRITE   - Control
#define RSV                         0x0B    // READ / WRITE   - Reserved           - DO NOT ACCESS
#define ASTC                        0x0C    // READ / WRITE   - Self-test
#define TS1                         0x0D    // READ / WRITE   - Test 1             - DO NOT ACCESS
#define TS2                         0x0E    // READ / WRITE   - Test 2             - DO NOT ACCESS
#define I2CDIS                      0x0F    // READ / WRITE   - I2C disable
#define ASAX                        0x10    // READ           - X-axis sensitivity adjustment value  - Fuse ROM
#define ASAY                        0x11    // READ           - Y-axis sensitivity adjustment value  - Fuse ROM
#define ASAZ                        0x12    // READ           - Z-axis sensitivity adjustment value  - Fuse ROM
  
/*============================================================================================================================*/ 
/*
 * Bits auxiliares para o MPU9250
 */ 
  
#define WRITE_BIT                   0x00    // Valor do bit de escrita
#define READ_BIT                    0x80    // Valor do bit de escrita
  
#define WHO_AM_I_RESULT_1           0x68    // Valor presente em WHO_AM_I (RESET VALUE - DATASHEET)
#define WHO_AM_I_RESULT_2           0x71    // Valor presente em WHO_AM_I (ALSO DATASHEET)
#define MPU9250_ADDRESS             0x68    // SIGNAL_PATH_RESET reg
  
#define WIA_RESULT                  0x48    // Valor presente em WIA
#define AK8963_ADDRESS              0x0C    // ASTC reg


/*============================================================================================================================*/
/*
 *  Mapa de registradores do NRF24L01+
 */ 
#define NRF_CONFIG                  0x00    // READ / WRITE   - Configuration Register
#define NRF_EN_AA                   0x01    // READ / WRITE   - Enable "Auto Acknowledgment" Function Disable this functionality to be compatible with nRF2401
#define NRF_EN_RXADDR               0x02    // READ / WRITE   - Enabled RX Addresses
#define NRF_SETUP_AW                0x03    // READ / WRITE   - Setup of Address Widths (common for all data pipes)
#define NRF_SETUP_RETR              0x04    // READ / WRITE   - Setup of Automatic Retransmission
#define NRF_RF_CH                   0x05    // READ / WRITE   - RF Channel
#define NRF_RF_SETUP                0x06    // READ / WRITE   - RF Setup Register
#define NRF_STATUS                  0x07    // READ / WRITE   - Status Register (In parallel to the SPI command word applied on the MOSI pin, the STATUS register is shifted serially out on the MISO pin)
#define NRF_OBSERVE_TX              0x08    // READ           - Transmit observe register
#define NRF_RPD                     0x09    // READ           - Received Power Detector. This register is called CD (Carrier Detect) in the nRF24L01. The name is different in nRF24L01+ due to the different input power level threshold for this bit.
#define NRF_RX_ADDR_P0              0x0A    // READ / WRITE   - Receive address data pipe 0. 5 Bytes maximum length. (LSByte is written first. Write the number of bytes defined by SETUP_AW)        
#define NRF_RX_ADDR_P1              0x0B    // READ / WRITE   - Receive address data pipe 1. 5 Bytes maximum length. (LSByte is written first. Write the number of bytes defined by SETUP_AW)
#define NRF_RX_ADDR_P2              0x0C    // READ / WRITE   - Receive address data pipe 2. Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
#define NRF_RX_ADDR_P3              0x0D    // READ / WRITE   - Receive address data pipe 3. Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
#define NRF_RX_ADDR_P4              0x0E    // READ / WRITE   - Receive address data pipe 4. Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
#define NRF_RX_ADDR_P5              0x0F    // READ / WRITE   - Receive address data pipe 5. Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
#define NRF_TX_ADDR                 0x10    // READ / WRITE   - Transmit address. Used for a PTX device only. (LSByte is written first) Set RX_ADDR_P0 equal to this address to handle automatic acknowledge if this is a PTX device with Enhanced ShockBurst enabled.        
#define NRF_RX_PW_P0                0x11    // READ / WRITE   - Number of bytes in RX payload in data pipe 0 (1 to 32 bytes). 0 Pipe not used; 1 = 1 byte; ...; 32 = 32 bytes
#define NRF_RX_PW_P1                0x12    // READ / WRITE   - Number of bytes in RX payload in data pipe 1 (1 to 32 bytes). 0 Pipe not used; 1 = 1 byte; ...; 32 = 32 bytes
#define NRF_RX_PW_P2                0x13    // READ / WRITE   - Number of bytes in RX payload in data pipe 2 (1 to 32 bytes). 0 Pipe not used; 1 = 1 byte; ...; 32 = 32 bytes
#define NRF_RX_PW_P3                0x14    // READ / WRITE   - Number of bytes in RX payload in data pipe 3 (1 to 32 bytes). 0 Pipe not used; 1 = 1 byte; ...; 32 = 32 bytes
#define NRF_RX_PW_P4                0x15    // READ / WRITE   - Number of bytes in RX payload in data pipe 4 (1 to 32 bytes). 0 Pipe not used; 1 = 1 byte; ...; 32 = 32 bytes
#define NRF_RX_PW_P5                0x16    // READ / WRITE   - Number of bytes in RX payload in data pipe 5 (1 to 32 bytes). 0 Pipe not used; 1 = 1 byte; ...; 32 = 32 bytes
#define NRF_FIFO_STATUS             0x17    // READ / WRITE   - FIFO Status Register
#define NRF_DYNPD                   0x1C    // READ / WRITE   - Enable dynamic payload length
#define NRF_FEATURE                 0x1D    // READ / WRITE   - Feature Register
  
/*============================================================================================================================*/ 
/*
 * Comandos NRF24L01+
 */ 
  
#define NRF_R_REGISTER              0x00    // Read command and status registers. 000A AAAA; AAAAA = 5 bit Register Map Address
#define NRF_W_REGISTER              0x20    // Write command and status registers. 001A AAAA; AAAAA = 5 bit Register Map Address Executable in power down or standby modes only.
#define NRF_R_RX_PAYLOAD            0x61    // Read RX-payload: 1 - 32 bytes. A read operation always starts at byte 0. Payload is deleted from FIFO after it is read. Used in RX mode.
#define NRF_W_TX_PAYLOAD            0xA0    // Write TX-payload: 1 - 32 bytes. A write operation always starts at byte 0 used in TX payload.
#define NRF_FLUSH_TX                0xE1    // Flush TX FIFO, used in TX mode.
#define NRF_FLUSH_RX                0xE2    // Flush RX FIFO, used in RX mode. Should not be executed during transmission of acknowledge, that is, acknowledge package will not be completed.
#define NRF_REUSE_TX_PL             0xE3    // Used for a PTX device Reuse last transmitted payload. TX payload reuse is active until W_TX_PAYLOAD or FLUSH TX is executed. TX payload reuse must not be activated or deactivated during package transmission.
#define NRF_R_RX_PL_WID             0x60    // Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO.
#define NRF_W_ACK_PAYLOAD           0xAB    // Used in RX mode. Write Payload to be transmitted together with ACK packet on PIPE PPP. (PPP valid in the range from 000 to 101). Maximum three ACK packet payloads can be pending. Payloads with same PPP are handled using first in - first out principle. Write payload: 1 - 32 bytes. A write operation always starts at byte 0.
#define NRF_W_TX_PAYLOAD_NO_ACK     0xB0    // Used in TX mode. Disables AUTOACK on this specific packet.
#define NRF_NOP                     0xFF    // No Operation. Might be used to read the STATUS register
  
/*============================================================================================================================*/ 
/*
 * Bits auxiliares para o  NRF24L01+
 */ 
  
//#define ADDRESS                     0x5252E1AB12 //"node3"
/*
#define ENDTX                       0x0009D10830
                                 // 0x3008D10900
#define ENDRX                       0x52A0C1711E                                                   
                                 // 0x1E71C1A052
*/ 

/*============================================================================================================================*/
/*
 * Protótipo das funções
 */

uint16_t spi_xfer(uint16_t mensagem);
uint16_t getADC(int sensor);
void sleep_mode();
void wake_up();
void tipo_interrupt(int i);
float NTC_To_Temperature(uint16_t adc_value); 
int porcentagem_bateria(float bateria);
void error(int16_t erro);

/*============================================================================================================================*/
/*
 * Variáveis globais
 */


extern bool flag_timer;             // Flag interrupção timer
extern bool flag_mpu;               // Flag interrupção mpu
extern bool flag_nrf;               // Flag interrupção nrf
extern bool enviar_dados;           // Flag para enviar dados
extern bool dados_recebidos;        // Flag para dados recebidos   
extern int16_t receivedCommand;     // Comando recebido pelo NRF24L01+
extern int16_t accel[3], gyro[3], mag[3], temp; //variáveis mpu
extern float temperatura;           // Temperatura em °C
extern float tensao_bateria;        // Tensão da bateria em V
extern float strain_gauge_x;        // Leitura do strain gauge X
extern float strain_gauge_y;        // Leitura do strain gauge Y
extern float strain_gauge_z;        // Leitura do strain gauge Z

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

