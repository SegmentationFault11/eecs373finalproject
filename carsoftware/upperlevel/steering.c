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

	// Start steering
	*settings.steer_loc = 1<<21;

	// Center wheels
	*settings.steer_loc = 512;
}

inline void start_steer() {
	*settings.steer_loc = 1<<21;
}

inline void kill_steer() {
	*settings.steer_loc = 1<<22;
}

inline void set_steer(int angle) {
	*settings.steer_loc = (uint32_t)angle;
}

inline void center_steer() {
	*settings.steer_loc = 512 + settings.steer_offset;
}

inline void adjust_center(uint8_t amt) {
	settings.steer_offset += amt;
}

