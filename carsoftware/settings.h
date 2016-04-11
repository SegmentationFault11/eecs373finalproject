#ifndef SETTINGS_H_
#define SETTINGS_H_
#include <inttypes.h>

typedef struct Settings {
	// Memory location of modules
	uint32_t* steer_loc;
	uint32_t* motor_loc;
	uint32_t* controller_loc;

	// Steering offset
	uint8_t steer_offset;
} settings_t;

void init_settings();

#endif /* SETTINGS_H_ */
