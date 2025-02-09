#ifndef MPU9250_H
#define MPU9250_H

#include <xc.h>
#include <stdint.h>
#include "defines.h"  // Inclui o arquivo defines.h

// Protótipos das funções
void MPU9250_Init(void);
/**
    <p><b>Protótipo da função:</b></p>
    void MPU9250_Init(void);
  
    <p><b>Resumo:</b></p>
    Inicializa o MPU9250, configurando os pinos SPI, a interface SPI1, a interrupção externa e os registradores do MPU9250.

    <p><b>Descrição:</b></p>
    Esta função configura os pinos SPI, inicializa a interface SPI1, configura a interrupção externa no pino IRQMPU e inicializa os registradores do MPU9250 para operação normal.

    <p><b>Pré-condição:</b></p>
    O sistema deve estar configurado corretamente, e os pinos SPI devem estar definidos no arquivo defines.h.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    MPU9250_Init();
    </code>

    <p><b>Observações:</b></p>
    Esta função deve ser chamada antes de qualquer outra função do MPU9250.
*/
void MPU9250_WriteRegister(uint8_t reg, uint8_t data);
/**
    <p><b>Protótipo da função:</b></p>
    void MPU9250_WriteRegister(uint8_t reg, uint8_t data);
  
    <p><b>Resumo:</b></p>
    Escreve um valor em um registrador do MPU9250.

    <p><b>Descrição:</b></p>
    Esta função escreve um valor de 8 bits em um registrador específico do MPU9250 usando a interface SPI.

    <p><b>Pré-condição:</b></p>
    A função MPU9250_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - reg: Endereço do registrador a ser escrito.
    - data: Valor a ser escrito no registrador.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    MPU9250_WriteRegister(MPU9250_PWR_MGMT_1, 0x00);
    </code>

    <p><b>Observações:</b></p>
    O endereço do registrador deve ser válido conforme o datasheet do MPU9250.
*/
uint8_t MPU9250_ReadRegister(uint8_t reg);
/**
    <p><b>Protótipo da função:</b></p>
    uint8_t MPU9250_ReadRegister(uint8_t reg);
  
    <p><b>Resumo:</b></p>
    Lê um valor de um registrador do MPU9250.

    <p><b>Descrição:</b></p>
    Esta função lê um valor de 8 bits de um registrador específico do MPU9250 usando a interface SPI.

    <p><b>Pré-condição:</b></p>
    A função MPU9250_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - reg: Endereço do registrador a ser lido.

    <p><b>Retorna:</b></p>
    O valor lido do registrador.

    <p><b>Exemplo:</b></p>
    <code>
    uint8_t who_am_i = MPU9250_ReadRegister(MPU9250_WHO_AM_I);
    </code>

    <p><b>Observações:</b></p>
    O endereço do registrador deve ser válido conforme o datasheet do MPU9250.
*/
void MPU9250_ReadData(int16_t *accel, int16_t *gyro, int16_t *mag, int16_t *temp);
/**
    <p><b>Protótipo da função:</b></p>
    void MPU9250_ReadData(int16_t *accel, int16_t *gyro, int16_t *mag, int16_t *temp);
  
    <p><b>Resumo:</b></p>
    Lê os dados de aceleração, giroscópio, magnetômetro e temperatura do MPU9250.

    <p><b>Descrição:</b></p>
    Esta função lê os dados brutos dos sensores do MPU9250 e os converte para valores de 16 bits.

    <p><b>Pré-condição:</b></p>
    A função MPU9250_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    - accel: Ponteiro para um array de 3 elementos para armazenar os dados de aceleração (X, Y, Z).
    - gyro: Ponteiro para um array de 3 elementos para armazenar os dados do giroscópio (X, Y, Z).
    - mag: Ponteiro para um array de 3 elementos para armazenar os dados do magnetômetro (X, Y, Z).
    - temp: Ponteiro para uma variável para armazenar a temperatura.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    int16_t accel[3], gyro[3], mag[3], temp;
    MPU9250_ReadData(accel, gyro, mag, &temp);
    </code>

    <p><b>Observações:</b></p>
    Os dados brutos devem ser convertidos para unidades físicas conforme o datasheet do MPU9250.
*/
void MPU9250_Sleep(void);
/**
    <p><b>Protótipo da função:</b></p>
    void MPU9250_Sleep(void);
  
    <p><b>Resumo:</b></p>
    Coloca o MPU9250 em modo de baixo consumo (sleep mode).

    <p><b>Descrição:</b></p>
    Esta função configura o MPU9250 para entrar em modo de baixo consumo, reduzindo o consumo de energia.

    <p><b>Pré-condição:</b></p>
    A função MPU9250_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    MPU9250_Sleep();
    </code>

    <p><b>Observações:</b></p>
    O MPU9250 deve ser acordado usando a função MPU9250_Wake antes de realizar novas leituras.
*/
void MPU9250_Wake(void);
/**
    <p><b>Protótipo da função:</b></p>
    void MPU9250_Wake(void);
  
    <p><b>Resumo:</b></p>
    Acorda o MPU9250 do modo de baixo consumo.

    <p><b>Descrição:</b></p>
    Esta função configura o MPU9250 para sair do modo de baixo consumo e retomar a operação normal.

    <p><b>Pré-condição:</b></p>
    A função MPU9250_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    Nada.

    <p><b>Exemplo:</b></p>
    <code>
    MPU9250_Wake();
    </code>

    <p><b>Observações:</b></p>
    O MPU9250 deve estar em modo de baixo consumo antes de chamar esta função.
*/
uint8_t MPU9250_CheckError(void);
/**
    <p><b>Protótipo da função:</b></p>
    uint8_t MPU9250_CheckError(void);
  
    <p><b>Resumo:</b></p>
    Verifica se houve algum erro durante a operação do MPU9250.

    <p><b>Descrição:</b></p>
    Esta função lê o registrador de status do MPU9250 e verifica se houve algum erro.

    <p><b>Pré-condição:</b></p>
    A função MPU9250_Init deve ter sido chamada para configurar a interface SPI.

    <p><b>Parâmetros:</b></p>
    Nenhum.

    <p><b>Retorna:</b></p>
    1 se houve erro, 0 caso contrário.

    <p><b>Exemplo:</b></p>
    <code>
    if (MPU9250_CheckError()) {
        // Tratar erro
    }
    </code>

    <p><b>Observações:</b></p>
    O registrador de status deve ser interpretado conforme o datasheet do MPU9250.
*/
void MPU9250_read_accel(uint8_t* xH, uint8_t* xL, uint8_t* yH, uint8_t* yL, uint8_t* zH, uint8_t* zL);
void MPU9250_read_gyro(uint8_t* xH, uint8_t* xL, uint8_t* yH, uint8_t* yL, uint8_t* zH, uint8_t* zL);
void MPU9250_read_mag(uint8_t* xH, uint8_t* xL, uint8_t* yH, uint8_t* yL, uint8_t* zH, uint8_t* zL);
void MPU9250_ReadData(int16_t *accel, int16_t *gyro, int16_t *mag, int16_t *temp);
/*
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void);
/ **
    <p><b>Protótipo da função:</b></p>
    void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void);
  
    <p><b>Resumo:</b></p>
    Função de interrupção externa para o pino IRQMPU.

    <p><b>Descrição:</b></p>
    Esta função é chamada quando ocorre uma interrupção no pino IRQMPU. Ela lê os dados do MPU9250 e os processa.

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

#endif // MPU9250_H