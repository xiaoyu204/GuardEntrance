/**
 ****************************************************************************************************
 * @file        atk_rgblcd.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       正点原子RGB LCD模块驱动代码
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

#ifndef __ATK_RGBLCD_H
#define __ATK_RGBLCD_H

#include "main.h"
#include "ltdc.h"
#include <stdio.h>

/* 定义是否使用DMA2D */
#define ATK_RGBLCD_USING_DMA2D 0

/* 定义是否使用RGB LCD模块触摸 */
#define ATK_RGBLCD_USING_TOUCH 1

/* 定义RGB LCD模块启用的字体 */
#define ATK_RGBLCD_FONT_12     1
#define ATK_RGBLCD_FONT_16     1
#define ATK_RGBLCD_FONT_24     1
#define ATK_RGBLCD_FONT_32     1

/* 默认启用DMA2D */
#ifndef ATK_RGBLCD_USING_DMA2D
#    define ATK_RGBLCD_USING_DMA2D 1
#endif

/* 默认启用触摸 */
#ifndef ATK_RGBLCD_USING_TOUCH
#    define ATK_RGBLCD_USING_TOUCH 1
#endif

/* 默认启用12号字体 */
#if ((ATK_RGBLCD_FONT_12 == 0) && (ATK_RGBLCD_FONT_16 == 0) && (ATK_RGBLCD_FONT_24 == 0) && (ATK_RGBLCD_FONT_32 == 0))
#    undef ATK_RGBLCD_FONT_12
#    define ATK_RGBLCD_FONT_12 1
#endif

/* RGB LCD模块时序结构体 */
typedef struct {
    uint32_t clock_freq;
    uint8_t  pixel_clock_polarity;
    uint16_t hactive;
    uint16_t hback_porch;
    uint16_t hfront_porch;
    uint16_t hsync_len;
    uint16_t vactive;
    uint16_t vback_porch;
    uint16_t vfront_porch;
    uint16_t vsync_len;
} atk_rgblcd_timing_t;

/* RGB LCD触摸IC类型枚举 */
typedef enum {
    ATK_RGBLCD_TOUCH_TYPE_GTXX = 0x00, /* GT9111、GT9147、GT9271 */
    ATK_RGBLCD_TOUCH_TYPE_FTXX,        /* FT5206 */
} atk_rgblcd_touch_type_t;

/* 包含头文件 */
#if (ATK_RGBLCD_USING_TOUCH != 0)
#    include "atk_rgblcd_touch.h"
#endif

/* 引脚定义 */
#define ATK_RGBLCD_BL_GPIO_PORT BL_EN_GPIO_Port
#define ATK_RGBLCD_BL_GPIO_PIN  BL_EN_Pin
/* IO操作 */
#define ATK_RGBLCD_BL(x)                                                                                               \
    do {                                                                                                               \
        x ? HAL_GPIO_WritePin(ATK_RGBLCD_BL_GPIO_PORT, ATK_RGBLCD_BL_GPIO_PIN, GPIO_PIN_SET)                           \
          : HAL_GPIO_WritePin(ATK_RGBLCD_BL_GPIO_PORT, ATK_RGBLCD_BL_GPIO_PIN, GPIO_PIN_RESET);                        \
    } while (0)

#define ATK_RGBLCD_READ_M0()           1
#define ATK_RGBLCD_READ_M1()           0
#define ATK_RGBLCD_READ_M2()           0
#define ATK_RGBLCD_LTDC_LAYER_FB_ADDR  ((uint32_t)0xC0000000)

/* RGB LCD模块PID枚举 */
enum {
    ATK_RGBLCD_PID_4342 = 0x4342, /* ATK-MD0430R-480272 */
    ATK_RGBLCD_PID_7084 = 0x7084, /* ATK-MD0700R-800480 */
    ATK_RGBLCD_PID_7016 = 0x7016, /* ATK-MD0700R-1024600 */
    ATK_RGBLCD_PID_7018 = 0x7018, /* ATK-MD0700R-1280800 */
    ATK_RGBLCD_PID_4384 = 0x4384, /* ATK-MD0430R-800480 */
    ATK_RGBLCD_PID_1018 = 0x1018, /* ATK-MD1010R-1280800 */
};

/* RGB LCD模块LCD旋转方向枚举 */
typedef enum {
    ATK_RGBLCD_LCD_DISP_DIR_0 = 0x00, /* LCD顺时针旋转0°显示内容 */
    ATK_RGBLCD_LCD_DISP_DIR_90,       /* LCD顺时针旋转90°显示内容 */
    ATK_RGBLCD_LCD_DISP_DIR_180,      /* LCD顺时针旋转180°显示内容 */
    ATK_RGBLCD_LCD_DISP_DIR_270,      /* LCD顺时针旋转270°显示内容 */
} atk_rgblcd_lcd_disp_dir_t;

/* RGB LCD模块LCD显示字体枚举 */
typedef enum {
#if (ATK_RGBLCD_FONT_12 != 0)
    ATK_RGBLCD_LCD_FONT_12, /* 12号字体 */
#endif
#if (ATK_RGBLCD_FONT_16 != 0)
    ATK_RGBLCD_LCD_FONT_16, /* 16号字体 */
#endif
#if (ATK_RGBLCD_FONT_24 != 0)
    ATK_RGBLCD_LCD_FONT_24, /* 24号字体 */
#endif
#if (ATK_RGBLCD_FONT_32 != 0)
    ATK_RGBLCD_LCD_FONT_32, /* 32号字体 */
#endif
} atk_rgblcd_lcd_font_t;

/* RGB LCD模块LCD显示数字模式枚举 */
typedef enum {
    ATK_RGBLCD_NUM_SHOW_NOZERO = 0x00, /* 数字高位0不显示 */
    ATK_RGBLCD_NUM_SHOW_ZERO,          /* 数字高位0显示 */
} atk_rgblcd_num_mode_t;

/* 常用颜色定义 */
#define ATK_RGBLCD_WHITE   0xFFFF
#define ATK_RGBLCD_BLACK   0x0000
#define ATK_RGBLCD_BLUE    0x001F
#define ATK_RGBLCD_BRED    0xF81F
#define ATK_RGBLCD_GRED    0xFFE0
#define ATK_RGBLCD_GBLUE   0x07FF
#define ATK_RGBLCD_RED     0xF800
#define ATK_RGBLCD_MAGENTA 0xF81F
#define ATK_RGBLCD_GREEN   0x07E0
#define ATK_RGBLCD_CYAN    0x7FFF
#define ATK_RGBLCD_YELLOW  0xFFE0
#define ATK_RGBLCD_BROWN   0xBC40
#define ATK_RGBLCD_BRRED   0xFC07
#define ATK_RGBLCD_GRAY    0x8430

/* 错误代码 */
#define ATK_RGBLCD_EOK     0 /* 没有错误 */
#define ATK_RGBLCD_ERROR   1 /* 错误 */
#define ATK_RGBLCD_EINVAL  2 /* 非法参数 */

/* 操作函数 */
uint8_t                   atk_rgblcd_init(void);                                       /* RGB LCD模块初始化 */
uint16_t                  atk_rgblcd_get_pid(void);                                    /* 获取RGB LCD模块PID */
atk_rgblcd_touch_type_t   Get_TPType(void);                                            /* 获取触摸芯片类型 */
uint16_t                  atk_rgblcd_get_lcd_width(void);                              /* 获取RGB LCD模块LCD宽度 */
uint16_t                  atk_rgblcd_get_lcd_height(void);                             /* 获取RGB LCD模块LCD高度 */
void                      atk_rgblcd_backlight_on(void);                               /* 开启RGB LCD模块LCD背光 */
void                      atk_rgblcd_backlight_off(void);                              /* 关闭RGB LCD模块LCD背光 */
void                      atk_rgblcd_display_on(void);                                 /* 开启RGB LCD模块LCD显示 */
void                      atk_rgblcd_display_off(void);                                /* 关闭RGB LCD模块LCD显示 */
uint8_t                   atk_rgblcd_set_disp_dir(atk_rgblcd_lcd_disp_dir_t disp_dir); /* 设置RGB LCD模块LCD显示方向 */
atk_rgblcd_lcd_disp_dir_t atk_rgblcd_get_disp_dir(void);                               /* 获取RGB LCD模块LCD扫描方向 */
void atk_rgblcd_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color); /* RGB LCD模块LCD区域填充 */
void atk_rgblcd_dma2d_fill_lvgl(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t *color_p);
void atk_rgblcd_clear(uint16_t color);                              /* RGB LCD模块LCD清屏 */
void atk_rgblcd_draw_point(uint16_t x, uint16_t y, uint16_t color); /* RGB LCD模块LCD画点 */
uint16_t atk_rgblcd_read_point(uint16_t x, uint16_t y);             /* RGB LCD模块LCD读点 */

#endif
