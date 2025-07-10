#ifndef FLAGS_H
#define FLAGS_H

#include <stdbool.h>

// Flags voláteis para comunicação entre ISRs e o loop principal.
// 'volatile' garante que o compilador não otimize o acesso a estas variáveis,
// pois elas podem ser modificadas a qualquer momento por uma interrupção.

// Sinaliza que a interrupção do MPU9250 (dados prontos) ocorreu.
volatile bool flag_mpu_data_ready;

// Sinaliza que a interrupção do NRF24L01+ ocorreu.
volatile bool flag_nrf_irq;

// Sinaliza que a interrupção do Timer1 (período de amostragem) ocorreu.
volatile bool flag_timer_irq;

// Sinaliza que um comando válido foi recebido do gateway e está pronto para ser processado.
volatile bool flag_gateway_command_received;

#endif // FLAGS_H