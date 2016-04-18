#include "headers.h"

struct Settings settings;
struct Vehicle vehicle;

int VEHICLE_ID = 4;
#define STAND_ALONE 0

int main() {

	// Enable necessary drivers
	ACE_init();
	MSS_GPIO_init();
	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD,
		MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

	// Initialize settings
	init_settings();
	init_comms();

	kill_steer();
	kill_motor();

start:

	// Wait for type
	while (!STAND_ALONE && !settings.type_set);

	// Wait for start
	while (!STAND_ALONE && !settings.game_start);

	// Initialize vehicle
	if (STAND_ALONE) {
		init_vehicle(VEHICLE_ID, Script, VEHICLE_ID);
		start_steer();
		start_motor();
	}

	while (STAND_ALONE || settings.game_start);

	goto start;

	return 0;
}
