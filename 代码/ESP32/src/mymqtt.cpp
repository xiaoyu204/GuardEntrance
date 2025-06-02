#include "mymqtt.h"

void subCallback(char *topic, byte *payload, unsigned int length)
{
    extern Attributes_T attributes;

    // Serial.print("Message arrived [");
    // Serial.print(topic);
    // Serial.print("] ");
    // for (int i = 0; i < length; i++) {
    //     Serial.print((char)payload[i]);
    // }
    // Serial.println();

    JsonDocument         doc;
    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    // 情况1：直接解析 {"call": false/true}
    if (doc["call"].is<JsonVariant>()) {
        bool call_status = doc["call"] | false; // 如果字段不存在，默认 false

#if DEBUG
        Serial.print("Call: ");
        Serial.println(call_status ? "true" : "false");
#endif
        attributes.call = call_status;
    }
    /* 情况2：{"alarm": false/true} */
    else if (doc["alarm"].is<JsonVariant>()) {
        bool alarm_status = doc["alarm"] | false; // 如果字段不存在，默认 false

#if DEBUG
        Serial.print("Alarm: ");
        Serial.println(alarm_status ? "true" : "false");
#else
        Serial.write(CMD_SEND_ALARM);
        Serial.write(alarm_status);
#endif
        attributes.alarm = alarm_status;
    }
    /* 情况2：{"visitors": xx} */
    else if (doc["visitors"].is<JsonVariant>()) {
        int visitors = doc["visitors"] | 0; // 默认 0（如果字段不存在）

#if DEBUG
        Serial.print("Visitors: ");
        Serial.println(visitors);
#else
        Serial.write(CMD_SEND_VISITORS);
        Serial.write(visitors);
#endif
        attributes.visitors = visitors;
    }
    /* 情况4：解析嵌套结构（带 "attributes"） */
    else if (doc["attributes"].is<JsonObject>()) {
        JsonObject attr         = doc["attributes"];
        bool       call_status  = attr["call"];
        bool       alarm_status = attr["alarm"];
        uint8_t    visitors     = attr["visitors"];
#if DEBUG
        Serial.print("[Nested] Call: ");
        Serial.println(call_status ? "true" : "false");
        Serial.print("[Nested] Alarm: ");
        Serial.println(alarm_status ? "true" : "false");
        Serial.print("[Nested] Visitors: ");
        Serial.println(visitors);
#else
        attributes.call     = call_status;
        attributes.alarm    = alarm_status;
        attributes.visitors = visitors;
#endif
    }
    // 情况3：未知格式
    else {
        Serial.println("Unknown JSON format!");
    }
}

MyMQTT::MyMQTT() : client(espclient)
{
    client.setCallback(subCallback);
}

void MyMQTT::serverConnect()
{
    client.setServer(mqtt_server, mqtt_port);    // 设置MQTT服务器地址和端口
    if (client.connect(clientID, uname, upwd)) { // 连接到MQTTServer
        Serial.println("Connected to MQTTServer!");
    } else {
        Serial.println("Failed to connect to MQTTServer!");
    }

    client.subscribe(MQTTServer_TOPIC_ATTRIBUTES); // 订阅设备属性
    client.subscribe(MQTTServer_TOPIC_REPLY);      // 订阅设备下发属性
    // pubSetAttributes();
}

String MyMQTT::setAttributes(Attributes_T attributes)
{
    JsonDocument doc;
    String       jsonCode;

    doc["call"]     = attributes.call;
    doc["alarm"]    = attributes.alarm;
    doc["visitors"] = attributes.visitors;
    doc.shrinkToFit(); // optional

    serializeJson(doc, jsonCode);
#if DEBUG
    Serial.print("SetAttributes Json Code: ");
    Serial.println(jsonCode);
#endif

    return jsonCode;
}

String MyMQTT::getAttributes()
{
    JsonDocument doc;
    String       jsonCode;

    JsonArray keys = doc["keys"].to<JsonArray>();
    keys.add("call");
    keys.add("alarm");
    keys.add("visitors");
    doc.shrinkToFit(); // optional

    serializeJson(doc, jsonCode);
#if DEBUG
    Serial.print("GetAttributes Json Code: ");
    Serial.println(jsonCode);
#endif

    return jsonCode;
}

void MyMQTT::pubGetAttributes()
{
    client.publish(MQTTServer_TOPIC_GETATTRIBUTES, getAttributes().c_str());
}

void MyMQTT::pubSetAttributes(Attributes_T attributes)
{
    client.publish(MQTTServer_TOPIC_SETATTRIBUTES, setAttributes(attributes).c_str());
}

void MyMQTT::stayConnecte(void (*connfun)())
{
    if (WiFi.status() != WL_CONNECTED) {
        // 如果WiFi连接断开，重新连接WiFi
        connfun();
    }

    if (!client.connected()) {
        // 如果MQTT连接断开，重新连接MQTTServer
        serverConnect();
    }

    // 保持MQTT连接
    client.loop();
}
