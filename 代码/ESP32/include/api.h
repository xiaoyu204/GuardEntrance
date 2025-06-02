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

// class WeatherAPI
// {
// private:
// #if voice
//     const char *host = "api.seniverse.com"; // 将要连接的服务器地址
// #else
//     // const char *host = "vop.baidu.com"; // 将要连接的服务器地址
//     const char *host = "aip.baidubce.com"; // 将要连接的服务器地址
// #endif                        // !voice
//     const int httpPort = 443; // 将要连接的服务器端口

// #if voice
//     // 心知天气HTTP请求所需信息
//     String reqUserKey  = "SGLllpxgFbaAA30PY"; // 私钥
//     String reqLocation = "ZhouKou";           // 城市
//     String reqUnit     = "c";                 // 摄氏/华氏
// #else
//     String Header = "Content-Type: audio/wav;rate=16000";
//     // String Host   = "http://vop.baidu.com/server_api";
//     String API_KEY    = "MeWnMuFsysfU6vBABXM7F4T0";
//     String SECRET_KEY = "PqS25YVTRk4bMPCvRLwCIHdsEbEyC1BN";
//     String Token;
// #endif // !voice

//     void httpRequest(String reqRes);
//     void getToken();

// public:
//     void GetWeather();
// };