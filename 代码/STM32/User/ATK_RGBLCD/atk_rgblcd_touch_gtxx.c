/**
 ****************************************************************************************************
 * @file        atk_rgblcd_touch_gtxx.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       正点原子RGB LCD模块触摸驱动代码（GTXX）
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 H743开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "atk_rgblcd_touch.h"
// #include "atk_rgblcd_touch_iic.h"
#include "bsp_delay.h"
#include <string.h>

#if (ATK_RGBLCD_USING_TOUCH != 0)

/* RGB LCD模块触摸的PID */
#    define ATK_RGBLCD_TOUCH_PID             "911"
#    define ATK_RGBLCD_TOUCH_PID1            "9147"
#    define ATK_RGBLCD_TOUCH_PID2            "9271"
#    define ATK_RGBLCD_TOUCH_PID3            "1158"

/* RGB LCD模块最大触摸点数量 */
#    define ATK_RGBLCD_TOUCH_TP_MAX          5

/* RGB LCD模块触摸部分寄存器定义 */
#    define ATK_RGBLCD_TOUCH_REG_CTRL        0x8040 /* 控制寄存器 */
#    define ATK_RGBLCD_TOUCH_REG_PID         0x8140 /* PID寄存器 */
#    define ATK_RGBLCD_TOUCH_REG_TPINFO      0x814E /* 触摸状态寄存器 */
#    define ATK_RGBLCD_TOUCH_REG_TP1         0x8150 /* 触摸点1数据寄存器 */
#    define ATK_RGBLCD_TOUCH_REG_TP2         0x8158 /* 触摸点2数据寄存器 */
#    define ATK_RGBLCD_TOUCH_REG_TP3         0x8160 /* 触摸点3数据寄存器 */
#    define ATK_RGBLCD_TOUCH_REG_TP4         0x8168 /* 触摸点4数据寄存器 */
#    define ATK_RGBLCD_TOUCH_REG_TP5         0x8170 /* 触摸点5数据寄存器 */

/* 触摸状态寄存器掩码 */
#    define ATK_RGBLCD_TOUCH_TPINFO_MASK_STA 0x80
#    define ATK_RGBLCD_TOUCH_TPINFO_MASK_CNT 0x0F

/* RGB LCD模块触摸点数据寄存器 */
static const uint16_t g_atk_rgblcd_touch_tp_reg[ATK_RGBLCD_TOUCH_TP_MAX] = {
    ATK_RGBLCD_TOUCH_REG_TP1,
    ATK_RGBLCD_TOUCH_REG_TP2,
    ATK_RGBLCD_TOUCH_REG_TP3,
    ATK_RGBLCD_TOUCH_REG_TP4,
    ATK_RGBLCD_TOUCH_REG_TP5,
};

/* RGB LCD模块触摸状态结构体 */
static struct {
    atk_rgblcd_touch_iic_addr_t iic_addr;
} g_atk_rgblcd_touch_sta;

/**
 * @brief       RGB LCD模块触摸硬件复位
 * @param       addr: 复位后使用的IIC通讯地址
 * @retval      无
 */
static void atk_rgblcd_touch_hw_reset(atk_rgblcd_touch_iic_addr_t addr)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    /* 配置PEN引脚为输出 */
    gpio_init_struct.Pin   = ATK_RGBLCD_TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

    switch (addr) {
        case ATK_RGBLCD_TOUCH_IIC_ADDR_14: {
            ATK_RGBLCD_TOUCH_TCS(0);
            ATK_RGBLCD_TOUCH_PEN(0);
            ATK_RGBLCD_TOUCH_PEN(1);
            delay_ms(1);
            ATK_RGBLCD_TOUCH_TCS(1);
            delay_ms(10);
            break;
        }
        case ATK_RGBLCD_TOUCH_IIC_ADDR_5D: {
            ATK_RGBLCD_TOUCH_TCS(0);
            ATK_RGBLCD_TOUCH_PEN(0);
            delay_ms(1);
            ATK_RGBLCD_TOUCH_TCS(1);
            delay_ms(10);
            break;
        }
        default: {
            break;
        }
    }

    /* 重新配置PEN引脚为输入 */
    gpio_init_struct.Pin   = ATK_RGBLCD_TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;
    gpio_init_struct.Pull  = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

    g_atk_rgblcd_touch_sta.iic_addr = addr;
    delay_ms(100);
}

/**
 * @brief       写RGB LCD模块触摸寄存器
 * @param       reg: 待写寄存器地址
 *              buf: 待写入的数据
 *              len: 待写入数据的长度
 * @retval      ATK_RGBLCD_TOUCH_EOK  : 写ATK-RGBLCD模块触摸寄存器成功
 *              ATK_RGBLCD_TOUCH_ERROR: 写ATK-RGBLCD模块触摸寄存器失败
 */
static uint8_t atk_rgblcd_touch_write_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t reg_addr[2];
    uint8_t data[len + 2];
    uint8_t ret = HAL_ERROR;

    // 将寄存器地址拆分为高字节和低字节
    reg_addr[0] = (uint8_t)(reg >> 8) & 0xFF;
    reg_addr[1] = (uint8_t)reg & 0xFF;

    // 将寄存器地址和数据合并到一个数组中
    memcpy(data, reg_addr, 2);
    memcpy(data + 2, buf, len);
    ret = HAL_I2C_Master_Transmit(
        &ATK_RGBLCD_TOUCH_I2C, (g_atk_rgblcd_touch_sta.iic_addr << 1) | ATK_RGBLCD_TOUCH_IIC_WRITE, data, len + 2, 100);

    if (ret != HAL_OK) {
        return ATK_RGBLCD_TOUCH_ERROR;
    }

    return ATK_RGBLCD_TOUCH_EOK;
}

/**
 * @brief       读RGB LCD模块触摸寄存器
 * @param       reg: 待读寄存器地址
 *              buf: 读取的数据
 *              len: 待读取数据的长度
 * @retval      无
 */
static void atk_rgblcd_touch_read_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    // uint8_t buf_index;
    uint8_t reg_addr[2];
    reg_addr[0] = (uint8_t)(reg >> 8) & 0xFF;
    reg_addr[1] = (uint8_t)reg & 0xFF;

    HAL_I2C_Master_Transmit(
        &ATK_RGBLCD_TOUCH_I2C, (g_atk_rgblcd_touch_sta.iic_addr << 1) | ATK_RGBLCD_TOUCH_IIC_WRITE, reg_addr, 2, 100);
    HAL_I2C_Master_Receive(
        &ATK_RGBLCD_TOUCH_I2C, (g_atk_rgblcd_touch_sta.iic_addr << 1) | ATK_RGBLCD_TOUCH_IIC_READ, buf, len, 100);
}

/**
 * @brief       RGB LCD模块触摸软件复位
 * @param       无
 * @retval      无
 */
static void atk_rgblcd_touch_sw_reset(void)
{
    uint8_t dat;

    dat = 0x02;
    atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_CTRL, &dat, 1);
    delay_ms(10);

    dat = 0x00;
    atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_CTRL, &dat, 1);
}

/**
 * @brief       获取RGB LCD模块触摸的PID
 * @param       pid: 获取到的PID（ASCII）
 * @retval      无
 */
static void atk_rgblcd_touch_get_pid(char *pid)
{
    atk_rgblcd_touch_read_reg(ATK_RGBLCD_TOUCH_REG_PID, (uint8_t *)pid, 4);
    pid[4] = '\0';
}

/**
 * @brief       RGB LCD模块触摸初始化
 * @param       无
 * @retval      ATK_RGBLCD_TOUCH_EOK  : RGB LCD模块触摸初始化成功
 *              ATK_RGBLCD_TOUCH_ERROR: RGB LCD模块触摸初始化失败
 */
uint8_t atk_rgblcd_touch_init(atk_rgblcd_touch_type_t type)
{
    char pid[5];

    if (type != ATK_RGBLCD_TOUCH_TYPE_FTXX) {
        return ATK_RGBLCD_TOUCH_ERROR;
    }

    // atk_rgblcd_touch_hw_init();
    atk_rgblcd_touch_hw_reset(ATK_RGBLCD_TOUCH_IIC_ADDR_14);
    // atk_rgblcd_touch_iic_init();
    atk_rgblcd_touch_get_pid(pid);
    if ((strcmp(pid, ATK_RGBLCD_TOUCH_PID) != 0) && (strcmp(pid, ATK_RGBLCD_TOUCH_PID1) != 0)
        && (strcmp(pid, ATK_RGBLCD_TOUCH_PID2) != 0) && (strcmp(pid, ATK_RGBLCD_TOUCH_PID3) != 0)) {
        return ATK_RGBLCD_TOUCH_ERROR;
    }
    atk_rgblcd_touch_sw_reset();

    return ATK_RGBLCD_TOUCH_EOK;
}

/**
 * @brief       RGB LCD模块触摸扫描
 * @note        连续调用间隔需大于10ms
 * @param       point: 扫描到的触摸点信息
 *              cnt  : 需要扫描的触摸点数量（1~ATK_RGBLCD_TOUCH_TP_MAX）
 * @retval      0   : 没有扫描到触摸点
 *              其他: 实际获取到的触摸点信息数量
 */
uint8_t atk_rgblcd_touch_scan(atk_rgblcd_touch_point_t *point, uint8_t cnt)
{
    uint8_t                   tp_info;
    uint8_t                   tp_cnt;
    uint8_t                   point_index;
    atk_rgblcd_lcd_disp_dir_t dir;
    uint8_t                   tpn_info[6];
    atk_rgblcd_touch_point_t  point_raw;

    if ((cnt == 0) || (cnt > ATK_RGBLCD_TOUCH_TP_MAX)) {
        return 0;
    }

    for (point_index = 0; point_index < cnt; point_index++) {
        if (&point[point_index] == NULL) {
            return 0;
        }
    }

    atk_rgblcd_touch_read_reg(ATK_RGBLCD_TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));
    if ((tp_info & ATK_RGBLCD_TOUCH_TPINFO_MASK_STA) == ATK_RGBLCD_TOUCH_TPINFO_MASK_STA) {
        tp_cnt = tp_info & ATK_RGBLCD_TOUCH_TPINFO_MASK_CNT;
        tp_cnt = (cnt < tp_cnt) ? cnt : tp_cnt;

        for (point_index = 0; point_index < tp_cnt; point_index++) {
            atk_rgblcd_touch_read_reg(g_atk_rgblcd_touch_tp_reg[point_index], tpn_info, sizeof(tpn_info));
            point_raw.x    = (uint16_t)(tpn_info[1] << 8) | tpn_info[0];
            point_raw.y    = (uint16_t)(tpn_info[3] << 8) | tpn_info[2];
            point_raw.size = (uint16_t)(tpn_info[5] << 8) | tpn_info[4];

            dir = atk_rgblcd_get_disp_dir();
            switch (dir) {
                case ATK_RGBLCD_LCD_DISP_DIR_0: {
                    point[point_index].x = point_raw.x;
                    point[point_index].y = point_raw.y;
                    break;
                }
                case ATK_RGBLCD_LCD_DISP_DIR_90: {
                    point[point_index].x = point_raw.y;
                    point[point_index].y = atk_rgblcd_get_lcd_height() - point_raw.x;
                    break;
                }
                case ATK_RGBLCD_LCD_DISP_DIR_180: {
                    point[point_index].x = atk_rgblcd_get_lcd_width() - point_raw.x;
                    point[point_index].y = atk_rgblcd_get_lcd_height() - point_raw.y;
                    break;
                }
                case ATK_RGBLCD_LCD_DISP_DIR_270: {
                    point[point_index].x = atk_rgblcd_get_lcd_width() - point_raw.y;
                    point[point_index].y = point_raw.x;
                    break;
                }
            }

            point[point_index].size = point_raw.size;
        }

        tp_info = 0;
        atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));

        return tp_cnt;
    } else {
        return 0;
    }
}

#endif
