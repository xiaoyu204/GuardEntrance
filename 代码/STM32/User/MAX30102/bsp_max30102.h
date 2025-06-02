/**
 * ************************************************************************
 *
 * @file MAX30102.h
 * @author zxr
 * @brief
 *
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr
 * ************************************************************************
 */
#ifndef __BSP_MAX30102_H_
#define __BSP_MAX30102_H_

#include "main.h" // Device header
#include "i2c.h"
#include "stdbool.h"

#define MAX30102_Device_address 0xAE

// register addresses
#define REG_INTR_STATUS_1       0x00
#define REG_INTR_STATUS_2       0x01
#define REG_INTR_ENABLE_1       0x02
#define REG_INTR_ENABLE_2       0x03
#define REG_FIFO_WR_PTR         0x04
#define REG_OVF_COUNTER         0x05
#define REG_FIFO_RD_PTR         0x06
#define REG_FIFO_DATA           0x07
#define REG_FIFO_CONFIG         0x08
#define REG_MODE_CONFIG         0x09
#define REG_SPO2_CONFIG         0x0A
#define REG_LED1_PA             0x0C
#define REG_LED2_PA             0x0D
#define REG_PILOT_PA            0x10
#define REG_MULTI_LED_CTRL1     0x11
#define REG_MULTI_LED_CTRL2     0x12
#define REG_TEMP_INTR           0x1F
#define REG_TEMP_FRAC           0x20
#define REG_TEMP_CONFIG         0x21
#define REG_PROX_INT_THRESH     0x30
#define REG_REV_ID              0xFE
#define REG_PART_ID             0xFF

#define SAMPLES_PER_SECOND      100 // 检测频率

uint8_t Max30102_reset(void);
void    MAX30102_Config(void);
void    max30102_read_fifo(void);

uint8_t max30102_write_reg(uint8_t addr, uint8_t data);
uint8_t max30102_read_reg(uint8_t addr);

#endif
