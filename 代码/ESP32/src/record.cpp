#include "record.h"

void Record::InitAdcInput(adc1_channel_t adcChannel)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(adcChannel, ADC_ATTEN_11db);
}

Record::Record() : kalmanFilter(0.1, 1.0, 0.01)
{
    InitAdcInput(REC_ADC_CHANNEL);
}

size_t Record::Read(char *data, int numData)
{
    size_t recvSize = 0;
    // 手动触发ADC采样并填充到data
    for (int i = 0; i < numData; i += 2) {
        uint16_t adcValue = kalmanFilter.updateEstimate(adc1_get_raw(REC_ADC_CHANNEL));
        data[i]           = adcValue & 0xFF;        // 低8位
        data[i + 1]       = (adcValue >> 8) & 0xFF; // 高8位
        recvSize += 2;

        /* 执行一次采样命令时间25μs，百度语音识别采样率要求是16000，可以算出采样一次为31us，故延迟5us */
        delayMicroseconds(5);
    }
    return recvSize;
}

void Record::audioRecord(SdFs &sd, const char *fileName, uint8_t recordTime)
{
    // 删除并创建文件
    sd.remove(fileName);
    file = sd.open(fileName, O_WRITE | O_CREAT);
    if (!file) {
        Serial.println("crate file error");
        return;
    }

    const int waveDataSize = recordTime * SAMPLE_RATE * 2;
    auto      header       = CreateWaveHeader(1, SAMPLE_RATE, 16);
    header.riffSize        = waveDataSize + 44 - 8;
    header.dataSize        = waveDataSize;

    file.write(&header, 44);
    InitAdcInput(REC_ADC_CHANNEL);

    /* 延迟400ms再读取的数据。避免识别到唤醒词（喂） */
    delay(400);
    Serial.println("Start voice recording");

    for (int j = 0; j < waveDataSize / 1024; ++j) {
        auto  sz = Read((char *)communicationData, 2048);
        char *p  = (char *)(communicationData);

        for (int i = 0; i < sz / 2; i++) {
            communicationData[i] = communicationData[i] & 0x0FFF;
            communicationData[i] *= 6;

            if (i % 2 == 0) {
                partWavData[i]     = p[2 * i];
                partWavData[i + 1] = p[2 * i + 1];
            }
        }

        file.write((const byte *)partWavData, 1024);
    }

    file.close();
    if (recordTime == TIME_AUTH) {
        Serial.println("Finish voice recording");
    }
}
