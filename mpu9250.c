#include "mpu9250.h"

// Variáveis globais
extern bool flag_timer;             // Flag interrupção timer
extern bool flag_mpu;               // Flag interrupção mpu
extern bool flag_nrf;               // Flag interrupção nrf
extern bool enviar_dados;           // Flag para enviar dados
extern bool dados_recebidos;        // Flag para dados recebidos   
extern uint8_t receivedCommand;     // Comando recebido pelo NRF24L01+
extern int16_t accel[3], gyro[3], mag[3], temp; //variáveis mpu
extern float temperatura;           // Temperatura em °C
extern float tensao_bateria;        // Tensão da bateria em V
extern float strain_gauge_x;        // Leitura do strain gauge X
extern float strain_gauge_y;        // Leitura do strain gauge Y
extern float strain_gauge_z;        // Leitura do strain gauge Z

// Função para inicializar o MPU9250
void MPU9250_Init(void) {
    // Configuração dos pinos SPI
    TRISBbits.TRISB0 = 0;  // SCK como saída
    TRISBbits.TRISB1 = 1;  // SDO (MISO) como entrada
    TRISBbits.TRISB2 = 0;  // SDI (MOSI) como saída
    TRISBbits.TRISB4 = 0;  // SSMPU como saída

    // Configuração do SPI1
    SPI1CON1bits.MSTEN = 1;        // Modo Master
    SPI1CON1bits.CKP = 0;          // Clock idle em low
    SPI1CON1bits.CKE = 1;          // Transição de clock ativa na borda de subida
    SPI1CON1bits.SMP = 0;          // Dados amostrados no meio do tempo de clock
    SPI1CON1bits.MODE16 = 0;       // Modo 8-bit
    SPI1CON1bits.PPRE = 3;         // Prescaler primário 1:1
    SPI1CON1bits.SPRE = 6;         // Prescaler secundário 2:1
    SPI1STATbits.SPIEN = 1;        // Habilitar SPI1

    // Configuração da interrupção externa IRQMPU
    TRISBbits.TRISB3 = 1;          // IRQMPU como entrada
    INTCON2bits.INT1EP = 0;        // Interrupção na borda de subida
    IFS1bits.INT1IF = 0;           // Limpar flag de interrupção
    IEC1bits.INT1IE = 1;           // Habilitar interrupção externa INT1

    // Configuração inicial do MPU9250
    MPU9250_WriteRegister(PWR_MGMT_1, 0x00);  // Ligar o MPU9250
    MPU9250_WriteRegister(PWR_MGMT_2, 0x00);  // Habilitar todos os sensores
    MPU9250_WriteRegister(INT_PIN_CFG, 0x02); // Configurar interrupção
    MPU9250_WriteRegister(INT_ENABLE, 0x01);  // Habilitar interrupção

    // Configuração do acelerômetro e giroscópio
    MPU9250_WriteRegister(ACCEL_CONFIG, 0x00); // Acelerômetro ±2g   0x00 = +±2g    0x08 = ±4g      0x10 = ±8g       0x18 = ±16g
    MPU9250_WriteRegister(GYRO_CONFIG, 0x00);  // Giroscópio ±250°/s 0x00 = +250dps 0x08 = +500 dps 0x10 = +1000 dps 0x18 = +2000 dps

    // Configuração do magnetômetro (AK8963)
    MPU9250_WriteRegister(USER_CTRL, 0x20);    // Habilitar I2C Master
    MPU9250_WriteRegister(I2C_MST_CTRL, 0x0D); // Configurar I2C Master
    MPU9250_WriteRegister(I2C_SLV0_ADDR, 0x8C); // Endereço do AK8963 (leitura)
    MPU9250_WriteRegister(I2C_SLV0_REG, CNTL); // Registrador de controle
    MPU9250_WriteRegister(I2C_SLV0_CTRL, 0x81); // Habilitar leitura de 1 byte
    MPU9250_WriteRegister(I2C_SLV0_ADDR, 0x0C); // Endereço do AK8963 (escrita)
    MPU9250_WriteRegister(I2C_SLV0_REG, CNTL); // Registrador de controle
    MPU9250_WriteRegister(I2C_SLV0_CTRL, 0x81); // Habilitar escrita de 1 byte
    MPU9250_WriteRegister(I2C_SLV0_REG, 0x16); // Configurar magnetômetro para modo contínuo
}

// Função para escrever em um registrador do MPU9250
void MPU9250_WriteRegister(uint8_t reg, uint8_t data) {
    LATBbits.LATB4 = 0;  // Selecionar o MPU9250 (CS baixo)

    SPI1BUF = reg & 0x7F;  // Enviar endereço do registrador (bit 7 em 0 para escrita)
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    SPI1BUF = data;  // Enviar dado
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio

    LATBbits.LATB4 = 1;  // Deselecionar o MPU9250 (CS alto)
}

// Função para ler um registrador do MPU9250
uint8_t MPU9250_ReadRegister(uint8_t reg) {
    uint8_t data;

    LATBbits.LATB4 = 0;  // Selecionar o MPU9250 (CS baixo)

    SPI1BUF = reg | 0x80;  // Enviar endereço do registrador (bit 7 em 1 para leitura)
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    data = SPI1BUF;  // Ler dado

    LATBbits.LATB4 = 1;  // Deselecionar o MPU9250 (CS alto)

    return data;
}

// Função para ler os dados de aceleração, giroscópio, magnetômetro e temperatura
void MPU9250_ReadData(int16_t *accel, int16_t *gyro, int16_t *mag, int16_t *temp) {
    uint8_t buffer[21];

    LATBbits.LATB4 = 0;  // Selecionar o MPU9250 (CS baixo)

    SPI1BUF = ACCEL_XOUT_H | 0x80;  // Iniciar leitura a partir do registrador 0x3B
    for (int i = 0; i < 21; i++) {
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
        buffer[i] = SPI1BUF;
    }

    LATBbits.LATB4 = 1;  // Deselecionar o MPU9250 (CS alto)

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
    MPU9250_WriteRegister(PWR_MGMT_1, 0x40);  // Habilitar sleep mode
}

// Função para acordar o MPU9250
void MPU9250_Wake(void) {
    MPU9250_WriteRegister(PWR_MGMT_1, 0x00);  // Desabilitar sleep mode
}

// Função para verificar erros
uint8_t MPU9250_CheckError(void) {
    uint8_t status = MPU9250_ReadRegister(INT_STATUS);
    return (status & 0x01);  // Retorna 1 se houver erro
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