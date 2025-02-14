#ifndef NRF24L01_H
#define NRF24L01_H

/*
 * Arquivo.h para utiliza��o dos m�dulos NRF24L01+
 * Autor: Diogo Correia da Silva
 
 */

#include <xc.h>
#include <stdint.h>
#include "defines.h"  // Inclui o arquivo defines.h
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/clock.h"
#include "mcc_generated_files/ext_int.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/traps.h"
#include "mcc_generated_files/tmr1.h"

// Prot�tipos das fun��es  AS DESCRI��ES N�O EST�O ATUALIZADAS!
void NRF24L01_Init(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_Init(void);
  
    <p><b>Resumo:</b></p>
    Inicializa o NRF24L01+, configurando os pinos SPI, a interface SPI1, a interrup��o externa e os registradores do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o configura os pinos SPI, inicializa a interface SPI1, configura a interrup��o externa no pino IRQNRF e inicializa os registradores do NRF24L01+ para opera��o normal.

    <p><b>Pr�-condi��o:</b></p>
    O sistema deve estar configurado corretamente, e os pinos SPI devem estar definidos no arquivo defines.h.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_Init();
    </code>

    <p><b>Observa��es:</b></p>
    Esta fun��o deve ser chamada antes de qualquer outra fun��o do NRF24L01+.
*/
void NRF24L01_WriteRegister(uint8_t reg, uint8_t data);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_WriteRegister(uint8_t reg, uint8_t data);
  
    <p><b>Resumo:</b></p>
    Escreve um valor em um registrador do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o escreve um valor de 8 bits em um registrador espec�fico do NRF24L01+ usando a interface SPI.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - reg: Endere�o do registrador a ser escrito.
    - data: Valor a ser escrito no registrador.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_WriteRegister(NRF_CONFIG, 0x08);
    </code>

    <p><b>Observa��es:</b></p>
    O endere�o do registrador deve ser v�lido conforme o datasheet do NRF24L01+.
*/
uint8_t NRF24L01_ReadRegister(uint8_t reg);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    uint8_t NRF24L01_ReadRegister(uint8_t reg);
  
    <p><b>Resumo:</b></p>
    L� um valor de um registrador do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o l� um valor de 8 bits de um registrador espec�fico do NRF24L01+ usando a interface SPI.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - reg: Endere�o do registrador a ser lido.

    <p><b>Retorna:</b></p>
    O valor lido do registrador.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t status = NRF24L01_ReadRegister(NRF_STATUS);
    </code>

    <p><b>Observa��es:</b></p>
    O endere�o do registrador deve ser v�lido conforme o datasheet do NRF24L01+.
*/
void NRF24L01_WritePayload(int16_t *data, uint8_t length);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_WritePayload(uint8_t *data, uint8_t length);
  
    <p><b>Resumo:</b></p>
    Escreve um payload no NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o escreve um payload de at� 32 bytes no NRF24L01+ para transmiss�o.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - data: Ponteiro para os dados a serem transmitidos.
    - length: Tamanho do payload (1 a 32 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t payload[] = {0x01, 0x02, 0x03};
    NRF24L01_WritePayload(payload, 3);
    </code>

    <p><b>Observa��es:</b></p>
    O tamanho do payload deve ser compat�vel com a configura��o do NRF24L01+.
*/
void NRF24L01_ReadPayload(int16_t *data, uint8_t length);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_ReadPayload(uint8_t *data, uint8_t length);
  
    <p><b>Resumo:</b></p>
    L� um payload do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o l� um payload de at� 32 bytes do NRF24L01+ ap�s a recep��o de dados.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - data: Ponteiro para armazenar os dados recebidos.
    - length: Tamanho do payload (1 a 32 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t payload[32];
    NRF24L01_ReadPayload(payload, 32);
    </code>

    <p><b>Observa��es:</b></p>
    O tamanho do payload deve ser compat�vel com a configura��o do NRF24L01+.
*/
void NRF24L01_SetTXMode(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_SetTXMode(void);
  
    <p><b>Resumo:</b></p>
    Configura o NRF24L01+ no modo de transmiss�o (TX).

    <p><b>Descri��o:</b></p>
    Esta fun��o configura o NRF24L01+ para operar no modo de transmiss�o, permitindo o envio de dados.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetTXMode();
    </code>

    <p><b>Observa��es:</b></p>
    O NRF24L01+ deve estar configurado corretamente para o modo TX.
*/
void NRF24L01_SetRXMode(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_SetRXMode(void);
  
    <p><b>Resumo:</b></p>
    Configura o NRF24L01+ no modo de recep��o (RX).

    <p><b>Descri��o:</b></p>
    Esta fun��o configura o NRF24L01+ para operar no modo de recep��o, permitindo a recep��o de dados.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetRXMode();
    </code>

    <p><b>Observa��es:</b></p>
    O NRF24L01+ deve estar configurado corretamente para o modo RX.
*/
void NRF24L01_StandbyMode(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_StandbyMode(void);
  
    <p><b>Resumo:</b></p>
    Coloca o NRF24L01+ em modo de espera.

    <p><b>Descri��o:</b></p>
    Esta fun��o desativa o pino CE do NRF24L01+, colocando-o em modo de espera.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_StandbyMode();
    </code>

    <p><b>Observa��es:</b></p>
    O modo de espera reduz o consumo de energia do NRF24L01+.
*/
void NRF24L01_Sleep(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_Sleep(void);
  
    <p><b>Resumo:</b></p>
    Coloca o NRF24L01+ em modo de baixo consumo (sleep).

    <p><b>Descri��o:</b></p>
    Esta fun��o desativa o bit PWR_UP no registrador CONFIG, colocando o NRF24L01+ em modo de baixo consumo.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_Sleep();
    </code>

    <p><b>Observa��es:</b></p>
    O NRF24L01+ deve ser acordado usando a fun��o NRF24L01_Wake antes de realizar novas opera��es.
*/
void NRF24L01_Wake(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_Wake(void);
  
    <p><b>Resumo:</b></p>
    Acorda o NRF24L01+ do modo de baixo consumo.

    <p><b>Descri��o:</b></p>
    Esta fun��o ativa o bit PWR_UP no registrador CONFIG, colocando o NRF24L01+ em modo ativo.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_Wake();
    </code>

    <p><b>Observa��es:</b></p>
    Ap�s acordar, � necess�rio aguardar 1.5 ms para o NRF24L01+ estabilizar.
*/
void NRF24L01_SetAddress(uint8_t *address, uint8_t length);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_SetAddress(uint8_t *address, uint8_t length);
  
    <p><b>Resumo:</b></p>
    Configura o endere�o de envio/recebimento do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o define o endere�o de transmiss�o (TX_ADDR) do NRF24L01+. O endere�o pode ter at� 5 bytes.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - address: Ponteiro para o array de bytes que cont�m o endere�o.
    - length: Tamanho do endere�o (1 a 5 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t address[] = {0x52, 0xA0, 0xC1, 0x71, 0x1E};
    NRF24L01_SetAddress(address, 5);
    </code>

    <p><b>Observa��es:</b></p>
    O endere�o deve ser compat�vel com a configura��o do NRF24L01+.
*/
uint8_t NRF24L01_CheckStatus(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    uint8_t NRF24L01_CheckStatus(void);
  
    <p><b>Resumo:</b></p>
    Verifica o status do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o l� o registrador de status do NRF24L01+ e retorna seu valor.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    O valor do registrador de status.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t status = NRF24L01_CheckStatus();
    </code>

    <p><b>Observa��es:</b></p>
    O registrador de status deve ser interpretado conforme o datasheet do NRF24L01+.
*/
void NRF24L01_flush_tx(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_FlushTX(void);
  
    <p><b>Resumo:</b></p>
    Limpa a FIFO de transmiss�o do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o limpa a FIFO de transmiss�o do NRF24L01+, descartando qualquer dado pendente.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_FlushTX();
    </code>

    <p><b>Observa��es:</b></p>
    Esta fun��o deve ser usada para descartar dados antigos antes de enviar novos.
*/
void NRF24L01_flush_rx(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_FlushRX(void);
  
    <p><b>Resumo:</b></p>
    Limpa a FIFO de recep��o do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o limpa a FIFO de recep��o do NRF24L01+, descartando qualquer dado pendente.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_FlushRX();
    </code>

    <p><b>Observa��es:</b></p>
    Esta fun��o deve ser usada para descartar dados antigos antes de receber novos.
*/
void NRF24L01_ClearInterrupts(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_ClearInterrupts(void);
  
    <p><b>Resumo:</b></p>
    Limpa as interrup��es do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o limpa as flags de interrup��o no registrador de status do NRF24L01+.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_ClearInterrupts();
    </code>

    <p><b>Observa��es:</b></p>
    Esta fun��o deve ser chamada ap�s o tratamento de uma interrup��o.
*/
void nrf24l01_nop(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void nrf24l01_nop(void);
  
    <p><b>Resumo:</b></p>
    Executa uma opera��o NOP (No Operation) no NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o envia um comando NOP ao m�dulo NRF24L01+, que n�o realiza nenhuma opera��o, mas pode ser usada para ler o registrador de status.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    nrf24l01_nop(); // Executa uma opera��o NOP
    </code>

    <p><b>Observa��es:</b></p>
    O comando NOP � �til para ler o registrador de status sem alterar o estado do m�dulo.
*/
void NRF24L01_reuse_tx(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_reuse_tx(void);
  
    <p><b>Resumo:</b></p>
    Reutiliza o �ltimo payload transmitido no NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o permite reutilizar o �ltimo payload transmitido, sem a necessidade de reescrev�-lo na FIFO de transmiss�o.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_reuse_tx(); // Reutiliza o �ltimo payload transmitido
    </code>

    <p><b>Observa��es:</b></p>
    Esta fun��o � �til para retransmiss�es r�pidas do mesmo payload.
*/
void NRF24L01_pulse(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_pulse(void);
  
    <p><b>Resumo:</b></p>
    Gera um pulso no pino CE do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o gera um pulso alto no pino CE do m�dulo NRF24L01+, necess�rio para iniciar a transmiss�o ou recep��o de dados.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_pulse(); // Gera um pulso no pino CE
    </code>

    <p><b>Observa��es:</b></p>
    O pulso no pino CE � essencial para iniciar a comunica��o no modo TX ou RX.
*/
uint8_t NRF24L01_read_rx_width(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    uint8_t NRF24L01_read_rx_width(void);
  
    <p><b>Resumo:</b></p>
    L� o tamanho do payload recebido no NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o l� o tamanho do payload recebido na FIFO de recep��o do m�dulo NRF24L01+.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    O tamanho do payload recebido (em bytes).

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t payloadSize = NRF24L01_read_rx_width(); // L� o tamanho do payload recebido
    </code>

    <p><b>Observa��es:</b></p>
    O tamanho do payload deve ser compat�vel com a configura��o do m�dulo.
*/
void NRF24L01_write_ack(uint8_t pipe, uint8_t *data, uint8_t len);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_write_ack(uint8_t pipe, uint8_t *data, uint8_t len);
  
    <p><b>Resumo:</b></p>
    Escreve um payload de ACK no NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o escreve um payload de ACK no pipe especificado do m�dulo NRF24L01+, que ser� enviado automaticamente como resposta a um pacote recebido.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - pipe: Pipe no qual o payload de ACK ser� escrito (0 a 5).
    - data: Ponteiro para os dados a serem enviados como ACK.
    - len: Tamanho do payload de ACK (1 a 32 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t ackData[] = {0x01, 0x02, 0x03};
    NRF24L01_write_ack(0, ackData, 3); // Escreve um payload de ACK no pipe 0
    </code>

    <p><b>Observa��es:</b></p>
    O payload de ACK � enviado automaticamente ap�s a recep��o de um pacote no pipe especificado.
*/
void NRF24L01_write_tx_no_ack(uint8_t *data, uint8_t len);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_write_tx_no_ack(uint8_t *data, uint8_t len);
  
    <p><b>Resumo:</b></p>
    Escreve um payload no NRF24L01+ sem esperar por ACK.

    <p><b>Descri��o:</b></p>
    Esta fun��o escreve um payload na FIFO de transmiss�o do m�dulo NRF24L01+ sem esperar por uma confirma��o (ACK) do receptor.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - data: Ponteiro para os dados a serem transmitidos.
    - len: Tamanho do payload (1 a 32 bytes).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t payload[] = {0x01, 0x02, 0x03};
    NRF24L01_write_tx_no_ack(payload, 3); // Transmite um payload sem esperar por ACK
    </code>

    <p><b>Observa��es:</b></p>
    Esta fun��o � �til para transmiss�es r�pidas onde a confirma��o n�o � necess�ria.
*/
void NRF24L01_SetRFChannel(uint8_t channel);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_SetRFChannel(uint8_t channel);
  
    <p><b>Resumo:</b></p>
    Configura o canal RF do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o define o canal de frequ�ncia RF no qual o NRF24L01+ ir� operar. O canal pode variar de 0 a 125, correspondendo a frequ�ncias de 2.400 GHz a 2.525 GHz.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - channel: Canal RF a ser configurado (0 a 125).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetRFChannel(76); // Configura o canal RF para 76 (2.476 GHz)
    </code>

    <p><b>Observa��es:</b></p>
    O canal deve estar dentro do intervalo permitido (0 a 125). Caso contr�rio, ser� ajustado para o valor m�ximo (125).
*/
void NRF24L01_SetDataRate(uint8_t dataRate);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_SetDataRate(uint8_t dataRate);
  
    <p><b>Resumo:</b></p>
    Configura a taxa de dados do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o define a taxa de transmiss�o de dados do NRF24L01+. As op��es s�o 250 kbps, 1 Mbps (padr�o) ou 2 Mbps.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - dataRate: Taxa de dados desejada (250, 1000 ou 2000 kbps).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetDataRate(2000); // Configura a taxa de dados para 2 Mbps
    </code>

    <p><b>Observa��es:</b></p>
    A taxa de dados afeta o alcance e a robustez da comunica��o. Taxas mais baixas (250 kbps) proporcionam maior alcance, enquanto taxas mais altas (2 Mbps) permitem maior velocidade de transmiss�o.
*/
void NRF24L01_SetTXPower(uint8_t power);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    void NRF24L01_SetTXPower(uint8_t power);
  
    <p><b>Resumo:</b></p>
    Configura a pot�ncia de transmiss�o do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o define a pot�ncia de transmiss�o do NRF24L01+. As op��es s�o 0 dBm (m�xima pot�ncia), -6 dBm, -12 dBm ou -18 dBm (m�nima pot�ncia).

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    - power: Pot�ncia de transmiss�o desejada (0, 1, 2 ou 3, correspondendo a 0 dBm, -6 dBm, -12 dBm e -18 dBm, respectivamente).

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    NRF24L01_SetTXPower(0); // Configura a pot�ncia de transmiss�o para 0 dBm
    </code>

    <p><b>Observa��es:</b></p>
    A pot�ncia de transmiss�o afeta o consumo de energia e o alcance da comunica��o. Pot�ncias mais altas aumentam o alcance, mas tamb�m o consumo de energia.
*/
uint8_t NRF24L01_ReadRSSI(void);
/**
    <p><b>Prot�tipo da fun��o:</b></p>
    uint8_t NRF24L01_ReadRSSI(void);
  
    <p><b>Resumo:</b></p>
    L� o valor de RSSI (Received Signal Strength Indicator) do NRF24L01+.

    <p><b>Descri��o:</b></p>
    Esta fun��o l� o valor de RSSI, que indica a for�a do sinal recebido pelo m�dulo NRF24L01+.

    <p><b>Pr�-condi��o:</b></p>
    A fun��o NRF24L01_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    O valor de RSSI lido.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t rssi = NRF24L01_ReadRSSI(); // L� o valor de RSSI
    </code>

    <p><b>Observa��es:</b></p>
    O valor de RSSI pode ser usado para avaliar a qualidade do sinal recebido. Valores mais altos indicam um sinal mais forte.
*/
void NRF24L01_SetAddressWidth(uint8_t width);
void NRF24L01_SetPayloadSize(uint8_t size);
void NRF24L01_SetCrcLength(uint8_t crc_length);
void NRF24L01_OpenWritingPipe(uint64_t address);
void NRF24L01_OpenReadingPipe(uint8_t pipe, uint64_t address);
bool NRF24L01_tx_finished();
bool NRF24L01_rx_fifo_not_empty();

/*
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void);
/ **
    <p><b>Prot�tipo da fun��o:</b></p>
    void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void);
  
    <p><b>Resumo:</b></p>
    Fun��o de interrup��o externa para o pino IRQNRF.

    <p><b>Descri��o:</b></p>
    Esta fun��o � chamada quando ocorre uma interrup��o no pino IRQNRF. Ela verifica o status do NRF24L01+ e trata as interrup��es.

    <p><b>Pr�-condi��o:</b></p>
    A interrup��o externa deve estar configurada corretamente.

    <p><b>Par�metros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    // Configura��o da interrup��o no c�digo principal
    </code>

    <p><b>Observa��es:</b></p>
    Esta fun��o deve ser associada � interrup��o externa no c�digo principal.
* /
*/

#endif // NRF24L01_H
