#ifndef __BSP_ESP32_H_
#define __BSP_ESP32_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h" 
#include "usart.h"
#include "bsp_cmd.h"
#include "bsp_beep.h"
#include "queue.h"
#include "event_groups.h"
#include "blood.h"
#include "bsp_mlx90614.h"
#include "gui_guider.h"
#include "semphr.h"
#include "bsp_bit.h"

#define ESP32_UART     hlpuart1

typedef struct {
    bool    call;
    bool    alarm;
    uint8_t visitors;
    char    weather[20];
    uint8_t temperature;
} Attributes_T;

void ESP32_Task(void *pvParameters);

#endif // !__BSP_ESP32_H_
