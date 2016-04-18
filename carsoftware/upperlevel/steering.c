#include "steering.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

// Steering functions
void init_steering() {

	// Configure the GPIO
	MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(MSS_GPIO_4, MSS_GPIO_INPUT_MODE);

	// Enable GPIO interrupts
	MSS_GPIO_enable_irq(MSS_GPIO_4);
}

inline void sample_steer() {
	uint16_t pot_pos = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_2"))>>2);
	*settings.steer_loc = (pot_pos & 0b1111111111) + (1<<20);
}

inline void start_steer() {
	*settings.steer_loc = 1<<22;
	*settings.steer_loc = 512;
}

inline void kill_steer() {
	*settings.steer_loc = 1<<23;
}

inline void set_steer(int steer_amount) {
	steer_amount += settings.steer_offset;
	if (steer_amount > 1023) steer_amount = 1023;
	else if (steer_amount < 0) steer_amount = 0;
	*settings.steer_loc = (uint32_t)steer_amount;
}

inline void center_steer() {
	*settings.steer_loc = 512 + settings.steer_offset;
}

inline void adjust_center(uint8_t amt) {
	settings.steer_offset += amt;
}

