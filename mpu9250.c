#include "mpu9250.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/pin_manager.h"
#include <stddef.h>

// --- Helper Functions ---
static void mpu_write_reg(uint8_t reg, uint8_t value) {
    SSMPU_SetLow();
    SPI1_Exchange8bit(reg);
    SPI1_Exchange8bit(value);
    SSMPU_SetHigh();
}

static uint8_t mpu_read_reg(uint8_t reg) {
    uint8_t value;
    SSMPU_SetLow();
    SPI1_Exchange8bit(reg | 0x80); // Set R/W bit to 1 for read
    value = SPI1_Exchange8bit(0x00); // Send dummy byte to receive data
    SSMPU_SetHigh();
    return value;
}

static void mpu_read_regs(uint8_t startReg, uint8_t* buffer, uint8_t count) {
    SSMPU_SetLow();
    SPI1_Exchange8bit(startReg | 0x80);
    for (uint8_t i = 0; i < count; i++) {
        buffer[i] = SPI1_Exchange8bit(0x00);
    }
    SSMPU_SetHigh();
}

// --- Interface Functions ---

void MPU9250_Init(void) {
    // Reset device
    mpu_write_reg(MPU9250_REG_PWR_MGMT_1, 0x80);
    __delay_ms(100);
    
    // Wake up device and set clock source
    mpu_write_reg(MPU9250_REG_PWR_MGMT_1, 0x01);
    __delay_ms(10);
    
    // Configure Accelerometer: +/- 4g
    mpu_write_reg(MPU9250_REG_ACCEL_CONFIG, 0x08);
    
    // Configure Gyroscope: +/- 500 dps
    mpu_write_reg(MPU9250_REG_GYRO_CONFIG, 0x08);
    
    // Configure Interrupts
    mpu_write_reg(MPU9250_REG_INT_PIN_CFG, 0x10); // Active high, push-pull
    mpu_write_reg(MPU9250_REG_INT_ENABLE, 0x01);  // Enable Data Ready interrupt
}

void MPU9250_ReadData(int16_t* accelData, int16_t* gyroData, int16_t* magData, int16_t* tempData) {
    uint8_t buffer[14];
    
    mpu_read_regs(MPU9250_REG_ACCEL_XOUT_H, buffer, 14);
    
    if(accelData != NULL) {
        accelData[0] = (int16_t)(buffer[0] << 8 | buffer[1]);
        accelData[1] = (int16_t)(buffer[2] << 8 | buffer[3]);
        accelData[2] = (int16_t)(buffer[4] << 8 | buffer[5]);
    }

    if(tempData != NULL) {
        // Temperature data is between accel and gyro data
        *tempData = (int16_t)(buffer[6] << 8 | buffer[7]);
    }

    if(gyroData != NULL) {
        gyroData[0] = (int16_t)(buffer[8] << 8 | buffer[9]);
        gyroData[1] = (int16_t)(buffer[10] << 8 | buffer[11]);
        gyroData[2] = (int16_t)(buffer[12] << 8 | buffer[13]);
    }
    
    // Magnetometer reading is more complex and requires I2C bypass mode.
    // For this fix, we will return zeros for the magnetometer.
    if(magData != NULL) {
        magData[0] = 0;
        magData[1] = 0;
        magData[2] = 0;
    }
}
