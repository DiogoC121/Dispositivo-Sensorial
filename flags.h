#ifndef FLAGS_H
#define FLAGS_H

#include <stdbool.h>

// Flags vol�teis para comunica��o entre ISRs e o loop principal.
// 'volatile' garante que o compilador n�o otimize o acesso a estas vari�veis,
// pois elas podem ser modificadas a qualquer momento por uma interrup��o.

// Sinaliza que a interrup��o do MPU9250 (dados prontos) ocorreu.
extern volatile bool f_mpu;

// Sinaliza que a interrup��o do NRF24L01+ ocorreu.
extern volatile bool f_nrf;

// Sinaliza que a interrup��o do Timer1 (per�odo de amostragem) ocorreu.
extern volatile bool f_timer;

// Sinaliza que um comando v�lido foi recebido do gateway e est� pronto para ser processado.
extern volatile bool dados_recebidos;

// Sinaliza que houve conex�o
extern volatile bool f_conectado_gateway;
 
// Sinaliza erro.
extern volatile int f_erro;

#endif // FLAGS_H