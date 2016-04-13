#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "headers.h"

typedef struct Settings {
	// Memory location of modules
	uint32_t* steer_loc;
	uint32_t* motor_loc;
	uint32_t* controller_loc;
	uint32_t* gun_loc;
	uint32_t* receiver_loc;

	// Steering offset
	uint8_t steer_offset;
} settings_t;

inline void init_settings();

#endif /* SETTINGS_H_ */
