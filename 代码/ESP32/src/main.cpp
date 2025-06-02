#include <Arduino.h>
#include <Ticker.h>
#include "flash.h"
#include "uid.h"
#include "mymqtt.h"
#include "weather.h"
#include "voice.h"
#include "test.h"

#define SD_FREQ 18000000

const char    *ssid                = "213-WIFI";  // WiFi名称
const char    *password            = "213213213"; // WiFi密码
uint8_t        success             = 0;
bool           isListening         = false;
uint32_t       lastWakeTime        = 0;
const uint32_t wakeInterval        = 1000; // 1秒间隔
uint32_t       previousWeatherTime = 0;
const uint32_t weatherInterval     = 10000; // 60秒 = 60000毫秒
Attributes_T   attributes          = {.call = false, .alarm = false, .visitors = 0};

USER       user;
MyMQTT     mqtt;
Ticker     ticker;
SdFs       sd;
WeatherAPI weatherAPI("api.seniverse.com", 80);
VoiceAPI   voice("aip.baidubce.com", 443, sd, attributes);

void connectWiFi();
void connectSD();

void setup()
{
    Serial.begin(115200);
    connectSD();
    connectWiFi();
    mqtt.serverConnect();
    mqtt.pubGetAttributes();
}

void loop()
{
    uint8_t cmd;

    if (Serial.available()) {
        Serial.read(&cmd, 1);

        if (cmd == CMD_WAKE) {
            // Serial.println("Wake up");
            // /* STM32上电误触发 */
            // static bool     init_error      = true;
            // static uint32_t init_start_time = millis(); // 记录初始化时间

            // if (init_error) {
            //     // 如果还在 3 秒内，忽略触发
            //     if (millis() - init_start_time < 3000) {
            //         return;
            //     }
            //     // 超过 3 秒后，恢复正常逻辑
            //     init_error = false;
            // }
            isListening  = true;
            lastWakeTime = millis();
        } else if (cmd == CMD_PUB) {
            uint8_t publishMessage[2];

            Serial.readBytesUntil('Z', publishMessage, sizeof(publishMessage));
            attributes.alarm    = publishMessage[0];
            attributes.visitors = publishMessage[1];
#if DEBUG
            // 打印赋值结果
            Serial.print("alarm = ");
            Serial.println(attributes.alarm);
            Serial.print("visitors = ");
            Serial.println(attributes.visitors);
#endif
            mqtt.pubSetAttributes(attributes);
        } else {
            user.analyzeCMD(cmd);
        }
    }

    // 保持监听状态
    if (isListening) {
        voice.keepListening();

        // 检查是否超时（1秒未收到新CMD_WAKE）
        if (millis() - lastWakeTime > wakeInterval) {
            isListening = false;
        }
    }

    uint32_t currentTime = millis();
    // 检查是否到达60秒间隔
    if (currentTime - previousWeatherTime >= weatherInterval) {
        previousWeatherTime = currentTime;
        weatherAPI.getWeather();
    }

    mqtt.stayConnecte(connectWiFi);
}

void connectSD()
{
    uint8_t       retry     = 0;
    const uint8_t max_retry = 255;
retry:
    /* 初始化SD卡 */
    if (!sd.begin(SdSpiConfig(SS, DEDICATED_SPI, SD_FREQ))) {
        if (retry >= max_retry) {
            Serial.println("Init SD card error");
            while (1);
        } else {
            Serial.printf("SD connect retry %d time\n", ++retry);
            goto retry;
        }
    }
    Serial.println("Init SD card success");
}

void connectWiFi()
{
    WiFi.begin(ssid, password);     // 启动网络连接
    Serial.print("Connecting to "); // 串口监视器输出网络连接信息
    Serial.print(ssid);
    Serial.println(" ..."); // 告知用户正在尝试WiFi连接

    int i = 0;                              // 这一段程序语句用于检查WiFi是否连接成功
    while (WiFi.status() != WL_CONNECTED) { // WiFi.status()函数的返回值是由WiFi连接状态所决定的。
        delay(1000);                        // 如果WiFi连接成功则返回值为WL_CONNECTED
    } // 同时将通过串口监视器输出连接时长读秒。
      // 这个读秒是通过变量i每隔一秒自加1来实现的。
    Serial.println("Connection established!"); // 将通过串口监视器输出"连接成功"信息。
    Serial.print("IP address:    ");           // 同时还将输出的IP地址。这一功能是通过调用
    Serial.println(WiFi.localIP());            // WiFi.localIP()函数来实现的。该函数的返回值即的IP地址。
}
