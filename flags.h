#ifndef FLAGS_H
#define FLAGS_H

#include <stdbool.h>

// Flags voláteis para comunicação entre ISRs e o loop principal.
// 'volatile' garante que o compilador não otimize o acesso a estas variáveis,
// pois elas podem ser modificadas a qualquer momento por uma interrupção.

// Sinaliza que a interrupção do MPU9250 (dados prontos) ocorreu.
extern volatile bool f_mpu;

// Sinaliza que a interrupção do NRF24L01+ ocorreu.
extern volatile bool f_nrf;

// Sinaliza que a interrupção do Timer1 (período de amostragem) ocorreu.
extern volatile bool f_timer;

// Sinaliza que um comando válido foi recebido do gateway e está pronto para ser processado.
extern volatile bool dados_recebidos;

// Sinaliza que houve conexão
extern volatile bool f_conectado_gateway;
 
// Sinaliza erro.
extern volatile int f_erro;

#endif // FLAGS_H