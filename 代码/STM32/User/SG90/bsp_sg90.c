#include "bsp_sg90.h"

/**
 * @brief   初始化SG90舵机
 * @retval  无
 */
void SG90_Init(void)
{
    HAL_TIM_PWM_Start(&SG90_TIM, SG90_CHANNEL);
    SG90_SetAngle(0);
}

/**
 * @brief   设置SG90舵机角度
 * @param   angle: 角度
 * @retval  无
 */
void SG90_SetAngle(uint16_t angle)
{
    float pulse_time = (1.0 / 90.0) * angle + 0.5;
    uint16_t compare = (uint16_t)(pulse_time / 20.0 * 200);

    __HAL_TIM_SetCompare(&SG90_TIM, SG90_CHANNEL, compare);
}

/**
 * @brief   SG90舵机任务
 * @param   pvParameters: 任务参数
 * @retval  无
 */
void SG90_Task(void *pvParameters)
{
    extern EventGroupHandle_t DoorEventGroup;

    while (1) {
        /* 等待事件组的SG90_TASK_BIT */
        xEventGroupWaitBits(DoorEventGroup, SG90_TASK_BIT, pdTRUE, pdFALSE, portMAX_DELAY);

        /* 开门后5s关门 */
        SG90_SetAngle(90);
        // Beep_ON();
        vTaskDelay(5000);
        // Beep_OFF();
        SG90_SetAngle(0);
    }
}
