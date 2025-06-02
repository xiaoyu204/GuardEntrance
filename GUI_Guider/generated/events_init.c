/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

#ifdef USE
#include "usart.h"
#endif
// #ifdef USE
//     #include "main.h"
//     #include "FreeRTOS.h"
//     #include "task.h"
//     #include "cmsis_os.h"
//     #include "event_groups.h"
//     #include "bsp_bit.h"
// #endif
#define LOG(message) add_msg(guider_ui.admin_Interface_cont_debug, message)
#define MAX_MESSAGES 50
static lv_obj_t *msg_labels[MAX_MESSAGES]; // 存储所有标签指针
static int msg_index = 0;                  // 当前消息索引
static int msg_count = 0;                  // 当前消息总数
static bool auto_roll = 0;                 // 自动滚动

// 添加带自动换行的消息函数
void add_msg(lv_obj_t * scroll, const char *message) {
    // 如果已经达到最大消息数，移除最旧的消息
    if(msg_count >= MAX_MESSAGES) {
        lv_obj_del(msg_labels[msg_index]);
        msg_labels[msg_index] = NULL;
        msg_count--;
    }

    // 创建新标签
    lv_obj_t *label = lv_label_create(scroll);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, lv_pct(100));
    lv_label_set_text(label, message);

    // 存储标签指针
    msg_labels[msg_index] = label;

    // 更新索引和计数
    msg_index = (msg_index + 1) % MAX_MESSAGES;
    msg_count++;

    //自动滚动到底部
    if(auto_roll)
    {
        lv_obj_scroll_to_view(label, LV_ANIM_ON);
    }
}


#include <stdlib.h>
#ifdef USE
#include "bsp_as608.h"
#endif

const char *ID;
unsigned char ID_NUM;
char *endptr;
// RegisterID + ID字符串 + 1个空格 + 1个0x + 1个'\0' = 10 + 2 + 2 + 1 + 1 = 16
char add_ID[16]; // 固定大小的栈空间
// DeleteID + ID字符串 + 1个空格 + 1个0x + 1个'\0' = 8 + 2 + 2 + 1 + 1 = 14
char delete_ID[14]; // 固定大小的栈空间
void clear_msg() {
    for(int i = 0; i < MAX_MESSAGES; i++) {
        if(msg_labels[i] != NULL) {
            lv_obj_del(msg_labels[i]);
            msg_labels[i] = NULL;
        }
    }
    msg_index = 0;
    msg_count = 0;
}
#ifdef USE
#include "usart.h"
#include "bsp_esp32.h"
#endif
#include <stdlib.h>

#ifdef USE
#include "usart.h"
#include "bsp_rc522.h"
#endif

const char *first, *second, *third, *forth;
unsigned char card_num[6];
unsigned char readUID[4];
char *endptr;
// RegisterCard + 4个字符串 + 4个空格 + 4个0x + 1个'\0' = 12 + 2*4 + 2*4 + 4 + 1 = 33
char add_card[33]; // 固定大小的栈空间
// DeleteCard + 4个字符串 + 4个空格 + 4个0x + 1个'\0' = 10 + 2*4 + 2*4 + 4 + 1 = 31
char delete_card[31]; // 固定大小的栈空间

static void init_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.main, guider_ui.main_del, &guider_ui.init_del, setup_scr_main, LV_SCR_LOAD_ANIM_NONE, 0, 1000, true, false);
        break;
    }
    default:
        break;
    }
}

void events_init_init (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->init, init_event_handler, LV_EVENT_ALL, ui);
}

static void main_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        break;
    }
    default:
        break;
    }
}

static void main_admin_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.admin_Interface, guider_ui.admin_Interface_del, &guider_ui.main_del, setup_scr_admin_Interface, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, false);
        lv_obj_clear_flag(guider_ui.admin_Interface_cont_2, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void main_call_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
#ifdef USE
        unsigned char cmd = 0x9A;
        HAL_UART_Transmit(&hlpuart1, &cmd, 1, HAL_MAX_DELAY);
#endif
        break;
    }
    default:
        break;
    }
}

void events_init_main (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->main, main_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_admin, main_admin_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->main_call, main_call_event_handler, LV_EVENT_ALL, ui);
}

static void admin_Interface_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        lv_obj_clear_flag(guider_ui.admin_Interface_ta_5, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_clear_flag(guider_ui.admin_Interface_ta_4, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_clear_flag(guider_ui.admin_Interface_ta_2, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_clear_flag(guider_ui.admin_Interface_ta_3, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_clear_flag(guider_ui.admin_Interface_ta_1, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_flex_flow(guider_ui.admin_Interface_cont_debug, LV_FLEX_FLOW_COLUMN);

        // #ifdef USE
        //     extern EventGroupHandle_t DoorEventGroup;

        //     xEventGroupSetBits(DoorEventGroup, AS608_TASK_BIT);
        // #endif
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_card_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.admin_Interface_display_debug, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.admin_Interface_display_card, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.admin_Interface_display_fingerprint, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_exit_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.main, guider_ui.main_del, &guider_ui.admin_Interface_del, setup_scr_main, LV_SCR_LOAD_ANIM_NONE, 0, 0, true, false);
        lv_obj_clear_flag(guider_ui.admin_Interface_cont_2, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_debug_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.admin_Interface_display_debug, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.admin_Interface_display_card, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.admin_Interface_display_fingerprint, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_fingerprint_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.admin_Interface_display_debug, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.admin_Interface_display_card, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.admin_Interface_display_fingerprint, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_3_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ID = lv_textarea_get_text(guider_ui.admin_Interface_ta_5);
        snprintf(add_ID, sizeof(add_ID), "RegisterID 0x%s", ID);
        LOG(add_ID);

        ID_NUM = strtol(ID, &endptr, 16); // 16 表示 16 进制
        if (endptr == ID || *endptr != '\0') {
            LOG("无效的 16 进制字符串");
        }

#ifdef USE
        Add_FR(ID_NUM);
#endif
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_4_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ID = lv_textarea_get_text(guider_ui.admin_Interface_ta_5);
        snprintf(delete_ID, sizeof(delete_ID), "DeleteID 0x%s", ID);
        LOG(delete_ID);

        ID_NUM = strtol(ID, &endptr, 16); // 16 表示 16 进制
        if (endptr == ID || *endptr != '\0') {
            LOG("无效的 16 进制字符串");
        }

#ifdef USE
        Delete_FR(ID_NUM);
#endif
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_5_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
#ifdef USE
        Delete_ALL();
#endif
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_clear_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        clear_msg();
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_sw_auto_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        auto_roll = status;
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_clear_alarm_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
#ifdef USE
        extern Attributes_T attributes;
        unsigned char cmd[4] = {0xB6, 0x00, 0x00, 0x5A};

        cmd[2] = attributes.visitors;
        HAL_UART_Transmit(&ESP32_UART, cmd, 4, HAL_MAX_DELAY);
#endif
        lv_obj_add_flag(guider_ui.main_cont_2, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
#ifdef USE
        // card_num[0] = 0xA5;
        // card_num[5] = 0x5A;
        // for(unsigned char i = 0; i < 4; i++)
        // {
        //     readUID[i] = card_num[i+1];
        // }
        Scan_Card(readUID);
        // HAL_UART_Transmit(&ESP32_UART, card_num, sizeof(card_num), HAL_MAX_DELAY);
        SendCard(readUID, ADD);
#endif
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_add_hex_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        first = lv_textarea_get_text(guider_ui.admin_Interface_ta_1);
        second = lv_textarea_get_text(guider_ui.admin_Interface_ta_2);
        third = lv_textarea_get_text(guider_ui.admin_Interface_ta_3);
        forth = lv_textarea_get_text(guider_ui.admin_Interface_ta_4);

        // 使用 snprintf 安全拼接
        snprintf(add_card, sizeof(add_card), "RegisterCard 0x%s 0x%s 0x%s 0x%s", first, second, third, forth);
        LOG(add_card);

        const char* parts[4] = {first, second, third, forth};

        for (int i = 0; i < 4; i++) {
            readUID[i] = strtol(parts[i], &endptr, 16);  // 16 表示16进制
            // readUID[i] = card_num[i+1];
            if (endptr == parts[i] || *endptr != '\0') {
                LOG("无效的 16 进制字符串");
            }
        }

#ifdef USE
        // card_num[0] = 0xA5;
        // card_num[5] = 0x5A;
        // HAL_UART_Transmit(&ESP32_UART, card_num, sizeof(card_num), HAL_MAX_DELAY);
        // RegisterCard(readUID);
        Send_Card(readUID, DELETE);
#endif
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_6_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        first = lv_textarea_get_text(guider_ui.admin_Interface_ta_1);
        second = lv_textarea_get_text(guider_ui.admin_Interface_ta_2);
        third = lv_textarea_get_text(guider_ui.admin_Interface_ta_3);
        forth = lv_textarea_get_text(guider_ui.admin_Interface_ta_4);

        // 使用 snprintf 安全拼接
        snprintf(delete_card, sizeof(delete_card), "DeleteCard 0x%s 0x%s 0x%s 0x%s", first, second, third, forth);
        LOG(delete_card);

        const char* parts[4] = {first, second, third, forth};

        for (int i = 0; i < 4; i++) {
            readUID[i] = strtol(parts[i], &endptr, 16);  // 16 表示16进制
            // readUID[i] = card_num[i+1];
            if (endptr == parts[i] || *endptr != '\0') {
                LOG("无效的 16 进制字符串");
            }
        }

#ifdef USE
        // card_num[0] = 0x9B;
        // card_num[5] = 0x5A;
        // HAL_UART_Transmit(&ESP32_UART, card_num, sizeof(card_num), HAL_MAX_DELAY);
        // DeleteCard(readUID);
        Send_Card(readUID, DELETE);
#endif
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_7_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
#ifdef USE
        card_num[0] = 0x92;
        HAL_UART_Transmit(&ESP32_UART, card_num, 1, HAL_MAX_DELAY);
#endif
        break;
    }
    default:
        break;
    }
}

static void admin_Interface_btn_8_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
#ifdef USE
        Scan_Card(readUID);
        // HAL_UART_Transmit(&ESP32_UART, card_num, sizeof(card_num), HAL_MAX_DELAY);
        SendCard(readUID);
        lv_obj_add_flag(guider_ui.admin_Interface_label_9, LV_OBJ_FLAG_HIDDEN);
#endif
        break;
    }
    default:
        break;
    }
}

void events_init_admin_Interface (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->admin_Interface, admin_Interface_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_card, admin_Interface_card_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_exit, admin_Interface_exit_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_debug, admin_Interface_debug_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_fingerprint, admin_Interface_fingerprint_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_3, admin_Interface_btn_3_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_4, admin_Interface_btn_4_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_5, admin_Interface_btn_5_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_clear, admin_Interface_btn_clear_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_sw_auto, admin_Interface_sw_auto_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_clear_alarm, admin_Interface_btn_clear_alarm_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_1, admin_Interface_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_add_hex, admin_Interface_btn_add_hex_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_6, admin_Interface_btn_6_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_7, admin_Interface_btn_7_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->admin_Interface_btn_8, admin_Interface_btn_8_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
