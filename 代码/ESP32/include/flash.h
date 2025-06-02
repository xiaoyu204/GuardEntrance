#pragma once
#include <Arduino.h>
#include <SPIFFS.h> // 使用SPIFFS.h头文件

class Flash
{
public:
    void begin();
    void format();
    void remove(String file_name);
    void remove_floder(String folder_name);
    void read_file(String file_name);
    void read_file(String file_name, String *data);
    void read_floder(String floder_name);
    uint8_t write_file(String file_name, String file_content);
    uint8_t append(String file_name, String append_content);
};

