#pragma once

#include "cmd.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

String parseResponse(String response);
String sendRequest(String question);
