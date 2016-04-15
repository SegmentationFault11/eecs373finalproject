#include "settings.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

inline void init_settings() {
	settings.steer_loc = (uint32_t*)(0x40050000);
	settings.motor_loc = (uint32_t*)(0x40050100);
	settings.controller_loc = (uint32_t*)(0x40050200);
	settings.receiver_loc = (uint32_t*)(0x40050300);
	settings.gun_loc = (uint32_t*) (0x40050400);

	settings.center_acc = 280;

	settings.steer_offset = 0;

	settings.acc_counter = 0;
}
