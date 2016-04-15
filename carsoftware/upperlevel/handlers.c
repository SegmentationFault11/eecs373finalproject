#include "headers.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

void GPIO3_IRQHandler() {

}

void GPIO4_IRQHandler() {
	MSS_GPIO_clear_irq(MSS_GPIO_4);

	sample_steer();

	if (settings.acc_counter >> 3 == 1) {
		sample_acc();
		settings.acc_counter = 0;
	}
	else ++settings.acc_counter;

	decode_controller();

	trigger();
}

void GPIO6_IRQHandler(void) {
	got_shot();
	MSS_GPIO_clear_irq(MSS_GPIO_6);
}
