#include "controller.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

inline void init_controller() {
	MSS_GPIO_config(MSS_GPIO_10, MSS_GPIO_INPUT_MODE);
}

inline void decode_controller() {
	uint32_t bytemask1 = 0x000000FF;
	uint32_t bytemask2 = 0x0000FF00;
	uint32_t bytemask3 = 0x00FF0000;
	uint32_t bytemask4 = 0xFF000000;

	uint32_t curr_pressed = *settings.controller_loc;
	if (vehicle.controller.pressed_buttons != curr_pressed) {

		// Set steering
		int steer_amount = ((curr_pressed & bytemask3) >> 14) + settings.steer_offset;
		if (steer_amount > 1023) steer_amount = 1023;
		else if (steer_amount < 0) steer_amount = 0;
		set_steer(steer_amount);

		// Set motor
		set_motor(((curr_pressed & bytemask4) >> 24)*vehicle.performance.power_multiplier);

		// Set push buttons
		if ((vehicle.controller.pressed_buttons & bytemask1) != (curr_pressed & bytemask1)) {
			if ((!((vehicle.controller.pressed_buttons >> 7) & 1)) && ((curr_pressed >> 7) & 1) && (settings.steer_offset > -10)) {
				--settings.steer_offset;
			}
			if ((!((vehicle.controller.pressed_buttons >> 5) & 1)) && ((curr_pressed >> 5) & 1) && (settings.steer_offset > -10)) {
				++settings.steer_offset;
			}
		}
		if ((vehicle.controller.pressed_buttons & bytemask2) != (curr_pressed & bytemask2)) {
		}
		vehicle.controller.pressed_buttons = curr_pressed;
	}
}
