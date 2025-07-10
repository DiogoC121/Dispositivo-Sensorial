#ifndef FLAGS_H
#define FLAGS_H

#include <stdbool.h>

// Flags vol�teis para comunica��o entre ISRs e o loop principal.
// 'volatile' garante que o compilador n�o otimize o acesso a estas vari�veis,
// pois elas podem ser modificadas a qualquer momento por uma interrup��o.

// Sinaliza que a interrup��o do MPU9250 (dados prontos) ocorreu.
volatile bool flag_mpu_data_ready;

// Sinaliza que a interrup��o do NRF24L01+ ocorreu.
volatile bool flag_nrf_irq;

// Sinaliza que a interrup��o do Timer1 (per�odo de amostragem) ocorreu.
volatile bool flag_timer_irq;

// Sinaliza que um comando v�lido foi recebido do gateway e est� pronto para ser processado.
volatile bool flag_gateway_command_received;

#endif // FLAGS_H