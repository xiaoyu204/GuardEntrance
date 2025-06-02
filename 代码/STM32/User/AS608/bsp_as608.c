#include "bsp_as608.h"

/* 串口接收缓存区 */
uint8_t           AS608_RX_BUF[AS608_MAX_RECV_LEN]; // 接收缓冲,最大AS608_MAX_RECV_LEN个字节.
uint32_t          AS608Addr           = 0XFFFFFFFF; // 默认
uint8_t           Get_Device_Code[10] = {0x01, 0x00, 0x07, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b}; // 口令验证
SysPara           AS608Para;
uint8_t           ensure1;
uint16_t          ValidN;           // 模块内有效指纹个数
uint8_t           AS608_RX_STA = 0; // 串口是否接收到数据
SemaphoreHandle_t AS608Semphore;

/**
 * @brief   AS608指纹识别任务
 * @param   pvParameters: 任务参数
 * @retval  无
 */
void AS608_Task(void *pvParameters)
{
    // extern EventGroupHandle_t DoorEventGroup;

    while (1) {
        // xEventGroupWaitBits(DoorEventGroup, AS608_TASK_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
        xTaskNotifyWait(0, AS608_TASK_BIT, NULL, portMAX_DELAY);
        Verify_FR();
    }
}

/**
 * @brief   录指纹
 * @retval  无
 */
void Add_FR(uint8_t ID_NUM)
{
    uint8_t        retry_count = 0, ensure = 0;

    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn); // 禁用对应的 EXTI 中断

    // 第一步：第一次采集指纹
    while (1) {
        printf("Please put your finger\n");
        ensure = PS_GetImage();

        if (ensure == 0x00) {
            ensure = PS_GenChar(CharBuffer1); // 生成特征
            if (ensure == 0x00) {
                printf("Fingerprints are normal\n");
                break; // 成功，进入下一步
            } else {
                printf(EnsureMessage(ensure));
            }
        } else {
            printf(EnsureMessage(ensure));
        }

        retry_count++;
        if (retry_count >= 10) { // 超过10次失败则退出
            printf("Timeout, operation canceled\n");
            return;
        }
        vTaskDelay(400);
    }

    retry_count = 0; // 重置重试计数

    // 第二步：第二次采集指纹
    while (1) {
        printf("Please press again\n");
        ensure = PS_GetImage();

        if (ensure == 0x00) {
            ensure = PS_GenChar(CharBuffer2); // 生成特征
            if (ensure == 0x00) {
                printf("Fingerprints are normal\n");
                break; // 成功，进入下一步
            } else {
                printf(EnsureMessage(ensure));
            }
        } else {
            printf(EnsureMessage(ensure));
        }

        retry_count++;
        if (retry_count >= 10) { // 超过10次失败则退出
            printf("Timeout, operation canceled\n");
            return;
        }
        vTaskDelay(400);
    }

    // 第三步：比对两次指纹
    printf("Compare the two fingerprints\n");
    ensure = PS_Match();

    if (ensure != 0x00) {
        printf("Comparison fail\n");
        printf(EnsureMessage(ensure));
        return; // 比对失败，直接退出
    }
    printf("Comparison success\n");
    vTaskDelay(500);

    // 第四步：生成指纹模板
    printf("Generate a fingerprint template\n");
    ensure = PS_RegModel();

    if (ensure != 0x00) {
        printf(EnsureMessage(ensure));
        return; // 生成模板失败，直接退出
    }
    printf("The fingerprint template was generated successfully\n");
    vTaskDelay(1000);

    // 第五步：存储指纹
    ensure = PS_StoreChar(CharBuffer2, ID_NUM); // 储存模板

    if (ensure != 0x00) {
        printf(EnsureMessage(ensure));
        return; // 存储失败，直接退出
    }
    printf("Fingerprint entry was successful\n");
    LOG("Fingerprint add was successful");
    vTaskDelay(1500);

    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); // 重新启用对应的 EXTI 中断
}

/**
 * @brief   删除单个指纹
 * @retval  无
 */
void Delete_FR(uint8_t ID_NUM)
{
    uint8_t ensure;

    ensure = PS_DeletChar(ID_NUM, 1); // 删除单个指纹

    if (ensure == 0) {
        printf("删除指纹成功\r\n");
    } else
        printf(EnsureMessage(ensure));

    vTaskDelay(1500);
}

/**
 * @brief   删除所有指纹
 * @retval  无
 */
void Delete_ALL(void)
{
    uint8_t ensure;

    ensure = PS_Empty(); // 清空指纹库

    if (ensure == 0) {
        printf("清空指纹库成功\r\n");
    } else {
        printf(EnsureMessage(ensure));
        vTaskDelay(1500);
    }
}

/**
 * @brief   串口发送一个字节
 * @retval  无
 */
static uint8_t MYUSART_SendData(uint8_t data)
{
    if (HAL_UART_Transmit(&AS608_UART, &data, 1, 0xff) == HAL_OK)
        return 0;
    return 1;
}

/**
 * @brief   发送包头
 * @retval  无
 */
static void SendHead(void)
{
    // 发送前清空数据，因为所有都要发送包头，所以只需要在发送包头前清空即可
    memset(AS608_RX_BUF, 0, sizeof(AS608_RX_BUF));
    MYUSART_SendData(0xEF);
    MYUSART_SendData(0x01);
}

/**
 * @brief   发送地址
 * @retval  无
 */
static void SendAddr(void)
{
    MYUSART_SendData(AS608Addr >> 24);
    MYUSART_SendData(AS608Addr >> 16);
    MYUSART_SendData(AS608Addr >> 8);
    MYUSART_SendData(AS608Addr);
}

/**
 * @brief   发送包标识
 * @retval  无
 */
static void SendFlag(uint8_t flag)
{
    MYUSART_SendData(flag);
}

/**
 * @brief   发送包长度
 * @retval  无
 */
static void SendLength(int length)
{
    MYUSART_SendData(length >> 8);
    MYUSART_SendData(length);
}

/**
 * @brief   发送指令码
 * @retval  无
 */
static void Sendcmd(uint8_t cmd)
{
    MYUSART_SendData(cmd);
}

/**
 * @brief   发送校验和
 * @retval  无
 */
static void SendCheck(uint16_t check)
{
    MYUSART_SendData(check >> 8);
    MYUSART_SendData(check);
}

/**
 * @brief   检查连接
 * @retval  无
 */
static uint8_t AS608_Check(void)
{
    AS608_RX_BUF[9] = 1;

    SendHead();
    SendAddr();

    for (uint8_t i = 0; i < 10; i++) {
        MYUSART_SendData(Get_Device_Code[i]);
    }

    vTaskDelay(200); // 等待200ms

    if (AS608_RX_BUF[9] == 0)
        return 0;

    return 1;
}

/**
 * @brief   指纹模块初始化
 * @retval  无
 */
void AS608_Init(void)
{
    AS608Semphore = xSemaphoreCreateBinary();
    HAL_UARTEx_ReceiveToIdle_IT(&AS608_UART, AS608_RX_BUF,
                                sizeof(AS608_RX_BUF)); // 接收数据，且产生中断

    while (AS608_Check()) {
        printf("AS608 init faile\n");
    };

    printf("AS608 init success\r\n");
}

/**
 * @brief   判断指纹
 * @retval  无
 */
void Verify_FR(void)
{
    SearchResult seach;
    uint8_t      ensure;
    char         str[50];

    ensure = PS_GetImage();

    if (ensure == 0x00) // 获取图像成功
    {
        ensure = PS_GenChar(CharBuffer1);
        if (ensure == 0x00) // 生成特征成功
        {
            ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &seach);
            if (ensure == 0x00) // 搜索成功
            {
                printf("Fingerprint verification was successful");
                sprintf(str, " ID:%d Score:%d ", seach.pageID, seach.mathscore);
                printf("%s\r\n", str);
                lv_obj_add_flag(guider_ui.admin_Interface_cont_2, LV_OBJ_FLAG_HIDDEN);
                vTaskDelay(1500);
                vTaskDelay(1500);
            } else {
                printf("Verification failed\n");
                vTaskDelay(1500);
            }
        } else {
            printf("Please press your fingers\n");
        };
    }
}

/**
 * @brief   判断中断接收的数组有没有应答包
 * @param   waittime: 等待中断接收数据的时间（单位1ms）
 * @retval  数据包首地址
 */
static uint8_t *JudgeStr(uint16_t waittime)
{
    extern SemaphoreHandle_t AS608Semphore;

    char   *data;
    uint8_t str[8];

    str[0] = 0xef;
    str[1] = 0x01;
    str[2] = AS608Addr >> 24;
    str[3] = AS608Addr >> 16;
    str[4] = AS608Addr >> 8;
    str[5] = AS608Addr;
    str[6] = 0x07;
    str[7] = '\0';
    xSemaphoreTake(AS608Semphore, 0);

    while (--waittime) {
        vTaskDelay(1);

        if (xSemaphoreTake(AS608Semphore, 0) == pdTRUE) {
            data = strstr((const char *)AS608_RX_BUF, (const char *)str);
            if (data)
                return (uint8_t *)data;
        }
    }

    return 0;
}

/**
 * @brief   录入图像
 * @note    探测手指，探测到后录入指纹图像存于ImageBuffer。
 * @retval  确认码
 */
uint8_t PS_GetImage(void)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;
    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x03);
    Sendcmd(0x01);
    temp = 0x01 + 0x03 + 0x01;
    SendCheck(temp);
    data = JudgeStr(2000);
    if (data)
        ensure = data[9];
    else
        ensure = 0xff;
    return ensure;
}

/**
 * @brief   生成特征
 * @note    将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2
 * @param   BufferID: charBuffer1:0x01	charBuffer1:0x02
 * @retval  确认码
 */
uint8_t PS_GenChar(uint8_t BufferID)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;
    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x04);
    Sendcmd(0x02);
    MYUSART_SendData(BufferID);
    temp = 0x01 + 0x04 + 0x02 + BufferID;
    SendCheck(temp);
    data = JudgeStr(2000);
    if (data)
        ensure = data[9];
    else
        ensure = 0xff;
    return ensure;
}

/**
 * @brief   精确比对CharBuffer1 与CharBuffer2 中的特征文件
 * @retval  确认码
 */
uint8_t PS_Match(void)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;
    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x03);
    Sendcmd(0x03);
    temp = 0x01 + 0x03 + 0x03;
    SendCheck(temp);
    data = JudgeStr(2000);
    if (data)
        ensure = data[9];
    else
        ensure = 0xff;
    return ensure;
}

/**
 * @brief   搜索指纹
 * @note    以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。
 * @param   BufferID: charBuffer1:0x01	charBuffer1:0x02
 * @param   StartPage: 起始页
 * @param   PageNum: 页数
 * @retval  确认码
 */
uint8_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x08);
    Sendcmd(0x04);
    MYUSART_SendData(BufferID);
    MYUSART_SendData(StartPage >> 8);
    MYUSART_SendData(StartPage);
    MYUSART_SendData(PageNum >> 8);
    MYUSART_SendData(PageNum);
    temp = 0x01 + 0x08 + 0x04 + BufferID + (StartPage >> 8) + (uint8_t)StartPage + (PageNum >> 8) + (uint8_t)PageNum;
    SendCheck(temp);
    data = JudgeStr(2000);

    if (data) {
        ensure       = data[9];
        p->pageID    = (data[10] << 8) + data[11];
        p->mathscore = (data[12] << 8) + data[13];
    } else
        ensure = 0xff;

    return ensure;
}

/**
 * @brief   合并特征（生成模板）
 * @note    将CharBuffer1与CharBuffer2中的特征文件合并生成模板,结果存于CharBuffer1与CharBuffer2
 * @retval  确认码
 */
uint8_t PS_RegModel(void)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x03);
    Sendcmd(0x05);
    temp = 0x01 + 0x03 + 0x05;
    SendCheck(temp);
    data = JudgeStr(2000);

    if (data)
        ensure = data[9];
    else
        ensure = 0xff;

    return ensure;
}

/**
 * @brief   存储模版
 * @note    将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置
 * @param   BufferID: charBuffer1:0x01	charBuffer1:0x02
 * @param    PageID: 指纹库位置号
 * @retval  确认码
 */
uint8_t PS_StoreChar(uint8_t BufferID, uint16_t PageID)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x06);
    Sendcmd(0x06);
    MYUSART_SendData(BufferID);
    MYUSART_SendData(PageID >> 8);
    MYUSART_SendData(PageID);
    temp = 0x01 + 0x06 + 0x06 + BufferID + (PageID >> 8) + (uint8_t)PageID;
    SendCheck(temp);
    data = JudgeStr(2000);

    if (data)
        ensure = data[9];
    else
        ensure = 0xff;

    return ensure;
}

/**
 * @brief   删除flash数据库中指定ID号开始的N个指纹模板
 * @param   PageID: 指纹库模板号
 * @param   N: 删除的模板个数
 * @retval  确认码
 */
uint8_t PS_DeletChar(uint16_t PageID, uint16_t N)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x07);
    Sendcmd(0x0C);
    MYUSART_SendData(PageID >> 8);
    MYUSART_SendData(PageID);
    MYUSART_SendData(N >> 8);
    MYUSART_SendData(N);
    temp = 0x01 + 0x07 + 0x0C + (PageID >> 8) + (uint8_t)PageID + (N >> 8) + (uint8_t)N;
    SendCheck(temp);

    data = JudgeStr(2000);
    if (data)
        ensure = data[9];
    else
        ensure = 0xff;

    return ensure;
}

/**
 * @brief   清空指纹库
 * @param   BufferID: 指纹ID
 * @param   StartPage: 起始页
 * @param   PageNum: 页数
 * @retval  确认码
 */
uint8_t PS_Empty(void)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x03);
    Sendcmd(0x0D);
    temp = 0x01 + 0x03 + 0x0D;
    SendCheck(temp);
    data = JudgeStr(2000);

    if (data)
        ensure = data[9];
    else
        ensure = 0xff;

    return ensure;
}

/**
 * @brief   写系统寄存器
 * @param   RegNum:寄存器序号 4\5\6
 * @param   DATA: 数据
 * @retval  确认码
 */
uint8_t PS_WriteReg(uint8_t RegNum, uint8_t DATA)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x05);
    Sendcmd(0x0E);
    MYUSART_SendData(RegNum);
    MYUSART_SendData(DATA);
    temp = RegNum + DATA + 0x01 + 0x05 + 0x0E;
    SendCheck(temp);
    data = JudgeStr(2000);

    if (data)
        ensure = data[9];
    else
        ensure = 0xff;

    if (ensure == 0)
        printf("\r\n设置参数成功!");
    else
        printf("\r\n%s", EnsureMessage(ensure));

    return ensure;
}

/**
 * @brief   读系统基本参数
 * @param   p: AS608系统参数结构体指针
 * @retval  确认码
 */
uint8_t PS_ReadSysPara(SysPara *p)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;
    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x03);
    Sendcmd(0x0F);
    temp = 0x01 + 0x03 + 0x0F;
    SendCheck(temp);
    data = JudgeStr(1000);

    if (data) {
        ensure      = data[9];
        p->PS_max   = (data[14] << 8) + data[15];
        p->PS_level = data[17];
        p->PS_addr  = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
        p->PS_size  = data[23];
        p->PS_N     = data[25];
    } else
        ensure = 0xff;

    if (ensure == 0x00) {
        printf("\r\n模块最大指纹容量=%d", p->PS_max);
        printf("\r\n对比等级=%d", p->PS_level);
        printf("\r\n地址=%lx", p->PS_addr);
        printf("\r\n波特率=%d\r\n", p->PS_N * 9600);
    } else
        printf("\r\n%s", EnsureMessage(ensure));

    return ensure;
}

/**
 * @brief   设置模块地址
 * @param   PS_addr: 地址
 * @retval  确认码
 */
uint8_t PS_SetAddr(uint32_t PS_addr)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x07);
    Sendcmd(0x15);
    MYUSART_SendData(PS_addr >> 24);
    MYUSART_SendData(PS_addr >> 16);
    MYUSART_SendData(PS_addr >> 8);
    MYUSART_SendData(PS_addr);
    temp = 0x01 + 0x07 + 0x15 + (uint8_t)(PS_addr >> 24) + (uint8_t)(PS_addr >> 16) + (uint8_t)(PS_addr >> 8)
           + (uint8_t)PS_addr;
    SendCheck(temp);
    AS608Addr = PS_addr; // 发送完指令，更换地址
    data      = JudgeStr(2000);

    if (data)
        ensure = data[9];
    else
        ensure = 0xff;

    AS608Addr = PS_addr;

    if (ensure == 0x00)
        printf("\r\n设置地址成功!");
    else
        printf("\r\n%s", EnsureMessage(ensure));

    return ensure;
}

/**
 * @brief   模块内部为用户开辟了256bytes的FLASH空间用于存用户记事本,
            该记事本逻辑上被分成 16 个页。
 * @param   NotePageNum(0~15): 记事本页数
 * @param   Byte32: 要写入内容指针，32个字节
 * @retval  确认码
 */
uint8_t PS_WriteNotepad(uint8_t NotePageNum, uint8_t *Byte32)
{
    uint16_t temp = {0};
    uint8_t  ensure, i;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(36);
    Sendcmd(0x18);
    MYUSART_SendData(NotePageNum);

    for (i = 0; i < 32; i++) {
        MYUSART_SendData(Byte32[i]);
        temp += Byte32[i];
    }

    temp = 0x01 + 36 + 0x18 + NotePageNum + temp;
    SendCheck(temp);
    data = JudgeStr(2000);

    if (data)
        ensure = data[9];
    else
        ensure = 0xff;

    return ensure;
}

/**
 * @brief   读取FLASH用户区的128bytes数据
 * @param   NotePageNum(0~15): 记事本页数
 * @param   Byte32: 用户信息指针
 * @retval  确认码
 */
uint8_t PS_ReadNotepad(uint8_t NotePageNum, uint8_t *Byte32)
{
    uint16_t temp;
    uint8_t  ensure, i;
    uint8_t *data;
    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x04);
    Sendcmd(0x19);
    MYUSART_SendData(NotePageNum);
    temp = 0x01 + 0x04 + 0x19 + NotePageNum;
    SendCheck(temp);
    data = JudgeStr(2000);
    if (data) {
        ensure = data[9];
        for (i = 0; i < 32; i++) {
            Byte32[i] = data[10 + i];
        }
    } else
        ensure = 0xff;
    return ensure;
}

/**
 * @brief   高速搜索
 * @note    CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
            若搜索到，则返回页码,该指令对于的确存在于指纹库中 ，且登录时质量
            很好的指纹，会很快给出搜索结果。
 * @param   BufferID: charBuffer1:0x01	charBuffer1:0x02
 * @param   StartPage: 起始页
 * @param   PageNum: 页数
 * @retval  确认码
 */
uint8_t PS_HighSpeedSearch(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x08);
    Sendcmd(0x1b);
    MYUSART_SendData(BufferID);
    MYUSART_SendData(StartPage >> 8);
    MYUSART_SendData(StartPage);
    MYUSART_SendData(PageNum >> 8);
    MYUSART_SendData(PageNum);
    temp = 0x01 + 0x08 + 0x1b + BufferID + (StartPage >> 8) + (uint8_t)StartPage + (PageNum >> 8) + (uint8_t)PageNum;
    SendCheck(temp);
    data = JudgeStr(2000);
    if (data) {
        ensure       = data[9];
        p->pageID    = (data[10] << 8) + data[11];
        p->mathscore = (data[12] << 8) + data[13];
    } else
        ensure = 0xff;

    return ensure;
}

/**
 * @brief   读有效模板个数
 * @param   ValidN: 有效模板个数指针
 * @retval  确认码
 */
uint8_t PS_ValidTempleteNum(uint16_t *ValidN)
{
    uint16_t temp;
    uint8_t  ensure;
    uint8_t *data;

    SendHead();
    SendAddr();
    SendFlag(0x01); // 命令包标识
    SendLength(0x03);
    Sendcmd(0x1d);
    temp = 0x01 + 0x03 + 0x1d;
    SendCheck(temp);
    data = JudgeStr(2000);

    if (data) {
        ensure  = data[9];
        *ValidN = (data[10] << 8) + data[11];
    } else
        ensure = 0xff;

    if (ensure == 0x00) {
        printf("\r\n指纹有%d个\r\n", (data[10] << 8) + data[11]);
    } else
        printf("\r\n%s", EnsureMessage(ensure));

    return ensure;
}

/* // 与AS608握手 PS_HandShake
// 参数: PS_Addr地址指针
// 说明: 模块返新地址（正确地址）
uint8_t PS_HandShake(uint32_t *PS_Addr)
{
    SendHead();
    SendAddr();
    MYUSART_SendData(0X01);
    MYUSART_SendData(0X00);
    MYUSART_SendData(0X00);
    vTaskDelay(200);
    if (AS608_RX_STA & 0X8000) // 接收到数据
    {
        if ( // 判断是不是模块返回的应答包
            AS608_RX_BUF[0] == 0XEF && AS608_RX_BUF[1] == 0X01 && AS608_RX_BUF[6] == 0X07) {
            *PS_Addr = (AS608_RX_BUF[2] << 24) + (AS608_RX_BUF[3] << 16) + (AS608_RX_BUF[4] << 8)
                       + (AS608_RX_BUF[5]);
            AS608_RX_STA = 0;
            return 0;
        }
        AS608_RX_STA = 0;
    }
    return 1;
} */

/**
 * @brief   解析确认码错误信息返回信息
 * @param   ensure: 确认码
 * @retval  错误信息返回信息指针
 */
const char *EnsureMessage(uint8_t ensure)
{
    const char *p;

    switch (ensure) {
        case 0x00: p = "OK\r\n"; break;
        case 0x01: p = "数据包接收错误\r\n"; break;
        case 0x02: p = "传感器上没有手指\r\n"; break;
        case 0x03: p = "录入指纹图像失败\r\n"; break;
        case 0x04: p = "指纹图像太干、太淡而生不成特征\r\n"; break;
        case 0x05: p = "指纹图像太湿、太糊而生不成特征\r\n"; break;
        case 0x06: p = "指纹图像太乱而生不成特征\r\n"; break;
        case 0x07: p = "指纹图像正常，但特征点太少（或面积太小）而生不成特征\r\n"; break;
        case 0x08: p = "指纹不匹配\r\n"; break;
        case 0x09: p = "没搜索到指纹\r\n"; break;
        case 0x0a: p = "特征合并失败\r\n"; break;
        case 0x0b: p = "访问指纹库时地址序号超出指纹库范围\r\n";
        case 0x10: p = "删除模板失败\r\n"; break;
        case 0x11: p = "清空指纹库失败\r\n"; break;
        case 0x15: p = "缓冲区内没有有效原始图而生不成图像\r\n"; break;
        case 0x18: p = "读写 FLASH 出错\r\n"; break;
        case 0x19: p = "未定义错误\r\n"; break;
        case 0x1a: p = "无效寄存器号\r\n"; break;
        case 0x1b: p = "寄存器设定内容错误\r\n"; break;
        case 0x1c: p = "记事本页码指定错误\r\n"; break;
        case 0x1f: p = "指纹库满\r\n"; break;
        case 0x20: p = "地址错误\r\n"; break;
        default: p = "模块返回确认码有误\r\n"; break;
    }

    return p;
}
