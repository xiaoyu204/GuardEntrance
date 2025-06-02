#ifndef __BSP_BEEP_H_
#define __BSP_BEEP_H_

#include "main.h"

/* 开启蜂鸣器 */
#define Beep_ON()  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET)

/* 关闭蜂鸣器 */
#define Beep_OFF() HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)

/**
 * @brief 持续开启蜂鸣器
 * @param time: 蜂鸣器持续时间(ms)
 */
#define Beep_Last(time)                                                                                                \
    do {                                                                                                               \
        Beep_ON();                                                                                                     \
        vTaskDelay(time);                                                                                              \
        Beep_OFF();                                                                                                    \
    } while (0)

#endif // !__BSP_BEEP_H_