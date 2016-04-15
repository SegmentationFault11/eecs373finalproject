#include "headers.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

void GPIO4_IRQHandler() {
	MSS_GPIO_clear_irq(MSS_GPIO_4);

	uint16_t pot_pos = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_2"))>>2);
	uint16_t x_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_3"))>>4);
	uint16_t y_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_4"))>>4);

	*settings.steer_loc = pot_pos + (1<<20);

	decode_controller();

	if ((x_acc + y_acc) > 280) {
		printf("in int = %d\r\n", vehicle.performance.impact_thresh);
	}


	if (vehicle.weapon.count == 0) shoot();
	if (vehicle.weapon.type == burst && vehicle.weapon.count == 6000) shoot();
	if (vehicle.weapon.type == burst && vehicle.weapon.count == 12000) shoot();
	if (vehicle.weapon.count < 20000) ++vehicle.weapon.count;
}

void GPIO6_IRQHandler(void) {
	got_shot();
	MSS_GPIO_clear_irq(MSS_GPIO_6);
}
