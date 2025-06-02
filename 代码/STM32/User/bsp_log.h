#ifndef __BSP_LOG_H_
#define __BSP_LOG_H_

extern void add_msg(lv_obj_t *scoll, const char *message);

#define LOG(message) add_msg(guider_ui.admin_Interface_cont_debug, message)

#endif // !__BSP_LOG_H_
