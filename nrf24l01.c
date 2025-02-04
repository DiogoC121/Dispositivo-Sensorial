#include "nrf24l01.h"

// Variáveis globais
extern bool flag_timer;             // Flag interrupção timer
extern bool flag_mpu;               // Flag interrupção mpu
extern bool flag_nrf;               // Flag interrupção nrf
extern bool enviar_dados;           // Flag para enviar dados
extern bool dados_recebidos;        // Flag para dados recebidos   
extern uint8_t receivedCommand;     // Comando recebido pelo NRF24L01+

// Função para inicializar o NRF24L01+
void NRF24L01_Init(void) {
    
    /*
    // Configuração dos pinos SPI
    TRISBbits.TRISB0 = 0;  // SCK como saída
    TRISBbits.TRISB1 = 1;  // SDO (MISO) como entrada
    TRISBbits.TRISB2 = 0;  // SDI (MOSI) como saída
    TRISBbits.TRISB11 = 0; // SSNRF como saída
    TRISBbits.TRISB12 = 0; // CENRF como saída

    // Configuração do SPI1
    SPI1CON1bits.MSTEN = 1;        // Modo Master
    SPI1CON1bits.CKP = 0;          // Clock idle em low
    SPI1CON1bits.CKE = 1;          // Transição de clock ativa na borda de subida
    SPI1CON1bits.SMP = 0;          // Dados amostrados no meio do tempo de clock
    SPI1CON1bits.MODE16 = 0;       // Modo 8-bit
    SPI1CON1bits.PPRE = 3;         // Prescaler primário 1:1
    SPI1CON1bits.SPRE = 6;         // Prescaler secundário 2:1
    SPI1STATbits.SPIEN = 1;        // Habilitar SPI1

    // Configuração da interrupção externa IRQNRF
    TRISBbits.TRISB7 = 1;          // IRQNRF como entrada
    INTCON2bits.INT2EP = 0;        // Interrupção na borda de subida
    IFS1bits.INT2IF = 0;           // Limpar flag de interrupção
    IEC1bits.INT2IE = 1;           // Habilitar interrupção externa INT2
    */
    
    // Configuração inicial do NRF24L01+
    NRF24L01_WriteRegister(NRF_CONFIG, 0x08);  // Desabilitar CRC, Power Down, RX Mode
    NRF24L01_WriteRegister(NRF_EN_AA, 0x00);   // Desabilitar Auto Acknowledgment
    NRF24L01_WriteRegister(NRF_EN_RXADDR, 0x01); // Habilitar apenas o pipe 0
    NRF24L01_WriteRegister(NRF_SETUP_AW, 0x03);  // Endereço de 5 bytes
    NRF24L01_WriteRegister(NRF_SETUP_RETR, 0x00); // Desabilitar retransmissão
    NRF24L01_WriteRegister(NRF_RF_CH, 0x02);    // Canal RF 2
    NRF24L01_WriteRegister(NRF_RF_SETUP, 0x07); // Potência máxima (0 dBm), taxa de 1 Mbps
    NRF24L01_WriteRegister(NRF_STATUS, 0x70);   // Limpar flags de status
    NRF24L01_WriteRegister(NRF_RX_PW_P0, 0x20); // Tamanho do payload de 32 bytes no pipe 0
    NRF24L01_WriteRegister(NRF_FEATURE, 0x00);  // Desabilitar features extras
    NRF24L01_WriteRegister(NRF_DYNPD, 0x00);    // Desabilitar payload dinâmico
    NRF24L01_WriteRegister(NRF_CONFIG, 0x0A);   // Habilitar CRC de 2 bytes, Power Up, RX Mode
}

// Função para escrever em um registrador do NRF24L01+
void NRF24L01_WriteRegister(uint8_t reg, uint8_t data) {
    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_W_REGISTER | (reg & 0x1F);  // Enviar comando de escrita e endereço do registrador
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    SPI1BUF = data;  // Enviar dado
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)
}

// Função para ler um registrador do NRF24L01+
uint8_t NRF24L01_ReadRegister(uint8_t reg) {
    uint8_t data;

    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_R_REGISTER | (reg & 0x1F);  // Enviar comando de leitura e endereço do registrador
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    SPI1BUF = 0xFF;  // Enviar dummy byte para receber o dado
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    data = SPI1BUF;  // Ler dado

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)

    return data;
}

// Função para escrever um payload no NRF24L01+
void NRF24L01_WritePayload(uint8_t *data, uint8_t length) {
    NRF24L01_SetTXMode();  // Ativar modo TX
    LATBbits.LATB11 = 0;   // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_W_TX_PAYLOAD;  // Enviar comando de escrita de payload
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    for (uint8_t i = 0; i < length; i++) {
        SPI1BUF = data[i];  // Enviar dado
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    }

    LATBbits.LATB11 = 1;   // Deselecionar o NRF24L01+ (CS alto)
    NRF24L01_StandbyMode(); // Desativar o NRF24L01+ após a transmissão
}

// Função para ler um payload do NRF24L01+
void NRF24L01_ReadPayload(uint8_t *data, uint8_t length) {
    NRF24L01_SetRXMode();  // Ativar modo RX
    LATBbits.LATB11 = 0;   // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_R_RX_PAYLOAD;  // Enviar comando de leitura de payload
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    for (uint8_t i = 0; i < length; i++) {
        SPI1BUF = 0xFF;  // Enviar dummy byte para receber o dado
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
        data[i] = SPI1BUF;  // Ler dado
    }

    LATBbits.LATB11 = 1;   // Deselecionar o NRF24L01+ (CS alto)
    NRF24L01_StandbyMode(); // Desativar o NRF24L01+ após a recepção
}

// Função para configurar o NRF24L01+ no modo TX
void NRF24L01_SetTXMode(void) {
    uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
    config &= ~0x01;  // Limpar bit PRIM_RX para modo TX
    NRF24L01_WriteRegister(NRF_CONFIG, config);
    LATBbits.LATB12 = 1;  // Habilitar o NRF24L01+ (CE alto)
    __delay_us(15);       // Aguardar 15 µs (tempo mínimo para entrar no modo TX)
}

// Função para configurar o NRF24L01+ no modo RX
void NRF24L01_SetRXMode(void) {
    uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
    config |= 0x01;  // Setar bit PRIM_RX para modo RX
    NRF24L01_WriteRegister(NRF_CONFIG, config);
    LATBbits.LATB12 = 1;  // Habilitar o NRF24L01+ (CE alto)
    __delay_us(130);      // Aguardar 130 µs (tempo mínimo para entrar no modo RX)
}

// Função para colocar o NRF24L01+ em modo de espera
void NRF24L01_StandbyMode(void) {
    LATBbits.LATB12 = 0;  // Desativar o NRF24L01+ (CE baixo)
}

// Função para colocar o NRF24L01+ em modo de baixo consumo (sleep)
void NRF24L01_Sleep(void) {
    uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
    config &= ~0x02;  // Desligar o bit PWR_UP
    NRF24L01_WriteRegister(NRF_CONFIG, config);
}

// Função para acordar o NRF24L01+ do modo de baixo consumo
void NRF24L01_Wake(void) {
    uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
    config |= 0x02;  // Ligar o bit PWR_UP
    NRF24L01_WriteRegister(NRF_CONFIG, config);
    __delay_us(1500); // Aguardar 1.5 ms para o NRF24L01+ estabilizar
}

// Função para configurar o endereço de envio/recebimento
void NRF24L01_SetAddress(uint8_t *address, uint8_t length) {
    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_W_REGISTER | NRF_TX_ADDR;  // Enviar comando de escrita para o registrador TX_ADDR
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    for (uint8_t i = 0; i < length; i++) {
        SPI1BUF = address[i];  // Enviar byte do endereço
        while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    }

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)
}

// Função para verificar o status do NRF24L01+
uint8_t NRF24L01_CheckStatus(void) {
    return NRF24L01_ReadRegister(NRF_STATUS);
}

// Função para limpar a FIFO de TX
void NRF24L01_FlushTX(void) {
    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_FLUSH_TX;  // Enviar comando para limpar FIFO de TX
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)
}

// Função para limpar a FIFO de RX
void NRF24L01_FlushRX(void) {
    LATBbits.LATB11 = 0;  // Selecionar o NRF24L01+ (CS baixo)

    SPI1BUF = NRF_FLUSH_RX;  // Enviar comando para limpar FIFO de RX
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio

    LATBbits.LATB11 = 1;  // Deselecionar o NRF24L01+ (CS alto)
}

// Função para limpar as interrupções
void NRF24L01_ClearInterrupts(void) {
    uint8_t status = NRF24L01_ReadRegister(NRF_STATUS);
    NRF24L01_WriteRegister(NRF_STATUS, status | 0x70);  // Limpar flags de interrupção
}

/*
// Função de interrupção externa para o pino IRQNRF
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void) {
    IFS1bits.INT2IF = 0;  // Limpar flag de interrupção

    // Aqui você pode adicionar o código para tratar a interrupção
    uint8_t status = NRF24L01_CheckStatus();
    if (status & 0x40) {
        // TX FIFO vazio
    }
    if (status & 0x20) {
        // Payload recebido
    }
    if (status & 0x10) {
        // Máximo de retransmissões atingido
    }

    NRF24L01_ClearInterrupts();
}
 */