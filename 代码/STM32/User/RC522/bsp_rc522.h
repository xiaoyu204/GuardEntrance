#ifndef __BSP_RC522_H
#define __BSP_RC522_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "spi.h"
#include "stdio.h"
#include "usart.h"
#include "bsp_esp32.h"
#include "bsp_sg90.h"
#include "bsp_bit.h"
#include "bsp_esp32.h"
#include "gui_guider.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CARD_FRAME_LENTH        6
#define UID_LENTH               4

/* RC522寄存器定义 */
// PAGE0
#define RC522_RFU00             0x00
#define RC522_CommandReg        0x01
#define RC522_ComIEnReg         0x02
#define RC522_DivlEnReg         0x03
#define RC522_ComIrqReg         0x04
#define RC522_DivIrqReg         0x05
#define RC522_ErrorReg          0x06
#define RC522_Status1Reg        0x07
#define RC522_Status2Reg        0x08
#define RC522_FIFODataReg       0x09
#define RC522_FIFOLevelReg      0x0A
#define RC522_WaterLevelReg     0x0B
#define RC522_ControlReg        0x0C
#define RC522_BitFramingReg     0x0D
#define RC522_CollReg           0x0E
#define RC522_RFU0F             0x0F
// PAGE1
#define RC522_RFU10             0x10
#define RC522_ModeReg           0x11
#define RC522_TxModeReg         0x12
#define RC522_RxModeReg         0x13
#define RC522_TxControlReg      0x14
#define RC522_TxAutoReg         0x15 // 中文手册有误
#define RC522_TxSelReg          0x16
#define RC522_RxSelReg          0x17
#define RC522_RxThresholdReg    0x18
#define RC522_DemodReg          0x19
#define RC522_RFU1A             0x1A
#define RC522_RFU1B             0x1B
#define RC522_MifareReg         0x1C
#define RC522_RFU1D             0x1D
#define RC522_RFU1E             0x1E
#define RC522_SerialSpeedReg    0x1F
// PAGE2
#define RC522_RFU20             0x20
#define RC522_CRCResultRegM     0x21
#define RC522_CRCResultRegL     0x22
#define RC522_RFU23             0x23
#define RC522_ModWidthReg       0x24
#define RC522_RFU25             0x25
#define RC522_RFCfgReg          0x26
#define RC522_GsNReg            0x27
#define RC522_CWGsCfgReg        0x28
#define RC522_ModGsCfgReg       0x29
#define RC522_TModeReg          0x2A
#define RC522_TPrescalerReg     0x2B
#define RC522_TReloadRegH       0x2C
#define RC522_TReloadRegL       0x2D
#define RC522_TCounterValueRegH 0x2E
#define RC522_TCounterValueRegL 0x2F
// PAGE3
#define RC522_RFU30             0x30
#define RC522_TestSel1Reg       0x31
#define RC522_TestSel2Reg       0x32
#define RC522_TestPinEnReg      0x33
#define RC522_TestPinValueReg   0x34
#define RC522_TestBusReg        0x35
#define RC522_AutoTestReg       0x36
#define RC522_VersionReg        0x37
#define RC522_AnalogTestReg     0x38
#define RC522_TestDAC1Reg       0x39
#define RC522_TestDAC2Reg       0x3A
#define RC522_TestADCReg        0x3B
#define RC522_RFU3C             0x3C
#define RC522_RFU3D             0x3D
#define RC522_RFU3E             0x3E
#define RC522_RFU3F             0x3F

/* RC522的FIFO长度定义 */
#define RC522_FIFO_LENGTH       64

/* RC522传输的帧长定义 */
#define RC522_MAXRLEN           18

/* RC522命令集,中文手册P59 */
#define RC522_IDLE              0x00 // 取消当前命令的执行
#define RC522_CALCCRC           0x03 // 激活CRC计算
#define RC522_TRANSMIT          0x04 // 发送FIFO缓冲区内容
#define RC522_NOCMDCHANGE       0x07 // 无命令改变
#define RC522_RECEIVE           0x08 // 激活接收器接收数据
#define RC522_TRANSCEIVE        0x0C // 发送并接收数据
#define RC522_AUTHENT           0x0E // 执行Mifare认证(验证密钥)
#define RC522_RESETPHASE        0x0F // 复位RC522

/* Mifare1卡片命令字 */
#define PICC_REQIDL             0x26 // 寻天线区内未进入休眠状态的卡
#define PICC_REQALL             0x52 // 寻天线区内全部卡
#define PICC_ANTICOLL1          0x93 // 防冲撞
#define PICC_ANTICOLL2          0x95 // 防冲撞
#define PICC_AUTHENT1A          0x60 // 验证A密钥
#define PICC_AUTHENT1B          0x61 // 验证B密钥
#define PICC_READ               0x30 // 读块
#define PICC_WRITE              0xA0 // 写块
#define PICC_DECREMENT          0xC0 // 减值(扣除)
#define PICC_INCREMENT          0xC1 // 增值(充值)
#define PICC_TRANSFER           0xB0 // 转存(传送)
#define PICC_RESTORE            0xC2 // 恢复(重储)
#define PICC_HALT               0x50 // 休眠

/* RS522通讯时返回的错误代码 */
#define RS522_OK                (char)0    // 成功
#define RS522_NOTAGERR          (char)(-1) // 无卡
#define RS522_ERR               (char)(-2) // 出错

/* RC522通讯时返回的错误代码 */
#define RC522_OK                (char)0
#define RC522_NOTAGERR          (char)(-1)
#define RC522_ERR               (char)(-2)

#define RC522_SPI               hspi1
#define osDelay                 vTaskDelay
#define RS522_RST_Disable()     HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, GPIO_PIN_SET)
#define RS522_RST_Enable()      HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, GPIO_PIN_RESET)
#define RS522_CS_Disable()      HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_SET)
#define RS522_CS_Enable()       HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET)
#define MAX_USERS               10 // 最大的用户数

enum FRAME_TYPE {
    ADD = 0,
    DELETE,
};

/* RC522函数声明 */
void    RC522_WriteReg(uint8_t addr, uint8_t data);
uint8_t RC522_ReadReg(uint8_t addr);
void    RC522_SetBitMask(uint8_t addr, uint8_t mask);
void    RC522_ClrBitMask(uint8_t addr, uint8_t mask);
void    RC522_CalulateCRC(uint8_t *pInData, uint8_t len, uint8_t *pOutData);
char    RC522_CmdFrame(uint8_t cmd, uint8_t *pInData, uint8_t InLenByte, uint8_t *pOutData, uint16_t *pOutLenBit);
void    RC522_Init(void);
void    RS522_Reset(void);
void    RS522_AntennaOn(void);
void    RS522_AntennaOff(void);
char    RS522_Request(uint8_t RequestMode, uint8_t *pCardType); // 寻卡，并返回卡的类型
char    RS522_Anticoll(uint8_t *pSnr);                          // 防冲突,返回卡号
char    RS522_Select(uint8_t *pSnr);                            // 选卡
char    RS522_AuthState(uint8_t AuthMode, uint8_t BlockAddr, uint8_t *pKey,
                        uint8_t *pSnr);                      // 验证密码(密码A和密码B)
char    RS522_WriteBlock(uint8_t BlockAddr, uint8_t *pData); // 写数据
char    RS522_ReadBlock(uint8_t BlockAddr, uint8_t *pData);  // 读数据
char    RS522_Value(uint8_t mode, uint8_t BlockAddr, uint8_t *pValue);
char    RS522_BakValue(uint8_t sourceBlockAddr, uint8_t goalBlockAddr);
char    RS522_Halt(void);

/* 应用层函数 */
void    RC522_Task(void *pvParameters);
bool    Scan_Card(uint8_t *readUID);
uint8_t ReadCard(uint8_t *readUID, void (*funCallBack)(void));
// void    RegisterCard(const uint8_t (*writeUIDs)[4], uint8_t rows);
void RegisterCard(const uint8_t *writeUID);
// void DeleteCard(uint8_t (*deleteUIDs)[4], uint8_t rows);
void DeleteCard(const uint8_t *deleteUID);
void Send_Card(const uint8_t *writeUID, enum FRAME_TYPE type);

#endif // __BSP_RC522_H
