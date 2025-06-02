#ifndef __BSP_SR04_H_
#define __BSP_SR04_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "bsp_delay.h"
#include "bsp_rc522.h"
#include "bsp_cmd.h"
#include "gui_guider.h"
#include "semphr.h"

#define SR04_TIM htim12
#define SR04_CHANNEL TIM_CHANNEL_2

#define TRIG_H() HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET)
#define TRIG_L() HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET)

float SR04_GetData(void);
void  SR04_Task(void *pvParameters);

#endif // !__BSP_SR04_H_