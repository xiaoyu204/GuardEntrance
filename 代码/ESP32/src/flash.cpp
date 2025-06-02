#include "flash.h"

/**
 * @brief   格式化SPIFFS
 * @retval  无
 */
void Flash::format()
{
    Serial.println("SPIFFS format start");
    SPIFFS.format(); // 格式化SPIFFS
    Serial.println("SPIFFS format finish");
}

/**
 * @brief   SPIFFS写入文件
 * @param   file_name: 文件名
 * @param   file_content: 文件内容
 * @retval  无
 */
uint8_t Flash::write_file(String file_name, String file_content)
{
    /* 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息 */
    File dataFile = SPIFFS.open(file_name, "w");

    if (dataFile) {
        dataFile.print(file_content); // 向dataFile写入字符串信息
        dataFile.close();             // 完成文件写入后关闭文件
    } 

    return 1;
}

/**
 * @brief   启动SPIFFS
 * @retval  无
 */
void Flash::begin()
{
    /* 启动SPIFFS，true表示格式化失败时自动重新格式化 */
    if (SPIFFS.begin(true)) {
        Serial.println("SPIFFS Started.");
    } else {
        Serial.println("SPIFFS Failed to Start.");
    }
}

/**
 * @brief   读取文件内容
 * @param   file_name: 文件名
 * @retval  无
 */
void Flash::read_file(String file_name)
{
    /* 确认闪存中是否有file_name文件 */
    if (SPIFFS.exists(file_name)) {
        Serial.print(file_name);
        Serial.println(" FOUND.");
    } else {
        Serial.print(file_name);
        Serial.print(" NOT FOUND.");
        return;
    }

    /* 建立File对象用于从SPIFFS中读取文件 */
    File dataFile = SPIFFS.open(file_name, "r");

    /* 读取文件内容并且通过串口监视器输出文件信息 */
    for (int i = 0; i < dataFile.size(); i++) {
        Serial.print((char)dataFile.read());
    }

    /* 完成文件读取后关闭文件 */
    dataFile.close();
}

/**
 * @brief   读取文件内容
 * @param   file_name: 文件名
 * @param   data: 输出文件内容
 * @retval  无
 */
void Flash::read_file(String file_name, String *data)
{
    /* 确认闪存中是否有file_name文件 */
    if (SPIFFS.exists(file_name)) {
        Serial.print(file_name);
        Serial.println(" FOUND.");
    } else {
        Serial.print(file_name);
        Serial.println(" NOT FOUND.");
        return;
    }

    /* 建立File对象用于从SPIFFS中读取文件 */
    File dataFile = SPIFFS.open(file_name, "r");

    /* 读取文件内容并且通过串口监视器输出文件信息 */
    for (int i = 0; i < dataFile.size(); i++) {
        *data += String((char)dataFile.read());
    }

    /* 完成文件读取后关闭文件 */
    dataFile.close();
}

/**
 * @brief   追加文件内容
 * @param   file_name: 文件名
 * @param   append_content: 追加内容
 * @retval  无
 */
uint8_t Flash::append(String file_name, String append_content)
{
    /* 确认闪存中是否有file_name文件 */
    if (SPIFFS.exists(file_name)) {
        /* 建立File对象用于向SPIFFS中的file对象写入信息 */
        File dataFile = SPIFFS.open(file_name, "a");
        dataFile.print(append_content); // 向dataFile添加字符串信息
        dataFile.close();                 // 完成文件操作后关闭文件
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief   删除文件
 * @param   file_name: 文件名
 * @retval  无
 */
void Flash::remove(String file_name)
{
    /* 从闪存中删除file_name文件 */
    if (SPIFFS.remove(file_name)) {

        Serial.print(file_name);
        Serial.println(" remove sucess");

    } else {
        Serial.print(file_name);
        Serial.println(" remove fail");
    }
}

/**
 * @brief   读取目录下的所有文件并输出属性
 * @param   floder_name: 文件夹名
 * @retval  无
 */
void Flash::read_floder(String floder_name)
{
    /* 显示目录中文件名,存储路径以及文件大小 */
    File dir = SPIFFS.open(floder_name); // 建立“目录”对象

    if (!dir.isDirectory()) {
        Serial.println("Not a directory.");
        return;
    }

    File file = dir.openNextFile();
    while (file) {
        Serial.print("  FILE: ");
        Serial.print(file.name()); // 输出文件名
        Serial.print("\tSIZE: ");
        Serial.println(file.size()); // 输出文件大小
        Serial.print("  ");
        Serial.println(file.path());
        file = dir.openNextFile();
    }
}

/**
 * @brief   删除文件夹
 * @param   folder_name: 文件夹名
 * @retval  无
 */
void Flash::remove_floder(String folder_name)
{
    /* 列出文件夹中的所有文件 */
    File root = SPIFFS.open(folder_name);

    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }

    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();

    while (file) {
        if (file.isDirectory()) {
            /* 如果是子目录，递归删除 */
            String path = String(folder_name) + "/" + file.name();
            Flash::remove_floder(path);
        } else {
            /* 删除文件 */
            String path = String(folder_name) + "/" + file.name();
            SPIFFS.remove(path);
            Serial.println("Removed file: " + path);
        }

        file = root.openNextFile();
    }

    // 关闭目录
    root.close();

    /* 删除空文件夹
    注意：SPIFFS 不支持删除非空文件夹，这里假设文件夹已经为空
    SPIFFS.rmdir(folder_name); // SPIFFS 不支持删除非空文件夹
    SPIFFS 会在删除目录下所有文件后自动清理相关目录数据，显式
    调用 rmdir 会警告
    SPIFFS.rmdir(folder_name); */
    Serial.println("Folder removed: " + folder_name);
}