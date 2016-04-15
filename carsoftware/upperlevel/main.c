#include "headers.h"

struct Settings settings;
struct Vehicle vehicle;

#define VEHICLE_ID 1

int main()
{
	printf("Program start\r\n");

	// Enable necessary drivers
	ACE_init();
	MSS_GPIO_init();

	// Initialize settings
	init_settings();

	// Initialize vehicle
	init_vehicle(VEHICLE_ID, Script, VEHICLE_ID);

	printf("Init complete\r\n");

loop:
	goto loop;

	return 0;
}
