#include "vehicle.h"

extern struct Vehicle vehicle;

void init_vehicle(uint8_t id, vehicleT_t intype, uint8_t team) {
	init_steering();
	init_motor();

	vehicle.id = id;
	vehicle.type = intype;
	vehicle.team = team;

	if (intype == Script || intype == Tank || intype == Toyota || intype == LOL || intype == F1) {
		vehicle.status.HP = 1000;
		vehicle.status.lives = 5;
		vehicle.status.alive = 1;
		vehicle.status.hit_pending = 0;

		vehicle.performance.power_multiplier = 1.f;
		vehicle.performance.steer_multiplier = 1.f;
		vehicle.performance.weight = 1000;

		vehicle.weapon.type = laser;
		vehicle.weapon.damage = 10;
		vehicle.weapon.reload_rate = 0;

	}
}

void tmnt_vehicle() {
	kill_steering();
	kill_motor();
}
