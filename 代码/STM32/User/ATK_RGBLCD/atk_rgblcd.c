/**
 ****************************************************************************************************
 * @file        atk_rgblcd.c
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

#include "atk_rgblcd.h"
#include "atk_rgblcd_font.h"
#include "usart.h"
#include "tim.h"

#define ATK_RGBLCD_LCD_RAW_WIDTH  g_atk_rgblcd_sta.param->width
#define ATK_RGBLCD_LCD_RAW_HEIGHT g_atk_rgblcd_sta.param->height
#define ATK_RGBLCD_LCD_WIDTH      g_atk_rgblcd_sta.width
#define ATK_RGBLCD_LCD_HEIGHT     g_atk_rgblcd_sta.height
#define ATK_RGBLCD_FB             g_atk_rgblcd_sta.fb

/* RGB LCD模块参数结构体 */
typedef struct {
    uint8_t                 id;
    uint16_t                pid;
    uint16_t                width;
    uint16_t                height;
    atk_rgblcd_timing_t     timing;
    atk_rgblcd_touch_type_t touch_type;
} atk_rgblcd_param_t;

/* RGB LCD模块参数匹配表 */
static const atk_rgblcd_param_t atk_rgblcd_param[] = {
    {0,
     ATK_RGBLCD_PID_4342,
     480,
     272,
     {9000000, 1, 480, 40, 5, 1, 272, 8, 8, 1},
     ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD0430R-480272
    {1,
     ATK_RGBLCD_PID_7084,
     800,
     480,
     {33000000, 1, 800, 46, 210, 1, 480, 23, 22, 1},
     ATK_RGBLCD_TOUCH_TYPE_FTXX}, // ATK-MD0700R-800480
    {2,
     ATK_RGBLCD_PID_7016,
     1024,
     600,
     {45000000, 1, 1024, 140, 160, 20, 600, 20, 12, 3},
     ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD0700R-1024600
    {3,
     ATK_RGBLCD_PID_7018,
     1280,
     800,
     {0, 1, 1280, 0, 0, 0, 800, 0, 0, 0},
     ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD0700R-1280800
    {4,
     ATK_RGBLCD_PID_4384,
     800,
     480,
     {33000000, 1, 800, 88, 40, 48, 480, 32, 13, 3},
     ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD0430R-800480
    {5,
     ATK_RGBLCD_PID_1018,
     1280,
     800,
     {45000000, 0, 1280, 140, 10, 10, 800, 10, 10, 3},
     ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD1010R-1280800
};

/* RGB LCD模块状态数据结构体 */
static struct {
    const atk_rgblcd_param_t *param;
    uint16_t                  width;
    uint16_t                  height;
    atk_rgblcd_lcd_disp_dir_t disp_dir;
    uint16_t                 *fb;
} g_atk_rgblcd_sta = {0};

/**
 * @brief       RGB LCD模块触摸芯片类型获取
 * @param       无
 * @retval      RGB LCD模块触摸芯片类型
 */
atk_rgblcd_touch_type_t Get_TPType(void)
{
    return g_atk_rgblcd_sta.param->touch_type;
}

/**
 * @brief       RGB LCD模块ID获取
 * @param       无
 * @retval      RGB LCD模块ID
 */
static uint8_t atk_rgblcd_get_id(void)
{
    uint8_t id;

    id = ATK_RGBLCD_READ_M0();
    id |= ATK_RGBLCD_READ_M1() << 1;
    id |= ATK_RGBLCD_READ_M2() << 2;

    return id;
}

/**
 * @brief       RGB LCD模块参数初始化
 * @param       通过RGB LCD的ID确定RGB LCD的尺寸和时序
 * @retval      ATK_RGBLCD_EOK   : RGB LCD模块参数初始化成功
 *              ATK_RGBLCD_EINVAL: 输入ID无效
 */
static uint8_t atk_rgblcd_setup_param_by_id(uint8_t id)
{
    uint8_t index;

    for (index = 0; index < (sizeof(atk_rgblcd_param) / sizeof(atk_rgblcd_param[0])); index++) {
        if (id == atk_rgblcd_param[index].id) {
            g_atk_rgblcd_sta.param = &atk_rgblcd_param[index];
            return ATK_RGBLCD_EOK;
        }
    }

    return ATK_RGBLCD_EINVAL;
}

/**
 * @brief       根据RGB LCD模块显示方向换算坐标值
 * @param       x: X坐标值
 *              y: Y坐标值
 * @retval      无
 */
static inline void atk_rgblcd_pos_transform(uint16_t *x, uint16_t *y)
{
    uint16_t x_target = 0;
    uint16_t y_target = 0;

    switch (g_atk_rgblcd_sta.disp_dir) {
        case ATK_RGBLCD_LCD_DISP_DIR_0: {
            x_target = *x;
            y_target = *y;
            break;
        }
        case ATK_RGBLCD_LCD_DISP_DIR_90: {
            x_target = ATK_RGBLCD_LCD_RAW_WIDTH - *y - 1;
            y_target = *x;
            break;
        }
        case ATK_RGBLCD_LCD_DISP_DIR_180: {
            x_target = ATK_RGBLCD_LCD_RAW_WIDTH - *x - 1;
            y_target = ATK_RGBLCD_LCD_RAW_HEIGHT - *y - 1;
            break;
        }
        case ATK_RGBLCD_LCD_DISP_DIR_270: {
            x_target = *y;
            y_target = ATK_RGBLCD_LCD_RAW_HEIGHT - *x - 1;
            break;
        }
    }

    *x = x_target;
    *y = y_target;
}

#if (ATK_RGBLCD_USING_DMA2D != 0)

/**
 * @brief       DMA2D LCD区域填充
 * @param       xs   : 区域起始X坐标
 *              ys   : 区域起始Y坐标
 *              xe   : 区域终止X坐标
 *              ye   : 区域终止Y坐标
 *              color: 区域填充颜色
 * @retval      无
 */
static inline void atk_rgblcd_dma2d_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color)
{
    atk_rgblcd_pos_transform(&xs, &ys);
    atk_rgblcd_pos_transform(&xe, &ye);

    if (xs > xe) {
        xs = xs ^ xe;
        xe = xs ^ xe;
        xs = xs ^ xe;
    }
    if (ys > ye) {
        ys = ys ^ ye;
        ye = ys ^ ye;
        ys = ys ^ ye;
    }

    DMA2D->CR &= ~DMA2D_CR_START;                                               /* 停止DMA2D */
    DMA2D->OOR  = ATK_RGBLCD_LCD_RAW_WIDTH - (xe - xs + 1);                     /* 行偏移 */
    DMA2D->OMAR = (uint32_t)&ATK_RGBLCD_FB[ys * ATK_RGBLCD_LCD_RAW_WIDTH + xs]; /* 存储器地址 */
    DMA2D->NLR &= ~DMA2D_NLR_PL_Msk;                                            /* 每行的像素数 */
    DMA2D->NLR |= ((xe - xs + 1) << DMA2D_NLR_PL_Pos);
    DMA2D->NLR &= ~DMA2D_NLR_NL_Msk; /* 总的行数 */
    DMA2D->NLR |= ((ye - ys + 1) << DMA2D_NLR_NL_Pos);
    DMA2D->OCOLR = color;                                    /* 输出颜色 */
    DMA2D->CR |= DMA2D_CR_START;                             /* 开启DMA2D */
    while ((DMA2D->ISR & DMA2D_ISR_TCIF) != DMA2D_ISR_TCIF); /* 等待DMA2D传输完成 */
    DMA2D->IFCR |= DMA2D_IFCR_CTCIF;                         /* 清除传输完成标志 */
}
#endif

inline void atk_rgblcd_dma2d_fill_lvgl(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t *color_p)
{
    extern uint8_t g_gpu_state;

    /* 坐标变换 */
    atk_rgblcd_pos_transform(&xs, &ys);
    atk_rgblcd_pos_transform(&xe, &ye);

    if (xs > xe) {
        xs = xs ^ xe;
        xe = xs ^ xe;
        xs = xs ^ xe;
    }
    if (ys > ye) {
        ys = ys ^ ye;
        ye = ys ^ ye;
        ys = ys ^ ye;
    }

    // __HAL_RCC_DMA2D_CLK_ENABLE();                                               /* 开启DMA2D时钟 */
    DMA2D->CR &= ~DMA2D_CR_START;              /* 停止DMA2D */
    DMA2D->CR      = DMA2D_M2M;                /* 内存到内存模式 */
    DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565; /* 前景层采用的RGB565颜色格式 */
    // DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_RGB565;                                  /* 输出区域采用的RGB565颜色格式 */
    DMA2D->FGOR  = 0;                                                            /* 前景层输入偏移 */
    DMA2D->OOR   = ATK_RGBLCD_LCD_RAW_WIDTH - (xe - xs + 1);                     /* 输出偏移 */
    DMA2D->FGMAR = (uint32_t)color_p;                                            /* 源地址 */
    DMA2D->OMAR  = (uint32_t)&ATK_RGBLCD_FB[ys * ATK_RGBLCD_LCD_RAW_WIDTH + xs]; /* 目标地址 */
    DMA2D->NLR   = (ye - ys + 1) | ((xe - xs + 1) << DMA2D_NLR_PL_Pos);          /* 设定行数寄存器 */
    DMA2D->CR |= DMA2D_IT_TC | DMA2D_IT_TE | DMA2D_IT_CE;                        /* 开启中断 */
    DMA2D->CR |= DMA2D_CR_START;                                                 /* 启动传输 */
    // while (DMA2D->CR & DMA2D_CR_START);                                         /* 等待DMA2D传输完成 */
    // DMA2D->IFCR |= DMA2D_IFCR_CTCIF;                                            /* 清除传输完成标志 */
    g_gpu_state = 1;
}

/**
 * @brief       RGB LCD模块初始化
 * @param       无
 * @retval      ATK_RGBLCD_EOK  : RGB LCD模块初始化成功
 *              ATK_RGBLCD_ERROR: RGB LCD模块初始化失败
 */
uint8_t atk_rgblcd_init(void)
{
    uint8_t id;
    uint8_t ret;

    id  = atk_rgblcd_get_id();              /* RGB LCD模块ID获取 */
    ret = atk_rgblcd_setup_param_by_id(id); /* RGB LCD模块参数初始化 */
    if (ret != ATK_RGBLCD_EOK) {
        return ATK_RGBLCD_ERROR;
    }
    g_atk_rgblcd_sta.fb = (uint16_t *)ATK_RGBLCD_LTDC_LAYER_FB_ADDR;
    atk_rgblcd_set_disp_dir(ATK_RGBLCD_LCD_DISP_DIR_0);
    // atk_rgblcd_clear(ATK_RGBLCD_WHITE);
    // #if (ATK_RGBLCD_USING_TOUCH != 0)
    //     ret = atk_rgblcd_touch_init(Get_TPType());
    //     if (ret != ATK_RGBLCD_TOUCH_EOK) {
    //         printf("touch init error\n");
    //         return ATK_RGBLCD_ERROR;
    //     }
    // #endif

    return ATK_RGBLCD_EOK;
}

/**
 * @brief       获取RGB LCD模块PID
 * @param       无
 * @retval      0     : RGB LCD模块未初始化
 *              其他值: RGB LCD模块PID
 */
uint16_t atk_rgblcd_get_pid(void)
{
    return g_atk_rgblcd_sta.param->pid;
}

/**
 * @brief       获取RGB LCD模块LCD宽度
 * @param       无
 * @retval      0     : RGB LCD模块未初始化
 *              其他值: RGB LCD模块LCD宽度
 */
uint16_t atk_rgblcd_get_lcd_width(void)
{
    return g_atk_rgblcd_sta.width;
}

/**
 * @brief       获取RGB LCD模块LCD高度
 * @param       无
 * @retval      0     : RGB LCD模块未初始化
 *              其他值: RGB LCD模块LCD高度
 */
uint16_t atk_rgblcd_get_lcd_height(void)
{
    return g_atk_rgblcd_sta.height;
}

/**
 * @brief       开启RGB LCD模块LCD背光
 * @param       无
 * @retval      无
 */
void atk_rgblcd_backlight_on(void)
{
    ATK_RGBLCD_BL(1);
}

/**
 * @brief       关闭RGB LCD模块LCD背光
 * @param       无
 * @retval      无
 */
void atk_rgblcd_backlight_off(void)
{
    ATK_RGBLCD_BL(0);
}

/**
 * @brief       开启RGB LCD模块LCD显示
 * @param       无
 * @retval      无
 */
void atk_rgblcd_display_on(void)
{
    __HAL_LTDC_ENABLE(&hltdc);
}

/**
 * @brief       关闭RGB LCD模块LCD显示
 * @param       无
 * @retval      无
 */
void atk_rgblcd_display_off(void)
{
    __HAL_LTDC_DISABLE(&hltdc);
}

/**
 * @brief       设置RGB LCD模块LCD显示方向
 * @param       disp_dir: ATK_RGBLCD_LCD_DISP_DIR_0  : LCD顺时针旋转0°显示内容
 *                        ATK_RGBLCD_LCD_DISP_DIR_90 : LCD顺时针旋转90°显示内容
 *                        ATK_RGBLCD_LCD_DISP_DIR_180: LCD顺时针旋转180°显示内容
 *                        ATK_RGBLCD_LCD_DISP_DIR_270: LCD顺时针旋转270°显示内容
 * @retval      ATK_RGBLCD_EOK   : 设置RGB LCD模块LCD显示方向成功
 *              ATK_RGBLCD_EINVAL: 传入参数错误
 */
uint8_t atk_rgblcd_set_disp_dir(atk_rgblcd_lcd_disp_dir_t disp_dir)
{
    switch (disp_dir) {
        case ATK_RGBLCD_LCD_DISP_DIR_0:
        case ATK_RGBLCD_LCD_DISP_DIR_180: {
            g_atk_rgblcd_sta.width  = g_atk_rgblcd_sta.param->width;
            g_atk_rgblcd_sta.height = g_atk_rgblcd_sta.param->height;
            break;
        }
        case ATK_RGBLCD_LCD_DISP_DIR_90:
        case ATK_RGBLCD_LCD_DISP_DIR_270: {
            g_atk_rgblcd_sta.width  = g_atk_rgblcd_sta.param->height;
            g_atk_rgblcd_sta.height = g_atk_rgblcd_sta.param->width;
            break;
        }
        default: {
            return ATK_RGBLCD_EINVAL;
        }
    }

    g_atk_rgblcd_sta.disp_dir = disp_dir;

    return ATK_RGBLCD_EOK;
}

/**
 * @brief       获取RGB LCD模块LCD扫描方向
 * @param       无
 * @retval      RGB LCD模块LCD扫描方向
 */
atk_rgblcd_lcd_disp_dir_t atk_rgblcd_get_disp_dir(void)
{
    return g_atk_rgblcd_sta.disp_dir;
}

/**
 * @brief       RGB LCD模块LCD区域填充
 * @param       xs   : 区域起始X坐标
 *              ys   : 区域起始Y坐标
 *              xe   : 区域终止X坐标
 *              ye   : 区域终止Y坐标
 *              color: 区域填充颜色
 * @retval      无
 */
void atk_rgblcd_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color)
{
    if (xe >= ATK_RGBLCD_LCD_WIDTH) {
        xe = ATK_RGBLCD_LCD_WIDTH - 1;
    }
    if (ye >= ATK_RGBLCD_LCD_HEIGHT) {
        ye = ATK_RGBLCD_LCD_HEIGHT - 1;
    }

#if (ATK_RGBLCD_USING_DMA2D != 0)
    atk_rgblcd_dma2d_fill(xs, ys, xe, ye, color);
#else
    uint16_t x_index;
    uint16_t y_index;

    for (y_index = ys; y_index < ye + 1; y_index++) {
        for (x_index = xs; x_index < xe + 1; x_index++) {
            atk_rgblcd_pos_transform(&x_index, &y_index);
            ATK_RGBLCD_FB[y_index * ATK_RGBLCD_LCD_RAW_WIDTH + x_index] = color;
        }
    }
#endif
}

/**
 * @brief       RGB LCD模块LCD清屏
 * @param       color: 清屏颜色
 * @retval      无
 */
void atk_rgblcd_clear(uint16_t color)
{
    atk_rgblcd_fill(0, 0, ATK_RGBLCD_LCD_WIDTH - 1, ATK_RGBLCD_LCD_HEIGHT - 1, color);
}

/**
 * @brief       RGB LCD模块LCD画点
 * @param       x    : 待画点的X坐标
 *              y    : 待画点的Y坐标
 *              color: 待画点的颜色
 * @retval      无
 */
void atk_rgblcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    atk_rgblcd_pos_transform(&x, &y);
    ATK_RGBLCD_FB[y * ATK_RGBLCD_LCD_RAW_WIDTH + x] = color;
}

/**
 * @brief       RGB LCD模块LCD读点
 * @param       x    : 待读点的X坐标
 *              y    : 待读点的Y坐标
 * @retval      待读点的颜色
 */
uint16_t atk_rgblcd_read_point(uint16_t x, uint16_t y)
{
    atk_rgblcd_pos_transform(&x, &y);
    return ATK_RGBLCD_FB[y * ATK_RGBLCD_LCD_RAW_WIDTH + x];
}

uint16_t *re_fb(void)
{
    return ATK_RGBLCD_FB;
}