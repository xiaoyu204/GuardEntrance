#pragma once
#include <Arduino.h>

class KalmanFilter
{
private:
    float    err_measure;      // 测量噪声
    float    err_estimate;     // 估计误差
    float    q;                // 过程噪声
    uint16_t current_estimate; // 当前估计值
    uint16_t last_estimate;    // 上一次估计值
    float    kalman_gain;      // 卡尔曼增益

public:
    KalmanFilter(float mea_e, float est_e, float q);
    uint16_t updateEstimate(uint16_t mea);
    void     setMeasurementError(float mea_e) { err_measure = mea_e; }
    void     setEstimateError(float est_e) { err_estimate = est_e; }
    void     setProcessNoise(float q) { this->q = q; }
    float    getKalmanGain() { return kalman_gain; }
};