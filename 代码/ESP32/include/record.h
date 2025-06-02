#pragma once
#include <Arduino.h>
#include <driver/adc.h>
#include <SPI.h>
#include <SdFat.h>
#include "kalman.h"
#include "wave.h"

#define REC_ADC_CHANNEL ADC1_CHANNEL_7

enum RecordTime { TIME_AUTH = 3, TIME_MESSAGE = 8 };

class Record
{
private:
    FsFile       file;                    // 录音文件
    int32_t      communicationData[1024]; // 接收缓冲区
    char         partWavData[1024];
    KalmanFilter kalmanFilter;

    void   InitAdcInput(adc1_channel_t adcChannel);
    size_t Read(char *data, int numData);

public:
    Record();
    void audioRecord(SdFs &sd, const char *fileName, uint8_t recordTime);
};