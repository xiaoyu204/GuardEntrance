#include "bsp_esp32.h"

/**
 * @brief   ESP32任务
 * @param   pvParameters: 任务参数
 * @retval  无
 */
void ESP32_Task(void *pvParameters)
{
    extern EventGroupHandle_t DoorEventGroup;
    extern QueueHandle_t      ESP32Queue;
    extern lv_ui              guider_ui;
    // extern SemaphoreHandle_t  MutexSemaphore; /* lv_task_handler()线程不安全必须加互斥量 */

    while (1) {
        extern lv_ui guider_ui;
        uint8_t      authen;

        xQueueReceive(ESP32Queue, &authen, portMAX_DELAY);
        switch (authen) {
            case CMD_OPEN_DOOR: {
                printf("Open the door\n");
                float temperature    = 0;
                // int   dis_spo2_valid = 0, dis_hr_valid = 0;
                bool  test_fail = false;

                lv_label_set_text(guider_ui.main_label_tip, "请测量体温");

                do {
                    HAL_Delay(2000);
                    temperature = SMBus_ReadTemp();
                    // temperature = 36.0;
                    printf("human temperature = %.2f\n", temperature);

                    if (temperature <= 28.0) {
                        printf("temperature detection please try again\n");
                    } else if (temperature <= 37.5) {
                        printf("temperature pass\n");
                        lv_label_set_text_fmt(guider_ui.main_label_person_tem, "%.1f C", temperature);
                        xEventGroupSetBits(DoorEventGroup, RC522_TASK_BIT);
                        break; // 温度合格，退出循环
                    } else {
                        printf("temperature too high\n");
                        lv_obj_clear_flag(guider_ui.main_cont_2, LV_OBJ_FLAG_HIDDEN);
                        Beep_Last(2000);
                        test_fail = true;
                        goto fail; // 温度过高，退出循环（可根据需求调整）
                    }
                } while (1);

                // do {
                //     HAL_Delay(2000);
                //     // blood_Loop(&dis_hr_valid, &dis_spo2_valid);
                //     dis_hr_valid   = 70;
                //     dis_spo2_valid = 98;
                //     printf("dis_hr_valid = %d, dis_spo2_valid = %d \n", dis_hr_valid, dis_spo2_valid);

                //     if (dis_hr_valid <= 50) {
                //         printf("Heart rate detection please try again\n");
                //     } else if (dis_hr_valid <= 120) {
                //         printf("Heart rate pass\n");
                //         lv_label_set_text_fmt(guider_ui.main_label_person_heart, "%d 次", dis_hr_valid);
                //     } else {
                //         printf("Heart rate too high\n");
                //         Beep_Last(2000);
                //         test_fail = true;
                //         goto fail; // 过高，退出循环（可根据需求调整）
                //     }

                //     if (dis_spo2_valid <= 80) {
                //         printf("Blood oxygen Please try again\n");
                //     } else if (dis_spo2_valid <= 90) {
                //         printf("Blood oxygen too low\n");
                //         test_fail = true;
                //         Beep_Last(2000);
                //         goto fail;
                //     } else {
                //         printf("Blood oxygen pass\n");
                //         lv_label_set_text_fmt(guider_ui.main_label_person_spo2, "%d %%", dis_spo2_valid);
                //         xEventGroupSetBits(DoorEventGroup, RC522_TASK_BIT);
                //         break; // 合格，退出循环
                //     }
                // } while (1);

            fail:
                if (test_fail) {
                    printf("Check fail\n");
                }
                lv_label_set_text(guider_ui.main_label_tip, "");
                break;
            }

            case CMD_DETECTED_SOUND:
                printf("Detected Sound\n");
                // lv_obj_clear_flag(guider_ui.main_label_tip, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text(guider_ui.main_label_tip, "请讲");
                vTaskDelay(3000);
                lv_label_set_text(guider_ui.main_label_tip, "");
                // lv_obj_add_flag(guider_ui.main_label_tip, LV_OBJ_FLAG_HIDDEN);
                break;

            default: printf("Receive Byte: 0x%X\n", authen); break;
        }
    }
}