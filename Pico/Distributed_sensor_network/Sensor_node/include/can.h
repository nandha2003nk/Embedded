#ifndef CAN_H
#define CAN_H

#include <stdint.h>

/* CAN Message IDs */
#define CAN_ID_ACCEL       0x100
#define CAN_ID_GYRO        0x101
#define CAN_ID_BMP280      0x102
#define CAN_ID_DHT22       0x103
#define CAN_ID_LIGHT       0x104
#define CAN_ID_HEARTBEAT   0x1FF

/* Initialization */
void can_init(void);

/* CAN transmit functions */
void can_send_accel(void);
void can_send_gyro(void);
void can_send_bmp280(void);
void can_send_dht22(void);
void can_send_light(void);
void can_send_heartbeat(uint32_t uptime);

#endif /* CAN_H */