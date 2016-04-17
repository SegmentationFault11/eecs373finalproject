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
	//printf("ctl = 0x%08x\r\n", curr_pressed);
	if (vehicle.controller.pressed_buttons != curr_pressed) {

		// Set steering
		set_steer((curr_pressed & bytemask3) >> 14);

		// Set motor
		set_motor((uint8_t)((curr_pressed & bytemask4) >> 24));

		// Set push buttons
		if ((vehicle.controller.pressed_buttons & bytemask1) != (curr_pressed & bytemask1)) {
			// Select
			if (((vehicle.controller.pressed_buttons >> 0) & 1) && !((curr_pressed >> 0) & 1)) {
				printf("SL is pressed\r\n");
			}
			// L3
			if (((vehicle.controller.pressed_buttons >> 1) & 1) && !((curr_pressed >> 1) & 1)) {
				printf("L3 is pressed\r\n");
			}
			// R3
			if (((vehicle.controller.pressed_buttons >> 2) & 1) && !((curr_pressed >> 2) & 1)) {
				printf("R3 is pressed\r\n");
			}
			// Start
			if (((vehicle.controller.pressed_buttons >> 3) & 1) && !((curr_pressed >> 3) & 1)) {
				//init_base_stats();
			}
			// Up
			if (((vehicle.controller.pressed_buttons >> 4) & 1) && !((curr_pressed >> 4) & 1)) {
				start_steer();
				start_motor();
			}
			// Right
			if (((vehicle.controller.pressed_buttons >> 5) & 1) && !((curr_pressed >> 5) & 1) && (settings.steer_offset < 100)) {
				settings.steer_offset += 10;
				center_steer();
			}
			// Down
			if (((vehicle.controller.pressed_buttons >> 6) & 1) && !((curr_pressed >> 6) & 1)) {
				tmnt_vehicle();
			}
			// Left
			if (((vehicle.controller.pressed_buttons >> 7) & 1) && !((curr_pressed >> 7) & 1) && (settings.steer_offset > -100)) {
				settings.steer_offset -= 10;
				center_steer();
			}

		}
		if ((vehicle.controller.pressed_buttons & bytemask2) != (curr_pressed & bytemask2)) {
			// L1
			if (((vehicle.controller.pressed_buttons >> 8) & 1) && !((curr_pressed >> 8) & 1)) {
				printf("L1 is pressed\r\n");
			}
			// R1
			if (((vehicle.controller.pressed_buttons >> 9) & 1) && !((curr_pressed >> 9) & 1)) {
				vehicle.weapon.count = 0;
			}
			// L2
			if (((vehicle.controller.pressed_buttons >> 10) & 1) && !((curr_pressed >> 10) & 1)) {
				printf("L2 is pressed\r\n");
			}
			// R2
			if (((vehicle.controller.pressed_buttons >> 11) & 1) && !((curr_pressed >> 11) & 1)) {
				printf("R2 is pressed\r\n");
			}
			// Triangle
			if (((vehicle.controller.pressed_buttons >> 12) & 1) && !((curr_pressed >> 12) & 1)) {
				restore_health();
			}
			// Circle
			if (((vehicle.controller.pressed_buttons >> 13) & 1) && !((curr_pressed >> 13) & 1)) {
				upgrade_gun();
			}
			// Cross
			if (((vehicle.controller.pressed_buttons >> 14) & 1) && !((curr_pressed >> 14) & 1)) {
				upgrade_motor();
			}
			// Square
			if (((vehicle.controller.pressed_buttons >> 15) & 1) && !((curr_pressed >> 15) & 1)) {
				//receive_upgrade(1);
			}
		}
		vehicle.controller.pressed_buttons = curr_pressed;
	}
}
