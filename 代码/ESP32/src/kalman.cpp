#include "kalman.h"

KalmanFilter::KalmanFilter(float mea_e, float est_e, float q)
{
    err_measure  = mea_e;
    err_estimate = est_e;
    this->q      = q;
}

uint16_t KalmanFilter::updateEstimate(uint16_t mea)
{
    /* 预测步骤 */
    err_estimate += q;

    /* 计算卡尔曼增益 */
    kalman_gain = err_estimate / (err_estimate + err_measure);

    /* 更新当前估计值 */
    current_estimate = last_estimate + kalman_gain * (mea - last_estimate);

    /* 更新估计误差 */
    err_estimate = (1.0 - kalman_gain) * err_estimate;

    /* 保存当前估计值用于下一次计算 */
    last_estimate = current_estimate;

    return current_estimate;
}