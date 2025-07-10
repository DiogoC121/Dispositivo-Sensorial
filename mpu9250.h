#ifndef MPU9250_H
#define MPU9250_H

#include <stdint.h>
#include <stdbool.h>

// --- MPU9250 Register Map ---
#define MPU9250_REG_WHO_AM_I      0x75
#define MPU9250_REG_PWR_MGMT_1    0x6B
#define MPU9250_REG_ACCEL_XOUT_H  0x3B
#define MPU9250_REG_GYRO_CONFIG   0x1B
#define MPU9250_REG_ACCEL_CONFIG  0x1C
#define MPU9250_REG_INT_PIN_CFG   0x37
#define MPU9250_REG_INT_ENABLE    0x38

/**
 * @brief Initializes the MPU9250 sensor.
 */
void MPU9250_Init(void);

/**
 * @brief Reads data from all relevant sensors in the MPU9250.
 */
void MPU9250_ReadData(int16_t* accelData, int16_t* gyroData, int16_t* magData, int16_t* tempData);

/**
 * @brief Puts the MPU9250 into sleep mode.
 */
void MPU9250_Sleep(void);

/**
 * @brief Wakes the MPU9250 from sleep mode.
 */
void MPU9250_Wake(void);

#endif // MPU9250_H
