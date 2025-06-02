#include "bsp_mlx90614.h"

/**
 * @brief   发起始位
 * @retval  无
 */
void SMBus_StartBit(void)
{
    SMBUS_SDA_H();  // Set SDA line拉高数据线sda
    SMBus_Delay(5); // Wait a few microseconds
    SMBUS_SCK_H();  // Set SCL line拉高scl
    SMBus_Delay(5); // Generate bus free time between Stop
    SMBUS_SDA_L();  // Clear SDA line突变产生起始信号
    SMBus_Delay(5); // Hold time after (Repeated) Start
    // Condition. After this period, the first clock is generated.
    //(Thd:sta=4.0us min)在SCK=1时，检测到SDA由1到0表示通信开始（下降沿）
    SMBUS_SCK_L();  // Clear SCL line
    SMBus_Delay(5); // Wait a few microseconds
}

/**
 * @brief   发停止位
 * @retval  无
 */
void SMBus_StopBit(void)
{
    SMBUS_SCK_L();  // Clear SCL line
    SMBus_Delay(5); // Wait a few microseconds
    SMBUS_SDA_L();  // Clear SDA line
    SMBus_Delay(5); // Wait a few microseconds
    SMBUS_SCK_H();  // Set SCL line
    SMBus_Delay(5); // Stop condition setup time(Tsu:sto=4.0us min)
    SMBUS_SDA_H();  // Set SDA line
}

/**
 * @brief   发送一个字节
 * @param   Tx_buffer: 发送的数据
 * @retval  无
 */
uint8_t SMBus_SendByte(uint8_t Tx_buffer)
{
    uint8_t Bit_counter;
    uint8_t Ack_bit;
    uint8_t bit_out;

    for (Bit_counter = 8; Bit_counter; Bit_counter--) {
        if (Tx_buffer & 0x80) // 如果最高位为1
        {
            bit_out = 1; // 把最高位置1
        } else           // 如果最高位为0
        {
            bit_out = 0; // 把最高位置0
        }
        SMBus_SendBit(bit_out); // 把最高位发送出去
        Tx_buffer <<=
            1; // 左移一位把最高位移出去等待下一个最高位，循环8次，每次都发最高位，就可把一个字节发出去了
    }
    Ack_bit = SMBus_ReceiveBit();
    return Ack_bit;
}

/**
 * @brief   发送一个位
 * @param   bit_out: 发送的数据
 * @retval  无
 */
void SMBus_SendBit(uint8_t bit_out)
{
    if (bit_out == 0) {
        SMBUS_SDA_L();
    } else {
        SMBUS_SDA_H();
    }
    SMBus_Delay(2); // Tsu:dat = 250ns minimum
    SMBUS_SCK_H();  // Set SCL line
    SMBus_Delay(6); // High Level of Clock Pulse
    SMBUS_SCK_L();  // Clear SCL line
    SMBus_Delay(3); // Low Level of Clock Pulse
                    //	SMBUS_SDA_H();				    // Master release SDA line ,
    return;
}

/**
 * @brief   接收一个位
 * @retval  接收的数据
 */
uint8_t SMBus_ReceiveBit(void)
{
    uint8_t Ack_bit;

    SMBUS_SDA_H();  // 引脚靠外部电阻上拉，当作输入
    SMBus_Delay(2); // High Level of Clock Pulse
    SMBUS_SCK_H();  // Set SCL line
    SMBus_Delay(5); // High Level of Clock Pulse
    if (SMBUS_SDA_PIN()) {
        Ack_bit = 1;
    } else {
        Ack_bit = 0;
    }
    SMBUS_SCK_L();  // Clear SCL line
    SMBus_Delay(3); // Low Level of Clock Pulse

    return Ack_bit;
}

/**
 * @brief   接收一个字节
 * @param   ack_nack: 应答位
 * @retval  接受的数据
 */
uint8_t SMBus_ReceiveByte(uint8_t ack_nack)
{
    uint8_t RX_buffer   = 0;
    uint8_t Bit_Counter = 0;

    for (Bit_Counter = 8; Bit_Counter; Bit_Counter--) {
        if (SMBus_ReceiveBit()) // Get a bit from the SDA line
        {
            RX_buffer <<= 1;   // If the bit is HIGH save 1  in RX_buffer
            RX_buffer |= 0x01; // 如果Ack_bit=1，把收到应答信号1与0000 0001 进行或运算，确保为1
        } else {
            RX_buffer <<= 1;   // If the bit is LOW save 0 in RX_buffer
            RX_buffer &= 0xfe; // 如果Ack_bit=1，把收到应答信号0与1111 1110 进行与运算，确保为0
        }
    }
    SMBus_SendBit(ack_nack); // Sends acknowledgment bit
                             // 把应答信号发出去，如果1，就进行下一次通信，如果为0、，就拜拜了
    return RX_buffer;
}

/*******************************************************************************
 * 函数名: SMBus_Delay
 * 功能: 延时  一次循环约1us
 * Input          : time
 * Output         : None
 * Return         : None
 *******************************************************************************/
// void SMBus_Delay(uint16_t time)
// {
//     // uint16_t i, j;
//     // for (i = 0; i < 4; i++) {
//     //     for (j = 0; j < time; j++);
//     // }

//     uint16_t differ = 0xffff - time - 5;
//     __HAL_TIM_SET_COUNTER(&htim4, differ); // 设定TIM1计数器起始值
//     HAL_TIM_Base_Start(&htim4);            // 启动定时器

//     while (differ < 0xffff - 5) {               // 判断
//         differ = __HAL_TIM_GET_COUNTER(&htim4); // 查询计数器的计数值
//     }
//     HAL_TIM_Base_Stop(&htim4);
// }

/**
 * @brief   SMBus初始化
 * @retval  无
 */
void SMBus_Init()
{
    SMBUS_SCK_H();
    SMBUS_SDA_H();
}

/**
 * @brief   从RAM和EEPROM中读取数据
 * @param   slaveAddress: IIC设备的从地址
 * @param   command: 从command这个寄存器开始读取
 * @retval  无
 */
uint16_t SMBus_ReadMemory(uint8_t slaveAddress, uint8_t command)
{
    uint16_t data;         // Data storage (DataH:DataL)
    uint8_t  Pec;          // PEC byte storage
    uint8_t  DataL = 0;    // Low data byte storage
    uint8_t  DataH = 0;    // High data byte storage
    uint8_t  arr[6];       // Buffer for the sent bytes
    uint8_t  PecReg;       // Calculated PEC byte storage
    uint8_t  ErrorCounter; // Defines the number of the attempts for communication with MLX90614

    ErrorCounter = 0xff; // Initialising of ErrorCounter
    slaveAddress <<= 1;  // 2-7位表示从机地址 从机地址左移一位，把读写位空出来

    do {
    repeat:
        SMBus_StopBit();   // If slave send NACK stop comunication
        --ErrorCounter;    // Pre-decrement ErrorCounter
        if (!ErrorCounter) // ErrorCounter=0?
        {
            break; // Yes,go out from do-while{}
        }

        SMBus_StartBit();                 // Start condition
        if (SMBus_SendByte(slaveAddress)) // Send SlaveAddress 最低位Wr=0表示接下来写命令
        {
            goto repeat; // Repeat comunication again
        }
        if (SMBus_SendByte(command)) // Send command
        {
            goto repeat; // Repeat comunication again
        }

        SMBus_StartBit();                     // Repeated Start condition
        if (SMBus_SendByte(slaveAddress + 1)) // Send SlaveAddress 最低位Rd=1表示接下来读数据
        {
            goto repeat; // Repeat comunication again
        }

        DataL = SMBus_ReceiveByte(ACK);  // Read low data,master must send ACK
        DataH = SMBus_ReceiveByte(ACK);  // Read high data,master must send ACK
        Pec   = SMBus_ReceiveByte(NACK); // Read PEC byte, master must send NACK
        SMBus_StopBit();                 // Stop condition

        arr[5] = slaveAddress;         //
        arr[4] = command;              //
        arr[3] = slaveAddress + 1;     // Load array arr
        arr[2] = DataL;                //
        arr[1] = DataH;                //
        arr[0] = 0;                    //
        PecReg = PEC_Calculation(arr); // Calculate CRC 数据校验
    } while (PecReg != Pec); // If received and calculated CRC are equal go out from do-while{}

    data = (DataH << 8) | DataL; // data=DataH:DataL
    return data;
}

/**
 * @brief   数据校验
 * @param   pec[]: 校验的数据
 * @retval  pec[0]-this byte contains calculated crc value
 */
uint8_t PEC_Calculation(uint8_t pec[])
{
    uint8_t crc[6];           // 存放多项式
    uint8_t BitPosition = 47; // 存放所有数据最高位，6*8=48 最高位就是47位
    uint8_t shift;
    uint8_t i;
    uint8_t j;
    uint8_t temp;

    do {
        /*Load pattern value 0x00 00 00 00 01 07*/
        crc[5] = 0;
        crc[4] = 0;
        crc[3] = 0;
        crc[2] = 0;
        crc[1] = 0x01;
        crc[0] = 0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition = 47;

        /*Set shift position at 0*/
        shift = 0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i = 5;
        j = 0;
        while ((pec[i] & (0x80 >> j)) == 0 && i > 0) {
            BitPosition--;
            if (j < 7) {
                j++;
            } else {
                j = 0x00;
                i--;
            }
        } /*End of while */

        /*Get shift value for pattern value*/
        shift = BitPosition - 8;

        /*Shift pattern value */
        while (shift) {
            for (i = 5; i < 0xFF; i--) {
                if ((crc[i - 1] & 0x80) && (i > 0)) {
                    temp = 1;
                } else {
                    temp = 0;
                }
                crc[i] <<= 1;
                crc[i] += temp;
            } /*End of for*/
            shift--;
        } /*End of while*/

        /*Exclusive OR between pec and crc*/
        for (i = 0; i <= 5; i++) {
            pec[i] ^= crc[i];
        } /*End of for*/
    } while (BitPosition > 8); /*End of do-while*/

    return pec[0];
}

/**
 * @brief   计算并返回温度值
 * @retval  温度值
 */
float SMBus_ReadTemp(void)
{
    return (SMBus_ReadMemory(SA, RAM_ACCESS | RAM_TOBJ1) * 0.02 - 273.15);
}
