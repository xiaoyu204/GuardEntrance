#include "uid.h"

const String uid_file  = "/user/uid.txt";  // 卡号文件位置和名称
const String unum_file = "/user/unum.txt"; // 卡号数量文件位置和名称

/**
 * @brief  USER类构造函数
 * @retval 无
 */
USER::USER()
{
    flash.begin();
    flash.read_file(uid_file, &uid_file_data);
    flash.read_file(unum_file, &unum_file_data);
}

/**
 * @brief  增加卡号
 * @param  无
 * @retval 无
 */
void USER::addCard()
{
    uid = Serial.readStringUntil('Z');

    if (!flash.append(uid_file, uid)) {
        flash.write_file(uid_file, uid);
    }

    unum_file_data = String(atoi(unum_file_data.c_str()) + 1);
    flash.write_file(unum_file, unum_file_data);
    uid_file_data += uid;
    Serial.println(uid);
    uid.clear();
}

/**
 * @brief  删除卡号
 * @param  无
 * @retval 无
 */
void USER::deleteCard()
{
    // 读取要删除的卡号
    String uidToDelete = Serial.readStringUntil('Z');

    // 检查卡号是否存在
    int index = uid_file_data.indexOf(uidToDelete);
    if (index == -1) {
        Serial.println("Card not found");
        return;
    }

    // 从数据中移除卡号
    uid_file_data.remove(index, uidToDelete.length());

    // 更新闪存中的卡号数据
    flash.write_file(uid_file, uid_file_data);

    // 更新卡号计数
    unum_file_data = String(atoi(unum_file_data.c_str()) - 1);
    flash.write_file(unum_file, unum_file_data);

    Serial.println("Card deleted");
    uidToDelete.clear();
}

/**
 * @brief  查询所有的卡号
 * @param  无
 * @retval 无
 */
void USER::getCard()
{
    // 分批发送 uid_file_data
    size_t       send_index = 0;
    const size_t chunk_size = 4;
    size_t       remaining  = uid_file_data.length();
    size_t       total_size = remaining;

    if (!total_size) {
        return;
    }

    do {
        String chunk       = uid_file_data.substring(total_size - remaining, total_size - remaining + chunk_size);
        byte   prefix_byte = CMD_SEND_UID;
        chunk              = String((char)prefix_byte) + chunk;

        Serial.print(chunk);
        remaining -= chunk_size;
        delay(10);
    } while (remaining > 0);
}

/**
 * @brief  分析传输的命令并执行相应的操作
 * @param  无
 * @retval 无
 */
void USER::analyzeCMD(uint8_t cmd)
{
    extern Attributes_T attributes;
    extern MyMQTT       mqtt;

    if (cmd == CMD_ADD_UID) {
        addCard();
    } else if (cmd == CMD_GET_UNUM) {
        Serial.print(unum_file_data);
    } else if (cmd == CMD_GET_INIT) {
        getCard();
        Serial.write(CMD_SEND_VISITORS);
        Serial.write(attributes.visitors);
        delay(10);
        Serial.write(CMD_SEND_ALARM);
        Serial.write(attributes.alarm);
    } else if (cmd == CMD_CLEAR_UID) {
        flash.remove(uid_file);
        flash.remove(unum_file);
        uid_file_data.clear();
        unum_file_data.clear();
    } else if (cmd == CMD_RESTART) {
        Serial.write(CMD_RESTART_OK);
    } else if (cmd == CMD_EMERGENCY_CALL) {
        attributes.call = true;
        mqtt.pubSetAttributes(attributes);
    }  else if (cmd == CMD_DELETE_CARD) {
        deleteCard();
    }
}
