#ifndef __BSP_SG90_H_
#define __BSP_SG90_H_

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "event_groups.h"
#include "bsp_beep.h"
#include "bsp_bit.h"

#define SG90_TIM      htim15
#define SG90_CHANNEL  TIM_CHANNEL_1

void SG90_Init(void);
void SG90_SetAngle(uint16_t angle);
void SG90_Task(void *pvParameters);

#endif // !__BSP_SG90_H_