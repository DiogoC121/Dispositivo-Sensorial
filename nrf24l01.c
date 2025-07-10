#include "nrf24l01.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/pin_manager.h"
#include "defines.h"

// --- NRF24L01 Register Map ---
#define NRF_REG_CONFIG      0x00
#define NRF_REG_EN_AA       0x01
#define NRF_REG_EN_RXADDR   0x02
#define NRF_REG_SETUP_AW    0x03
#define NRF_REG_RF_CH       0x05
#define NRF_REG_RF_SETUP    0x06
#define NRF_REG_STATUS      0x07
#define NRF_REG_RX_ADDR_P0  0x0A
#define NRF_REG_TX_ADDR     0x10
#define NRF_REG_RX_PW_P0    0x11
#define NRF_REG_FIFO_STATUS 0x17
#define NRF_REG_DYNPD       0x1C
#define NRF_REG_FEATURE     0x1D

// --- NRF24L01 Commands ---
#define NRF_CMD_W_REGISTER    0x20
#define NRF_CMD_R_RX_PAYLOAD  0x61
#define NRF_CMD_W_TX_PAYLOAD  0xA0
#define NRF_CMD_FLUSH_TX      0xE1
#define NRF_CMD_FLUSH_RX      0xE2

// --- Helper Functions ---
static void nrf_write_reg(uint8_t reg, uint8_t value) {
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_W_REGISTER | reg);
    SPI1_Exchange8bit(value);
    SSNRF_SetHigh();
}

static void nrf_write_reg_multi(uint8_t reg, const uint8_t* data, uint8_t length) {
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_W_REGISTER | reg);
    for (uint8_t i = 0; i < length; i++) {
        SPI1_Exchange8bit(data[i]);
    }
    SSNRF_SetHigh();
}

static uint8_t nrf_read_reg(uint8_t reg) {
    uint8_t value;
    SSNRF_SetLow();
    SPI1_Exchange8bit(reg);
    value = SPI1_Exchange8bit(0xFF); // Dummy byte
    SSNRF_SetHigh();
    return value;
}

static void nrf_set_rx_mode(void) {
    uint8_t config = nrf_read_reg(NRF_REG_CONFIG);
    config |= (1 << 0); // PRIM_RX = 1
    nrf_write_reg(NRF_REG_CONFIG, config);
    CENRF_SetHigh(); // Start listening
    __delay_us(150);
}

static void nrf_set_tx_mode(void) {
    CENRF_SetLow();
    uint8_t config = nrf_read_reg(NRF_REG_CONFIG);
    config &= ~(1 << 0); // PRIM_RX = 0
    nrf_write_reg(NRF_REG_CONFIG, config);
}

// --- Interface Functions ---

void NRF24L01_Init(void) {
    CENRF_SetLow();
    SSNRF_SetHigh();
    __delay_ms(100);

    // Config: CRC enabled (2 bytes), Power Up, TX mode
    nrf_write_reg(NRF_REG_CONFIG, 0x0E); 
    __delay_ms(5);
    
    nrf_write_reg(NRF_REG_EN_AA, 0x01);      // Enable Auto-Ack on Pipe 0
    nrf_write_reg(NRF_REG_EN_RXADDR, 0x01);  // Enable RX on Pipe 0
    nrf_write_reg(NRF_REG_SETUP_AW, 0x03);   // 5-byte address width
    nrf_write_reg(NRF_REG_RF_CH, NRF_CHANNEL); // Set RF channel
    nrf_write_reg(NRF_REG_RF_SETUP, 0x06);   // 1Mbps, 0dBm power
    
    uint8_t addr[] = NRF_ADDRESS;
    nrf_write_reg_multi(NRF_REG_TX_ADDR, addr, 5);
    nrf_write_reg_multi(NRF_REG_RX_ADDR_P0, addr, 5);
    
    nrf_write_reg(NRF_REG_RX_PW_P0, BUFFER_SIZE * sizeof(int16_t)); // Payload size
    
    nrf_set_rx_mode(); // Default to receiver mode
}

bool NRF24L01_Check(void) {
    uint8_t setup_aw = nrf_read_reg(NRF_REG_SETUP_AW);
    return (setup_aw == 0x03);
}

bool NRF24L01_TransmitPayload(uint8_t* payload, uint8_t length) {
    nrf_set_tx_mode();

    // Write payload to TX FIFO
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_W_TX_PAYLOAD);
    for (uint8_t i = 0; i < length; i++) {
        SPI1_Exchange8bit(payload[i]);
    }
    SSNRF_SetHigh();

    // Pulse CE to transmit
    CENRF_SetHigh();
    __delay_us(15);
    CENRF_SetLow();

    // Wait for transmission to complete (or fail)
    uint8_t status;
    uint16_t timeout = 5000; // ~5ms timeout
    while(timeout > 0) {
        status = nrf_read_reg(NRF_REG_STATUS);
        if (status & ((1 << 5) | (1 << 4))) { // TX_DS or MAX_RT
            break;
        }
        __delay_us(1);
        timeout--;
    }

    bool success = false;
    if (status & (1 << 5)) { // TX_DS (Data Sent)
        success = true;
    }
    
    // Clear status flags and flush TX FIFO
    nrf_write_reg(NRF_REG_STATUS, (1 << 5) | (1 << 4));
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_FLUSH_TX);
    SSNRF_SetHigh();

    nrf_set_rx_mode(); // Go back to listening
    return success;
}

bool NRF24L01_IsDataAvailable(void) {
    uint8_t status = nrf_read_reg(NRF_REG_STATUS);
    return (status & (1 << 6)); // Check RX_DR flag
}

void NRF24L01_ReadPayload(uint8_t* payload, uint8_t length) {
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_R_RX_PAYLOAD);
    for (uint8_t i = 0; i < length; i++) {
        payload[i] = SPI1_Exchange8bit(0xFF);
    }
    SSNRF_SetHigh();
    
    // Clear RX_DR flag
    nrf_write_reg(NRF_REG_STATUS, (1 << 6));
}
