#pragma once
#include <Arduino.h>
#include "flash.h"
#include "cmd.h"
#include "weather.h"
#include "mymqtt.h"

class USER
{
private:
    Flash      flash;
    String     uid_file_data;
    uint8_t    cmd;
    String     uid;

public:
    String unum_file_data;

    USER();
    void analyzeCMD(uint8_t cmd);
    void addCard();
    void deleteCard();
    void getCard();
    // void clearUID();
};
