/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *init;
	bool init_del;
	lv_obj_t *init_cont_1;
	lv_obj_t *init_spinner_1;
	lv_obj_t *init_label_init;
	lv_obj_t *main;
	bool main_del;
	lv_obj_t *main_cont_1;
	lv_obj_t *main_card;
	lv_obj_t *main_label_15;
	lv_obj_t *main_label_23;
	lv_obj_t *main_label_22;
	lv_obj_t *main_label_21;
	lv_obj_t *main_label_18;
	lv_obj_t *main_label_19;
	lv_obj_t *main_label_24;
	lv_obj_t *main_label_person_heart;
	lv_obj_t *main_label_person_spo2;
	lv_obj_t *main_label_person_tem;
	lv_obj_t *main_label_3;
	lv_obj_t *main_label_distance;
	lv_obj_t *main_label_30;
	lv_obj_t *main_label_weather;
	lv_obj_t *main_label_32;
	lv_obj_t *main_display;
	lv_obj_t *main_label_visitor;
	lv_obj_t *main_label_1;
	lv_obj_t *main_label_6;
	lv_obj_t *main_label_weather_tem;
	lv_obj_t *main_label_tip;
	lv_obj_t *main_admin;
	lv_obj_t *main_admin_label;
	lv_obj_t *main_call;
	lv_obj_t *main_call_label;
	lv_obj_t *main_message;
	lv_obj_t *main_label_5;
	lv_obj_t *main_message_content;
	lv_obj_t *main_label_message_1;
	lv_obj_t *main_label_message_2;
	lv_obj_t *main_label_message_3;
	lv_obj_t *main_label_message_4;
	lv_obj_t *main_label_message_6;
	lv_obj_t *main_label_message_5;
	lv_obj_t *main_label_message_7;
	lv_obj_t *main_label_message_8;
	lv_obj_t *main_cont_2;
	lv_obj_t *main_label_33;
	lv_obj_t *main_card_error;
	lv_obj_t *main_label_34;
	lv_obj_t *main_card_correct;
	lv_obj_t *main_label_35;
	lv_obj_t *admin_Interface;
	bool admin_Interface_del;
	lv_obj_t *admin_Interface_cont_1;
	lv_obj_t *admin_Interface_menu;
	lv_obj_t *admin_Interface_card;
	lv_obj_t *admin_Interface_card_label;
	lv_obj_t *admin_Interface_exit;
	lv_obj_t *admin_Interface_exit_label;
	lv_obj_t *admin_Interface_debug;
	lv_obj_t *admin_Interface_debug_label;
	lv_obj_t *admin_Interface_fingerprint;
	lv_obj_t *admin_Interface_fingerprint_label;
	lv_obj_t *admin_Interface_display_fingerprint;
	lv_obj_t *admin_Interface_label_6;
	lv_obj_t *admin_Interface_btn_3;
	lv_obj_t *admin_Interface_btn_3_label;
	lv_obj_t *admin_Interface_ta_5;
	lv_obj_t *admin_Interface_btn_4;
	lv_obj_t *admin_Interface_btn_4_label;
	lv_obj_t *admin_Interface_btn_5;
	lv_obj_t *admin_Interface_btn_5_label;
	lv_obj_t *admin_Interface_display_debug;
	lv_obj_t *admin_Interface_cont_debug;
	lv_obj_t *admin_Interface_btn_clear;
	lv_obj_t *admin_Interface_btn_clear_label;
	lv_obj_t *admin_Interface_label_10;
	lv_obj_t *admin_Interface_sw_auto;
	lv_obj_t *admin_Interface_btn_clear_alarm;
	lv_obj_t *admin_Interface_btn_clear_alarm_label;
	lv_obj_t *admin_Interface_display_card;
	lv_obj_t *admin_Interface_label_1;
	lv_obj_t *admin_Interface_label_2;
	lv_obj_t *admin_Interface_ta_1;
	lv_obj_t *admin_Interface_ta_3;
	lv_obj_t *admin_Interface_ta_2;
	lv_obj_t *admin_Interface_ta_4;
	lv_obj_t *admin_Interface_btn_1;
	lv_obj_t *admin_Interface_btn_1_label;
	lv_obj_t *admin_Interface_btn_add_hex;
	lv_obj_t *admin_Interface_btn_add_hex_label;
	lv_obj_t *admin_Interface_btn_6;
	lv_obj_t *admin_Interface_btn_6_label;
	lv_obj_t *admin_Interface_btn_7;
	lv_obj_t *admin_Interface_btn_7_label;
	lv_obj_t *admin_Interface_btn_8;
	lv_obj_t *admin_Interface_btn_8_label;
	lv_obj_t *admin_Interface_cont_2;
	lv_obj_t *admin_Interface_label_12;
	lv_obj_t *font;
	bool font_del;
	lv_obj_t *font_label_font;
	lv_obj_t *g_kb_top_layer;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_init(lv_ui *ui);
void setup_scr_main(lv_ui *ui);
void setup_scr_admin_Interface(lv_ui *ui);
void setup_scr_font(lv_ui *ui);

LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_50)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_26)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_25)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_24)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_20)


#ifdef __cplusplus
}
#endif
#endif
