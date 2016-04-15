#include "headers.h"

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
