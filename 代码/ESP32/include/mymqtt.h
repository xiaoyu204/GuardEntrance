#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "cmd.h"

#define clientID                       "ClientID"         // 随便填(写的时候用的是thingscloud，其他的云服务器按情况处理)
#define uname                          "2us5eawdu3ib3zxt" // user name
#define upwd                           "CKx8d4Fl4u"       // password

#define MQTTServer_TOPIC_SETATTRIBUTES "attributes"                // 上报属性到云平台
#define MQTTServer_TOPIC_GETATTRIBUTES "attributes/get/1000"       // 从云平台获取当前属性
#define MQTTServer_TOPIC_ATTRIBUTES    "attributes/get/response/+" // 设备属性话题
#define MQTTServer_TOPIC_REPLY         "attributes/push"           // 实时接收云平台下发的属性

typedef struct {
    bool    call;
    bool    alarm;
    uint8_t visitors;
} Attributes_T;

class MyMQTT
{
private:
    WiFiClient   espclient;                             // 创建一个WiFiClient对象
    PubSubClient client;                                // 创建一个PubSubClient对象
    const char  *mqtt_server = "bj-2-mqtt.iot-api.com"; // MQTT服务器地址
    const int    mqtt_port   = 1883;                    // MQTT服务器端口

public:
    MyMQTT();
    void   serverConnect();
    String setAttributes(Attributes_T attributes);
    String getAttributes();
    void   pubGetAttributes();
    void   pubSetAttributes(Attributes_T attributes);
    void   stayConnecte(void (*connfun)());
};