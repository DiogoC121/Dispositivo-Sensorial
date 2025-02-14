/*
 * Arquivo.c para utilização dos módulos NRF24L01+
 * Autor: Diogo Correia da Silva
 
 */
 #include "nrf24l01.h"

 // Variáveis globais
 extern bool flag_timer;             // Flag interrupção timer
 extern bool flag_nrf;               // Flag interrupção nrf
 extern bool enviar_dados;           // Flag para enviar dados
 extern bool dados_recebidos;        // Flag para dados recebidos   
 extern int16_t receivedCommand;     // Comando recebido pelo NRF24L01+
 //extern const uint8_t addressnrf[5];
 
 // Função para inicializar o NRF24L01+
 void NRF24L01_Init(void) {
     
     // Configuração inicial do NRF24L01+
     NRF24L01_WriteRegister(NRF_CONFIG, 0x08);  // Desabilitar CRC, Power Down, RX Mode
     NRF24L01_WriteRegister(NRF_EN_AA, 0x00);   // Desabilitar Auto Acknowledgment
     NRF24L01_WriteRegister(NRF_EN_RXADDR, 0x01); // Habilitar apenas o pipe 0
     NRF24L01_WriteRegister(NRF_SETUP_AW, 0x03);  // Endereço de 5 bytes
     NRF24L01_WriteRegister(NRF_SETUP_RETR, 0x00); // Desabilitar retransmissão ----------------------------------------------
     NRF24L01_WriteRegister(NRF_RF_CH, 0x02);    // Canal RF 2
     NRF24L01_WriteRegister(NRF_RF_SETUP, 0x07); // Potência máxima (0 dBm), taxa de 1 Mbps
     NRF24L01_WriteRegister(NRF_STATUS, 0x70);   // Limpar flags de status
     NRF24L01_WriteRegister(NRF_RX_PW_P0, 0x20); // Tamanho do payload de 32 bytes no pipe 0
     NRF24L01_WriteRegister(NRF_FEATURE, 0x00);  // Desabilitar features extras
     NRF24L01_WriteRegister(NRF_DYNPD, 0x00);    // Desabilitar payload dinâmico
     NRF24L01_WriteRegister(NRF_CONFIG, 0x0A);   // Habilitar CRC de 2 bytes, Power Up, RX Mode
 
     NRF24L01_SetAddressWidth(5); // Endereço de 5 bytes
     NRF24L01_SetPayloadSize(30); // Tamanho do payload para o buffer de 30 bytes
     NRF24L01_SetCrcLength(0x01); // CRC de 2 bytes
     NRF24L01_OpenWritingPipe(ADDRESS);
     NRF24L01_OpenReadingPipe(1, ADDRESS);
     NRF24L01_SetRXMode(); // Inicia no modo RX
 }
 
 // Função para escrever em um registrador do NRF24L01+
 void NRF24L01_WriteRegister(uint8_t reg, uint8_t data) {
     
    CENRF_SetLow();                                                                       // Set CSN low to enable the chip select
    
    
    SSMPU_SetHigh();
    SSNRF_SetLow();    
 
    //uint8_t datis = data;
    SPI1_Exchange8bit(NRF_W_REGISTER | reg);
    while (!SPI1STATbits.SPIRBF);                    // Esperar o buffer de recepção estar cheio
    SPI1_Exchange8bit(data);
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
 
    SSNRF_SetHigh(); 
 }
 
 // Função para ler um registrador do NRF24L01+
 uint8_t NRF24L01_ReadRegister(uint8_t reg) {
    CENRF_SetLow();                                      
    uint8_t received;
  
    SSMPU_SetHigh();
    SSNRF_SetLow();         
    
    SPI1_Exchange8bit(NRF_R_REGISTER | reg);
    while (!SPI1STATbits.SPIRBF);                    // Esperar o buffer de recepção estar cheio
    received = SPI1_Exchange8bit(0xFF);                  // Read data from the buffer
    while (!SPI1STATbits.SPIRBF);                    // Esperar o buffer de recepção estar cheio
 
    SSNRF_SetHigh();                                      
 
    return received;
 }
 
 // Função para escrever um payload no NRF24L01+
 void NRF24L01_WritePayload(int16_t *data, uint8_t length) {
     LED_SetHigh();
     NRF24L01_WriteRegister(NRF_STATUS, 0x70);
     NRF24L01_SetTXMode();  // Ativar modo TX
     NRF24L01_flush_tx();
     SSNRF_SetLow();                                                                              // Set CSN low to enable the chip select
 
     SPI1_Exchange8bit(NRF_W_TX_PAYLOAD);
     //SPI1BUF = NRF_W_TX_PAYLOAD;  // Enviar comando de escrita de payload
     while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
     for (uint8_t i = 0; i < length; i++) {
         SPI1_Exchange8bit(data[i]);
         //SPI1BUF = data[i];  // Enviar dado
         while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
     }
 
     SSNRF_SetHigh();   // Deselecionar o NRF24L01+ (CS alto)
     NRF24L01_pulse();
     __delay_ms(10);
     NRF24L01_flush_tx();
     NRF24L01_SetRXMode();
     
     //NRF24L01_StandbyMode(); // Desativar o NRF24L01+ após a transmissão
     LED_SetLow();
 
 }
 
 // Função para ler um payload do NRF24L01+
 void NRF24L01_ReadPayload(int16_t *data, uint8_t length) {
     LED_SetHigh();
     NRF24L01_SetRXMode();  // Ativar modo RX
     NRF24L01_flush_rx();
     SSNRF_SetLow();                                                                              // Set CSN low to enable the chip select
 
     SPI1_Exchange8bit(NRF_R_RX_PAYLOAD);
     //SPI1BUF = NRF_R_RX_PAYLOAD;  // Enviar comando de leitura de payload
     while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
     for (uint8_t i = 0; i < length; i++) {
         data[i] = SPI1_Exchange8bit(0xFF);
         //SPI1BUF = 0xFF;  // Enviar dummy byte para receber o dado
         while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
         //data[i] = SPI1BUF;  // Ler dado
     }
 
     SSNRF_SetHigh();   // Deselecionar o NRF24L01+ (CS alto)
     NRF24L01_flush_rx();
     //NRF24L01_StandbyMode(); // Desativar o NRF24L01+ após a recepção
     LED_SetLow();
 }
 
 // Função para configurar o NRF24L01+ no modo TX
 void NRF24L01_SetTXMode(void) {
     uint32_t read_data;
    uint8_t reg;
    
    read_data = NRF24L01_ReadRegister(NRF_CONFIG);
    reg = (uint8_t)(read_data >> 8);
    reg |= ~(1 << 0);                                                                                // Set the PRIM_RX bit
    NRF24L01_WriteRegister(NRF_CONFIG, reg);
    
    CENRF_SetHigh();
    __delay_us(15);       // Aguardar 15 µs (tempo mínimo para entrar no modo TX)
 
 }
 
 // Função para configurar o NRF24L01+ no modo RX
 void NRF24L01_SetRXMode(void) {
    uint32_t read_data;
    uint8_t reg;
    
    read_data = NRF24L01_ReadRegister(NRF_CONFIG);
    reg = (uint8_t)(read_data >> 8);
    reg |= (1 << 0);                                                                                // Set the PRIM_RX bit
    NRF24L01_WriteRegister(NRF_CONFIG, reg);
    __delay_us(130);      // Aguardar 130 µs (tempo mínimo para entrar no modo RX)
 
     uint8_t config = NRF24L01_ReadRegister(NRF_CONFIG);
     config |= 0x01;  // Setar bit PRIM_RX para modo RX
     NRF24L01_WriteRegister(NRF_CONFIG, config);
    
    CENRF_SetHigh();
 }
 
 // Função para colocar o NRF24L01+ em modo de espera
 void NRF24L01_StandbyMode(void) {
     CENRF_SetLow();  // Desativar o NRF24L01+ (CE baixo)
 }
 
 // Função para colocar o NRF24L01+ em modo de baixo consumo (sleep)
 void NRF24L01_Sleep(void) {
    uint32_t read_data;
    uint8_t reg;
    
    read_data = NRF24L01_ReadRegister(NRF_CONFIG);
    reg = (uint8_t)(read_data >> 8);
    reg &= ~(1 << 1);                                                                               // Clear the PWR_UP bit
    NRF24L01_WriteRegister(NRF_CONFIG, reg);
 }
 
 // Função para acordar o NRF24L01+ do modo de baixo consumo
 void NRF24L01_Wake(void) {
     uint32_t read_data;
    uint8_t reg;
    
    read_data = NRF24L01_ReadRegister(NRF_CONFIG);
    reg = (uint8_t)(read_data >> 8);
    reg |= (1 << 1);                                                                                // Set the PWR_UP bit
    NRF24L01_WriteRegister(NRF_CONFIG, reg);
    __delay_us(1500);                                                                                 // Wait for the power-up time
 
    NRF24L01_SetRXMode();
 }
 
 // Função para configurar o endereço de envio/recebimento
 void NRF24L01_SetAddress(uint8_t *address, uint8_t length) {
    SSNRF_SetLow();                                    
 
     SPI1_Exchange8bit(NRF_W_REGISTER | NRF_TX_ADDR);  // Enviar comando de escrita para o registrador TX_ADDR
     while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
     for (uint8_t i = 0; i < length; i++) {
         SPI1_Exchange8bit(address[i]);  // Enviar byte do endereço
         while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
     }
 
    SSNRF_SetHigh();
 }
 
 // Função para verificar o status do NRF24L01+
 uint8_t NRF24L01_CheckStatus(void) {
     return NRF24L01_ReadRegister(NRF_STATUS);
 }
 
 // Função para o comando NOP do NRF24L01+
 void nrf24l01_nop(void)
 {  
    SSNRF_SetLow();                                    
    
    SPI1_Exchange8bit(NRF_NOP);                                                                // Send the NOP command                                    
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
 
    SSNRF_SetHigh();
 } /* end nrf24l01_nop */
 
 // Função para o comando REUSE TX do NRF24L01+
 void NRF24L01_reuse_tx(void)
 {
    SSNRF_SetLow();                                    
    
    SPI1_Exchange8bit(NRF_REUSE_TX_PL);                                                        // Send the REUSE_TX_PL command                                  
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    
    SSNRF_SetLow();
 } /* end nrf24l01_reuse_tx */
 
 // Função para DAR UM PULSO ALTO de 1ms NO CE DO NRF24L01+
 void NRF24L01_pulse(void)
 {
     CENRF_SetHigh();                                      
     __delay_ms(1);
     CENRF_SetLow();                                      
                                                                             // Seta CE para nÃ­vel lÃ³gico baixo para por o chip em modo standby
 } /* end nrf24l01_pulse */
 
 // Função de FLUSH TX do NRF24L01+
 void NRF24L01_flush_tx(void)
 {
    SSNRF_SetLow();                                    
    
    SPI1_Exchange8bit(NRF_FLUSH_TX);                                                           // Send the FLUSH_TX command 
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
 
    SSNRF_SetHigh();
 } /* end nrf24l01_flush_tx */
 
 // Função de FLUSH RX do NRF24L01+
 void NRF24L01_flush_rx(void)
 {  
    SSNRF_SetLow();                                    
    
    SPI1_Exchange8bit(NRF_FLUSH_RX);                                                           // Send the FLUSH_RX command 
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
 
    SSNRF_SetHigh();
 } /* end nrf24l01_flush_rx */
 
 // Função para limpar as interrupções
 void NRF24L01_ClearInterrupts(void) {
     uint8_t status = NRF24L01_ReadRegister(NRF_STATUS);
     NRF24L01_WriteRegister(NRF_STATUS, status | 0x70);  // Limpar flags de interrupção
 }
 
 // Função para o comando READ TX WIDTH do NRF24L01+
 uint8_t NRF24L01_read_rx_width(void)
 {
    uint8_t width = 0x00;
    SSNRF_SetLow();                                    
    
    width = SPI1_Exchange8bit(NRF_R_RX_PL_WID);                                                // Send the R_RX_PL_WID command                                 
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
 
    SSNRF_SetHigh();
    
    return width;
 } /* end nrf24l01_read_rx_width */
 
 // Função para o comando WRITE ACK do NRF24L01+
 void NRF24L01_write_ack(uint8_t pipe, uint8_t *data, uint8_t len)
 {
    SSNRF_SetLow();                                    
    
    SPI1_Exchange8bit((NRF_W_ACK_PAYLOAD | pipe));                                             // Send the W_ACK_PAYLOAD command                                   
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    while (len--)
    {
     SPI1_Exchange8bit(*data++); 
     while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
     // Send the data to write to the FIFO
    }
    
    SSNRF_SetHigh();
 } /* end nrf24l01_write_ack */
 
 // Função para o comando WRITE NO SCK do NRF24L01+
 void NRF24L01_write_tx_no_ack(uint8_t *data, uint8_t len)
 {
    SSNRF_SetLow();                                    
    
    SPI1_Exchange8bit(NRF_W_TX_PAYLOAD_NO_ACK);                                                // Send the W_TX_PAYLOAD_NO_ACK command                                    
    while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
    while (len--)
    {
       SPI1_Exchange8bit(*data++);
       while (!SPI1STATbits.SPIRBF);  // Esperar o buffer de recepção estar cheio
       // Send the data to write to the FIFO
    }
    
    SSNRF_SetHigh();
 } /* end nrf24l01_write_tx_no_ack */
 
 // Função para configurar o canal RF
 void NRF24L01_SetRFChannel(uint8_t channel) {
     if (channel > 125) channel = 125; // O canal deve estar entre 0 e 125
     NRF24L01_WriteRegister(NRF_RF_CH, channel);
 }
 
 // Função para configurar a taxa de dados
 void NRF24L01_SetDataRate(uint8_t dataRate) {
     uint8_t rfSetup = NRF24L01_ReadRegister(NRF_RF_SETUP);
     rfSetup &= ~0x28; // Limpa os bits de taxa de dados
     if (dataRate == 250) {
         rfSetup |= 0x20; // 250 kbps
     } else if (dataRate == 2000) {
         rfSetup |= 0x08; // 2 Mbps
     } else {
         rfSetup |= 0x00; // 1 Mbps (default)
     }
     NRF24L01_WriteRegister(NRF_RF_SETUP, rfSetup);
 }
 
 // Função para configurar a potência de transmissão
 void NRF24L01_SetTXPower(uint8_t power) {
     uint8_t rfSetup = NRF24L01_ReadRegister(NRF_RF_SETUP);
     rfSetup &= ~0x06; // Limpa os bits de potência
     rfSetup |= (power << 1); // Configura a potência
     NRF24L01_WriteRegister(NRF_RF_SETUP, rfSetup);
 }
 
 // Função para ler o valor de RSSI
 uint8_t NRF24L01_ReadRSSI(void) {
     return NRF24L01_ReadRegister(NRF_RPD); // Supondo que o registrador de RSSI seja NRF_RPD
 }
 
 void NRF24L01_SetAddressWidth(uint8_t width) {
     NRF24L01_WriteRegister(0x01, width - 2); // Configura a largura do endereço (3 a 5 bytes)
 }
 
 void NRF24L01_SetPayloadSize(uint8_t size) {
     NRF24L01_WriteRegister(0x11, size); // Configura o tamanho do payload (30 bytes)
 }
 
 void NRF24L01_SetCrcLength(uint8_t crc_length) {
     uint8_t config = NRF24L01_ReadRegister(0x00); // Lê o valor atual do registrador CONFIG
     config |= (crc_length << 2); // Define o comprimento do CRC
     NRF24L01_WriteRegister(0x00, config); // Escreve o novo valor no registrador CONFIG
 }
 
 void NRF24L01_OpenWritingPipe(uint64_t address) {
     SSNRF_SetLow();                                                                              // Set CSN low to enable the chip select
     SPI1_Exchange8bit(0x20 | 0x0A); // Comando W_REGISTER + registrador TX_ADDR
     for (uint8_t i = 0; i < 5; i++) {
         SPI1_Exchange8bit((address >> (8 * i)) & 0xFF); // Escreve o endereço byte a byte
     }
     SSNRF_SetHigh();   // Deselecionar o NRF24L01+ (CS alto)
 }
 
 void NRF24L01_OpenReadingPipe(uint8_t pipe, uint64_t address) {
     SSNRF_SetLow();                                                                              // Set CSN low to enable the chip select
     SPI1_Exchange8bit(0x20 | (0x0A + pipe)); // Comando W_REGISTER + registrador RX_ADDR_P0-P5
     for (uint8_t i = 0; i < 5; i++) {
         SPI1_Exchange8bit((address >> (8 * i)) & 0xFF); // Escreve o endereço byte a byte
     }
     SSNRF_SetHigh();   // Deselecionar o NRF24L01+ (CS alto)
 }
 
 bool NRF24L01_tx_finished() {
     uint8_t status = NRF24L01_ReadRegister(0x07); // Lê o registrador STATUS
     return (status & 0x20); // Retorna o bit TX_DS (transmissão concluída)
 }
 
 bool NRF24L01_rx_fifo_not_empty() {
     uint8_t status = NRF24L01_ReadRegister(0x07); // Lê o registrador STATUS
     return (status & 0x40); // Retorna o bit RX_DR (dados recebidos)
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