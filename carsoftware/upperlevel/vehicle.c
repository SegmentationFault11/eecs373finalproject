#include "vehicle.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

void init_vehicle(uint8_t id, vehicleT_t intype, uint8_t team) {
	init_steering();
	init_motor();
	init_controller();

	vehicle.id = id;
	vehicle.type = intype;
	vehicle.team = team;

	vehicle.status.gameover = 0;
	vehicle.performance.steer_multiplier = 1.f;
	vehicle.performance.upgrades_available = 0;
	vehicle.weapon.type = single;
	vehicle.weapon.reload_rate = 0;
	vehicle.status.lives = DEFAULT_LIVES;


	void init_base_stats();
}

void init_base_stats() {
	vehicle.killer_id = 69;
	if (vehicle.type == Tank) {
		vehicle.status.HP = TANK_BASE_HEALTH;
		vehicle.performance.power_multiplier = TANK_BASE_POWER;
		vehicle.performance.impact_thresh = TANK_IMPACT_THRESH;
		vehicle.performance.weight = TANK_BASE_WEIGHT;
		vehicle.weapon.damage = TANK_BASE_DAMAGE;
	}
	else if (vehicle.type == F1) {
		vehicle.status.HP = F1_BASE_HEALTH;
		vehicle.performance.power_multiplier = F1_BASE_POWER;
		vehicle.performance.impact_thresh = F1_IMPACT_THRESH;
		vehicle.performance.weight = F1_BASE_WEIGHT;
		vehicle.weapon.damage = F1_BASE_DAMAGE;
	}
	else if (vehicle.type == Toyota) {
		vehicle.status.HP = TOYOTA_BASE_HEALTH;
		vehicle.performance.power_multiplier = TOYOTA_BASE_POWER;
		vehicle.performance.impact_thresh = TOYOTA_IMPACT_THRESH;
		vehicle.performance.weight = TOYOTA_BASE_WEIGHT;
		vehicle.weapon.damage = TOYOTA_BASE_DAMAGE;
	}
	else if (vehicle.type == LOL) {
		vehicle.status.HP = LOL_BASE_HEALTH;
		vehicle.performance.power_multiplier = LOL_BASE_POWER;
		vehicle.performance.impact_thresh = LOL_IMPACT_THRESH;
		vehicle.performance.weight = LOL_BASE_WEIGHT;
		vehicle.weapon.damage = LOL_BASE_DAMAGE;
	}
	else if (vehicle.type == Script) {
		vehicle.status.HP = SCRIPT_BASE_HEALTH;
		vehicle.performance.power_multiplier = SCRIPT_BASE_POWER;
		vehicle.performance.impact_thresh = SCRIPT_IMPACT_THRESH;
		vehicle.performance.weight = SCRIPT_BASE_WEIGHT;
		vehicle.weapon.damage = SCRIPT_BASE_DAMAGE;
	}
}

void receive_upgrade(uint8_t num) {
	if (vehicle.performance.upgrades_available + num > 5) {
		vehicle.performance.upgrades_available = 5;
	}
	else if (vehicle.performance.upgrades_available + num < 0) {
		vehicle.performance.upgrades_available = 0;
	}
	else {
		vehicle.performance.upgrades_available += num;
	}
}

void upgrade_motor() {
	if (vehicle.type == Tank) {
		if (vehicle.performance.power_multiplier < TANK_MAX_POWER)
			vehicle.performance.power_multiplier += TANK_INCR_POWER;
	}
	else if (vehicle.type == F1) {
		if (vehicle.performance.power_multiplier < F1_MAX_POWER)
			vehicle.performance.power_multiplier += F1_INCR_POWER;
	}
	else if (vehicle.type == Toyota) {
		if (vehicle.performance.power_multiplier < TOYOTA_MAX_POWER)
			vehicle.performance.power_multiplier += TOYOTA_INCR_POWER;
	}
	else if (vehicle.type == LOL) {
		if (vehicle.performance.power_multiplier < LOL_MAX_POWER)
			vehicle.performance.power_multiplier += LOL_INCR_POWER;
	}
	else if (vehicle.type == Script) {
		if (vehicle.performance.power_multiplier < SCRIPT_MAX_POWER)
			vehicle.performance.power_multiplier += SCRIPT_INCR_POWER;
	}
}

void upgrade_gun() {
	if (vehicle.weapon.type == single) {
		vehicle.weapon.type = burst;
	}
	else if (vehicle.type == Tank) {
		if (vehicle.weapon.damage < TANK_MAX_DAMAGE)
			vehicle.weapon.damage += TANK_INCR_DAMAGE;
	}
	else if (vehicle.type == F1) {
		if (vehicle.weapon.damage < F1_MAX_DAMAGE)
			vehicle.weapon.damage += F1_INCR_DAMAGE;
	}
	else if (vehicle.type == Toyota) {
		if (vehicle.weapon.damage < TOYOTA_MAX_DAMAGE)
			vehicle.weapon.damage += TOYOTA_INCR_DAMAGE;
	}
	else if (vehicle.type == LOL) {
		if (vehicle.weapon.damage < LOL_MAX_DAMAGE)
			vehicle.weapon.damage += LOL_INCR_DAMAGE;
	}
	else if (vehicle.type == Script) {
		if (vehicle.weapon.damage < SCRIPT_MAX_DAMAGE)
			vehicle.weapon.damage += SCRIPT_INCR_DAMAGE;
	}
}

void restore_health() {
	if (vehicle.type == Tank) {
		vehicle.status.HP = TANK_BASE_HEALTH;
	}
	else if (vehicle.type == F1) {
		vehicle.status.HP = F1_BASE_HEALTH;
	}
	else if (vehicle.type == Toyota) {
		vehicle.status.HP = TOYOTA_BASE_HEALTH;
	}
	else if (vehicle.type == LOL) {
		vehicle.status.HP = LOL_BASE_HEALTH;
	}
	else if (vehicle.type == Script) {
		vehicle.status.HP = SCRIPT_BASE_HEALTH;
	}
}

void revive_vehicle() {
	kill_steer();
	kill_motor();

	// Send message of killer to server

	// Wait a few seconds
	volatile uint32_t i = 0;
	for (i = 0; i < 100000; ++i);

	if (vehicle.status.lives > 0) {
		--vehicle.status.lives;
		start_steer();
		start_motor();
	}
	else {
		vehicle.status.gameover = 1;
	}
}

void tmnt_vehicle() {
	kill_steer();
	kill_motor();
}
