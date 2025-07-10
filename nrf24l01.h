#ifndef NRF24L01_H
#define NRF24L01_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the NRF24L01 module.
 */
void NRF24L01_Init(void);

/**
 * @brief Checks if the NRF24L01 module is connected and responding.
 * @return True if the module is found, false otherwise.
 */
bool NRF24L01_Check(void);

/**
 * @brief Transmits a data payload.
 * @param payload Pointer to the data buffer to be sent.
 * @param length Length of the data buffer in bytes.
 * @return True on successful transmission (ACK received), false on failure.
 */
bool NRF24L01_TransmitPayload(uint8_t* payload, uint8_t length);

/**
 * @brief Checks if there is data available in the RX FIFO.
 * @return True if data is available, false otherwise.
 */
bool NRF24L01_IsDataAvailable(void);

/**
 * @brief Reads the received payload from the RX FIFO.
 * @param payload Pointer to a buffer where the received data will be stored.
 * @param length The number of bytes to read.
 */
void NRF24L01_ReadPayload(uint8_t* payload, uint8_t length);

#endif // NRF24L01_H
