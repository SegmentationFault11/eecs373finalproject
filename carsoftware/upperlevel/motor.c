#include "motor.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

void start_motor() {
	*settings.motor_loc = 1<<21;
	*settings.motor_loc = 128;
}

void kill_motor() {
	*settings.motor_loc = 1<<22;
	*settings.motor_loc = 128;
}

void set_motor(uint8_t speed) {
	*settings.motor_loc = (uint8_t)(speed*vehicle.performance.power_multiplier + (1-vehicle.performance.power_multiplier)*128);
}
