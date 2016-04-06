#include "vehicle.h"
#include "steering.h"
#include "settings.h"
#include "motor.h"
#include "drivers/mss_ace/mss_ace.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include <stdio.h>
#include <inttypes.h>

struct Settings settings;
struct Vehicle vehicle;

int main() {

    printf("Program start\r\n");

    // Enable necessary drivers
    ACE_init();
    MSS_GPIO_init();

    // Initialize settings
    init_settings();

    // Initialize vehicle
    init_vehicle(1, Script, 1);

    volatile unsigned long i = 0;
    while(1) {
        set_steer(0);
        set_motor(0);
        printf("steer = 0\r\n");
        for (i = 0; i < 10000000; ++i);
        set_steer(512);
        set_motor(200);
        printf("steer = 512\r\n");
        for (i = 0; i < 10000000; ++i);
        set_steer(1023);
        set_motor(50);
        printf("steer = 1023\r\n");
        for (i = 0; i < 10000000; ++i);
    }

    return 0;
}
