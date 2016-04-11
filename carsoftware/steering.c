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

}

