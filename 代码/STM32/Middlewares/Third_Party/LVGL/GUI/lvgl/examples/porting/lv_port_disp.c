/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#    include "lv_port_disp.h"
#    include <stdbool.h>
#    include "atk_rgblcd.h"
#    include "dma2d.h"
#    include "tim.h"

/*********************
 *      DEFINES
 *********************/
#    ifndef MY_DISP_HOR_RES
// #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default
// value 320 is used for now.
#        define MY_DISP_HOR_RES 800
#    endif

#    ifndef MY_DISP_VER_RES
// #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height,
// default value 240 is used for now.
#        define MY_DISP_VER_RES 480
#    endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
static void DMA2Dcallback(DMA2D_HandleTypeDef *hdma2d);

/**********************
 *  STATIC VARIABLES
 **********************/
/*Descriptor of a display driver*/
static lv_disp_drv_t disp_drv;
/*A buffer for 10 rows*/
static lv_color_t buf_2_1[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((section(".sdram_buf1")));
/*An other buffer for 10 rows*/
static lv_color_t buf_2_2[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((section(".sdram_buf2")));
volatile uint8_t  g_gpu_state = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your
     * display. The buffer has to be greater than 1 display row
     *
     * Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     */

    /* Example for 2) */
    static lv_disp_draw_buf_t draw_buf_dsc_2;

    /*Initialize the display buffer*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * MY_DISP_VER_RES);

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
    atk_rgblcd_init();
    hdma2d.XferCpltCallback = DMA2Dcallback;
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    // if (disp_flush_enabled) {
    //     // atk_rgblcd_fill(area->x1, area->y1, area->x2, area->y2, color_p->full);
    //     // atk_rgblcd_dma2d_fill_lvgl(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_p);

    //     // int32_t x;
    //     // int32_t y;
    //     // for (y = area->y1; y <= area->y2; y++) {
    //     //     for (x = area->x1; x <= area->x2; x++) {
    //     //         /*Put a piarea->x2l to the display. For example:*/
    //     //         /*put_px(x, y, *color_p)*/
    //     //         atk_rgblcd_draw_point(x, y, color_p->full);
    //     //         color_p++;
    //     //     }
    //     // }
    // }

    atk_rgblcd_dma2d_fill_lvgl(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_p);

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    // lv_disp_flush_ready(disp_drv);
}

static void DMA2Dcallback(DMA2D_HandleTypeDef *hdma2d)
{
    if (g_gpu_state == 1) {
        g_gpu_state = 0;
        lv_disp_flush_ready(&disp_drv);
    }
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
