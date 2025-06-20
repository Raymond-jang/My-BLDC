#include "AS5600.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

static HAL_StatusTypeDef AS5600_ReadRegister(uint8_t reg, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(&hi2c1, AS5600_SLAVE_ADDRESS, &reg, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) return status;
    status = HAL_I2C_Master_Receive(&hi2c1, AS5600_SLAVE_ADDRESS, data, len, HAL_MAX_DELAY);
    return status;
}

uint16_t AS5600_ReadAngle(uint8_t angle_reg)
{
    uint8_t angle_bytes[2];
    uint16_t angle = 0;
    if (AS5600_ReadRegister(angle_reg, angle_bytes, 2) == HAL_OK)
    {
        angle = ((uint16_t)angle_bytes[0] << 8) | angle_bytes[1];
        angle &= 0x0FFF;
    }
    else
    {
        angle = 0xFFFF;
    }
    return angle;
}

uint8_t AS5600_ReadStatus(void)
{
    uint8_t status_byte = 0xFF;
    AS5600_ReadRegister(AS5600_STATUS_REG, &status_byte, 1);
    return status_byte;
}

void AS5600_CheckError(HAL_StatusTypeDef status)
{
    if (status == HAL_ERROR)
        printf("I2C Error: HAL_ERROR (no ACK)\r\n");
    else if (status == HAL_TIMEOUT)
        printf("I2C Error: HAL_TIMEOUT\r\n");
    else if (status == HAL_BUSY)
        printf("I2C Error: HAL_BUSY\r\n");
}


#if 0
#include "main.h"
#include <stdio.h>

// --- AS5600 I2C Slave Address ---
// The AS5600's default 7-bit I2C address is 0x36.
// For HAL I2C functions, it needs to be 8-bit (shifted left by 1).
#define AS5600_SLAVE_ADDRESS (0x36 << 1)

// --- AS5600 Register Addresses ---
#define AS5600_ZMCO_REG       0x00 // Z-axis Magnitude and Configuration register
#define AS5600_ZPOS_REG       0x01 // Zero Position register (high byte)
#define AS5600_MPOS_REG       0x03 // Maximum Position register (high byte)
#define AS5600_MANG_REG       0x05 // Maximum Angle register (high byte)
#define AS5600_CONF_REG       0x07 // Configuration register (high byte)
#define AS5600_RAW_ANGLE_REG  0x0C // Raw Angle register (high byte) - 12-bit value
#define AS5600_ANGLE_REG      0x0E // Filtered Angle register (high byte) - 12-bit value
#define AS5600_STATUS_REG     0x0B // Status register
#define AS5600_AGC_REG        0x1A // Automatic Gain Control register
#define AS5600_MAGNITUDE_REG  0x1B // Magnitude register (high byte)
#define AS5600_BURN_REG       0xFF // Burn Command register

extern I2C_HandleTypeDef hi2c1;

uint16_t AS5600_ReadAngle(uint8_t angle_reg);
uint8_t AS5600_ReadStatus(void);
void AS5600_CheckError(HAL_StatusTypeDef status);


uint16_t AS5600_ReadAngleStatus(void)
{
	uint16_t raw_angle = 0;
	uint16_t filtered_angle = 0;
	uint8_t status = 0;

    // Read status bits
    status = AS5600_ReadStatus();

    // Read raw angle (12-bit value)
    raw_angle = AS5600_ReadAngle(AS5600_RAW_ANGLE_REG);

    // Read filtered angle (12-bit value)
    filtered_angle = AS5600_ReadAngle(AS5600_ANGLE_REG);



    // Print values (assuming printf is configured)
    printf("Raw Angle: %lu, Filtered Angle: %lu, Status: 0x%02X\r\n",
           (unsigned long)raw_angle, (unsigned long)filtered_angle, status);

    // Interpret status bits (optional, for debugging)
    if (status & 0x08) { // MD - Magnet Detected
        printf("  Magnet Detected!\r\n");
    } else {
        printf("  Magnet NOT Detected!\r\n");
    }
    if (status & 0x10) { // ML - Magnet Low
        printf("  Magnet Too Weak! Move magnet closer.\r\n");
    }
    if (status & 0x20) { // MH - Magnet High
        printf("  Magnet Too Strong! Move magnet further away.\r\n");
    }

    return raw_angle;  // range: 0 to 4095
}

// --- AS5600 specific functions ---

/**
  * @brief Reads data from an AS5600 register.
  * @param reg_addr: Address of the register to read.
  * @param data: Pointer to buffer to store read data.
  * @param len: Number of bytes to read.
  * @retval HAL_StatusTypeDef: Status of the I2C transaction.
  */
HAL_StatusTypeDef AS5600_ReadRegister(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
  HAL_StatusTypeDef status = HAL_OK;
  // Send register address byte
  status = HAL_I2C_Master_Transmit(&hi2c1, AS5600_SLAVE_ADDRESS, &reg_addr, 1, HAL_MAX_DELAY);
  AS5600_CheckError(status);
  if (status != HAL_OK) return status;

  // Receive data bytes
  status = HAL_I2C_Master_Receive(&hi2c1, AS5600_SLAVE_ADDRESS, data, len, HAL_MAX_DELAY);
  AS5600_CheckError(status);
  return status;
}

/**
  * @brief Reads a 12-bit angle value from the AS5600.
  * @param angle_reg: The angle register address (AS5600_RAW_ANGLE_REG or AS5600_ANGLE_REG).
  * @retval uint16_t: The 12-bit angle value (0-4095).
  */
uint16_t AS5600_ReadAngle(uint8_t angle_reg)
{
  uint8_t angle_bytes[2];
  uint16_t angle = 0;
  HAL_StatusTypeDef status;

  status = AS5600_ReadRegister(angle_reg, angle_bytes, 2);
  if (status == HAL_OK)
  {
    // The AS5600 returns the 12-bit value in two bytes:
    // Byte 0: Bits 11-8 (MSB nibble)
    // Byte 1: Bits 7-0 (LSB)
    // So, we combine them: (MSB << 8) | LSB
    // And mask to 12 bits (0xFFF) because the highest 4 bits of the MSB byte are 0.
    angle = ((uint16_t)angle_bytes[0] << 8) | angle_bytes[1];
    angle &= 0x0FFF; // Mask to ensure only the 12 bits are used (0 to 4095)
  }
  else
  {
    // Handle I2C error, maybe return a specific error code or last valid reading
    angle = 0xFFFF; // Indicate an error
  }
  return angle;
}

/**
  * @brief Reads the AS5600 Status register.
  * @retval uint8_t: The 8-bit status register value.
  * Bits: MD (0x08), ML (0x10), MH (0x20)
  */
uint8_t AS5600_ReadStatus(void)
{
  uint8_t status_byte;
  HAL_StatusTypeDef status;

  status = AS5600_ReadRegister(AS5600_STATUS_REG, &status_byte, 1);
  if (status == HAL_OK)
  {
    return status_byte;
  }
  else
  {
    return 0xFF; // Indicate an error
  }
}

/**
  * @brief Basic error checking for HAL I2C functions.
  * Prints an error message if the status is not HAL_OK.
  * @param status: The HAL_StatusTypeDef to check.
  */
void AS5600_CheckError(HAL_StatusTypeDef status)
{
  if (status != HAL_OK)
  {
//    printf("I2C Error: %d\r\n", status);  	// 1 = HAL_ERROR, 3 = HAL_TIMEOUT, 4 = HAL_BUSY
    // You might want to implement more robust error handling here,
    // like re-initializing I2C, or going into an error state.
  }
}
#endif
