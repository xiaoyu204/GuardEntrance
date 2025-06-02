#include "api.h"

/**
 * @brief   向https服务器请求信息并对信息进行解析
 * @param   reqRes: 请求资源
 * @param   mode: 请求方式
 * @retval  void
 */
void API::httpsRequest(String reqRes, NetReqMode mode)
{
    WiFiClientSecure client;
    String           httpsReqMode;

    switch (mode) {
        case POST: httpsReqMode = "POST "; break;
        case GET: httpsReqMode = "GET "; break;
    }

    // 建立http请求信息
    String httpsRequest =
        httpsReqMode + reqRes + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
    client.setInsecure(); // 跳过SSL证书验证（仅用于测试，生产环境建议配置证书）
#if DEBUG
    Serial.print("Connecting to ");
    Serial.print(host);

    // 尝试连接服务器
    if (client.connect(host, port)) {
        Serial.println(" Success!");

        // 向服务器发送http请求信息
        client.print(httpsRequest);
        Serial.println("Sending request: ");
        Serial.print(httpsRequest);

        // 获取并显示服务器响应状态行
        String status_response = client.readStringUntil('\n');
        Serial.print("status_response: ");
        Serial.println(status_response);

        // 使用find跳过HTTPS响应头
        if (client.find("\r\n\r\n")) {
            Serial.println("Found Header End. Start Parsing.");
        } else {
            Serial.println("Not Found Header End");
        }

        // 利用ArduinoJson库解析响应信息
        parseInfo(client);
    } else {
        Serial.println(" connection failed!");
    }
#else
    // 尝试连接服务器
    if (client.connect(host, port)) {
        // 向服务器发送http请求信息
        client.print(httpsRequest);

        // 获取并显示服务器响应状态行
        String status_response = client.readStringUntil('\n');

        // 使用find跳过HTTPS响应头
        if (!client.find("\r\n\r\n")) {
            Serial.println("Not Found Header End");
        }

        // 利用ArduinoJson库解析响应信息
        parseInfo(client);
    } else {
        Serial.println(" connection failed!");
    }
#endif
    // 断开客户端与服务器连接工作
    client.stop();
}

/**
 * @brief   向http服务器请求信息并对信息进行解析
 * @param   reqRes: 请求资源
 * @param   mode: 请求方式
 * @retval  void
 */
void API::httpRequest(String reqRes, NetReqMode mode)
{
    WiFiClient client;
    String     httpReqMode;

    switch (mode) {
        case POST: httpReqMode = "POST "; break;
        case GET: httpReqMode = "GET "; break;
    }

    // 建立http请求信息
    String httpRequest =
        httpReqMode + reqRes + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
#if DEBUG
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(host);

    // 尝试连接服务器
    if (client.connect(host, port)) {
        Serial.println(" Success!");

        // 向服务器发送http请求信息
        client.print(httpRequest);
        Serial.println("Sending request: ");
        Serial.print(httpRequest);

        // 获取并显示服务器响应状态行
        String status_response = client.readStringUntil('\n');
        Serial.print("status_response: ");
        Serial.println(status_response);

        // 使用find跳过HTTPS响应头
        if (client.find("\r\n\r\n")) {
            Serial.println("Found Header End. Start Parsing.");
        } else {
            Serial.println("Not Found Header End");
        }

        // 利用ArduinoJson库解析响应信息
        parseInfo(client);
    } else {
        Serial.println(" connection failed!");
    }
    // 断开客户端与服务器连接工作
    client.stop();

#else
    // 尝试连接服务器
    if (client.connect(host, port)) {
        // 向服务器发送http请求信息
        client.print(httpRequest);

        // 获取并显示服务器响应状态行
        String status_response = client.readStringUntil('\n');

        // 使用find跳过HTTPS响应头
        if (!client.find("\r\n\r\n")) {
            Serial.println("Not Found Header End");
        }

        // 利用ArduinoJson库解析响应信息
        parseInfo(client);
    } else {
        Serial.println(" connection failed!");
    }
    // 断开客户端与服务器连接工作
    client.stop();
#endif
}