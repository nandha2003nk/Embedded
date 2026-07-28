#include "can.h"

#include <string.h>

#include "mcp2515.h"
#include "mpu6050.h"
#include "bmp280.h"
#include "dht22.h"
#include "checksum.h"

/*----------------------------------------------------------*/
/* Private helper function                                  */
/*----------------------------------------------------------*/

static void can_send_frame(uint16_t id, uint8_t *data, uint8_t length)
{
    mcp2515_send(id, length, data);
}

/*----------------------------------------------------------*/
/* Public Functions                                         */
/*----------------------------------------------------------*/

void can_init(void)
{
    /* Reserved for future use */
}

void can_send_accel(void)
{
    /* TODO
       Read MPU6050
       Pack X,Y,Z acceleration
       Compute checksum
       Send frame
    */
}

void can_send_gyro(void)
{
    /* TODO
       Read MPU6050
       Pack gyro values
       Compute checksum
       Send frame
    */
}

void can_send_bmp280(void)
{
    /* TODO
       Read BMP280
       Pack temperature & pressure
       Send frame
    */
}

void can_send_dht22(void)
{
    /* TODO
       Read DHT22
       Pack humidity & temperature
       Send frame
    */
}

void can_send_light(void)
{
    /* TODO
       Read ADC
       Pack light value
       Send frame
    */
}

void can_send_heartbeat(uint32_t uptime)
{
    uint8_t frame[8] = {0};

    frame[0] = (uptime >> 24) & 0xFF;
    frame[1] = (uptime >> 16) & 0xFF;
    frame[2] = (uptime >> 8) & 0xFF;
    frame[3] = uptime & 0xFF;

    frame[7] = xor_checksum(frame, 7);

    can_send_frame(CAN_ID_HEARTBEAT, frame, 8);
}