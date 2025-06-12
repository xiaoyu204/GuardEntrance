#include "ai.h"

// 讯飞Spark 4.0Ultra API信息
const char  *host        = "spark-api-open.xf-yun.com";
const int    httpsPort   = 443;
const String apiPassword = "tMQqcrapnpxdmaeifDSH:aYrczqAhzgvVPwWLTDoJ";
const String servicePath = "/v1/chat/completions";
const String model       = "4.0Ultra";
const String setting =
    "你是一个数据分析助手，根据我发送的内容，推测出对应的命令，并直接告诉我答案，不要有其他的回答。现在有三个命令，分别为留言，开门和呼叫。";

String generateRequestBody(String question)
{
    JsonDocument doc;

    // 必填字段
    doc["model"]                  = model;
    doc["messages"][0]["role"]    = "assistant";
    doc["messages"][0]["content"] = setting;
    doc["messages"][1]["role"]    = "user";
    doc["messages"][1]["content"] = question;

    // 可选字段
    // doc["temperature"] = 0.5;
    // doc["max_tokens"] = 1024;
    // doc["stream"] = false;

    String requestBody;
    serializeJson(doc, requestBody);
    return requestBody;
}

String sendRequest(String question)
{
    // 创建新的客户端对象，确保每次都是新连接
    WiFiClientSecure client;
    client.setInsecure(); // 跳过SSL验证

    if (!client.connect(host, httpsPort)) {
        return "Connection failed";
    }

    String requestBody = generateRequestBody(question);
    String headers     = "POST " + servicePath + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Authorization: Bearer "
                     + apiPassword + "\r\n" + "Content-Type: application/json\r\n"
                     + "Content-Length: " + requestBody.length() + "\r\n\r\n";

    client.print(headers + requestBody);

    // 等待响应
    unsigned long timeout = millis();
    while (client.connected() && !client.available()) {
        if (millis() - timeout > 5000) {
            client.stop();
            return "Timeout";
        }
        delay(10);
    }

    String response;
    while (client.available()) {
        response += client.readString();
    }

    client.stop(); // 显式关闭连接
    return response;
}

String parseResponse(String response)
{
    // 查找JSON开始位置（即第一个 '{'）
    int jsonStart = response.indexOf('{');
    if (jsonStart == -1) {
#if DEBUG
        Serial.println("错误:未找到JSON数据起始位置");
        Serial.println("原始响应:");
        Serial.println(response);
#endif
        return "Failed to parse JSON";
    }

    // 提取纯JSON部分
    String jsonContent = response.substring(jsonStart);
    jsonContent.trim(); // 去除可能的空白字符

#if DEBUG
    Serial.println("提取的JSON内容:");
    Serial.println(jsonContent);
#endif

    JsonDocument         doc;
    DeserializationError error = deserializeJson(doc, jsonContent);

    if (error) {
#if DEBUG
        Serial.print("JSON解析失败: ");
        Serial.println(error.c_str());
        Serial.println("原始响应内容:");
        Serial.println(response);
#endif
        return "Failed to parse JSON";
    }

    // 检查基础响应字段
    if (!doc["code"].is<JsonVariant>() || !doc["choices"].is<JsonVariant>()) {
#if DEBUG
        Serial.println("无效的API响应格式");
#endif
        serializeJsonPretty(doc, Serial);
        return "Invalid API response format";
    }

    int code = doc["code"];
    if (code != 0) {
#if DEBUG
        Serial.print("API返回错误 (代码: ");
        Serial.print(code);
        Serial.print("): ");
        Serial.println(doc["message"].as<String>());
#endif
        return "API returns error";
    }

    // 提取AI回复内容
    String answer = doc["choices"][0]["message"]["content"];
#if DEBUG
    Serial.println("");
    Serial.println("AI回答:");
    Serial.println(answer);

    // 显示token使用情况（调试信息）
    Serial.println("");
    Serial.println("Token使用情况:");
    Serial.printf("提问Token: %d\n", doc["usage"]["prompt_tokens"].as<int>());
    Serial.printf("回答Token: %d\n", doc["usage"]["completion_tokens"].as<int>());
    Serial.printf("总计Token: %d\n", doc["usage"]["total_tokens"].as<int>());

    // 显示会话ID（用于技术支持）
    Serial.println("会话ID: " + doc["sid"].as<String>());
#endif

    return answer;
}