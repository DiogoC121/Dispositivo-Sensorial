#include "mpu9250.h"

// Vari�veis globais
extern bool flag_timer;             // Flag interrup��o timer
extern bool flag_mpu;               // Flag interrup��o mpu
extern bool flag_nrf;               // Flag interrup��o nrf
extern bool enviar_dados;           // Flag para enviar dados
extern bool dados_recebidos;        // Flag para dados recebidos   
extern int16_t receivedCommand;     // Comando recebido pelo NRF24L01+
extern int16_t accel[3], gyro[3], mag[3], temp; //vari�veis mpu
extern float temperatura;           // Temperatura em �C
extern float tensao_bateria;        // Tens�o da bateria em V
extern float strain_gauge_x;        // Leitura do strain gauge X
extern float strain_gauge_y;        // Leitura do strain gauge Y
extern float strain_gauge_z;        // Leitura do strain gauge Z

// Fun��o para inicializar o MPU9250
void MPU9250_Init(void) {

    // Configura��o inicial do MPU9250
    MPU9250_WriteRegister(PWR_MGMT_1, 0x01);  // Ligar o MPU9250
    MPU9250_WriteRegister(PWR_MGMT_2, 0x00);  // Habilitar todos os sensores
    MPU9250_WriteRegister(INT_PIN_CFG, 0x12); // Configurar interrup��o
    MPU9250_WriteRegister(INT_ENABLE, 0x01);  // Habilitar interrup��o

    // Configura��o do aceler�metro e girosc�pio
    MPU9250_WriteRegister(ACCEL_CONFIG, 0x10); // Aceler�metro �2g   0x00 = +�2g    0x08 = �4g      0x10 = �8g       0x18 = �16g
    MPU9250_WriteRegister(GYRO_CONFIG, 0x08);  // Girosc�pio �250�/s 0x00 = +250dps 0x08 = +500 dps 0x10 = +1000 dps 0x18 = +2000 dps

    // Configura��o do magnet�metro (AK8963)
    MPU9250_WriteRegister(USER_CTRL, 0x20);    // Habilitar I2C Master
    MPU9250_WriteRegister(I2C_MST_CTRL, 0x0D); // Configurar I2C Master
    MPU9250_WriteRegister(I2C_SLV0_ADDR, 0x8C); // Endere�o do AK8963 (leitura)
    MPU9250_WriteRegister(I2C_SLV0_REG, CNTL); // Registrador de controle
    MPU9250_WriteRegister(I2C_SLV0_CTRL, 0x81); // Habilitar leitura de 1 byte
    MPU9250_WriteRegister(I2C_SLV0_ADDR, 0x0C); // Endere�o do AK8963 (escrita)
    MPU9250_WriteRegister(I2C_SLV0_REG, CNTL); // Registrador de controle
    MPU9250_WriteRegister(I2C_SLV0_CTRL, 0x81); // Habilitar escrita de 1 byte
    MPU9250_WriteRegister(I2C_SLV0_REG, 0x16); // Configurar magnet�metro para modo cont�nuo
}

// Fun��o para escrever em um registrador do MPU9250
void MPU9250_WriteRegister(uint8_t reg, uint8_t data) {
   uint16_t send = 0;
  
   send = (WRITE_BIT | reg) << 8;
   send = (send | data);
   
   SSNRF_SetHigh();
   SSMPU_SetLow();                                                                             // Ativa o chip MPU9250
   
   spi_xfer (send);
   while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio                                                                     // send write command, register address and data
 
   SSMPU_SetHigh();                                                                             // Desativa o chip MPU9250
}

// Fun��o para ler um registrador do MPU9250
uint8_t MPU9250_ReadRegister(uint8_t reg) {
   uint16_t send = 0;
  
   send = (READ_BIT | reg) << 8; 
   while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
   send = (send | 0xFF);
   while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
   uint16_t received = 0x0000;

   SSNRF_SetHigh();
   SSMPU_SetLow();                                                                             // Ativa o chip MPU9250
   
   received = spi_xfer (send);                                                          // send read command and register address and read data
    
   SSMPU_SetHigh();                                                                             // Desativa o chip MPU9250
  
   return received;
}

// Fun��o para ler o sensor de temperatura do MPU9250
void MPU9250_read_temp(uint8_t* tempH, uint8_t* tempL)
{
    // read temperature data
    *tempH = MPU9250_ReadRegister(TEMP_OUT_H);
    *tempL = MPU9250_ReadRegister(TEMP_OUT_L);      
}

// Fun��o para ler o sensor de aceleração do MPU9250
void MPU9250_read_accel(uint8_t* xH, uint8_t* xL, uint8_t* yH, uint8_t* yL, uint8_t* zH, uint8_t* zL)
{ 
    // read accelerometer data
    *xH = MPU9250_ReadRegister(ACCEL_XOUT_H);
    *xL = MPU9250_ReadRegister(ACCEL_XOUT_L);
    *yH = MPU9250_ReadRegister(ACCEL_YOUT_H);
    *yL = MPU9250_ReadRegister(ACCEL_YOUT_L);
    *zH = MPU9250_ReadRegister(ACCEL_ZOUT_H);
    *zL = MPU9250_ReadRegister(ACCEL_ZOUT_L);
}

// Fun��o para ler o sensor de giro do MPU9250
void MPU9250_read_gyro(uint8_t* xH, uint8_t* xL, uint8_t* yH, uint8_t* yL, uint8_t* zH, uint8_t* zL)
{
    // read gyroscope data
    *xH = MPU9250_ReadRegister(GYRO_XOUT_H);
    *xL = MPU9250_ReadRegister(GYRO_XOUT_L);
    *yH = MPU9250_ReadRegister(GYRO_YOUT_H);
    *yL = MPU9250_ReadRegister(GYRO_YOUT_L);
    *zH = MPU9250_ReadRegister(GYRO_ZOUT_H);
    *zL = MPU9250_ReadRegister(GYRO_ZOUT_L);
}

// Fun��o para ler o sensor magnético do MPU9250
void MPU9250_read_mag(uint8_t* xH, uint8_t* xL, uint8_t* yH, uint8_t* yL, uint8_t* zH, uint8_t* zL)
{
    // read magnetometer data
    *xH = MPU9250_ReadRegister(HXH);
    *xL = MPU9250_ReadRegister(HXL);
    *yH = MPU9250_ReadRegister(HYH);
    *yL = MPU9250_ReadRegister(HYL);
    *zH = MPU9250_ReadRegister(HZH);
    *zL = MPU9250_ReadRegister(HZL);
}

// Fun��o para ler os dados de acelera��o, girosc�pio, magnet�metro e temperatura
void MPU9250_ReadData(int16_t *accel, int16_t *gyro, int16_t *mag, int16_t *temp) {
    uint8_t buffer[21];

   SSMPU_SetLow();                                                                             // Ativa o chip MPU9250

    SPI1BUF = ACCEL_XOUT_H | 0x80;  // Iniciar leitura a partir do registrador 0x3B
    for (int i = 0; i < 21; i++) {
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
        buffer[i] = SPI1BUF;
    }

   SSMPU_SetHigh();                                                                             // Desativa o chip MPU9250

    // Converter os dados lidos para inteiros de 16 bits
    accel[0] = (buffer[0] << 8) | buffer[1];  // Acelera��o X
    accel[1] = (buffer[2] << 8) | buffer[3];  // Acelera��o Y
    accel[2] = (buffer[4] << 8) | buffer[5];  // Acelera��o Z
    temp[0] = (buffer[6] << 8) | buffer[7];   // Temperatura
    gyro[0] = (buffer[8] << 8) | buffer[9];   // Girosc�pio X
    gyro[1] = (buffer[10] << 8) | buffer[11]; // Girosc�pio Y
    gyro[2] = (buffer[12] << 8) | buffer[13]; // Girosc�pio Z
    mag[0] = (buffer[15] << 8) | buffer[14];  // Magnet�metro X
    mag[1] = (buffer[17] << 8) | buffer[16];  // Magnet�metro Y
    mag[2] = (buffer[19] << 8) | buffer[18];  // Magnet�metro Z
}

// Fun��o para colocar o MPU9250 em modo de baixo consumo (sleep)
void MPU9250_Sleep(void) {
    //MPU9250_WriteRegister(CNTL, 0x00);                                                                // power-down mode manetometer
    MPU9250_WriteRegister(PWR_MGMT_1, 0x40);  // Habilitar sleep mode
}

// Fun��o para acordar o MPU9250
void MPU9250_Wake(void) {
    //MPU9250_WriteRegister(CNTL, 0x02);                                                                // continuous mode read of the magnetometer
    MPU9250_WriteRegister(PWR_MGMT_1, 0x01);  // Desabilitar sleep mode
}

// Fun��o para verificar erros
uint8_t MPU9250_CheckError(void) {
    uint8_t status = MPU9250_ReadRegister(INT_STATUS);
    return (status & 0x01);  // Retorna 1 se houver erro
}

/*
// Fun��o de interrup��o externa para o pino IRQMPU
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void) {
    IFS1bits.INT1IF = 0;  // Limpar flag de interrup��o

    // Aqui voc� pode adicionar o c�digo para tratar a interrup��o
    int16_t accel[3], gyro[3], mag[3], temp;
    MPU9250_ReadData(accel, gyro, mag, &temp);

    // Fa�a algo com os dados lidos...
}
 */