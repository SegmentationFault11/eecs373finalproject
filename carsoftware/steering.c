#include "steering.h"

extern struct Settings settings;

// Steering functions
void init_steering() {

	// Configure the GPIO
	MSS_GPIO_config(MSS_GPIO_4, MSS_GPIO_INPUT_MODE);

	// Enable GPIO interrupts
	MSS_GPIO_enable_irq(MSS_GPIO_4);

	// Start steering
	*settings.steer_loc = 1<<21;

	// Center wheels
	*settings.steer_loc = 512;

	// Print completion
	printf("Steering initialized\r\n");
}

void start_steer() {
	*settings.steer_loc = 1<<21;
}

void kill_steer() {
	*settings.steer_loc = 1<<22;
}

void set_steer(int angle) {
	uint32_t des_pos = angle;

	*settings.steer_loc = des_pos;
}


// Interrupt Handling
void sample_int() {
	uint16_t pot_pos = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_2"))>>2);
	uint16_t x_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_3"))>>4);
	uint16_t y_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_4"))>>4);
	uint16_t z_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_5"))>>4);

	//printf("wheel loc = %" PRIu16 "\r\n", pot_pos);

	*settings.steer_loc = pot_pos + (1<<20);
}

void GPIO4_IRQHandler() {
	MSS_GPIO_clear_irq(MSS_GPIO_4);

	sample_int();
}


// Helper functions
void angle2val(uint32_t des_pos, int angle) {
	des_pos = angle;
}

