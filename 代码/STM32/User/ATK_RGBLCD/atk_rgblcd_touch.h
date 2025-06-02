/**
 ****************************************************************************************************
 * @file        atk_rgblcd_touch.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       正点原子RGB LCD模块触摸驱动代码
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

#ifndef __ATK_RGBLCD_TOUCH_H
#define __ATK_RGBLCD_TOUCH_H

#include "atk_rgblcd.h"
#include "i2c.h"

#define ATK_RGBLCD_TOUCH_I2C hi2c4

#if (ATK_RGBLCD_USING_TOUCH != 0)

/* 控制IIC通讯是读操作还是写操作 */
#    define ATK_RGBLCD_TOUCH_IIC_WRITE     0
#    define ATK_RGBLCD_TOUCH_IIC_READ      1

/* 错误代码 */
#    define ATK_RGBLCD_TOUCH_IIC_EOK       0 /* 没有错误 */
#    define ATK_RGBLCD_TOUCH_IIC_ERROR     1 /* 错误 */

/* 引脚定义 */
#    define ATK_RGBLCD_TOUCH_PEN_GPIO_PORT GPIOH
#    define ATK_RGBLCD_TOUCH_PEN_GPIO_PIN  GPIO_PIN_7

#    define ATK_RGBLCD_TOUCH_TCS_GPIO_PORT GPIOH
#    define ATK_RGBLCD_TOUCH_TCS_GPIO_PIN  GPIO_PIN_6

/* IO操作 */
#    define ATK_RGBLCD_TOUCH_READ_PEN() HAL_GPIO_ReadPin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN)
#    define ATK_RGBLCD_TOUCH_PEN(x)                                                                                    \
        do {                                                                                                           \
            x ? HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN, GPIO_PIN_SET)         \
              : HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN, GPIO_PIN_RESET);      \
        } while (0)
#    define ATK_RGBLCD_TOUCH_TCS(x)                                                                                    \
        do {                                                                                                           \
            x ? HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_TCS_GPIO_PORT, ATK_RGBLCD_TOUCH_TCS_GPIO_PIN, GPIO_PIN_SET)         \
              : HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_TCS_GPIO_PORT, ATK_RGBLCD_TOUCH_TCS_GPIO_PIN, GPIO_PIN_RESET);      \
        } while (0)

/* RGB LCD触摸IIC通讯地址枚举 */
typedef enum {
    ATK_RGBLCD_TOUCH_IIC_ADDR_14 = 0x14, /* 0x14 */
    ATK_RGBLCD_TOUCH_IIC_ADDR_5D = 0x5D, /* 0x5D */
    ATK_RGBLCD_TOUCH_IIC_ADDR_38 = 0x38, /* 0x38 */
} atk_rgblcd_touch_iic_addr_t;

/* 触摸点坐标数据结构 */
typedef struct {
    uint16_t x;    /* 触摸点X坐标 */
    uint16_t y;    /* 触摸点Y坐标 */
    uint16_t size; /* 触摸点大小 */
} atk_rgblcd_touch_point_t;

/* 错误代码 */
#    define ATK_RGBLCD_TOUCH_EOK   0 /* 没有错误 */
#    define ATK_RGBLCD_TOUCH_ERROR 1 /* 错误 */

/* 操作函数 */
uint8_t atk_rgblcd_touch_init(atk_rgblcd_touch_type_t type);                 /* RGB LCD模块触摸初始化 */
uint8_t atk_rgblcd_touch_scan(atk_rgblcd_touch_point_t *point, uint8_t cnt); /* RGB LCD模块触摸扫描 */

#endif

#endif
