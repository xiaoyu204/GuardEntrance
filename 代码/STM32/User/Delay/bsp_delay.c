#include "bsp_delay.h"

/**
 * @brief   us延迟
 * @param   nus: us数
 * @retval  无
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, reload, tcnt = 0;
    // if ((0x0001 & (SysTick->CTRL)) == 0) // 定时器未工作
    //     vPortSetupTimerInterrupt();      // 初始化定时器

    reload = SysTick->LOAD;                     // 获取重装载寄存器值
    ticks  = nus * (SystemCoreClock / 1000000); // 计数时间值

    vTaskSuspendAll(); // 阻止OS调度，防止打断us延时

    told = SysTick->VAL; // 获取当前数值寄存器值（开始时数值）
    while (1) {
        tnow = SysTick->VAL; // 获取当前数值寄存器值
        if (tnow != told)    // 当前值不等于开始值说明已在计数
        {
            if (tnow < told)         // 当前值小于开始数值，说明未计到0
                tcnt += told - tnow; // 计数值=开始值-当前值

            else                              // 当前值大于开始数值，说明已计到0并重新计数
                tcnt += reload - tnow + told; // 计数值=重装载值-当前值+开始值  （
                                              // 已从开始值计到0）

            told = tnow; // 更新开始值
            if (tcnt >= ticks)
                break; // 时间超过/等于要延迟的时间,则退出.
        }
    }

    xTaskResumeAll(); // 恢复OS调度
}
// SystemCoreClock为系统时钟(system_stmf4xx.c中)，通常选择该时钟作为
// systick定时器时钟，根据具体情况更改

/******************** 激活函数 *******************/
uint8_t ReLU(int8_t input)
{
    return (input < 0) ? 0 : input;
}

// void delay_us(uint16_t us)
// { // 微秒延时
//     uint16_t differ = 0xffff - us - 5;
//     __HAL_TIM_SET_COUNTER(&DELAY_TIM, differ); // 设定TIM4计数器起始值
//     HAL_TIM_Base_Start(&DELAY_TIM);            // 启动定时器

//     while (differ < 0xffff - 5) {                   // 判断
//         differ = __HAL_TIM_GET_COUNTER(&DELAY_TIM); // 查询计数器的计数值
//     }
//     HAL_TIM_Base_Stop(&DELAY_TIM);
// }