#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "cmd.h"

enum NetReqMode { POST = 1, GET = 2 };

class API
{
private:
    const char *host; // 将要连接的服务器地址
    const int   port; // 将要连接的服务器端口

protected:
    API(const char *host, int port) : host(host), port(port) {}
    void httpRequest(String reqRes, NetReqMode mode);
    void httpsRequest(String reqRes, NetReqMode mode);
    virtual void parseInfo(Client &client) = 0;
};
