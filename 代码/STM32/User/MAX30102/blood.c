/**
 * ************************************************************************
 *
 * @file blood.c
 * @author zxr
 * @brief
 *
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr
 * ************************************************************************
 */
#include "blood.h"
#include "usart.h"

int   heart; // 定义心率
float SpO2;  // 定义血氧饱和度

// 调用外部变量
extern uint16_t fifo_red; // 定义FIFO中的红光数据
extern uint16_t fifo_ir;  // 定义FIFO中的红外光数据

uint16_t     g_fft_index = 0; // fft输入输出下标
struct compx s1[FFT_N + 16];  // FFT输入和输出：从S[1]开始存放，根据大小自己定义
struct compx s2[FFT_N + 16];  // FFT输入和输出：从S[1]开始存放，根据大小自己定义

#define CORRECTED_VALUE 47 // 标定血液氧气含量

/**
 * ************************************************************************
 * @brief 更新血氧数据
 * @note 从 MAX30102 的 FIFO 中读取红光和红外数据，并将它们存储到两个复数数组s1和s2中,
 * 		 这些数据随后可以用于进行傅里叶变换等后续处理
 *
 * ************************************************************************
 */
void blood_data_update(void)
{
    // 标志位被使能时 读取FIFO
    g_fft_index = 0;
    while (g_fft_index < FFT_N) {
        while (max30102_read_reg(REG_INTR_STATUS_1) & 0x40) {
            // 读取FIFO
            max30102_read_fifo(); // read from MAX30102 FIFO2
            // 将数据写入fft输入并清除输出
            if (g_fft_index < FFT_N) {
                // 将数据写入fft输入并清除输出
                s1[g_fft_index].real = fifo_red;
                s1[g_fft_index].imag = 0;
                s2[g_fft_index].real = fifo_ir;
                s2[g_fft_index].imag = 0;
                g_fft_index++;
            }
        }
    }
}

/**
 * ************************************************************************
 * @brief 血液信息转换
 *
 *
 * ************************************************************************
 */
void blood_data_translate(void)
{
    float    n_denom;
    uint16_t i;

    // 直流滤波
    float dc_red = 0;
    float dc_ir  = 0;
    float ac_red = 0;
    float ac_ir  = 0;

    for (i = 0; i < FFT_N; i++) {
        dc_red += s1[i].real;
        dc_ir += s2[i].real;
    }
    dc_red = dc_red / FFT_N;
    dc_ir  = dc_ir / FFT_N;
    for (i = 0; i < FFT_N; i++) {
        s1[i].real = s1[i].real - dc_red;
        s2[i].real = s2[i].real - dc_ir;
    }

    // 移动平均滤波
    for (i = 1; i < FFT_N - 1; i++) {
        n_denom    = (s1[i - 1].real + 2 * s1[i].real + s1[i + 1].real);
        s1[i].real = n_denom / 4.00;

        n_denom    = (s2[i - 1].real + 2 * s2[i].real + s2[i + 1].real);
        s2[i].real = n_denom / 4.00;
    }

    // 八点平均滤波
    for (i = 0; i < FFT_N - 8; i++) {
        n_denom    = (s1[i].real + s1[i + 1].real + s1[i + 2].real + s1[i + 3].real + s1[i + 4].real + s1[i + 5].real
                   + s1[i + 6].real + s1[i + 7].real);
        s1[i].real = n_denom / 8.00;

        n_denom    = (s2[i].real + s2[i + 1].real + s2[i + 2].real + s2[i + 3].real + s2[i + 4].real + s2[i + 5].real
                   + s2[i + 6].real + s2[i + 7].real);
        s2[i].real = n_denom / 8.00;
    }

    // 开始变换显示
    g_fft_index = 0;
    // 快速傅里叶变换
    FFT(s1);
    FFT(s2);

    for (i = 0; i < FFT_N; i++) {
        s1[i].real = sqrtf(s1[i].real * s1[i].real + s1[i].imag * s1[i].imag);
        s1[i].real = sqrtf(s2[i].real * s2[i].real + s2[i].imag * s2[i].imag);
    }
    // 计算交流分量
    for (i = 1; i < FFT_N; i++) {
        ac_red += s1[i].real;
        ac_ir += s2[i].real;
    }

    for (i = 0; i < 50; i++) {
        if (s1[i].real <= 10)
            break;
    }

    // 读取峰值点的横坐标  结果的物理意义为
    int s1_max_index = find_max_num_index(s1, 60);
    int s2_max_index = find_max_num_index(s2, 60);

    // 检查HbO2和Hb的变化频率是否一致
    if (i >= 45) {
        // 心率计算
        uint16_t Heart_Rate = 60.00 * SAMPLES_PER_SECOND * s1_max_index / FFT_N;
        heart               = Heart_Rate;

        // 血氧含量计算
        float R        = (ac_ir * dc_red) / (ac_red * dc_ir);
        float sp02_num = -45.060 * R * R + 30.354 * R + 94.845;
        SpO2           = sp02_num;

        // 状态正常
    } else // 数据发生异常
    {
        heart = 0;
        SpO2  = 0;
    }
    // 结束变换显示
}

/**
 * ************************************************************************
 * @brief 心率血氧循环函数
 *
 *
 * ************************************************************************
 */
void blood_Loop(int *dis_hr_valid, int *dis_spo2_valid)
{
    // 血液信息获取
    blood_data_update();
    // 血液信息转换
    blood_data_translate();

    SpO2 = (SpO2 > 99.99) ? 99.99 : SpO2;

    *dis_hr_valid = heart;
    *dis_spo2_valid = (int)SpO2;
    printf("心率%3d/min; 血氧%2d%%\n", heart, (int)SpO2);
}
