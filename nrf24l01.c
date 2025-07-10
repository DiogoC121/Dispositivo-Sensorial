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
#define NRF_CMD_NOP           0xFF

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
    value = SPI1_Exchange8bit(NRF_CMD_NOP);
    SSNRF_SetHigh();
    return value;
}

// --- Interface Functions ---

void NRF24L01_Init(void) {
    CENRF_SetLow();
    SSNRF_SetHigh();
    __delay_ms(100);

    nrf_write_reg(NRF_REG_CONFIG, 0x0E); 
    __delay_ms(5);
    
    nrf_write_reg(NRF_REG_EN_AA, 0x01);
    nrf_write_reg(NRF_REG_EN_RXADDR, 0x01);
    nrf_write_reg(NRF_REG_SETUP_AW, 0x03);
    nrf_write_reg(NRF_REG_RF_CH, NRF_CHANNEL);
    nrf_write_reg(NRF_REG_RF_SETUP, 0x06);
    
    uint8_t addr[] = NRF_ADDRESS;
    nrf_write_reg_multi(NRF_REG_TX_ADDR, addr, 5);
    nrf_write_reg_multi(NRF_REG_RX_ADDR_P0, addr, 5);
    
    nrf_write_reg(NRF_REG_RX_PW_P0, BUFFER_SIZE * sizeof(int16_t));
    
    NRF24L01_SetRXMode();
}

bool NRF24L01_Check(void) {
    uint8_t setup_aw = nrf_read_reg(NRF_REG_SETUP_AW);
    return (setup_aw == 0x03);
}

void NRF24L01_SetTXMode(void) {
    CENRF_SetLow();
    uint8_t config = nrf_read_reg(NRF_REG_CONFIG);
    nrf_write_reg(NRF_REG_CONFIG, config & 0xFE); // PRIM_RX = 0
}

void NRF24L01_SetRXMode(void) {
    uint8_t config = nrf_read_reg(NRF_REG_CONFIG);
    nrf_write_reg(NRF_REG_CONFIG, config | 0x01); // PRIM_RX = 1
    CENRF_SetHigh();
}

bool NRF24L01_WritePayload(uint8_t* payload, uint8_t length) {
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_W_TX_PAYLOAD);
    SPI1_Exchange8bitBuffer(payload, length, NULL);
    SSNRF_SetHigh();

    CENRF_SetHigh();
    __delay_us(15);
    CENRF_SetLow();

    uint8_t status;
    uint16_t timeout = 5000;
    while(timeout > 0) {
        status = NRF24L01_CheckStatus();
        if (status & ((1 << 5) | (1 << 4))) {
            break;
        }
        __delay_us(1);
        timeout--;
    }

    if (status & (1 << 5)) {
        nrf_write_reg(NRF_REG_STATUS, (1 << 5));
        NRF24L01_flush_tx();
        return true;
    }
    
    NRF24L01_flush_tx();
    return false;
}

void NRF24L01_ReadPayload(uint8_t* payload, uint8_t length) {
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_R_RX_PAYLOAD);
    SPI1_Exchange8bitBuffer(NULL, length, payload);
    SSNRF_SetHigh();
    nrf_write_reg(NRF_REG_STATUS, (1 << 6)); // Clear RX_DR
}

uint8_t NRF24L01_CheckStatus(void) {
    return nrf_read_reg(NRF_REG_STATUS);
}

bool NRF24L01_rx_fifo_not_empty(void) {
    uint8_t fifo_status = nrf_read_reg(NRF_REG_FIFO_STATUS);
    return !(fifo_status & (1 << 0));
}

void NRF24L01_flush_tx(void) {
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_FLUSH_TX);
    SSNRF_SetHigh();
}

void NRF24L01_flush_rx(void) {
    SSNRF_SetLow();
    SPI1_Exchange8bit(NRF_CMD_FLUSH_RX);
    SSNRF_SetHigh();
}

void NRF24L01_ClearInterrupts(void) {
    nrf_write_reg(NRF_REG_STATUS, (1 << 6) | (1 << 5) | (1 << 4));
}

void NRF24L01_Sleep(void) {
    uint8_t config = nrf_read_reg(NRF_REG_CONFIG);
    nrf_write_reg(NRF_REG_CONFIG, config & ~(1 << 1)); // PWR_UP = 0
}

void NRF24L01_Wake(void) {
    uint8_t config = nrf_read_reg(NRF_REG_CONFIG);
    nrf_write_reg(NRF_REG_CONFIG, config | (1 << 1)); // PWR_UP = 1
    __delay_ms(5); // Wait for oscillator to stabilize
}
