/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lvgl.h"
#include "semphr.h"
// #include "lv_demos.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "tim.h"
#include "bsp_sg90.h"
#include "bsp_rc522.h"
#include "bsp_as608.h"
#include "bsp_sr04.h"
#include "bsp_mlx90614.h"
#include "bsp_esp32.h"
#include "gui_guider.h"
#include "events_init.h"
#include "bsp_cmd.h"
#include "blood.h"
#include "guider_customer_fonts.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* FreeRTOS变量 */
static BaseType_t  xHigherPriorityTaskWoken = pdFALSE;
EventGroupHandle_t DoorEventGroup;
QueueHandle_t      ESP32Queue;
TaskHandle_t       AS608_Task_Handle;
SemaphoreHandle_t  MutexSemaphore; /* lv_task_handler()线程不安全必须加互斥量 */

/* 普通变量 */
Attributes_T      attributes;
lv_ui             guider_ui;
volatile uint32_t CPU_RunTime       = 0;
static uint8_t    ESP32_RX_BUF[100] = {0};
char              message[100];
// static uint8_t     ESP32_Val         = CMD_RESTART;
// uint8_t            unum              = 0;
static uint8_t cmd         = CMD_GET_INIT;
static uint8_t message_num = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 640 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Start_Task(void *pvParameters);
void LVGL_Task(void *pvParameters);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
    CPU_RunTime = 0ul;
}

__weak unsigned long getRunTimeCounterValue(void)
{
    return CPU_RunTime;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook(void)
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()). */
    lv_tick_inc(1);
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
    called if a stack overflow is detected. */
    printf("Stack overflow in task: %s\n", pcTaskName);
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created. It is also called by various parts of the
    demo application. If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    printf("Malloc Failed\n");
}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* 创建启动任务 */
    xTaskCreate((TaskFunction_t)Start_Task, "Start_Task", 450, NULL, osPriorityNormal, NULL);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    // char pcTaskInfo[300]; /* 用于保存任务状态信息 */
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    // lv_demo_stress();
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    // lv_demo_widgets();
    HAL_GPIO_WritePin(AS608_TOUCH_GPIO_Port, AS608_TOUCH_Pin, GPIO_PIN_RESET);
    HAL_UARTEx_ReceiveToIdle_IT(&ESP32_UART, ESP32_RX_BUF, sizeof(ESP32_RX_BUF));
    HAL_UART_Transmit(&ESP32_UART, &cmd, 1, HAL_MAX_DELAY);

    /* Infinite loop */
    for (;;) {
        //   vTaskGetRunTimeStats(pcTaskInfo);
        //   printf("Task Name\tRun Time\tpercent\t \r\n");
        //   printf("%s\r\n", pcTaskInfo);
        // blood_Loop(&dis_hr_valid, &dis_spo2_valid);1

        if (attributes.alarm) {
            Beep_ON();
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
            vTaskDelay(500);
            Beep_OFF();
            vTaskDelay(500);
        } else {
            // uint8_t temp[4] = {0xB6, 0x00, 0x41, 0x5A};
            // HAL_UART_Transmit(&ESP32_UART, temp, sizeof(temp), HAL_MAX_DELAY);
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
            vTaskDelay(500);
        }
    }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
 * @brief   LVGL任务
 * @param   pvParameters: 任务参数
 * @retval  无
 */
void LVGL_Task(void *pvParameters)
{
    while (1) {
        xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
        lv_task_handler();
        xSemaphoreGive(MutexSemaphore);
        vTaskDelay(4);
    }
}

/**
 * @brief   开始任务
 * @param   pvParameters: 任务参数
 * @retval  无
 */
void Start_Task(void *pvParameters)
{
    BaseType_t xReturn;

    /* 初始化 */
    SG90_Init();
    RC522_Init();
    // MAX30102_Init();
    // MAX30102_GET_Init();
    // Max30102_reset();
    // MAX30102_Config();
    AS608_Init();
    SMBus_Init();
    // HAL_UART_Transmit(&ESP32_UART, &ESP32_Val, 1, HAL_MAX_DELAY);
    // HAL_UART_Receive(&ESP32_UART, &ESP32_Val, 1, HAL_MAX_DELAY);
    // while (ESP32_Val != CMD_RESTART_OK);
    // printf("ESP32 restart OK\n");

    // HAL_UART_Receive(&ESP32_UART, &unum, 1, HAL_MAX_DELAY);
    // printf("unum: %d\n", unum);

    // uint8_t TempUID[4] = {0xA5, 0x29, 0xF6, 0x04};
    // RegisterCard(TempUID);

    /* 创建事件组 */
    DoorEventGroup = xEventGroupCreate();
    if (DoorEventGroup == NULL) {
        // 处理事件组创建失败的情况
        printf("Failed to create DoorEventGroup\n");
        printf("Press reset to restart\n");
        while (1);
    }

    /* 创建队列 */
    ESP32Queue = xQueueCreate(1, sizeof(uint8_t));
    if (ESP32Queue == NULL) {
        printf("Failed to create ESP32Queue\n");
        printf("Press reset to restart\n");
        while (1);
    }

    /* 创建LVGL互斥锁 */
    MutexSemaphore = xSemaphoreCreateMutex();
    if (MutexSemaphore == NULL) {
        printf("Failed to create MutexSemaphore\n");
        printf("Press reset to restart\n");
        while (1);
    }

    /* 创建LVGL任务 */
    xReturn = xTaskCreate((TaskFunction_t)LVGL_Task, "LVGL_Task", 800, NULL, osPriorityNormal + 1, NULL);
    if (xReturn != pdPASS) {
        // 处理任务创建失败的情况
        printf("Failed to create LVGL_Task\n");
        printf("press reset to restart\n");
    }

    /* 创建SG90舵机任务 */
    xReturn = xTaskCreate((TaskFunction_t)SG90_Task, "SG90_Task", 70, NULL, osPriorityNormal, NULL);
    if (xReturn != pdPASS) {
        // 处理任务创建失败的情况
        printf("Failed to create SG90_Task\n");
        printf("press reset to restart\n");
    }

    /* 创建RC522读卡器任务 */
    xReturn = xTaskCreate((TaskFunction_t)RC522_Task, "RC522_Task", 200, NULL, osPriorityNormal, NULL);
    if (xReturn != pdPASS) {
        // 处理任务创建失败的情况
        printf("Failed to create RC522_Task\n");
        printf("press reset to restart\n");
    }

    /* 创建AS608指纹识别任务 */
    xReturn = xTaskCreate((TaskFunction_t)AS608_Task, "AS608_Task", 150, NULL, osPriorityNormal, &AS608_Task_Handle);
    if (xReturn != pdPASS) {
        // 处理任务创建失败的情况
        printf("Failed to create AS608_Task\n");
        printf("press reset to restart\n");
    }

    /* 创建ESP32任务 */
    xReturn = xTaskCreate((TaskFunction_t)ESP32_Task, "ESP32_Task", 200, NULL, osPriorityNormal, NULL);
    if (xReturn != pdPASS) {
        // 处理任务创建失败的情况
        printf("Failed to create ESP32_Task\n");
        printf("press reset to restart\n");
    }

    /* 创建SR04超声波任务 */
    xReturn = xTaskCreate((TaskFunction_t)SR04_Task, "SR04_Task", 380, NULL, osPriorityNormal, NULL);
    if (xReturn != pdPASS) {
        // 处理任务创建失败的情况
        printf("Failed to create SR04_Task\n");
        printf("press reset to restart\n");
    }


    /* 自杀 */
    vTaskDelete(NULL);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* 进入临界段，临界段可以嵌套 */
    uint32_t ulReturn = taskENTER_CRITICAL_FROM_ISR();

    if (GPIO_Pin == AS608_TOUCH_Pin) {
        xTaskNotifyFromISR(AS608_Task_Handle, AS608_TASK_BIT, eSetBits, &xHigherPriorityTaskWoken);
        // xEventGroupSetBitsFromISR(DoorEventGroup, AS608_TASK_BIT, &xHigherPriorityTaskWoken);
        printf("Finger Touch\n");
    }

    /* 退出临界段 */
    taskEXIT_CRITICAL_FROM_ISR(ulReturn);

    /* 如果有更高优先级的任务被唤醒，则进行上下文切换 */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    /* 进入临界段，临界段可以嵌套 */
    uint32_t ulReturn = taskENTER_CRITICAL_FROM_ISR();

    /* 串口接收不定长数据 */
    if (huart == &AS608_UART) {
        extern SemaphoreHandle_t AS608Semphore;

        xSemaphoreGiveFromISR(AS608Semphore, &xHigherPriorityTaskWoken);

        /* 必须再调用HAL_UARTEx_ReceiveToIdle_IT函数，否则只会接收数据一次 */
        HAL_UARTEx_ReceiveToIdle_IT(&AS608_UART, AS608_RX_BUF, sizeof(AS608_RX_BUF));
    }

    if (huart == &ESP32_UART) {
        if (Size == 1) {
            xQueueSendToBackFromISR(ESP32Queue, ESP32_RX_BUF, &xHigherPriorityTaskWoken);
        } else if (Size == 5) {
            if (ESP32_RX_BUF[0] == CMD_RECEIVE_UID) {
                RegisterCard(ESP32_RX_BUF + 1);
            } else if (ESP32_RX_BUF[0] == CMD_RECEIVE_WEATHER) {
                memcpy(attributes.weather, ESP32_RX_BUF + 1, Size - 1);
                attributes.weather[Size] = '\0'; // 添加空终止符
                printf("weather: %s\n", attributes.weather);
                lv_label_set_text(guider_ui.main_label_weather, attributes.weather);
            }
        } else {
            switch (ESP32_RX_BUF[0]) {
                case CMD_RECEIVE_CALL:
                    // memcpy(attributes.call, ESP32_RX_BUF + 1, Size - 1);
                    // attributes.call[Size] = '\0'; // 添加空终止符
                    attributes.call = ESP32_RX_BUF[1];
                    printf("call: %d\n", attributes.call);
                    break;
                case CMD_RECEIVE_ALARM:
                    // memcpy(attributes.alarm, ESP32_RX_BUF + 1, Size - 1);
                    // attributes.alarm[Size] = '\0'; // 添加空终止符
                    attributes.alarm = ESP32_RX_BUF[1];
                    printf("alarm: %d\n", attributes.alarm);
                    break;
                case CMD_RECEIVE_VISITORS:
                    attributes.visitors = ESP32_RX_BUF[1];
                    printf("visitors: %d\n", attributes.visitors);
                    lv_label_set_text_fmt(guider_ui.main_label_visitor, "%d 人", attributes.visitors);
                    break;
                case CMD_RECEIVE_WEATHER:
                    memcpy(attributes.weather, ESP32_RX_BUF + 1, Size - 1);
                    attributes.weather[Size] = '\0'; // 添加空终止符
                    printf("weather: %s\n", attributes.weather);
                    lv_label_set_text(guider_ui.main_label_weather, attributes.weather);
                    break;
                case CMD_RECEIVE_TEMPERATURE:
                    attributes.temperature = ESP32_RX_BUF[1];
                    printf("weather temperature: %d\n", attributes.temperature);
                    lv_label_set_text_fmt(guider_ui.main_label_weather_tem, "%d C", attributes.temperature);
                    break;
                case CMD_LEAVE_MESSAGE:
                    memcpy(message, ESP32_RX_BUF + 1, Size - 1);
                    message[Size] = '\0';
                    printf("message: %s\n", message);
                    switch (message_num++) {
                        case 0: lv_label_set_text(guider_ui.main_label_message_1, message); break;
                        case 1: lv_label_set_text(guider_ui.main_label_message_2, message); break;
                        case 2: lv_label_set_text(guider_ui.main_label_message_3, message); break;
                        case 3: lv_label_set_text(guider_ui.main_label_message_4, message); break;
                        case 4: lv_label_set_text(guider_ui.main_label_message_5, message); break;
                        case 5: lv_label_set_text(guider_ui.main_label_message_6, message); break;
                        case 6: lv_label_set_text(guider_ui.main_label_message_7, message); break;
                        case 7: lv_label_set_text(guider_ui.main_label_message_8, message); break;
                        default: break;
                    }
                    break;
                default: printf("\nReceive String: %s", ESP32_RX_BUF); break;
            }
        }

        HAL_UARTEx_ReceiveToIdle_IT(&ESP32_UART, ESP32_RX_BUF, sizeof(ESP32_RX_BUF));
    }

    /* 退出临界段 */
    taskEXIT_CRITICAL_FROM_ISR(ulReturn);

    /* 如果有更高优先级的任务被唤醒，则进行上下文切换 */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    /* 进入临界段，临界段可以嵌套 */
    uint32_t ulReturn = taskENTER_CRITICAL_FROM_ISR();

    if (htim == &SR04_TIM) // 判断触发的中断的定时器为SR04_TIM
    {
        extern uint8_t  measure_Cnt;
        extern uint32_t measure_Buf[3];

        switch (measure_Cnt) {
            case 1:
                measure_Buf[0] = HAL_TIM_ReadCapturedValue(&SR04_TIM, SR04_CHANNEL); // 获取当前的捕获值.
                __HAL_TIM_SET_CAPTUREPOLARITY(&SR04_TIM, SR04_CHANNEL,
                                              TIM_ICPOLARITY_FALLING); // 设置为下降沿捕获
                measure_Cnt++;
                break;
            case 2:
                measure_Buf[1] = HAL_TIM_ReadCapturedValue(&SR04_TIM, SR04_CHANNEL); // 获取当前的捕获值.
                HAL_TIM_IC_Stop_IT(&SR04_TIM,
                                   SR04_CHANNEL); // 停止捕获   或者: __HAL_TIM_DISABLE(&SR04_TIM);
                measure_Cnt++;
        }
    }

    /* 退出临界段 */
    taskEXIT_CRITICAL_FROM_ISR(ulReturn);

    /* 如果有更高优先级的任务被唤醒，则进行上下文切换 */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

int _write(int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++) {
        while ((USART1->ISR & 0X40) == 0); // 等待上一次串口数据发送完成
        USART1->TDR = (uint8_t)pBuffer[i]; // 写TDR,串口将发送数据
    }
    return size;
}

/* USER CODE END Application */

