#ifndef NRF24L01_H
#define NRF24L01_H

/*
 * Arquivo.h para utilização dos módulos NRF24L01+
 * Autor: Diogo Correia da Silva
 
 */

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
/**
    <p><b>Protótipo da função:</b></p>
    void nrf24l01_nop(void);
  
    <p><b>Resumo:</b></p>
    Executa uma operação NOP (No Operation) no NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função envia um comando NOP ao módulo NRF24L01+, que não realiza nenhuma operação, mas pode ser usada para ler o registrador de status.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    nrf24l01_nop(); // Executa uma operação NOP
    </code>

    <p><b>Observações:</b></p>
    O comando NOP é útil para ler o registrador de status sem alterar o estado do módulo.
*/
void NRF24L01_reuse_tx(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_reuse_tx(void);
  
    <p><b>Resumo:</b></p>
    Reutiliza o último payload transmitido no NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função permite reutilizar o último payload transmitido, sem a necessidade de reescrevê-lo na FIFO de transmissão.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_reuse_tx(); // Reutiliza o último payload transmitido
    </code>

    <p><b>Observações:</b></p>
    Esta função é útil para retransmissões rápidas do mesmo payload.
*/
void NRF24L01_pulse(void);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_pulse(void);
  
    <p><b>Resumo:</b></p>
    Gera um pulso no pino CE do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função gera um pulso alto no pino CE do módulo NRF24L01+, necessário para iniciar a transmissão ou recepção de dados.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_pulse(); // Gera um pulso no pino CE
    </code>

    <p><b>Observações:</b></p>
    O pulso no pino CE é essencial para iniciar a comunicação no modo TX ou RX.
*/
uint8_t NRF24L01_read_rx_width(void);
/**
    <p><b>Protótipo da função:</b></p>
    uint8_t NRF24L01_read_rx_width(void);
  
    <p><b>Resumo:</b></p>
    Lê o tamanho do payload recebido no NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função lê o tamanho do payload recebido na FIFO de recepção do módulo NRF24L01+.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    O tamanho do payload recebido (em bytes).

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t payloadSize = NRF24L01_read_rx_width(); // Lê o tamanho do payload recebido
    </code>

    <p><b>Observações:</b></p>
    O tamanho do payload deve ser compatível com a configuração do módulo.
*/
void NRF24L01_write_ack(uint8_t pipe, uint8_t *data, uint8_t len);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_write_ack(uint8_t pipe, uint8_t *data, uint8_t len);
  
    <p><b>Resumo:</b></p>
    Escreve um payload de ACK no NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função escreve um payload de ACK no pipe especificado do módulo NRF24L01+, que será enviado automaticamente como resposta a um pacote recebido.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - pipe: Pipe no qual o payload de ACK será escrito (0 a 5).
    - data: Ponteiro para os dados a serem enviados como ACK.
    - len: Tamanho do payload de ACK (1 a 32 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t ackData[] = {0x01, 0x02, 0x03};
    NRF24L01_write_ack(0, ackData, 3); // Escreve um payload de ACK no pipe 0
    </code>

    <p><b>Observações:</b></p>
    O payload de ACK é enviado automaticamente após a recepção de um pacote no pipe especificado.
*/
void NRF24L01_write_tx_no_ack(uint8_t *data, uint8_t len);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_write_tx_no_ack(uint8_t *data, uint8_t len);
  
    <p><b>Resumo:</b></p>
    Escreve um payload no NRF24L01+ sem esperar por ACK.

    <p><b>Descrição:</b></p>
    Esta função escreve um payload na FIFO de transmissão do módulo NRF24L01+ sem esperar por uma confirmação (ACK) do receptor.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - data: Ponteiro para os dados a serem transmitidos.
    - len: Tamanho do payload (1 a 32 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t payload[] = {0x01, 0x02, 0x03};
    NRF24L01_write_tx_no_ack(payload, 3); // Transmite um payload sem esperar por ACK
    </code>

    <p><b>Observações:</b></p>
    Esta função é útil para transmissões rápidas onde a confirmação não é necessária.
*/
void NRF24L01_SetRFChannel(uint8_t channel);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_SetRFChannel(uint8_t channel);
  
    <p><b>Resumo:</b></p>
    Configura o canal RF do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função define o canal de frequência RF no qual o NRF24L01+ irá operar. O canal pode variar de 0 a 125, correspondendo a frequências de 2.400 GHz a 2.525 GHz.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - channel: Canal RF a ser configurado (0 a 125).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetRFChannel(76); // Configura o canal RF para 76 (2.476 GHz)
    </code>

    <p><b>Observações:</b></p>
    O canal deve estar dentro do intervalo permitido (0 a 125). Caso contrário, será ajustado para o valor máximo (125).
*/
void NRF24L01_SetDataRate(uint8_t dataRate);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_SetDataRate(uint8_t dataRate);
  
    <p><b>Resumo:</b></p>
    Configura a taxa de dados do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função define a taxa de transmissão de dados do NRF24L01+. As opções são 250 kbps, 1 Mbps (padrão) ou 2 Mbps.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - dataRate: Taxa de dados desejada (250, 1000 ou 2000 kbps).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetDataRate(2000); // Configura a taxa de dados para 2 Mbps
    </code>

    <p><b>Observações:</b></p>
    A taxa de dados afeta o alcance e a robustez da comunicação. Taxas mais baixas (250 kbps) proporcionam maior alcance, enquanto taxas mais altas (2 Mbps) permitem maior velocidade de transmissão.
*/
void NRF24L01_SetTXPower(uint8_t power);
/**
    <p><b>Protótipo da função:</b></p>
    void NRF24L01_SetTXPower(uint8_t power);
  
    <p><b>Resumo:</b></p>
    Configura a potência de transmissão do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função define a potência de transmissão do NRF24L01+. As opções são 0 dBm (máxima potência), -6 dBm, -12 dBm ou -18 dBm (mínima potência).

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - power: Potência de transmissão desejada (0, 1, 2 ou 3, correspondendo a 0 dBm, -6 dBm, -12 dBm e -18 dBm, respectivamente).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetTXPower(0); // Configura a potência de transmissão para 0 dBm
    </code>

    <p><b>Observações:</b></p>
    A potência de transmissão afeta o consumo de energia e o alcance da comunicação. Potências mais altas aumentam o alcance, mas também o consumo de energia.
*/
uint8_t NRF24L01_ReadRSSI(void);
/**
    <p><b>Protótipo da função:</b></p>
    uint8_t NRF24L01_ReadRSSI(void);
  
    <p><b>Resumo:</b></p>
    Lê o valor de RSSI (Received Signal Strength Indicator) do NRF24L01+.

    <p><b>Descrição:</b></p>
    Esta função lê o valor de RSSI, que indica a força do sinal recebido pelo módulo NRF24L01+.

    <p><b>Pré-condição:</b></p>
    A função NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    O valor de RSSI lido.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t rssi = NRF24L01_ReadRSSI(); // Lê o valor de RSSI
    </code>

    <p><b>Observações:</b></p>
    O valor de RSSI pode ser usado para avaliar a qualidade do sinal recebido. Valores mais altos indicam um sinal mais forte.
*/

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
