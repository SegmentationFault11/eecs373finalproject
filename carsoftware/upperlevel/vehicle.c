#include "vehicle.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

inline void init_vehicle(uint8_t id, vehicleT_t intype, uint8_t team) {
	vehicle.id = id;
	vehicle.type = intype;
	vehicle.team = team;
	vehicle.last_hit_id = 255;

	vehicle.status.gameover = 0;
	vehicle.performance.steer_multiplier = 1.f;
	vehicle.performance.upgrades_available = 0;
	vehicle.weapon.reload_rate = 0;
	vehicle.status.lives = DEFAULT_LIVES;
	vehicle.weapon.count = 1000000;

	init_base_stats();
	settings.type_set = 1;
	printf("init called\r\n");

	center_acc();
	init_controller();
	init_receiver();
	init_steering();
}

inline void init_base_stats() {
	vehicle.weapon.count = 1000000;
	vehicle.weapon.type = single;
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

inline void receive_upgrade(uint8_t num) {
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

inline void upgrade_motor() {
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

inline void upgrade_gun() {
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

inline void restore_health() {
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

inline void take_damage(uint32_t damage) {
	if (vehicle.status.HP < damage) vehicle.status.HP = 0;
	else vehicle.status.HP -= damage;
	printf("took %d damage from %d health %d\r\n", damage, vehicle.last_hit_id, vehicle.status.HP);
	send_event();
	if (vehicle.status.HP == 0) deathed();
}

inline void deathed() {
	tmnt_vehicle();

	// Wait a few seconds
	volatile uint32_t i = 0;
	for (i = 0; i < 100000000; ++i);

	start_steer();
	start_motor();

	init_base_stats();

//	if (vehicle.status.lives > 0) {
//		--vehicle.status.lives;
//		start_steer();
//		start_motor();
//		init_base_stats();
//	}
//	else {
//		vehicle.status.gameover = 1;
//	}
}

inline void tmnt_vehicle() {
	kill_steer();
	kill_motor();
}
