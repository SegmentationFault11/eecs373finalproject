#include "settings.h"

extern struct Settings settings;

void init_settings() {
	settings.steer_loc = (uint32_t*)(0x40050000);
	settings.motor_loc = (uint32_t*)(0x40050100);
	settings.controller_loc = (uint32_t*)(0x40050200);

	settings.steer_offset = 0;
}
