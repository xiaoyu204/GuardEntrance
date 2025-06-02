/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_admin_Interface(lv_ui *ui)
{
    //Write codes admin_Interface
    ui->admin_Interface = lv_obj_create(NULL);
    lv_obj_set_size(ui->admin_Interface, 800, 480);
    lv_obj_set_scrollbar_mode(ui->admin_Interface, LV_SCROLLBAR_MODE_OFF);

    //Write style for admin_Interface, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_cont_1
    ui->admin_Interface_cont_1 = lv_obj_create(ui->admin_Interface);
    lv_obj_set_pos(ui->admin_Interface_cont_1, 0, 0);
    lv_obj_set_size(ui->admin_Interface_cont_1, 800, 480);
    lv_obj_set_scrollbar_mode(ui->admin_Interface_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for admin_Interface_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_cont_1, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_cont_1, lv_color_hex(0xDFE2E8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_menu
    ui->admin_Interface_menu = lv_obj_create(ui->admin_Interface_cont_1);
    lv_obj_set_pos(ui->admin_Interface_menu, 15, 25);
    lv_obj_set_size(ui->admin_Interface_menu, 120, 430);
    lv_obj_set_scrollbar_mode(ui->admin_Interface_menu, LV_SCROLLBAR_MODE_OFF);

    //Write style for admin_Interface_menu, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_menu, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_menu, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_menu, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_menu, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_menu, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_menu, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_menu, lv_color_hex(0xf2f4f7), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_menu, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_card
    ui->admin_Interface_card = lv_btn_create(ui->admin_Interface_menu);
    ui->admin_Interface_card_label = lv_label_create(ui->admin_Interface_card);
    lv_label_set_text(ui->admin_Interface_card_label, "门禁");
    lv_label_set_long_mode(ui->admin_Interface_card_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_card_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_card, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_card_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_card, 10, 10);
    lv_obj_set_size(ui->admin_Interface_card, 100, 30);

    //Write style for admin_Interface_card, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_card, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_card, lv_color_hex(0x2FCADA), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_card, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_card, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_card, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_card, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_card, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_card, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_exit
    ui->admin_Interface_exit = lv_btn_create(ui->admin_Interface_menu);
    ui->admin_Interface_exit_label = lv_label_create(ui->admin_Interface_exit);
    lv_label_set_text(ui->admin_Interface_exit_label, "退出");
    lv_label_set_long_mode(ui->admin_Interface_exit_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_exit_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_exit, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_exit_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_exit, 10, 384);
    lv_obj_set_size(ui->admin_Interface_exit, 100, 30);

    //Write style for admin_Interface_exit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_exit, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_exit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_exit, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_exit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_exit, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_exit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_exit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_debug
    ui->admin_Interface_debug = lv_btn_create(ui->admin_Interface_menu);
    ui->admin_Interface_debug_label = lv_label_create(ui->admin_Interface_debug);
    lv_label_set_text(ui->admin_Interface_debug_label, "调试信息");
    lv_label_set_long_mode(ui->admin_Interface_debug_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_debug_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_debug, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_debug_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_debug, 10, 260);
    lv_obj_set_size(ui->admin_Interface_debug, 100, 30);

    //Write style for admin_Interface_debug, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_debug, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_debug, lv_color_hex(0xc6ef72), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_debug, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_debug, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_debug, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_debug, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_debug, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_debug, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_fingerprint
    ui->admin_Interface_fingerprint = lv_btn_create(ui->admin_Interface_menu);
    ui->admin_Interface_fingerprint_label = lv_label_create(ui->admin_Interface_fingerprint);
    lv_label_set_text(ui->admin_Interface_fingerprint_label, "指纹");
    lv_label_set_long_mode(ui->admin_Interface_fingerprint_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_fingerprint_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_fingerprint, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_fingerprint_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_fingerprint, 10, 135);
    lv_obj_set_size(ui->admin_Interface_fingerprint, 100, 30);

    //Write style for admin_Interface_fingerprint, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_fingerprint, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_fingerprint, lv_color_hex(0x2FDA64), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_fingerprint, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_fingerprint, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_fingerprint, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_fingerprint, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_fingerprint, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_fingerprint, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_fingerprint, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_fingerprint, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_display_fingerprint
    ui->admin_Interface_display_fingerprint = lv_obj_create(ui->admin_Interface_cont_1);
    lv_obj_set_pos(ui->admin_Interface_display_fingerprint, 152, 25);
    lv_obj_set_size(ui->admin_Interface_display_fingerprint, 625, 430);
    lv_obj_set_scrollbar_mode(ui->admin_Interface_display_fingerprint, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->admin_Interface_display_fingerprint, LV_OBJ_FLAG_HIDDEN);

    //Write style for admin_Interface_display_fingerprint, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_display_fingerprint, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_display_fingerprint, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_display_fingerprint, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_display_fingerprint, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_display_fingerprint, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_display_fingerprint, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_display_fingerprint, lv_color_hex(0xf2f4f7), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_display_fingerprint, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_display_fingerprint, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_display_fingerprint, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_display_fingerprint, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_display_fingerprint, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_display_fingerprint, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_label_6
    ui->admin_Interface_label_6 = lv_label_create(ui->admin_Interface_display_fingerprint);
    lv_label_set_text(ui->admin_Interface_label_6, "指纹ID(16 进制)");
    lv_label_set_long_mode(ui->admin_Interface_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->admin_Interface_label_6, 15, 15);
    lv_obj_set_size(ui->admin_Interface_label_6, 177, 29);

    //Write style for admin_Interface_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_label_6, &lv_font_SourceHanSerifSC_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_label_6, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_3
    ui->admin_Interface_btn_3 = lv_btn_create(ui->admin_Interface_display_fingerprint);
    ui->admin_Interface_btn_3_label = lv_label_create(ui->admin_Interface_btn_3);
    lv_label_set_text(ui->admin_Interface_btn_3_label, "添加指纹");
    lv_label_set_long_mode(ui->admin_Interface_btn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_3_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_3, 15, 120);
    lv_obj_set_size(ui->admin_Interface_btn_3, 100, 50);

    //Write style for admin_Interface_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_btn_3, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_btn_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_3, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_3, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_3, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_ta_5
    ui->admin_Interface_ta_5 = lv_textarea_create(ui->admin_Interface_display_fingerprint);
    lv_textarea_set_text(ui->admin_Interface_ta_5, "");
    lv_textarea_set_placeholder_text(ui->admin_Interface_ta_5, "ID");
    lv_textarea_set_password_bullet(ui->admin_Interface_ta_5, "*");
    lv_textarea_set_password_mode(ui->admin_Interface_ta_5, false);
    lv_textarea_set_one_line(ui->admin_Interface_ta_5, true);
    lv_textarea_set_accepted_chars(ui->admin_Interface_ta_5, "1234567890abcdef");
    lv_textarea_set_max_length(ui->admin_Interface_ta_5, 2);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->admin_Interface_ta_5, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->admin_Interface_ta_5, 15, 55);
    lv_obj_set_size(ui->admin_Interface_ta_5, 100, 30);
    lv_obj_add_flag(ui->admin_Interface_ta_5, LV_OBJ_FLAG_SCROLL_CHAIN_HOR);

    //Write style for admin_Interface_ta_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->admin_Interface_ta_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_ta_5, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_ta_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_ta_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_ta_5, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_ta_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_ta_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_ta_5, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_ta_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_ta_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_ta_5, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_ta_5, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_ta_5, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_5, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for admin_Interface_ta_5, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_5, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_5, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_5, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_5, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_4
    ui->admin_Interface_btn_4 = lv_btn_create(ui->admin_Interface_display_fingerprint);
    ui->admin_Interface_btn_4_label = lv_label_create(ui->admin_Interface_btn_4);
    lv_label_set_text(ui->admin_Interface_btn_4_label, "清除指纹");
    lv_label_set_long_mode(ui->admin_Interface_btn_4_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_4_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_4, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_4_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_4, 263, 120);
    lv_obj_set_size(ui->admin_Interface_btn_4, 100, 50);

    //Write style for admin_Interface_btn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_btn_4, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_btn_4, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_4, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_4, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_4, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_5
    ui->admin_Interface_btn_5 = lv_btn_create(ui->admin_Interface_display_fingerprint);
    ui->admin_Interface_btn_5_label = lv_label_create(ui->admin_Interface_btn_5);
    lv_label_set_text(ui->admin_Interface_btn_5_label, "清除所有");
    lv_label_set_long_mode(ui->admin_Interface_btn_5_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_5_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_5, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_5_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_5, 511, 120);
    lv_obj_set_size(ui->admin_Interface_btn_5, 100, 50);

    //Write style for admin_Interface_btn_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_btn_5, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_btn_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_5, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_5, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_5, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_display_debug
    ui->admin_Interface_display_debug = lv_obj_create(ui->admin_Interface_cont_1);
    lv_obj_set_pos(ui->admin_Interface_display_debug, 150, 25);
    lv_obj_set_size(ui->admin_Interface_display_debug, 625, 430);
    lv_obj_set_scrollbar_mode(ui->admin_Interface_display_debug, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_add_flag(ui->admin_Interface_display_debug, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->admin_Interface_display_debug, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for admin_Interface_display_debug, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_display_debug, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_display_debug, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_display_debug, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_display_debug, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_display_debug, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_display_debug, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_display_debug, lv_color_hex(0xf2f4f7), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_display_debug, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_display_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_display_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_display_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_display_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_display_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_cont_debug
    ui->admin_Interface_cont_debug = lv_obj_create(ui->admin_Interface_display_debug);
    lv_obj_set_pos(ui->admin_Interface_cont_debug, 10, 50);
    lv_obj_set_size(ui->admin_Interface_cont_debug, 605, 370);
    lv_obj_set_scrollbar_mode(ui->admin_Interface_cont_debug, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_add_flag(ui->admin_Interface_cont_debug, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for admin_Interface_cont_debug, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_cont_debug, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_cont_debug, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_cont_debug, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_cont_debug, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_cont_debug, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_cont_debug, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_cont_debug, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_cont_debug, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_cont_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_cont_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_cont_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_cont_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_cont_debug, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_clear
    ui->admin_Interface_btn_clear = lv_btn_create(ui->admin_Interface_display_debug);
    ui->admin_Interface_btn_clear_label = lv_label_create(ui->admin_Interface_btn_clear);
    lv_label_set_text(ui->admin_Interface_btn_clear_label, "清除");
    lv_label_set_long_mode(ui->admin_Interface_btn_clear_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_clear_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_clear, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_clear_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_clear, 530, 10);
    lv_obj_set_size(ui->admin_Interface_btn_clear, 70, 30);

    //Write style for admin_Interface_btn_clear, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_clear, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_clear, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_clear, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_clear, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->admin_Interface_btn_clear, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->admin_Interface_btn_clear, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->admin_Interface_btn_clear, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->admin_Interface_btn_clear, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->admin_Interface_btn_clear, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_clear, lv_color_hex(0x191717), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_clear, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_clear, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_clear, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_label_10
    ui->admin_Interface_label_10 = lv_label_create(ui->admin_Interface_display_debug);
    lv_label_set_text(ui->admin_Interface_label_10, "自动滚动");
    lv_label_set_long_mode(ui->admin_Interface_label_10, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->admin_Interface_label_10, 380, 17);
    lv_obj_set_size(ui->admin_Interface_label_10, 90, 20);

    //Write style for admin_Interface_label_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_label_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_label_10, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_label_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_label_10, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_sw_auto
    ui->admin_Interface_sw_auto = lv_switch_create(ui->admin_Interface_display_debug);
    lv_obj_set_pos(ui->admin_Interface_sw_auto, 480, 15);
    lv_obj_set_size(ui->admin_Interface_sw_auto, 40, 20);

    //Write style for admin_Interface_sw_auto, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_sw_auto, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_sw_auto, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_sw_auto, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_sw_auto, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_sw_auto, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_sw_auto, lv_color_hex(0x00bdff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_sw_auto, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_sw_auto, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_sw_auto, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for admin_Interface_sw_auto, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->admin_Interface_sw_auto, 223, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->admin_Interface_sw_auto, lv_color_hex(0x13359a), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_sw_auto, LV_GRAD_DIR_HOR, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(ui->admin_Interface_sw_auto, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_main_stop(ui->admin_Interface_sw_auto, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_stop(ui->admin_Interface_sw_auto, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->admin_Interface_sw_auto, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for admin_Interface_sw_auto, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_sw_auto, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_sw_auto, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_sw_auto, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_sw_auto, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_sw_auto, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_clear_alarm
    ui->admin_Interface_btn_clear_alarm = lv_btn_create(ui->admin_Interface_display_debug);
    ui->admin_Interface_btn_clear_alarm_label = lv_label_create(ui->admin_Interface_btn_clear_alarm);
    lv_label_set_text(ui->admin_Interface_btn_clear_alarm_label, "解除警报");
    lv_label_set_long_mode(ui->admin_Interface_btn_clear_alarm_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_clear_alarm_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_clear_alarm, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_clear_alarm_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_clear_alarm, 10, 10);
    lv_obj_set_size(ui->admin_Interface_btn_clear_alarm, 90, 30);

    //Write style for admin_Interface_btn_clear_alarm, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_clear_alarm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_clear_alarm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_clear_alarm, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_clear_alarm, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->admin_Interface_btn_clear_alarm, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->admin_Interface_btn_clear_alarm, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->admin_Interface_btn_clear_alarm, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->admin_Interface_btn_clear_alarm, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->admin_Interface_btn_clear_alarm, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_clear_alarm, lv_color_hex(0x191717), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_clear_alarm, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_clear_alarm, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_clear_alarm, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_display_card
    ui->admin_Interface_display_card = lv_obj_create(ui->admin_Interface_cont_1);
    lv_obj_set_pos(ui->admin_Interface_display_card, 150, 25);
    lv_obj_set_size(ui->admin_Interface_display_card, 625, 430);
    lv_obj_set_scrollbar_mode(ui->admin_Interface_display_card, LV_SCROLLBAR_MODE_OFF);

    //Write style for admin_Interface_display_card, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_display_card, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_display_card, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_display_card, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_display_card, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_display_card, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_display_card, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_display_card, lv_color_hex(0xf2f4f7), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_display_card, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_display_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_display_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_display_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_display_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_display_card, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_label_1
    ui->admin_Interface_label_1 = lv_label_create(ui->admin_Interface_display_card);
    lv_label_set_text(ui->admin_Interface_label_1, "添加新卡");
    lv_label_set_long_mode(ui->admin_Interface_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->admin_Interface_label_1, 15, 25);
    lv_obj_set_size(ui->admin_Interface_label_1, 100, 25);

    //Write style for admin_Interface_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_label_1, &lv_font_SourceHanSerifSC_Regular_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_label_2
    ui->admin_Interface_label_2 = lv_label_create(ui->admin_Interface_display_card);
    lv_label_set_text(ui->admin_Interface_label_2, "卡号(16进制)");
    lv_label_set_long_mode(ui->admin_Interface_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->admin_Interface_label_2, 15, 50);
    lv_obj_set_size(ui->admin_Interface_label_2, 100, 20);

    //Write style for admin_Interface_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_label_2, lv_color_hex(0x6f6f6f), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_label_2, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_ta_1
    ui->admin_Interface_ta_1 = lv_textarea_create(ui->admin_Interface_display_card);
    lv_textarea_set_text(ui->admin_Interface_ta_1, "");
    lv_textarea_set_placeholder_text(ui->admin_Interface_ta_1, "first");
    lv_textarea_set_password_bullet(ui->admin_Interface_ta_1, "*");
    lv_textarea_set_password_mode(ui->admin_Interface_ta_1, false);
    lv_textarea_set_one_line(ui->admin_Interface_ta_1, false);
    lv_textarea_set_accepted_chars(ui->admin_Interface_ta_1, "1234567890abcdef");
    lv_textarea_set_max_length(ui->admin_Interface_ta_1, 2);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->admin_Interface_ta_1, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->admin_Interface_ta_1, 25, 80);
    lv_obj_set_size(ui->admin_Interface_ta_1, 70, 30);

    //Write style for admin_Interface_ta_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->admin_Interface_ta_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_ta_1, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_ta_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_ta_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_ta_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_ta_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for admin_Interface_ta_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_1, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes admin_Interface_ta_3
    ui->admin_Interface_ta_3 = lv_textarea_create(ui->admin_Interface_display_card);
    lv_textarea_set_text(ui->admin_Interface_ta_3, "");
    lv_textarea_set_placeholder_text(ui->admin_Interface_ta_3, "third");
    lv_textarea_set_password_bullet(ui->admin_Interface_ta_3, "*");
    lv_textarea_set_password_mode(ui->admin_Interface_ta_3, false);
    lv_textarea_set_one_line(ui->admin_Interface_ta_3, false);
    lv_textarea_set_accepted_chars(ui->admin_Interface_ta_3, "1234567890abcdef");
    lv_textarea_set_max_length(ui->admin_Interface_ta_3, 2);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->admin_Interface_ta_3, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->admin_Interface_ta_3, 208, 80);
    lv_obj_set_size(ui->admin_Interface_ta_3, 70, 30);

    //Write style for admin_Interface_ta_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->admin_Interface_ta_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_ta_3, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_ta_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_ta_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_ta_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_ta_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_ta_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_ta_3, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_ta_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_ta_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_ta_3, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_ta_3, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_ta_3, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_3, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for admin_Interface_ta_3, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_3, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_3, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_3, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_3, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes admin_Interface_ta_2
    ui->admin_Interface_ta_2 = lv_textarea_create(ui->admin_Interface_display_card);
    lv_textarea_set_text(ui->admin_Interface_ta_2, "");
    lv_textarea_set_placeholder_text(ui->admin_Interface_ta_2, "second");
    lv_textarea_set_password_bullet(ui->admin_Interface_ta_2, "*");
    lv_textarea_set_password_mode(ui->admin_Interface_ta_2, false);
    lv_textarea_set_one_line(ui->admin_Interface_ta_2, false);
    lv_textarea_set_accepted_chars(ui->admin_Interface_ta_2, "1234567890abcdef");
    lv_textarea_set_max_length(ui->admin_Interface_ta_2, 2);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->admin_Interface_ta_2, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->admin_Interface_ta_2, 116, 80);
    lv_obj_set_size(ui->admin_Interface_ta_2, 70, 30);

    //Write style for admin_Interface_ta_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->admin_Interface_ta_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_ta_2, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_ta_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_ta_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_ta_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_ta_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_ta_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_ta_2, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_ta_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_ta_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_ta_2, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_ta_2, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_ta_2, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_2, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for admin_Interface_ta_2, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_2, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_2, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_2, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_2, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes admin_Interface_ta_4
    ui->admin_Interface_ta_4 = lv_textarea_create(ui->admin_Interface_display_card);
    lv_textarea_set_text(ui->admin_Interface_ta_4, "");
    lv_textarea_set_placeholder_text(ui->admin_Interface_ta_4, "forth");
    lv_textarea_set_password_bullet(ui->admin_Interface_ta_4, "*");
    lv_textarea_set_password_mode(ui->admin_Interface_ta_4, false);
    lv_textarea_set_one_line(ui->admin_Interface_ta_4, false);
    lv_textarea_set_accepted_chars(ui->admin_Interface_ta_4, "1234567890abcdef");
    lv_textarea_set_max_length(ui->admin_Interface_ta_4, 2);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->admin_Interface_ta_4, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->admin_Interface_ta_4, 298, 80);
    lv_obj_set_size(ui->admin_Interface_ta_4, 70, 30);

    //Write style for admin_Interface_ta_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->admin_Interface_ta_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_ta_4, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_ta_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_ta_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_ta_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_ta_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_ta_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_ta_4, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_ta_4, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_ta_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_ta_4, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_ta_4, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_ta_4, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_4, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for admin_Interface_ta_4, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_ta_4, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_ta_4, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_ta_4, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_ta_4, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_1
    ui->admin_Interface_btn_1 = lv_btn_create(ui->admin_Interface_display_card);
    ui->admin_Interface_btn_1_label = lv_label_create(ui->admin_Interface_btn_1);
    lv_label_set_text(ui->admin_Interface_btn_1_label, "扫卡添卡");
    lv_label_set_long_mode(ui->admin_Interface_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_1, 15, 200);
    lv_obj_set_size(ui->admin_Interface_btn_1, 100, 50);

    //Write style for admin_Interface_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_btn_1, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_btn_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_1, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_1, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_1, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_add_hex
    ui->admin_Interface_btn_add_hex = lv_btn_create(ui->admin_Interface_display_card);
    ui->admin_Interface_btn_add_hex_label = lv_label_create(ui->admin_Interface_btn_add_hex);
    lv_label_set_text(ui->admin_Interface_btn_add_hex_label, "添加");
    lv_label_set_long_mode(ui->admin_Interface_btn_add_hex_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_add_hex_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_add_hex, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_add_hex_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_add_hex, 16, 130);
    lv_obj_set_size(ui->admin_Interface_btn_add_hex, 100, 50);

    //Write style for admin_Interface_btn_add_hex, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_add_hex, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_add_hex, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_btn_add_hex, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_btn_add_hex, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_btn_add_hex, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_add_hex, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_add_hex, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_add_hex, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_add_hex, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_add_hex, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_add_hex, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_6
    ui->admin_Interface_btn_6 = lv_btn_create(ui->admin_Interface_display_card);
    ui->admin_Interface_btn_6_label = lv_label_create(ui->admin_Interface_btn_6);
    lv_label_set_text(ui->admin_Interface_btn_6_label, "删除");
    lv_label_set_long_mode(ui->admin_Interface_btn_6_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_6_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_6, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_6_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_6, 262, 130);
    lv_obj_set_size(ui->admin_Interface_btn_6, 100, 50);

    //Write style for admin_Interface_btn_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_btn_6, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_btn_6, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_6, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_6, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_6, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_7
    ui->admin_Interface_btn_7 = lv_btn_create(ui->admin_Interface_display_card);
    ui->admin_Interface_btn_7_label = lv_label_create(ui->admin_Interface_btn_7);
    lv_label_set_text(ui->admin_Interface_btn_7_label, "删除所有");
    lv_label_set_long_mode(ui->admin_Interface_btn_7_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_7_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_7, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_7_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_7, 508, 130);
    lv_obj_set_size(ui->admin_Interface_btn_7, 100, 50);

    //Write style for admin_Interface_btn_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_btn_7, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_btn_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_btn_7, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_btn_7, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_7, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_7, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_btn_8
    ui->admin_Interface_btn_8 = lv_btn_create(ui->admin_Interface_display_card);
    ui->admin_Interface_btn_8_label = lv_label_create(ui->admin_Interface_btn_8);
    lv_label_set_text(ui->admin_Interface_btn_8_label, "扫卡删卡");
    lv_label_set_long_mode(ui->admin_Interface_btn_8_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->admin_Interface_btn_8_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->admin_Interface_btn_8, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->admin_Interface_btn_8_label, LV_PCT(100));
    lv_obj_set_pos(ui->admin_Interface_btn_8, 262, 200);
    lv_obj_set_size(ui->admin_Interface_btn_8, 100, 50);

    //Write style for admin_Interface_btn_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->admin_Interface_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->admin_Interface_btn_8, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_btn_8, lv_color_hex(0x5482a9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_btn_8, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_btn_8, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_btn_8, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_btn_8, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_btn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_cont_2
    ui->admin_Interface_cont_2 = lv_obj_create(ui->admin_Interface);
    lv_obj_set_pos(ui->admin_Interface_cont_2, 0, 0);
    lv_obj_set_size(ui->admin_Interface_cont_2, 800, 480);
    lv_obj_set_scrollbar_mode(ui->admin_Interface_cont_2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->admin_Interface_cont_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->admin_Interface_cont_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for admin_Interface_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->admin_Interface_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->admin_Interface_cont_2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->admin_Interface_cont_2, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->admin_Interface_cont_2, lv_color_hex(0xf00000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->admin_Interface_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes admin_Interface_label_12
    ui->admin_Interface_label_12 = lv_label_create(ui->admin_Interface_cont_2);
    lv_label_set_text(ui->admin_Interface_label_12, "请验证指纹");
    lv_label_set_long_mode(ui->admin_Interface_label_12, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->admin_Interface_label_12, 271, 213);
    lv_obj_set_size(ui->admin_Interface_label_12, 258, 54);

    //Write style for admin_Interface_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->admin_Interface_label_12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->admin_Interface_label_12, &lv_font_SourceHanSerifSC_Regular_50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->admin_Interface_label_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->admin_Interface_label_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->admin_Interface_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of admin_Interface.


    //Update current screen layout.
    lv_obj_update_layout(ui->admin_Interface);

    //Init events for screen.
    events_init_admin_Interface(ui);
}
