#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_ace/mss_ace.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include "vehicle.h"
#include "steering.h"
#include "settings.h"
#include "motor.h"
#include "controller.h"

struct Settings settings;
struct Vehicle vehicle;

int main()
{
	printf("Program start\r\n");

	// Enable necessary drivers
	ACE_init();
	MSS_GPIO_init();

	// Initialize settings
	init_settings();

	// Initialize vehicle
	init_vehicle(1, Script, 1);


loop:
	goto loop;

	return 0;
}
