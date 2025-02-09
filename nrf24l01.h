#ifndef NRF24L01_H
#define NRF24L01_H

#include <xc.h>
#include <stdint.h>
#include "defines.h"  // Inclui o arquivo defines.h

// Protótipos das funções  AS DESCRIÇÕES NÃO ESTÃO ATUALIZADAS!
void NRF24L01_Init(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_Init(void);
  
    <p><b>Resumo:</b></p>
    Inicializa o NRF24L01+, configurando os pinos SPI, a interface SPI1, a interrupção externa e os registradores do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função configura os pinos SPI, inicializa a interface SPI1, configura a interrupção externa no pino IRQNRF e inicializa os registradores do NRF24L01+ para operação normal.

    <p><b>Pré-condição:</b></p>
    O sistema deve estar configurado corretamente, e os pinos SPI devem estar definidos no arquivo defines.h.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_Init();
    </code>

    <p><b>Observações:</b></p>
    Esta função deve ser chamada antes de qualquer outra função do NRF24L01+.
*/
void NRF24L01_WriteRegister(uint8_t reg, uint8_t data);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_WriteRegister(uint8_t reg, uint8_t data);
  
    <p><b>Resumo:</b></p>
    Escreve um valor em um registrador do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função escreve um valor de 8 bits em um registrador específico do NRF24L01+ usando a interface SPI.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parámetros:</b></p>
    - reg: Endereço do registrador a ser escrito.
    - data: Valor a ser escrito no registrador.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_WriteRegister(NRF_CONFIG, 0x08);
    </code>

    <p><b>Observações:</b></p>
    O endereço do registrador deve ser válido conforme o datasheet do NRF24L01+.
*/
uint8_t NRF24L01_ReadRegister(uint8_t reg);
/**
    <p><b>Protótipo da função:</b></p>
    uint8_t NRF24L01_ReadRegister(uint8_t reg);
  
    <p><b>Resumo:</b></p>
    Lê um valor de um registrador do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função lê um valor de 8 bits de um registrador específico do NRF24L01+ usando a interface SPI.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - reg: Endereço do registrador a ser lido.

    <p><b>Retorna:</b></p>
    O valor lido do registrador.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t status = NRF24L01_ReadRegister(NRF_STATUS);
    </code>

    <p><b>Observações:</b></p>
    O endereço do registrador deve ser válido conforme o datasheet do NRF24L01+.
*/
void NRF24L01_WritePayload(int16_t *data, uint8_t length);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_WritePayload(uint8_t *data, uint8_t length);
  
    <p><b>Resumo:</b></p>
    Escreve um payload no NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função escreve um payload de até 32 bytes no NRF24L01+ para transmissão.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - data: Ponteiro para os dados a serem transmitidos.
    - length: Tamanho do payload (1 a 32 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t payload[] = {0x01, 0x02, 0x03};
    NRF24L01_WritePayload(payload, 3);
    </code>

    <p><b>Observações:</b></p>
    O tamanho do payload deve ser compatível com a configuração do NRF24L01+.
*/
void NRF24L01_ReadPayload(int16_t *data, uint8_t length);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_ReadPayload(uint8_t *data, uint8_t length);
  
    <p><b>Resumo:</b></p>
    Lê um payload do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função lê um payload de até 32 bytes do NRF24L01+ após a recepção de dados.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - data: Ponteiro para armazenar os dados recebidos.
    - length: Tamanho do payload (1 a 32 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t payload[32];
    NRF24L01_ReadPayload(payload, 32);
    </code>

    <p><b>Observações:</b></p>
    O tamanho do payload deve ser compatível com a configuração do NRF24L01+.
*/
void NRF24L01_SetTXMode(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_SetTXMode(void);
  
    <p><b>Resumo:</b></p>
    Configura o NRF24L01+ no modo de transmissão (TX).

    <p><b>Descrição:</b></p>
    Esta função configura o NRF24L01+ para operar no modo de transmissão, permitindo o envio de dados.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetTXMode();
    </code>

    <p><b>Observações:</b></p>
    O NRF24L01+ deve estar configurado corretamente para o modo TX.
*/
void NRF24L01_SetRXMode(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_SetRXMode(void);
  
    <p><b>Resumo:</b></p>
    Configura o NRF24L01+ no modo de recepção (RX).

    <p><b>Descrição:</b></p>
    Esta função configura o NRF24L01+ para operar no modo de recepção, permitindo a recepção de dados.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetRXMode();
    </code>

    <p><b>Observações:</b></p>
    O NRF24L01+ deve estar configurado corretamente para o modo RX.
*/
void NRF24L01_StandbyMode(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_StandbyMode(void);
  
    <p><b>Resumo:</b></p>
    Coloca o NRF24L01+ em modo de espera.

    <p><b>Descrição:</b></p>
    Esta função desativa o pino CE do NRF24L01+, colocando-o em modo de espera.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_StandbyMode();
    </code>

    <p><b>Observações:</b></p>
    O modo de espera reduz o consumo de energia do NRF24L01+.
*/
void NRF24L01_Sleep(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_Sleep(void);
  
    <p><b>Resumo:</b></p>
    Coloca o NRF24L01+ em modo de baixo consumo (sleep).

    <p><b>Descrição:</b></p>
    Esta função desativa o bit PWR_UP no registrador CONFIG, colocando o NRF24L01+ em modo de baixo consumo.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_Sleep();
    </code>

    <p><b>Observações:</b></p>
    O NRF24L01+ deve ser acordado usando a função NRF24L01_Wake antes de realizar novas operações.
*/
void NRF24L01_Wake(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_Wake(void);
  
    <p><b>Resumo:</b></p>
    Acorda o NRF24L01+ do modo de baixo consumo.

    <p><b>Descrição:</b></p>
    Esta função ativa o bit PWR_UP no registrador CONFIG, colocando o NRF24L01+ em modo ativo.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_Wake();
    </code>

    <p><b>Observações:</b></p>
    Após acordar, é necessário aguardar 1.5 ms para o NRF24L01+ estabilizar.
*/
void NRF24L01_SetAddress(uint8_t *address, uint8_t length);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_SetAddress(uint8_t *address, uint8_t length);
  
    <p><b>Resumo:</b></p>
    Configura o endereço de envio/recebimento do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função define o endereço de transmissão (TX_ADDR) do NRF24L01+. O endereço pode ter até 5 bytes.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - address: Ponteiro para o array de bytes que contém o endereço.
    - length: Tamanho do endereço (1 a 5 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t address[] = {0x52, 0xA0, 0xC1, 0x71, 0x1E};
    NRF24L01_SetAddress(address, 5);
    </code>

    <p><b>Observações:</b></p>
    O endereço deve ser compatível com a configuração do NRF24L01+.
*/
uint8_t NRF24L01_CheckStatus(void);
/**
    <p><b>Protótipo da função:</b></p>
    uint8_t NRF24L01_CheckStatus(void);
  
    <p><b>Resumo:</b></p>
    Verifica o status do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função lê o registrador de status do NRF24L01+ e retorna seu valor.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    O valor do registrador de status.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t status = NRF24L01_CheckStatus();
    </code>

    <p><b>Observações:</b></p>
    O registrador de status deve ser interpretado conforme o datasheet do NRF24L01+.
*/
void NRF24L01_flush_tx(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_FlushTX(void);
  
    <p><b>Resumo:</b></p>
    Limpa a FIFO de transmissão do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função limpa a FIFO de transmissão do NRF24L01+, descartando qualquer dado pendente.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_FlushTX();
    </code>

    <p><b>Observações:</b></p>
    Esta função deve ser usada para descartar dados antigos antes de enviar novos.
*/
void NRF24L01_flush_rx(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_FlushRX(void);
  
    <p><b>Resumo:</b></p>
    Limpa a FIFO de recepção do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função limpa a FIFO de recepção do NRF24L01+, descartando qualquer dado pendente.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_FlushRX();
    </code>

    <p><b>Observações:</b></p>
    Esta função deve ser usada para descartar dados antigos antes de receber novos.
*/
void NRF24L01_ClearInterrupts(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_ClearInterrupts(void);
  
    <p><b>Resumo:</b></p>
    Limpa as interrupções do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função limpa as flags de interrupção no registrador de status do NRF24L01+.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_ClearInterrupts();
    </code>

    <p><b>Observações:</b></p>
    Esta função deve ser chamada após o tratamento de uma interrupção.
*/
void nrf24l01_nop(void);
void NRF24L01_reuse_tx(void);
void NRF24L01_pulse(void);
uint8_t NRF24L01_read_rx_width(void);
void NRF24L01_write_ack(uint8_t pipe, uint8_t *data, uint8_t len);
void NRF24L01_write_tx_no_ack(uint8_t *data, uint8_t len);

/*
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void);
/ **
    <p><b>Protótipo da função:</b></p>
    void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void);
  
    <p><b>Resumo:</b></p>
    Função de interrupção externa para o pino IRQNRF.

    <p><b>Descrição:</b></p>
    Esta função é chamada quando ocorre uma interrupção no pino IRQNRF. Ela verifica o status do NRF24L01+ e trata as interrupções.

    <p><b>Pré-condição:</b></p>
    A interrupção externa deve estar configurada corretamente.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    // Configuração da interrupção no código principal
    </code>

    <p><b>Observações:</b></p>
    Esta função deve ser associada à interrupção externa no código principal.
* /
*/

#endif // NRF24L01_H
