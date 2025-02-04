#include "mpu9250.h"

// Vari�veis globais
extern bool flag_timer;             // Flag interrup��o timer
extern bool flag_mpu;               // Flag interrup��o mpu
extern bool flag_nrf;               // Flag interrup��o nrf
extern bool enviar_dados;           // Flag para enviar dados
extern bool dados_recebidos;        // Flag para dados recebidos   
extern uint8_t receivedCommand;     // Comando recebido pelo NRF24L01+
extern int16_t accel[3], gyro[3], mag[3], temp; //vari�veis mpu
extern float temperatura;           // Temperatura em �C
extern float tensao_bateria;        // Tens�o da bateria em V
extern float strain_gauge_x;        // Leitura do strain gauge X
extern float strain_gauge_y;        // Leitura do strain gauge Y
extern float strain_gauge_z;        // Leitura do strain gauge Z

// Fun��o para inicializar o MPU9250
void MPU9250_Init(void) {
    // Configura��o dos pinos SPI
    TRISBbits.TRISB0 = 0;  // SCK como sa�da
    TRISBbits.TRISB1 = 1;  // SDO (MISO) como entrada
    TRISBbits.TRISB2 = 0;  // SDI (MOSI) como sa�da
    TRISBbits.TRISB4 = 0;  // SSMPU como sa�da

    // Configura��o do SPI1
    SPI1CON1bits.MSTEN = 1;        // Modo Master
    SPI1CON1bits.CKP = 0;          // Clock idle em low
    SPI1CON1bits.CKE = 1;          // Transi��o de clock ativa na borda de subida
    SPI1CON1bits.SMP = 0;          // Dados amostrados no meio do tempo de clock
    SPI1CON1bits.MODE16 = 0;       // Modo 8-bit
    SPI1CON1bits.PPRE = 3;         // Prescaler prim�rio 1:1
    SPI1CON1bits.SPRE = 6;         // Prescaler secund�rio 2:1
    SPI1STATbits.SPIEN = 1;        // Habilitar SPI1

    // Configura��o da interrup��o externa IRQMPU
    TRISBbits.TRISB3 = 1;          // IRQMPU como entrada
    INTCON2bits.INT1EP = 0;        // Interrup��o na borda de subida
    IFS1bits.INT1IF = 0;           // Limpar flag de interrup��o
    IEC1bits.INT1IE = 1;           // Habilitar interrup��o externa INT1

    // Configura��o inicial do MPU9250
    MPU9250_WriteRegister(PWR_MGMT_1, 0x00);  // Ligar o MPU9250
    MPU9250_WriteRegister(PWR_MGMT_2, 0x00);  // Habilitar todos os sensores
    MPU9250_WriteRegister(INT_PIN_CFG, 0x02); // Configurar interrup��o
    MPU9250_WriteRegister(INT_ENABLE, 0x01);  // Habilitar interrup��o

    // Configura��o do aceler�metro e girosc�pio
    MPU9250_WriteRegister(ACCEL_CONFIG, 0x00); // Aceler�metro �2g   0x00 = +�2g    0x08 = �4g      0x10 = �8g       0x18 = �16g
    MPU9250_WriteRegister(GYRO_CONFIG, 0x00);  // Girosc�pio �250�/s 0x00 = +250dps 0x08 = +500 dps 0x10 = +1000 dps 0x18 = +2000 dps

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
    LATBbits.LATB4 = 0;  // Selecionar o MPU9250 (CS baixo)

    SPI1BUF = reg & 0x7F;  // Enviar endere�o do registrador (bit 7 em 0 para escrita)
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    SPI1BUF = data;  // Enviar dado
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio

    LATBbits.LATB4 = 1;  // Deselecionar o MPU9250 (CS alto)
}

// Fun��o para ler um registrador do MPU9250
uint8_t MPU9250_ReadRegister(uint8_t reg) {
    uint8_t data;

    LATBbits.LATB4 = 0;  // Selecionar o MPU9250 (CS baixo)

    SPI1BUF = reg | 0x80;  // Enviar endere�o do registrador (bit 7 em 1 para leitura)
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    data = SPI1BUF;  // Ler dado

    LATBbits.LATB4 = 1;  // Deselecionar o MPU9250 (CS alto)

    return data;
}

// Fun��o para ler os dados de acelera��o, girosc�pio, magnet�metro e temperatura
void MPU9250_ReadData(int16_t *accel, int16_t *gyro, int16_t *mag, int16_t *temp) {
    uint8_t buffer[21];

    LATBbits.LATB4 = 0;  // Selecionar o MPU9250 (CS baixo)

    SPI1BUF = ACCEL_XOUT_H | 0x80;  // Iniciar leitura a partir do registrador 0x3B
    for (int i = 0; i < 21; i++) {
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
        buffer[i] = SPI1BUF;
    }

    LATBbits.LATB4 = 1;  // Deselecionar o MPU9250 (CS alto)

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
    MPU9250_WriteRegister(PWR_MGMT_1, 0x40);  // Habilitar sleep mode
}

// Fun��o para acordar o MPU9250
void MPU9250_Wake(void) {
    MPU9250_WriteRegister(PWR_MGMT_1, 0x00);  // Desabilitar sleep mode
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