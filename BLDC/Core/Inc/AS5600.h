#ifndef AS5600_H
#define AS5600_H

#include "stm32g4xx_hal.h"

#define AS5600_SLAVE_ADDRESS (0x36 << 1)
#define AS5600_STATUS_REG     0x0B
#define AS5600_RAW_ANGLE_REG  0x0C
#define AS5600_ANGLE_REG      0x0E

uint16_t AS5600_ReadAngle(uint8_t angle_reg);
uint8_t  AS5600_ReadStatus(void);
void     AS5600_CheckError(HAL_StatusTypeDef status);

#endif



#if 0
uint16_t AS5600_ReadAngleStatus(void);
uint16_t AS5600_ReadRaw12(uint8_t angle_reg);
HAL_StatusTypeDef AS5600_ReadRegister(uint8_t reg_addr, uint8_t *data, uint16_t len);
uint8_t AS5600_ReadStatus(void);
void AS5600_CheckError(HAL_StatusTypeDef status);
#endif

