#include "mpu9250.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/pin_manager.h"
#include "defines.h" 
#include <stddef.h>

// --- Helper Functions for direct MPU9250 register access via SPI ---
static void mpu_write_reg(uint8_t reg, uint8_t value) {
    SSMPU_SetLow();
    SPI1_Exchange8bit(reg);
    SPI1_Exchange8bit(value);
    SSMPU_SetHigh();
}

static uint8_t mpu_read_reg(uint8_t reg) {
    uint8_t value;
    SSMPU_SetLow();
    SPI1_Exchange8bit(reg | 0x80);
    value = SPI1_Exchange8bit(0x00);
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

// --- Helper functions for accessing AK8963 via MPU's I2C Master interface ---
static void ak8963_write_reg(uint8_t reg, uint8_t value) {
    // Set the I2C slave address and register to write to
    mpu_write_reg(0x25, AK8963_ADDRESS); // I2C_SLV0_ADDR
    mpu_write_reg(0x26, reg);            // I2C_SLV0_REG
    // Write the data to the slave
    mpu_write_reg(0x63, value);          // I2C_SLV0_DO
    // Enable the transaction
    mpu_write_reg(0x27, 0x81);           // I2C_SLV0_CTRL: Enable, 1 byte
    // Wait for the transaction to complete
    while (!(mpu_read_reg(MPU9250_REG_I2C_MST_STATUS) & 0x40));
}

/*
static uint8_t ak8963_read_reg(uint8_t reg) {
    // Set the I2C slave address and register to read from
    mpu_write_reg(0x25, AK8963_ADDRESS | 0x80); // Set read bit
    mpu_write_reg(0x26, reg);                   // I2C_SLV0_REG
    // Enable the transaction to read 1 byte
    mpu_write_reg(0x27, 0x81);                  // I2C_SLV0_CTRL: Enable, 1 byte
    // Wait for the transaction to complete
    while (!(mpu_read_reg(MPU9250_REG_I2C_MST_STATUS) & 0x40));
    // Read the data from the external sensor data register
    return mpu_read_reg(MPU9250_REG_EXT_SENS_DATA_00);
}
*/

// --- Interface Functions ---

void MPU9250_Init(void) {
    // Reset device
    mpu_write_reg(MPU9250_REG_PWR_MGMT_1, 0x80);
    __delay_ms(100);
    
    // Wake up device and set clock source
    mpu_write_reg(MPU9250_REG_PWR_MGMT_1, 0x01);
    __delay_ms(10);
    
    // Enable I2C Master Mode
    mpu_write_reg(MPU9250_REG_USER_CTRL, 0x20);
    __delay_ms(10);
    
    // Configure I2C Master Clock speed
    mpu_write_reg(0x24, 0x0D); // I2C_MST_CTRL, 400kHz
    __delay_ms(10);
    
    // Configure Accelerometer: +/- 4g
    mpu_write_reg(MPU9250_REG_ACCEL_CONFIG, 0x08);
    
    // Configure Gyroscope: +/- 500 dps
    mpu_write_reg(MPU9250_REG_GYRO_CONFIG, 0x08);
    
    // --- Configure Magnetometer (AK8963) ---
    // Reset the magnetometer
    ak8963_write_reg(AK8963_REG_CNTL2, 0x01);
    __delay_ms(10);
    // Set to continuous measurement mode 2 (100Hz)
    ak8963_write_reg(AK8963_REG_CNTL1, 0x16); // 16-bit output, 100Hz
    __delay_ms(10);

    // Configure MPU to automatically read from magnetometer
    mpu_write_reg(0x25, AK8963_ADDRESS | 0x80); // Slave 0 address (read)
    mpu_write_reg(0x26, AK8963_REG_HXL);        // Start reading from HXL register
    mpu_write_reg(0x27, 0x87);                  // Enable slave 0, read 7 bytes (HXL to ST2)
}

void MPU9250_ReadData(int16_t* accelData, int16_t* gyroData, int16_t* magData, int16_t* tempData) {
    uint8_t buffer[21];
    
    // Read all sensor data in one go (Accel, Temp, Gyro, Mag)
    mpu_read_regs(MPU9250_REG_ACCEL_XOUT_H, buffer, 21);
    
    if(accelData != NULL) {
        accelData[0] = (int16_t)(buffer[0] << 8 | buffer[1]);
        accelData[1] = (int16_t)(buffer[2] << 8 | buffer[3]);
        accelData[2] = (int16_t)(buffer[4] << 8 | buffer[5]);
    }

    if(tempData != NULL) {
        *tempData = (int16_t)(buffer[6] << 8 | buffer[7]);
    }

    if(gyroData != NULL) {
        gyroData[0] = (int16_t)(buffer[8] << 8 | buffer[9]);
        gyroData[1] = (int16_t)(buffer[10] << 8 | buffer[11]);
        gyroData[2] = (int16_t)(buffer[12] << 8 | buffer[13]);
    }
    
    if(magData != NULL) {
        // Data is Little Endian: LSB first
        magData[0] = (int16_t)(buffer[15] << 8 | buffer[14]);
        magData[1] = (int16_t)(buffer[17] << 8 | buffer[16]);
        magData[2] = (int16_t)(buffer[19] << 8 | buffer[18]);
    }
}

void MPU9250_Sleep(void) {
    uint8_t pwr_mgmt_1 = mpu_read_reg(MPU9250_REG_PWR_MGMT_1);
    mpu_write_reg(MPU9250_REG_PWR_MGMT_1, pwr_mgmt_1 | 0x40); // Set SLEEP bit
}

void MPU9250_Wake(void) {
    uint8_t pwr_mgmt_1 = mpu_read_reg(MPU9250_REG_PWR_MGMT_1);
    mpu_write_reg(MPU9250_REG_PWR_MGMT_1, pwr_mgmt_1 & ~0x40); // Clear SLEEP bit
    __delay_ms(10);
}
