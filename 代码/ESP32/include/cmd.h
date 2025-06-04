#pragma once
/* COMMON */
#define DEBUG                0

/* Send */
#define CMD_SEND_TEMPERATURE 0xC1
#define CMD_SEND_WEATHER     0xC2
#define CMD_SEND_CALL        0xC6
#define CMD_SEND_ALARM       0xC7
#define CMD_SEND_VISITORS    0xC8
#define CMD_SEND_UID         0xCA

/* ESP32 */
#define CMD_RESTART          0x80
#define CMD_RESTART_OK       0x81
#define CMD_WAKE             0xFF

/* MQTT */
#define CMD_GET_UNUM         0xB2
#define CMD_GET_INIT         0xB5
#define CMD_PUB              0xB6

/* EXECUTE */
#define CMD_ADD_UID          0xA5
#define CMD_OPEN_DOOR        0x90
#define CMD_LEAVE_MESSAGE    0x91
#define CMD_CLEAR_UID        0x92
#define CMD_DETECTED_SOUND   0x93
#define CMD_START_LEAVE      0x94
#define CMD_EMERGENCY_CALL   0x9A
#define CMD_DELETE_CARD      0x9B