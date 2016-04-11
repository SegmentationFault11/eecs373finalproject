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

    // Enable necessary drivers
    ACE_init();
    MSS_GPIO_init();

    // Initialize settings
    init_settings();

    // Initialize vehicle
    init_vehicle(1, Script, 1);

    volatile unsigned long i = 0;

loop:
    goto loop;
    return 0;
}
