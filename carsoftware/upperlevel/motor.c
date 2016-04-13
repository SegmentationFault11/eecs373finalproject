#include "motor.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

void init_motor() {
	// Start motor
	*settings.motor_loc = 1<<21;

	// Set stationary
	*settings.motor_loc = 128;
}

void start_motor() {
	*settings.motor_loc = 1<<21;
}

void kill_motor() {
	*settings.motor_loc = 1<<22;
}

void set_motor(int speed) {
	*settings.motor_loc = speed*vehicle.performance.power_multiplier;
}
