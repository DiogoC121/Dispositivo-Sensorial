#ifndef NRF24L01_H
#define NRF24L01_H

#include <stdint.h>
#include <stdbool.h>

void NRF24L01_Init(void);
bool NRF24L01_Check(void);
void NRF24L01_SetTXMode(void);
void NRF24L01_SetRXMode(void);
bool NRF24L01_WritePayload(uint8_t* payload, uint8_t length);
void NRF24L01_ReadPayload(uint8_t* payload, uint8_t length);
uint8_t NRF24L01_CheckStatus(void);
bool NRF24L01_rx_fifo_not_empty(void);
void NRF24L01_flush_tx(void);
void NRF24L01_flush_rx(void);
void NRF24L01_ClearInterrupts(void);
void NRF24L01_Sleep(void);
void NRF24L01_Wake(void);

#endif // NRF24L01_H
