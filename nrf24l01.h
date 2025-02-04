#ifndef NRF24L01_H
#define NRF24L01_H

#include <xc.h>
#include <stdint.h>
#include "defines.h"  // Inclui o arquivo defines.h

// Prot�tipos das fun��es
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
void NRF24L01_WritePayload(uint8_t *data, uint8_t length);
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
void NRF24L01_ReadPayload(uint8_t *data, uint8_t length);
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
void NRF24L01_FlushTX(void);
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
void NRF24L01_FlushRX(void);
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
