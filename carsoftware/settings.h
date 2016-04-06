#ifndef SETTINGS_H_
#define SETTINGS_H_
#include <inttypes.h>

typedef struct Settings {
    int emergency_int;
    int steer_int;
    int controller_int;
    int xbee_int;
    int fire_int;
    int hit_int;

    uint32_t* steer_loc;
    uint32_t* motor_loc;
    uint32_t* controller_loc;
} Settings_t;

void init_settings();

#endif /* SETTINGS_H_ */
