#ifndef __BSP_MLX906114_H_
#define __BSP_MLX906114_H_

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "event_groups.h"
#include "tim.h"
#include "bsp_delay.h"
#include <stdio.h>

#define ACK             0    // 应答
#define NACK            1    // 无应答
#define SA              0x00 // Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS      0x00 // RAM access command RAM存取命令
#define EEPROM_ACCESS   0x20 // EEPROM access command EEPROM存取命令
#define RAM_TOBJ1       0x07 // To1 address in the eeprom 目标1温度,检测到的红外温度 -70.01 ~ 382.19度

#define SMBUS_PORT      GPIOA       // 端口(端口和下面的两个针脚可自定义)
#define SMBUS_SCK       GPIO_PIN_12 // SCL
#define SMBUS_SDA       GPIO_PIN_11 // SDA

#define SMBUS_SCK_H()   HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SCK, GPIO_PIN_SET)
#define SMBUS_SCK_L()   HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SCK, GPIO_PIN_RESET)
#define SMBUS_SDA_H()   HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SDA, GPIO_PIN_SET)
#define SMBUS_SDA_L()   HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SDA, GPIO_PIN_RESET)

#define SMBUS_SDA_PIN() HAL_GPIO_ReadPin(SMBUS_PORT, SMBUS_SDA)
#define SMBus_Delay     delay_us

void    SMBus_StartBit(void);
void    SMBus_StopBit(void);
void    SMBus_SendBit(uint8_t bit_out);
uint8_t SMBus_SendByte(uint8_t Tx_buffer);
uint8_t SMBus_ReceiveBit(void);
uint8_t SMBus_ReceiveByte(uint8_t ack_nack);
// void     SMBus_Delay(uint16_t time);
void     SMBus_Init(void);
uint16_t SMBus_ReadMemory(uint8_t slaveAddress, uint8_t command);
uint8_t  PEC_Calculation(uint8_t pec[]);
float    SMBus_ReadTemp(void); // 获取温度值

#endif // !__BSP_MLX906114_H_
