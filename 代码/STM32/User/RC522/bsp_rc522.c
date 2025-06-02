#include "bsp_rc522.h"

static uint8_t  USER_NUM = 0;   // 当前的用户数
static uint8_t *UID[MAX_USERS]; // 用于存储每个用户的UID

/**
 * @brief   发送需要的卡号给ESP
 * @param   无
 * @retval  无
 */
void Send_Card(const uint8_t *writeUID, enum FRAME_TYPE type)
{
    uint8_t send_frame[CARD_FRAME_LENTH];

    if (type == ADD) {
        if ((USER_NUM - 1) >= MAX_USERS) {
            printf("users is full\n");
            // printf("register sucessed %d\n", i / UID_LENTH);
            return;
        }

        /* 检查是否已经存在相同的卡号 */
        for (uint8_t i = 0; i < USER_NUM; i++) {
            if (!strncmp((char *)UID[i], (char *)writeUID, UID_LENTH)) {
                printf("card: %d-%d-%d-%d already registered\n", writeUID[0], writeUID[1], writeUID[2], writeUID[3]);
                return;
            }
        }

        send_frame[0]                    = CMD_ADD_UID;
        send_frame[CARD_FRAME_LENTH - 1] = CMD_FRAME_END;
        memcpy(send_frame + 1, writeUID, UID_LENTH);
        if (HAL_UART_Transmit(&ESP32_UART, send_frame, CARD_FRAME_LENTH, 1000) != HAL_OK) {
            printf("send card failed\n");
            return;
        }

        RegisterCard(writeUID);
        printf("send card susccess\n");
    } else if (type == DELETE) {
        send_frame[0]                    = CMD_DELETE_CARD;
        send_frame[CARD_FRAME_LENTH - 1] = CMD_FRAME_END;
        memcpy(send_frame + 1, writeUID, UID_LENTH);
        if (HAL_UART_Transmit(&ESP32_UART, send_frame, CARD_FRAME_LENTH, 1000) != HAL_OK) {
            printf("send card failed\n");
            return;
        }
        DeleteCard(writeUID);
    }
}

/**
 * @brief   注册卡号
 * @param   writeUIDs:卡号缓冲区
 * @retval  无
 */
// void RegisterCard(const uint8_t (*writeUIDs)[UID_LENTH], uint8_t rows)
void RegisterCard(const uint8_t *writeUID)
{
    uint8_t *temp = (uint8_t *)malloc(sizeof(uint8_t) * UID_LENTH);

    if (temp == NULL) {
        printf("malloc failed\n");
        // printf("register sucessed %d\n", i / UID_LENTH);
        return;
    } else {
        memcpy(temp, writeUID, UID_LENTH);
        UID[USER_NUM] = temp;
        // printf("card: %d-%d-%d-%d registerd sucessed\n", temp[0], temp[1], temp[2], temp[3]);
        printf("card: 0x%X-0x%X-0x%X-0x%X registerd sucessed\n", temp[0], temp[1], temp[2], temp[3]);
        writeUID++; // 切换到下一行
        USER_NUM++; // 用户数加1
    }

    printf("register card susccess\n");
    printf("user num is %d\n", USER_NUM);
}

/**
 * @brief   删除卡号
 * @param   deleteUIDs: 要删除的卡号缓冲区
 * @param   rows: 要删除的卡号数量
 * @retval  无
 */
// void DeleteCard(uint8_t (*deleteUIDs)[UID_LENTH], uint8_t rows)
void DeleteCard(const uint8_t *deleteUID)
{
    bool found = false;

    for (uint8_t i = 0; i < USER_NUM; i++) {
        if (!strncmp((char *)UID[i], (char *)deleteUID, UID_LENTH)) {
            found = true;
            free(UID[i]); // 释放内存
            // 将后面的UID向前移动
            for (uint8_t j = i; j < USER_NUM - 1; j++) {
                UID[j] = UID[j + 1];
            }
            USER_NUM--; // 用户数减1
            break;
        }
    }

    if (!found) {
        printf("card: %d-%d-%d-%d not found\n", deleteUID[0], deleteUID[1], deleteUID[2], deleteUID[3]);
    }

    printf("delete card success\n");
    printf("user num is %d\n", USER_NUM);
}

/**
 * @brief   读取卡号
 * @param   byte:要发送的数据
 * @retval  接收到的数据
 */
uint8_t ReadCard(uint8_t *readUID, void (*funCallBack)(void))
{
    uint8_t Temp[5];

    if (RS522_Request(0x52, Temp) == 0) {
        if (RS522_Anticoll(readUID) == 0) {
            if (funCallBack != NULL)
                funCallBack();
            return 0;
        }
    }

    return 1;
}

/**
 * @brief   扫描是否卡号
 * @param   无
 * @retval  true: 有 false:无
 */
bool Scan_Card(uint8_t *readUID)
{
    const TickType_t timeout = pdMS_TO_TICKS(5000); // 5秒超时

    // 设置5秒超时标志
    bool       timeoutOccurred = false;
    TickType_t startTime       = xTaskGetTickCount();

    // 尝试在5秒内读取卡片
    while (ReadCard(readUID, NULL)) {
        // 检查是否超时
        if ((xTaskGetTickCount() - startTime) >= timeout) {
            printf("The card was not read within 5 seconds\n");
            timeoutOccurred = true;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // 短暂延迟防止忙等待
    }

    // 如果超时则跳过后续处理
    if (timeoutOccurred) {
        return false;
    }

    return true;
}

/**
 * @brief   RC522任务
 * @param   pvParameters: 任务参数
 * @retval  无
 */
void RC522_Task(void *pvParameters)
{
    uint8_t                   readUID[UID_LENTH];
    extern EventGroupHandle_t DoorEventGroup;

    while (1) {
        // 等待RC522任务位被设置
        xEventGroupWaitBits(DoorEventGroup, RC522_TASK_BIT, pdTRUE, pdFALSE, portMAX_DELAY);

        if (!Scan_Card(readUID)) {
            continue;
        }

        // 以下是原来的卡片处理逻辑
        bool find = false;
        printf("card: 0x%X-0x%X-0x%X-0x%X\n", readUID[0], readUID[1], readUID[2], readUID[3]);

        for (uint8_t i = 0; i < USER_NUM; i++) {
            if (!strncmp((char *)readUID, (char *)UID[i], UID_LENTH)) {
                extern Attributes_T attributes;
                extern lv_ui        guider_ui;
                uint8_t             cmd[4];

                find = true;
                attributes.visitors++;
                printf("card correct\r\n");
                xEventGroupSetBits(DoorEventGroup, SG90_TASK_BIT);
                lv_label_set_text_fmt(guider_ui.main_label_visitor, "%d 人", attributes.visitors);
                cmd[0] = CMD_PUB;
                cmd[1] = attributes.alarm;
                cmd[2] = attributes.visitors;
                cmd[3] = 0x5A;
                HAL_UART_Transmit(&hlpuart1, cmd, 4, HAL_MAX_DELAY);
                lv_obj_clear_flag(guider_ui.main_card_correct, LV_OBJ_FLAG_HIDDEN);
                vTaskDelay(1000);
                lv_obj_add_flag(guider_ui.main_card_correct, LV_OBJ_FLAG_HIDDEN);
                break;
            }
        }

        if (!find) {
            printf("card error\r\n");
            lv_obj_clear_flag(guider_ui.main_card_error, LV_OBJ_FLAG_HIDDEN);
            vTaskDelay(1000);
            lv_obj_add_flag(guider_ui.main_card_error, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

/**
 * @brief   SPI读写一个字节
 * @param   byte:要发送的数据
 * @retval  接收到的数据
 */
static uint8_t ret; // 这些函数是HAL与标准库不同的地方【读写函数】
uint8_t        SPI_RW_Byte(uint8_t byte)
{
    /* 把byte 写入，并读出一个值，把它存入ret */
    HAL_SPI_TransmitReceive(&RC522_SPI, &byte, &ret, 1, 10);

    /* 入口是byte 的地址，读取时用的也是ret地址，一次只写入一个值10 */
    return ret;
}

/**
 * @brief   写一个寄存器
 * @param   addr:待写的寄存器地址
 * @param   data:待写的寄存器数据
 * @retval  无
 */
void RC522_WriteReg(uint8_t addr, uint8_t data)
{
    uint8_t AddrByte;
    AddrByte = (addr << 1) & 0x7E; // 求出地址字节
    RS522_CS_Enable();             // CS拉低
    SPI_RW_Byte(AddrByte);         // 写地址字节
    SPI_RW_Byte(data);             // 写数据
    RS522_CS_Disable();            // CS拉高
}

/**
 * @brief   读一个寄存器
 * @param   addr:待读的寄存器地址
 * @param   data:读到寄存器的数据
 * @retval  无
 */
uint8_t RC522_ReadReg(uint8_t addr)
{
    uint8_t AddrByte, data;

    AddrByte = ((addr << 1) & 0x7E) | 0x80; // 求出地址字节
    RS522_CS_Enable();                      // CS拉低
    SPI_RW_Byte(AddrByte);                  // 写地址字节
    data = SPI_RW_Byte(0x00);               // 读数据
    RS522_CS_Disable();                     // CS拉高

    return data;
}

/**
 * @brief   设置寄存器的位
 * @param   addr:待设置的寄存器地址
 * @param   mask:待设置寄存器的位(可同时设置多个bit)
 * @retval  无
 */
void RC522_SetBitMask(uint8_t addr, uint8_t mask)
{
    uint8_t temp;

    temp = RC522_ReadReg(addr);        // 先读回寄存器的值
    RC522_WriteReg(addr, temp | mask); // 处理过的数据再写入寄存器
}

/**
 * @brief   清除寄存器的位
 * @param   addr:待清除的寄存器地址
 * @param   mask:待清除寄存器的位(可同时清除多个bit)
 * @retval  无
 */
void RC522_ClrBitMask(uint8_t addr, uint8_t mask)
{
    uint8_t temp;

    temp = RC522_ReadReg(addr);         // 先读回寄存器的值
    RC522_WriteReg(addr, temp & ~mask); // 处理过的数据再写入寄存器
}

/**
 * @brief   用RC522计算CRC结果
 * @param   pInData：带进行CRC计算的数据
 * @param   len：带进行CRC计算的数据长度
 * @param   pOutData：CRC计算结果
 * @retval  无
 */
void RC522_CalulateCRC(uint8_t *pInData, uint8_t len, uint8_t *pOutData)
{
    // 0xc1 1        2           pInData[2]
    uint8_t  temp;
    uint32_t i;

    RC522_ClrBitMask(RC522_DivIrqReg, 0x04);      // 使能CRC中断
    RC522_WriteReg(RC522_CommandReg, RC522_IDLE); // 取消当前命令的执行
    RC522_SetBitMask(RC522_FIFOLevelReg, 0x80);   // 清除FIFO及其标志位

    for (i = 0; i < len; i++) // 将待CRC计算的数据写入FIFO
    {
        RC522_WriteReg(RC522_FIFODataReg, *(pInData + i));
    }

    RC522_WriteReg(RC522_CommandReg, RC522_CALCCRC); // 执行CRC计算

    i = 100000;
    do {
        temp = RC522_ReadReg(RC522_DivIrqReg); // 读取DivIrqReg寄存器的值
        i--;
    } while ((i != 0) && !(temp & 0x04)); // 等待CRC计算完成

    pOutData[0] = RC522_ReadReg(RC522_CRCResultRegL); // 读取CRC计算结果
    pOutData[1] = RC522_ReadReg(RC522_CRCResultRegM);
}

/**
 * @brief   RC522522和ISO14443A卡通讯的命令帧函数
 * @param   cmd：RC522522发送给MF1卡的命令
 * @param   pIndata：RC522522发送给MF1卡的数据的缓冲区首地址
 * @param   InLenByte：发送数据的字节长度
 * @param   pOutdata：用于接收MF1卡片返回数据的缓冲区首地址
 * @param   pOutLenBit：MF1卡返回数据的位长度
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RC522_CmdFrame(uint8_t cmd, uint8_t *pInData, uint8_t InLenByte, uint8_t *pOutData, uint16_t *pOutLenBit)
{
    uint8_t  lastBits;
    uint8_t  n;
    uint32_t i;
    char     status  = RC522_ERR;
    uint8_t  irqEn   = 0x00;
    uint8_t  waitFor = 0x00;

    /*根据命令设置标志位*/
    switch (cmd) {
        case RC522_AUTHENT: // Mifare认证
            irqEn   = 0x12;
            waitFor = 0x10; // idleIRq中断标志
            break;
        case RC522_TRANSCEIVE: // 发送并接收数据
            irqEn   = 0x77;
            waitFor = 0x30; // RxIRq和idleIRq中断标志
            break;
    }

    /*发送命令帧前准备*/
    RC522_WriteReg(RC522_ComIEnReg, irqEn | 0x80); // 开中断
    RC522_ClrBitMask(RC522_ComIrqReg, 0x80);       // 清除中断标志位SET1
    RC522_WriteReg(RC522_CommandReg, RC522_IDLE);  // 取消当前命令的执行
    RC522_SetBitMask(RC522_FIFOLevelReg, 0x80);    // 清除FIFO缓冲区及其标志位

    /*发送命令帧*/
    for (i = 0; i < InLenByte; i++) // 写入命令参数
    {
        RC522_WriteReg(RC522_FIFODataReg, pInData[i]);
    }

    RC522_WriteReg(RC522_CommandReg, cmd); // 执行命令

    if (cmd == RC522_TRANSCEIVE) {
        RC522_SetBitMask(RC522_BitFramingReg, 0x80); // 启动发送
    }

    i = 300000; // 根据时钟频率调整,操作M1卡最大等待时间25ms
    do {
        n = RC522_ReadReg(RC522_ComIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor)); // 等待命令完成

    RC522_ClrBitMask(RC522_BitFramingReg, 0x80); // 停止发送

    /*处理接收的数据*/
    if (i != 0) {
        if (!(RC522_ReadReg(RC522_ErrorReg) & 0x1B)) {
            status = RC522_OK;
            if (n & irqEn & 0x01) {
                status = RC522_NOTAGERR;
            }
            if (cmd == RC522_TRANSCEIVE) {
                n        = RC522_ReadReg(RC522_FIFOLevelReg);
                lastBits = RC522_ReadReg(RC522_ControlReg) & 0x07;
                if (lastBits) {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                } else {
                    *pOutLenBit = n * 8;
                }
                if (n == 0) {
                    n = 1;
                }
                if (n > RC522_MAXRLEN) {
                    n = RC522_MAXRLEN;
                }
                for (i = 0; i < n; i++) {
                    pOutData[i] = RC522_ReadReg(RC522_FIFODataReg);
                }
            }
        } else {
            status = RC522_ERR;
        }
    }

    RC522_SetBitMask(RC522_ControlReg, 0x80);     // 停止定时器运行
    RC522_WriteReg(RC522_CommandReg, RC522_IDLE); // 取消当前命令的执行

    return status;
}

/**
 * @brief   对MF1卡数据块增减值操作
 * @retval  无
 */
void RS522_Reset(void)
{
    /*硬复位*/
    RS522_RST_Disable(); // 用到复位引脚
    osDelay(2);
    RS522_RST_Enable();
    osDelay(2);
    RS522_RST_Disable();
    osDelay(2);

    /*软复位*/
    RC522_WriteReg(RC522_CommandReg, RC522_RESETPHASE);
    osDelay(2);

    /*复位后的初始化配置*/
    RC522_WriteReg(RC522_ModeReg, 0x3D);   // CRC初始值0x6363
    RC522_WriteReg(RC522_TReloadRegL, 30); // 定时器重装值
    RC522_WriteReg(RC522_TReloadRegH, 0);
    RC522_WriteReg(RC522_TModeReg, 0x8D);      // 定时器设置
    RC522_WriteReg(RC522_TPrescalerReg, 0x3E); // 定时器预分频值
    RC522_WriteReg(RC522_TxAutoReg, 0x40);     // 100%ASK

    RS522_AntennaOff(); // 关天线
    osDelay(2);
    RS522_AntennaOn(); // 开天线
}

/**
 * @brief   开启天线,使能RS522发送能量载波信号
 * @note    每次开启或关闭天线之间应至少有1ms的间隔
 * @retval  无
 */
void RS522_AntennaOn(void)
{
    uint8_t temp;

    temp = RC522_ReadReg(RC522_TxControlReg);

    if (!(temp & 0x03)) {
        RC522_SetBitMask(RC522_TxControlReg, 0x03);
    }
}

/**
 * @brief   关闭天线,失能RS522发送能量载波信号
 * @note    每次开启或关闭天线之间应至少有1ms的间隔
 * @retval  无
 */
void RS522_AntennaOff(void)
{
    RC522_ClrBitMask(RC522_TxControlReg, 0x03);
}

/**
 * @brief   读写器初始化
 * @retval  无
 */
void RC522_Init(void)
{
    RS522_CS_Disable(); // 设置默认电平
    RS522_RST_Disable();
    RS522_Reset(); // RS522复位  并初始化配置

    printf("RC522 Init Success\r\n");
}

/**
 * @brief   寻卡
 * @param   RequestMode：寻卡方式
 *                       = PICC_REQIDL：寻天线区内未进入休眠状态
 *                       = PICC_REQALL：寻天线区内全部卡
 * @param   pCardType：用于保存卡片类型
 *                     = 0x4400：Mifare_UltraLight
 *                     = 0x0400：Mifare_One(S50)
 *                     = 0x0200：Mifare_One(S70)
 *                     = 0x0800：Mifare_Pro(X)
 *                     = 0x4403：Mifare_DESFire
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_Request(uint8_t RequestMode, uint8_t *pCardType)
{
    int      status;
    uint16_t unLen;
    uint8_t  CmdFrameBuf[RC522_MAXRLEN];

    RC522_ClrBitMask(RC522_Status2Reg, 0x08);   // 关内部温度传感器
    RC522_WriteReg(RC522_BitFramingReg, 0x07);  // 存储模式，发送模式，是否启动发送等
    RC522_SetBitMask(RC522_TxControlReg, 0x03); // 配置调制信号13.56MHZ

    CmdFrameBuf[0] = RequestMode;

    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 1, CmdFrameBuf, &unLen);

    if ((status == RS522_OK) && (unLen == 0x10)) {
        *pCardType       = CmdFrameBuf[0];
        *(pCardType + 1) = CmdFrameBuf[1];
    }

    return status;
}

/**
 * @brief   防冲突,获取卡号
 * @param   pSnr：用于保存卡片序列号,4字节
 * @param   pSnr：卡片序列号,4字节
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_Anticoll(uint8_t *pSnr)
{
    char     status;
    uint8_t  i, snr_check = 0;
    uint16_t unLen;
    uint8_t  CmdFrameBuf[RC522_MAXRLEN];

    RC522_ClrBitMask(RC522_Status2Reg, 0x08);
    RC522_WriteReg(RC522_BitFramingReg, 0x00);
    RC522_ClrBitMask(RC522_CollReg, 0x80);

    CmdFrameBuf[0] = PICC_ANTICOLL1;
    CmdFrameBuf[1] = 0x20;

    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 2, CmdFrameBuf, &unLen);

    if (status == RS522_OK) {
        for (i = 0; i < 4; i++) {
            *(pSnr + i) = CmdFrameBuf[i];
            snr_check ^= CmdFrameBuf[i];
        }
        if (snr_check != CmdFrameBuf[i]) {
            status = RS522_ERR;
        }
    }

    RC522_SetBitMask(RC522_CollReg, 0x80);
    return status;
}

/**
 * @brief   选卡
 * @param   pSnr：卡片序列号,4字节
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_Select(uint8_t *pSnr)
{
    char     status;
    uint8_t  i;
    uint16_t unLen;
    uint8_t  CmdFrameBuf[RC522_MAXRLEN];

    CmdFrameBuf[0] = PICC_ANTICOLL1;
    CmdFrameBuf[1] = 0x70;
    CmdFrameBuf[6] = 0;
    for (i = 0; i < 4; i++) {
        CmdFrameBuf[i + 2] = *(pSnr + i);
        CmdFrameBuf[6] ^= *(pSnr + i);
    }
    RC522_CalulateCRC(CmdFrameBuf, 7, &CmdFrameBuf[7]);

    RC522_ClrBitMask(RC522_Status2Reg, 0x08);

    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 9, CmdFrameBuf, &unLen);

    if ((status == RS522_OK) && (unLen == 0x18)) {
        status = RS522_OK;
    } else {
        status = RS522_ERR;
    }
    return status;
}

/**
 * @brief   验证卡片密码
 * @param  AuthMode：验证模式
 *                = PICC_AUTHENT1A：验证A密码
 *                = PICC_AUTHENT1B：验证B密码
 * @param   BlockAddr：块地址(0~63)
 * @param   pKey：密码
 * @param   pSnr：卡片序列号,4字节
 * @note    验证密码时,以扇区为单位,BlockAddr参数可以是同一个扇区的任意块
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_AuthState(uint8_t AuthMode, uint8_t BlockAddr, uint8_t *pKey, uint8_t *pSnr)
{
    char     status;
    uint16_t unLen;
    uint8_t  i, CmdFrameBuf[RC522_MAXRLEN];
    CmdFrameBuf[0] = AuthMode;
    CmdFrameBuf[1] = BlockAddr;
    for (i = 0; i < 6; i++) {
        CmdFrameBuf[i + 2] = *(pKey + i);
    }
    for (i = 0; i < 4; i++) {
        CmdFrameBuf[i + 8] = *(pSnr + i);
    }

    status = RC522_CmdFrame(RC522_AUTHENT, CmdFrameBuf, 12, CmdFrameBuf, &unLen);
    if ((status != RS522_OK) || (!(RC522_ReadReg(RC522_Status2Reg) & 0x08))) {
        status = RS522_ERR;
    }

    return status;
}

/**
 * @brief   读MF1卡数据块
 * @param   BlockAddr：块地址
 * @param   pData: 用于保存待写入的数据,16字节
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_WriteBlock(uint8_t BlockAddr, uint8_t *pData)
{
    char     status;
    uint16_t unLen;
    uint8_t  i, CmdFrameBuf[RC522_MAXRLEN];

    CmdFrameBuf[0] = PICC_WRITE;
    CmdFrameBuf[1] = BlockAddr;
    RC522_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

    if ((status != RS522_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
        status = RS522_ERR;
    }

    if (status == RS522_OK) {
        for (i = 0; i < 16; i++) {
            CmdFrameBuf[i] = *(pData + i);
        }
        RC522_CalulateCRC(CmdFrameBuf, 16, &CmdFrameBuf[16]);

        status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 18, CmdFrameBuf, &unLen);

        if ((status != RS522_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
            status = RS522_ERR;
        }
    }

    return status;
}

/**
 * @brief   读MF1卡数据块
 * @param   BlockAddr：块地址
 * @param   pData: 用于保存读出的数据,16字节
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_ReadBlock(uint8_t BlockAddr, uint8_t *pData)
{
    char     status;
    uint16_t unLen;
    uint8_t  i, CmdFrameBuf[RC522_MAXRLEN];

    CmdFrameBuf[0] = PICC_READ;
    CmdFrameBuf[1] = BlockAddr;
    RC522_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
    if ((status == RS522_OK) && (unLen == 0x90)) {
        for (i = 0; i < 16; i++) {
            *(pData + i) = CmdFrameBuf[i];
        }
    } else {
        status = RS522_ERR;
    }

    return status;
}

/**
 * @brief   对MF1卡数据块增减值操作
 * @param   mode：数值块操作模式
 *                = PICC_INCREMENT：增值
 *                = PICC_DECREMENT：减值
 * @param   BlockAddr：块地址
 * @param   pValue：四字节增值的值,低位在前
 * @note    只能在同一个扇区内转存
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_Value(uint8_t mode, uint8_t BlockAddr, uint8_t *pValue)
{
    // 0XC1        1           Increment[4]={0x03, 0x01, 0x01, 0x01};
    char     status;
    uint16_t unLen;
    uint8_t  i, CmdFrameBuf[RC522_MAXRLEN];

    CmdFrameBuf[0] = mode;
    CmdFrameBuf[1] = BlockAddr;
    RC522_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

    if ((status != RS522_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
        status = RS522_ERR;
    }

    if (status == RS522_OK) {
        for (i = 0; i < 16; i++) {
            CmdFrameBuf[i] = *(pValue + i);
        }
        RC522_CalulateCRC(CmdFrameBuf, 4, &CmdFrameBuf[4]);
        unLen  = 0;
        status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 6, CmdFrameBuf, &unLen);
        if (status != RS522_ERR) {
            status = RS522_OK;
        }
    }

    if (status == RS522_OK) {
        CmdFrameBuf[0] = PICC_TRANSFER;
        CmdFrameBuf[1] = BlockAddr;
        RC522_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

        status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

        if ((status != RS522_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
            status = RS522_ERR;
        }
    }
    return status;
}

/**
 * @brief   备份钱包(块转存)
 * @param   sourceBlockAddr：源块地址
 * @param   goalBlockAddr：目标块地址
 * @note    只能在同一个扇区内转存
 * @retval  status：错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_BakValue(uint8_t sourceBlockAddr, uint8_t goalBlockAddr)
{
    char     status;
    uint16_t unLen;
    uint8_t  CmdFrameBuf[RC522_MAXRLEN];

    CmdFrameBuf[0] = PICC_RESTORE;
    CmdFrameBuf[1] = sourceBlockAddr;
    RC522_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);
    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
    if ((status != RS522_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
        status = RS522_ERR;
    }

    if (status == RS522_OK) {
        CmdFrameBuf[0] = 0;
        CmdFrameBuf[1] = 0;
        CmdFrameBuf[2] = 0;
        CmdFrameBuf[3] = 0;
        RC522_CalulateCRC(CmdFrameBuf, 4, &CmdFrameBuf[4]);
        status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 6, CmdFrameBuf, &unLen);
        if (status != RS522_ERR) {
            status = RS522_OK;
        }
    }

    if (status != RS522_OK) {
        return RS522_ERR;
    }

    CmdFrameBuf[0] = PICC_TRANSFER;
    CmdFrameBuf[1] = goalBlockAddr;
    RC522_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);
    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
    if ((status != RS522_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
        status = RS522_ERR;
    }

    return status;
}

/**
 * @brief   命令卡片进入休眠状态
 * @retval  status: 错误代码(RS522_OK、RS522_NOTAGERR、RS522_ERR)
 */
char RS522_Halt(void)
{
    char     status;
    uint16_t unLen;
    uint8_t  CmdFrameBuf[RC522_MAXRLEN];

    CmdFrameBuf[0] = PICC_HALT;
    CmdFrameBuf[1] = 0;
    RC522_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

    status = RC522_CmdFrame(RC522_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

    return status;
}
