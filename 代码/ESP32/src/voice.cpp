#include "voice.h"

const char  *cmdFile        = "/cmd.wav";
const char  *longFile       = "/long.wav";
const char  *shortFile      = "/short.wav";
const String speech_api_url = "https://vop.baidu.com/server_api";

/**
 * @brief   获取token
 * @retval  void
 */
void TokenAPI::getToken()
{
    String reqRes =
        "/oauth/2.0/token?grant_type=client_credentials&client_id=" + API_KEY + "&client_secret=" + SECRET_KEY;
    httpsRequest(reqRes, POST);
}

/**
 * @brief   利用ArduinoJson库解析Token响应信息
 * @param   client:  HTTP客户端
 * @retval  void
 */
void TokenAPI::parseInfo(Client &client)
{
    // 读取完整的响应（包括 HTTPS 头和 JSON）
    String response = client.readString();
    client.stop();

    // 查找 JSON 部分的起始位置（跳过 HTTP 头）
    int jsonStart = response.indexOf("{");
    if (jsonStart < 0) {
        Serial.println("No JSON data found!");
        return;
    }

    String               jsonStr = response.substring(jsonStart);
    JsonDocument         doc;
    DeserializationError error = deserializeJson(doc, jsonStr);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    access_token = doc["access_token"];
    // Serial.print(F("access_token: "));
    // Serial.println(access_token);
}

/**
 * @brief   检测是否有人靠近
 * @retval  true: 有人靠近 false: 没有人靠近
 */
bool VoiceAPI::isSomeoneApproach()
{
    int adcValue = adc1_get_raw(REC_ADC_CHANNEL); // 读取ADC值

    if (adcValue > SOUND_THRESHOLD) {
#if DEBUG
        Serial.println("Sound detected");
#else
        Serial.write(CMD_DETECTED_SOUND);
#endif
        return true;
    } else {
#if DEBUG
        Serial.println("No sound detected");
        delay(100); // 避免频繁打印
#endif
        return false;
    }
}

/**
 * @brief   利用ArduinoJson库解析短识别响应信息
 * @param   response:  HTTP响应
 * @retval  void
 */
void VoiceAPI::parseInfo(String response)
{
    // 解析JSON响应
    JsonDocument doc;
    deserializeJson(doc, response);

    if (doc["result"].as<String>()) {
        String result = doc["result"][0].as<String>();
#if DEBUG
        Serial.println("Identification result: " + result);
#endif
        sendExecuteCMD(result);
    } else if (doc["err_msg"].as<String>()) {
        String errMsg = doc["err_msg"].as<String>();
#if DEBUG
        Serial.println("Identification errMsg: " + errMsg);
#endif
        sendExecuteCMD(errMsg);
    }
}

/**
 * @brief   利用ArduinoJson库解析识别响应信息
 * @param   sd:  SD卡文件系统
 * @param   wavFilePath:  WAV文件路径
 * @retval  void
 */
void VoiceAPI::getIdentification(SdFs &sd, const char *wavFilePath)
{
    FsFile audioFile = sd.open(wavFilePath, O_RDONLY);
    if (!audioFile) {
        if (!sd.exists(wavFilePath)) {
            Serial.println("Audio file not exist");
            return;
        }
        Serial.println("Failed to open audio file");
        return;
    }

    // 获取文件大小
    size_t fileSize = audioFile.size();

    audioFile.seek(44); // 跳过WAV头
    size_t audioDataSize = fileSize - 44;

    // 读取音频数据
    uint8_t *audioData = (uint8_t *)malloc(audioDataSize);
    if (!audioData) {
        Serial.println("Audio malloc failed");
        audioFile.close();
        return;
    }

    // size_t bytesRead = audioFile.read(audioData, audioDataSize);
    // audioFile.close();
    // if (bytesRead != audioDataSize) {
    //     Serial.println("读取音频数据不完整");
    //     free(audioData);
    //     return;
    // }

    uint8_t       retry     = 0;
    const uint8_t max_retry = 3;
retry:

    size_t totalRead = 0;
    while (totalRead < audioDataSize) {
        size_t chunkSize = min((uint32_t)1024, audioDataSize - totalRead); // 每次读取1KB
        size_t bytesRead = audioFile.read(audioData + totalRead, chunkSize);
        if (bytesRead == 0)
            break; // 读取失败
        totalRead += bytesRead;
    }

    if (totalRead != audioDataSize) {
        if (retry >= max_retry) {
            Serial.println("The number of retries is too many. Give up reading the audio data");
            free(audioData);
            return;
        } else {
#if DEBUG
            Serial.printf("The audio data reading is incomplete. Try again %d times", ++retry);
#endif
            goto retry;
        }
    }

    audioFile.close();

    // 准备HTTP请求
    WiFiClientSecure client;
    HTTPClient       http;
    tokenAPI.getToken();
    String url = String(speech_api_url) + "?cuid=ESP32S3&token=" + tokenAPI.access_token;

    client.setInsecure(); // 跳过证书验证

    if (http.begin(client, url)) {
        http.addHeader("Content-Type", "audio/wav; rate=16000");

        // 发送音频数据
        int httpCode = http.POST(audioData, audioDataSize);

        free(audioData); // 释放内存

        if (httpCode == HTTP_CODE_OK) {
            String response = http.getString();
#if DEBUG
            Serial.println("Response:");
            Serial.println(response);
#endif
            if (wavFilePath == longFile) {
                // 解析JSON响应
                JsonDocument doc;
                deserializeJson(doc, response);
                if (doc["result"].as<String>()) {
                    String result = doc["result"][0].as<String>();
                    Serial.write(CMD_LEAVE_MESSAGE);
                    Serial.print(result);
                    // Serial.println("Long Identification result: " + result);
                } else if (doc["err_msg"].as<String>()) {
                    String errMsg = doc["err_msg"].as<String>();
                    Serial.write(CMD_LEAVE_MESSAGE);
                    Serial.print(errMsg);
                    // Serial.println("Long Identification errMsg: " + errMsg);
                }
            } else {
                parseInfo(response);
            }
        } else {
            Serial.printf("Speech recognition failed, HTTP error code: %d\n", httpCode);
            String response = http.getString();
            Serial.println(response);
        }

        http.end();
    } else {
        Serial.println("Unable to connect to Baidu's speech recognition server");
        free(audioData);
    }
}

/**
 * @brief   发送执行识别到的命令
 * @retval  void
 */
void VoiceAPI::sendExecuteCMD(String identification)
{
#if DEBUG
    if (identification == "开门。") {
        Serial.println("Execute the door opening command");
    } else if (identification == "留言。") {
        leaveMessage(longFile);
    } else {
        Serial.println(identification);
    }
#else
    if (identification == "开门。" || identification == "开开门") {
        Serial.write(CMD_OPEN_DOOR);
        delay(50);
        Serial.println("Execute the door opening command");
    } else if (identification == "留言。" || identification == "留留言。") {
        leaveMessage(longFile);
        getIdentification(sd, longFile);
    } else if (identification == "呼叫。" || identification == "呼呼叫。") {
        attributes.call = "Someone Call";
    } else {
        Serial.println(identification);
    }
#endif
}

/**
 * @brief   持续监听识别
 * @retval  void
 */
void VoiceAPI::keepListening()
{
    if (isSomeoneApproach()) {
        uint32_t startTime = millis();

        rec.audioRecord(sd, cmdFile, TIME_AUTH); // 开始录音
        getIdentification(sd, cmdFile);

        // // 持续录音一段时间，可根据需要调整
        // while (millis() - startTime < TIME_AUTH) {
        //     // 录音过程中保持运行
        // }

        // // 可选：添加延时避免重复触发
        // delay(1000);
    }
}

/**
 * @brief   录音留言功能
 * @retval  void
 */
inline void VoiceAPI::leaveMessage(const char *messageFile)
{
    rec.audioRecord(sd, messageFile, TIME_MESSAGE);
}
