#ifndef __BSP_CMD_H_
#define __BSP_CMD_H_

/* Receive */
#define CMD_RECEIVE_TEMPERATURE 0xC1
#define CMD_RECEIVE_WEATHER     0xC2
#define CMD_RECEIVE_CALL        0xC6
#define CMD_RECEIVE_ALARM       0xC7
#define CMD_RECEIVE_VISITORS    0xC8
#define CMD_RECEIVE_UID         0xCA

/* ESP32 */
#define CMD_WAKE                0xFF
#define CMD_RESTART             0x80
#define CMD_RESTART_OK          0x81

/* MQTT */
#define CMD_GET_INIT            0xB5
#define CMD_PUB                 0xB6

/* EXECUTE */
#define CMD_ADD_UID             0xA5
#define CMD_OPEN_DOOR           0x90
#define CMD_LEAVE_MESSAGE       0x91
#define CMD_CLEAR_UID           0x92
#define CMD_DETECTED_SOUND      0x93
#define CMD_DELETE_CARD         0x9B
#define CMD_FRAME_END           0x5A

#endif // !__BSP_CMD_H_