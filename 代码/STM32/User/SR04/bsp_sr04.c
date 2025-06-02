#include "bsp_sr04.h"

static float distant;              // 测量距离
uint32_t     measure_Buf[3] = {0}; // 存放定时器计数值的数组
uint8_t      measure_Cnt    = 0;   // 状态标志位
uint32_t     high_time;            // 超声波模块返回的高电平时间

/**
 * @brief   HCSR04读取距离
 * @retval  无
 */
float SR04_GetData(void)
{
    switch (measure_Cnt) {
        case 0:
            TRIG_H();
            delay_us(30);
            TRIG_L();

            measure_Cnt++;
            __HAL_TIM_SET_CAPTUREPOLARITY(&SR04_TIM, SR04_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
            HAL_TIM_IC_Start_IT(&SR04_TIM, SR04_CHANNEL); // 启动输入捕获       或者: __HAL_TIM_ENABLE(&SR04_TIM);
            return -1;
        case 3:
            high_time = measure_Buf[1] - measure_Buf[0]; // 高电平时间
            // printf("\r\n----高电平时间-%ld-us----\r\n", high_time);
            distant = (high_time * 0.034) / 2; // 单位cm
            // printf("\r\n-检测距离为-%.2f-cm-\r\n", distant);
            measure_Cnt            = 0; // 清空标志位
            SR04_TIM.Instance->CNT = 0; // 清空计时器计数
            return distant;
    }

    return -1;
}

/**
 * @brief   HCSR04超声波任务
 * @param   pvParameters: 任务参数
 * @retval  无
 */
void SR04_Task(void *pvParameters)
{
    extern lv_ui guider_ui;
    float        distance          = 0;
    bool         first_measurement = true;
    // extern SemaphoreHandle_t MutexSemaphore; /* lv_task_handler()线程不安全必须加互斥量 */

    while (1) {
        distance = 8.0;
        distance = SR04_GetData();

        // 如果是第一次测量且返回-1，则跳过显示
        if (first_measurement && distance == -1) {
            first_measurement = false;
            vTaskDelay(500);
            continue;
        }

        // xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
        lv_label_set_text_fmt(guider_ui.main_label_distance, "%.1f cm", distance);
        // xSemaphoreGive(MutexSemaphore);
        first_measurement = true;
        if (distance <= 10.0 && distance >= 0.0) {
            uint8_t data = CMD_WAKE;

            HAL_UART_Transmit(&ESP32_UART, &data, 1, HAL_MAX_DELAY);
        }
        vTaskDelay(500);
    }
}
