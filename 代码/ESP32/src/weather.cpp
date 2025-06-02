#include "weather.h"

/**
 * @brief   利用ArduinoJson库解析心知天气响应信息
 * @param   client 网络连接对象
 * @retval  void
 */
void WeatherAPI::parseInfo(Client &client)
{
    JsonDocument doc;

    deserializeJson(doc, client);

    JsonObject results_0 = doc["results"][0];

    JsonObject  results_0_now             = results_0["now"];
    const char *results_0_now_text        = results_0_now["text"];        // "Sunny"
    const char *results_0_now_code        = results_0_now["code"];        // "0"
    const char *results_0_now_temperature = results_0_now["temperature"]; // "32"

    const char *results_0_last_update = results_0["last_update"]; // "2020-06-02T14:40:00+08:00"

    // 通过串口监视器显示以上信息
    String results_0_now_text_str        = results_0_now["text"].as<String>();
    int    results_0_now_code_int        = results_0_now["code"].as<int>();
    int    results_0_now_temperature_int = results_0_now["temperature"].as<int>();

    String results_0_last_update_str = results_0["last_update"].as<String>();

#if DEBUG
    Serial.println(F("======Weather Now======="));
    Serial.print(F("Weather Now: "));
    Serial.print(results_0_now_text_str);
    Serial.print(F(" "));
    Serial.println(results_0_now_code_int);
    Serial.print(F("Temperature: "));
    Serial.println(results_0_now_temperature_int);
    Serial.print(F("Last Update: "));
    Serial.println(results_0_last_update_str);
    Serial.println(F("========================"));
#else
    delay(10);
    Serial.write(CMD_SEND_WEATHER);
    Serial.print(results_0_now_text_str);
    delay(10);
    Serial.write(CMD_SEND_TEMPERATURE);
    Serial.write(results_0_now_temperature_int);
#endif
}

/**
 * @brief   获取心知天气信息
 * @param   无
 * @retval  void
 */
void WeatherAPI::getWeather()
{
    // 建立心知天气API当前天气请求资源地址
    String reqRes =
        "/v3/weather/now.json?key=" + reqUserKey + "&location=" + reqLocation + "&language=en&unit=" + reqUnit;
    // 向心知天气服务器服务器请求信息并对信息进行解析
    httpRequest(reqRes, GET);
    delay(3000);
}