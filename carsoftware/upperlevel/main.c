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

	MSS_GPIO_config(MSS_GPIO_8, MSS_GPIO_INPUT_MODE);
	//MSS_GPIO_enable_irq(MSS_GPIO_8);

	// Initialize vehicle
	init_vehicle(1, Script, 1);


	int prev = 1;
	int curr = 1;
	int count = 1;
	while(1) {
		//*settings.gun_loc = (uint32_t)0b10110011010;
		curr = ((MSS_GPIO_get_inputs()>>8) & 1);
		if (prev && !curr) {
			printf("fire ");
			for (count = 0; count < 10; ++count) {
				printf("pew ");
				//*settings.gun_loc = 0b10110011010;
				shoot();
			}
			printf("\r\n");
		}
		prev = curr;
	}

	return 0;
}
