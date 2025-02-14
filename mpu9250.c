#include "mpu9250.h"

// Variáveis globais
extern bool flag_mpu;               // Flag interrupção mpu
extern bool enviar_dados;           // Flag para enviar dados

// Função para inicializar o MPU9250
void MPU9250_Init(void) {

    // Configuração inicial do MPU9250
    MPU9250_WriteRegister(PWR_MGMT_1, 0x01);  // Ligar o MPU9250
    MPU9250_WriteRegister(PWR_MGMT_2, 0x00);  // Habilitar todos os sensores
    MPU9250_WriteRegister(INT_PIN_CFG, 0x12); // Configurar interrupção
    MPU9250_WriteRegister(INT_ENABLE, 0x01);  // Habilitar interrupção

    // Configuração do acelerômetro e giroscópio
    MPU9250_WriteRegister(ACCEL_CONFIG, 0x10); // Acelerômetro ±2g   0x00 = +±2g    0x08 = ±4g      0x10 = ±8g       0x18 = ±16g
    MPU9250_WriteRegister(GYRO_CONFIG, 0x08);  // Giroscópio ±250°/s 0x00 = +250dps 0x08 = +500 dps 0x10 = +1000 dps 0x18 = +2000 dps

    // Configuração do magnetômetro (AK8963)
    MPU9250_WriteRegister(USER_CTRL, 0x20);    // Habilitar I2C Master
    MPU9250_WriteRegister(I2C_MST_CTRL, 0x0D); // Configurar I2C Master
    MPU9250_WriteRegister(I2C_SLV0_ADDR, 0x8C); // Endereço do AK8963 (leitura)
    MPU9250_WriteRegister(I2C_SLV0_REG, CNTL); // Registrador de controle
    //MPU9250_WriteRegister(I2C_SLV0_CTRL, 0x81); // Habilitar leitura de 1 byte
    //MPU9250_WriteRegister(I2C_SLV0_ADDR, 0x0C); // Endereço do AK8963 (escrita)
    //MPU9250_WriteRegister(I2C_SLV0_REG, CNTL); // Registrador de controle
    MPU9250_WriteRegister(I2C_SLV0_REG, 0x16); // Configurar magnetômetro para modo contínuo
    MPU9250_WriteRegister(I2C_SLV0_CTRL, 0x81); // Habilitar escrita de 1 byte
}

// Função para escrever em um registrador do MPU9250
void MPU9250_WriteRegister(uint8_t reg, uint8_t data) {
   
   SSNRF_SetHigh();
   SSMPU_SetLow(); 
  
   SPI1_Exchange8bit(WRITE_BIT | reg);
   SPI1_Exchange8bit(data);     
   
   while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio                                                                     // send write command, register address and data
 
   SSMPU_SetHigh();                                                                             // Desativa o chip MPU9250
}

// Função para ler um registrador do MPU9250
uint8_t MPU9250_ReadRegister(uint8_t reg) {
   uint8_t received;

   SSNRF_SetHigh();
   SSMPU_SetLow();    
  
   SPI1_Exchange8bit(READ_BIT | reg);
   while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
   received = SPI1_Exchange8bit(0xFF);             
   while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio           // send read command and register address and read data
    
   SSMPU_SetHigh();                                                                             // Desativa o chip MPU9250
  
   return received;
}

// Função para ler o sensor de temperatura do MPU9250
void MPU9250_read_temp(uint8_t* tempH, uint8_t* tempL)
{
    // read temperature data
    *tempH = MPU9250_ReadRegister(TEMP_OUT_H);
    *tempL = MPU9250_ReadRegister(TEMP_OUT_L);      
}

// Função para ler o sensor de aceleraÃ§Ã£o do MPU9250
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

// Função para ler o sensor de giro do MPU9250
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

// Função para ler o sensor magnÃ©tico do MPU9250
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

// Função para ler os dados de aceleração, giroscópio, magnetômetro e temperatura
void MPU9250_ReadData(int16_t *accel, int16_t *gyro, int16_t *mag, int16_t *temp) {
    uint8_t buffer[21];

   SSMPU_SetLow();                                                                             // Ativa o chip MPU9250

    SPI1BUF = ACCEL_XOUT_H | 0x80;  // Iniciar leitura a partir do registrador 0x3B
    for (int i = 0; i < 21; i++) {
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
        buffer[i] = SPI1BUF;
    }

   SSMPU_SetHigh();                                                                             // Desativa o chip MPU9250

    // Converter os dados lidos para inteiros de 16 bits
    accel[0] = (buffer[0] << 8) | buffer[1];  // Aceleração X
    accel[1] = (buffer[2] << 8) | buffer[3];  // Aceleração Y
    accel[2] = (buffer[4] << 8) | buffer[5];  // Aceleração Z
    temp[0] = (buffer[6] << 8) | buffer[7];   // Temperatura
    gyro[0] = (buffer[8] << 8) | buffer[9];   // Giroscópio X
    gyro[1] = (buffer[10] << 8) | buffer[11]; // Giroscópio Y
    gyro[2] = (buffer[12] << 8) | buffer[13]; // Giroscópio Z
    mag[0] = (buffer[15] << 8) | buffer[14];  // Magnetômetro X
    mag[1] = (buffer[17] << 8) | buffer[16];  // Magnetômetro Y
    mag[2] = (buffer[19] << 8) | buffer[18];  // Magnetômetro Z
}

// Função para colocar o MPU9250 em modo de baixo consumo (sleep)
void MPU9250_Sleep(void) {
    //MPU9250_WriteRegister(CNTL, 0x00);                                                                // power-down mode manetometer
    MPU9250_WriteRegister(PWR_MGMT_1, 0x40);  // Habilitar sleep mode
}

// Função para acordar o MPU9250
void MPU9250_Wake(void) {
    //MPU9250_WriteRegister(CNTL, 0x02);                                                                // continuous mode read of the magnetometer
    MPU9250_WriteRegister(PWR_MGMT_1, 0x01);  // Desabilitar sleep mode
}

// Função para verificar erros
uint8_t MPU9250_CheckError(void) {
    uint8_t who_am_i = MPU9250_ReadRegister(WHO_AM_I);
    if (who_am_i != WHO_AM_I_RESULT) {  // 0x71 é o valor esperado para o MPU9250
        return 1;  // Retorna 1 se houver erro
    }
    return 0;  // Retorna 0 se não houver erro
}

/*
// Função de interrupção externa para o pino IRQMPU
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void) {
    IFS1bits.INT1IF = 0;  // Limpar flag de interrupção

    // Aqui você pode adicionar o código para tratar a interrupção
    int16_t accel[3], gyro[3], mag[3], temp;
    MPU9250_ReadData(accel, gyro, mag, &temp);

    // Faça algo com os dados lidos...
}
 */