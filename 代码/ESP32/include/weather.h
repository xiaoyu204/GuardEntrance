#pragma once
#include "api.h"
#include "cmd.h"

class WeatherAPI : private API
{
private:
    // 心知天气HTTP请求所需信息
    String reqUserKey  = "SGLllpxgFbaAA30PY"; // 私钥
    String reqLocation = "ZhouKou";           // 城市
    String reqUnit     = "c";                 // 摄氏/华氏

    virtual void parseInfo(Client &client) override;

public:
    WeatherAPI(const char *host, int port) : API(host, port) {};
    void getWeather();
};