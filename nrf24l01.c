#include "nrf24l01.h"

// Vari�veis globais
extern bool flag_timer;             // Flag interrup��o timer
extern bool flag_mpu;               // Flag interrup��o mpu
extern bool flag_nrf;               // Flag interrup��o nrf
extern bool enviar_dados;           // Flag para enviar dados
extern bool dados_recebidos;        // Flag para dados recebidos   
extern uint8_t receivedCommand;     // Comando recebido pelo NRF24L01+

// Fun��o para inicializar o NRF24L01+
void NRF24L01_Init(void) {
    
    /*
    // Configura��o dos pinos SPI
    TRISBbits.TRISB0 = 0;  // SCK como sa�da
    TRISBbits.TRISB1 = 1;  // SDO (MISO) como entrada
    TRISBbits.TRISB2 = 0;  // SDI (MOSI) como sa�da
    TRISBbits.TRISB11 = 0; // SSNRF como sa�da
    TRISBbits.TRISB12 = 0; // CENRF como sa�da

    // Configura��o do SPI1
    SPI1CON1bits.MSTEN = 1;        // Modo Master
    SPI1CON1bits.CKP = 0;          // Clock idle em low
    SPI1CON1bits.CKE = 1;          // Transi��o de clock ativa na borda de subida
    SPI1CON1bits.SMP = 0;          // Dados amostrados no meio do tempo de clock
    SPI1CON1bits.MODE16 = 0;       // Modo 8-bit
    SPI1CON1bits.PPRE = 3;         // Prescaler prim�rio 1:1
    SPI1CON1bits.SPRE = 6;         // Prescaler secund�rio 2:1
    SPI1STATbits.SPIEN = 1;        // Habilitar SPI1

    // Configura��o da interrup��o externa IRQNRF
    TRISBbits.TRISB7 = 1;          // IRQNRF como entrada
    INTCON2bits.INT2EP = 0;        // Interrup��o na borda de subida
    IFS1bits.INT2IF = 0;           // Limpar flag de interrup��o
    IEC1bits.INT2IE = 1;           // Habilitar interrup��o externa INT2
    */
    
    // Configura��o inicial do NRF24L01+
    NRF24L01_WriteRegister(NRF_CONFIG, 0x08);  // Desabilitar CRC, Power Down, RX Mode
    NRF24L01_WriteRegister(NRF_EN_AA, 0x00);   // Desabilitar Auto Acknowledgment
    NRF24L01_WriteRegister(NRF_EN_RXADDR, 0x01); // Habilitar apenas o pipe 0
    NRF24L01_WriteRegister(NRF_SETUP_AW, 0x03);  // Endere�o de 5 bytes
    NRF24L01_WriteRegister(NRF_SETUP_RETR, 0x00); // Desabilitar retransmiss�o
    NRF24L01_WriteRegister(NRF_RF_CH, 0x02);    // Canal RF 2
    NRF24L01_WriteRegister(NRF_RF_SETUP, 0x07); // Pot�ncia m�xima (0 dBm), taxa de 1 Mbps
    NRF24L01_WriteRegister(NRF_STATUS, 0x70);   // Limpar flags de status
    NRF24L01_WriteRegister(NRF_RX_PW_P0, 0x20); // Tamanho do payload de 32 bytes no pipe 0
    NRF24L01_WriteRegister(NRF_FEATURE, 0x00);  // Desabilitar features extras
    NRF24L01_WriteRegister(NRF_DYNPD, 0x00);    // Desabilitar payload din�mico
    NRF24L01_WriteRegister(NRF_CONFIG, 0x0A);   // Habilitar CRC de 2 bytes, Power Up, RX Mode
}

// Fun��o para escrever em um registrador do NRF24L01+
void NRF24L01_WriteRegister(uint8_t reg, uint8_t data) {
    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_W_REGISTER | (reg & 0x1F);  // Enviar comando de escrita e endere�o do registrador
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    SPI1BUF = data;  // Enviar dado
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)
}

// Fun��o para ler um registrador do NRF24L01+
uint8_t NRF24L01_ReadRegister(uint8_t reg) {
    uint8_t data;

    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_R_REGISTER | (reg & 0x1F);  // Enviar comando de leitura e endere�o do registrador
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    SPI1BUF = 0xFF;  // Enviar dummy byte para receber o dado
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    data = SPI1BUF;  // Ler dado

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)

    return data;
}

// Fun��o para escrever um payload no NRF24L01+
void NRF24L01_WritePayload(uint8_t *data, uint8_t length) {
    NRF24L01_SetTXMode();  // Ativar modo TX
    LATBbits.LATB11 = 0;   // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_W_TX_PAYLOAD;  // Enviar comando de escrita de payload
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    for (uint8_t i = 0; i < length; i++) {
        SPI1BUF = data[i];  // Enviar dado
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    }

    LATBbits.LATB11 = 1;   // Deselecionar o NRF24L01+ (CS alto)
    NRF24L01_StandbyMode(); // Desativar o NRF24L01+ ap�s a transmiss�o
}

// Fun��o para ler um payload do NRF24L01+
void NRF24L01_ReadPayload(uint8_t *data, uint8_t length) {
    NRF24L01_SetRXMode();  // Ativar modo RX
    LATBbits.LATB11 = 0;   // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_R_RX_PAYLOAD;  // Enviar comando de leitura de payload
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    for (uint8_t i = 0; i < length; i++) {
        SPI1BUF = 0xFF;  // Enviar dummy byte para receber o dado
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
        data[i] = SPI1BUF;  // Ler dado
    }

    LATBbits.LATB11 = 1;   // Deselecionar o NRF24L01+ (CS alto)
    NRF24L01_StandbyMode(); // Desativar o NRF24L01+ ap�s a recep��o
}

// Fun��o para configurar o NRF24L01+ no modo TX
void NRF24L01_SetTXMode(void) {
    uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
    config &= ~0x01;  // Limpar bit PRIM_RX para modo TX
    NRF24L01_WriteRegister(NRF_CONFIG, config);
    LATBbits.LATB12 = 1;  // Habilitar o NRF24L01+ (CE alto)
    __delay_us(15);       // Aguardar 15 �s (tempo m�nimo para entrar no modo TX)
}

// Fun��o para configurar o NRF24L01+ no modo RX
void NRF24L01_SetRXMode(void) {
    uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
    config |= 0x01;  // Setar bit PRIM_RX para modo RX
    NRF24L01_WriteRegister(NRF_CONFIG, config);
    LATBbits.LATB12 = 1;  // Habilitar o NRF24L01+ (CE alto)
    __delay_us(130);      // Aguardar 130 �s (tempo m�nimo para entrar no modo RX)
}

// Fun��o para colocar o NRF24L01+ em modo de espera
void NRF24L01_StandbyMode(void) {
    LATBbits.LATB12 = 0;  // Desativar o NRF24L01+ (CE baixo)
}

// Fun��o para colocar o NRF24L01+ em modo de baixo consumo (sleep)
void NRF24L01_Sleep(void) {
    uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
    config &= ~0x02;  // Desligar o bit PWR_UP
    NRF24L01_WriteRegister(NRF_CONFIG, config);
}

// Fun��o para acordar o NRF24L01+ do modo de baixo consumo
void NRF24L01_Wake(void) {
    uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
    config |= 0x02;  // Ligar o bit PWR_UP
    NRF24L01_WriteRegister(NRF_CONFIG, config);
    __delay_us(1500); // Aguardar 1.5 ms para o NRF24L01+ estabilizar
}

// Fun��o para configurar o endere�o de envio/recebimento
void NRF24L01_SetAddress(uint8_t *address, uint8_t length) {
    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_W_REGISTER | NRF_TX_ADDR;  // Enviar comando de escrita para o registrador TX_ADDR
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    for (uint8_t i = 0; i < length; i++) {
        SPI1BUF = address[i];  // Enviar byte do endere�o
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio
    }

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)
}

// Fun��o para verificar o status do NRF24L01+
uint8_t NRF24L01_CheckStatus(void) {
    return NRF24L01_ReadRegister(NRF_STATUS);
}

// Fun��o para limpar a FIFO de TX
void NRF24L01_FlushTX(void) {
    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_FLUSH_TX;  // Enviar comando para limpar FIFO de TX
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)
}

// Fun��o para limpar a FIFO de RX
void NRF24L01_FlushRX(void) {
    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_FLUSH_RX;  // Enviar comando para limpar FIFO de RX
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recep��o estar cheio

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)
}

// Fun��o para limpar as interrup��es
void NRF24L01_ClearInterrupts(void) {
    uint8_t status = NRF24L01_ReadRegister(NRF_STATUS);
    NRF24L01_WriteRegister(NRF_STATUS, status | 0x70);  // Limpar flags de interrup��o
}

/*
// Fun��o de interrup��o externa para o pino IRQNRF
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void) {
    IFS1bits.INT2IF = 0;  // Limpar flag de interrup��o

    // Aqui voc� pode adicionar o c�digo para tratar a interrup��o
    uint8_t status = NRF24L01_CheckStatus();
    if (status & 0x40) {
        // TX FIFO vazio
    }
    if (status & 0x20) {
        // Payload recebido
    }
    if (status & 0x10) {
        // M�ximo de retransmiss�es atingido
    }

    NRF24L01_ClearInterrupts();
}
 */