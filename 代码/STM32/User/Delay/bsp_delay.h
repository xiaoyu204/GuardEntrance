#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"

#define delay_ms(x) HAL_Delay(x)
#define DELAY_TIM   htim17

// void delay_us(uint16_t us);
void delay_us(uint32_t us);

#endif // __BSP_DELAY_H__