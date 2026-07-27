#include "app.h"

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

#include "mcp2515.h"
#include "mpu6050.h"
#include "bmp280.h"
#include "dht22.h"
#include "can.h"
#include "sensors.h"

static uint32_t last_fast = 0;
static uint32_t last_slow = 0;
static uint32_t last_hb = 0;

static uint32_t uptime_s = 0;
static uint32_t last_uptime_tick = 0;

void app_init(void)
{
    stdio_init_all();

    /* Initialize peripherals */
    mcp2515_init();

    mpu6050_init();

    bmp280_init();

    gpio_init(DHT22_PIN);

    adc_init();
    adc_gpio_init(26);
    adc_select_input(LIGHT_ADC_INPUT);

    last_uptime_tick = to_ms_since_boot(get_absolute_time());

    printf("Sensor node started...\n");
}

void app_run(void)
{
    uint32_t now = to_ms_since_boot(get_absolute_time());

    /* Update uptime counter */
    if (now - last_uptime_tick >= 1000)
    {
        uptime_s++;
        last_uptime_tick += 1000;
    }

    /* Send IMU + BMP280 */
    if (now - last_fast >= SEND_PERIOD_MS)
    {
        last_fast = now;

        can_send_accel();

        can_send_gyro();

        can_send_bmp280();
    }

    /* Send DHT22 + Light */
    if (now - last_slow >= SLOW_SEND_PERIOD_MS)
    {
        last_slow = now;

        can_send_dht22();

        can_send_light();
    }

    /* Heartbeat */
    if (now - last_hb >= HEARTBEAT_PERIOD_MS)
    {
        last_hb = now;

        can_send_heartbeat(uptime_s);
    }

    sleep_ms(5);
}