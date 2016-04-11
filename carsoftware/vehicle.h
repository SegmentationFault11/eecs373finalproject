#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <inttypes.h>
#include "drivers/mss_ace/mss_ace.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"


#include "steering.h"
#include "settings.h"
#include "motor.h"
#include "controller.h"
#include "performance.h"

typedef enum {Tank, F1, Toyota, LOL, Script} vehicleT_t;
typedef enum {single, burst, full_auto, laser} gunT_t;

typedef struct Status {
	uint8_t HP;
	uint8_t lives;
	uint8_t gameover;
	uint8_t hit_pending;
} status_t;

typedef struct Weapon {
	gunT_t type;
	uint8_t damage;
	uint8_t reload_rate;
} weapon_t;

typedef struct Performance {
	float power_multiplier;
	float steer_multiplier;
	uint32_t impact_thresh;
	uint8_t weight;
	uint8_t upgrades_available;
} performance_t;

typedef struct Controller {
	uint32_t pressed_buttons;
} controller_t;


typedef struct Vehicle {
	uint8_t id;
	uint8_t killer_id;
	char name[50];
	uint8_t team;

	vehicleT_t type;
	status_t status;
	performance_t performance;
	weapon_t weapon;
	controller_t controller;
} vehicle_t;



// Member functions
void init_vehicle(uint8_t id, vehicleT_t intype, uint8_t team);

void init_base_stats();

void receive_upgrade(uint8_t num);

void upgrade_motor();

void upgrade_gun();

void restore_health();

void revive_vehicle();

void tmnt_vehicle();


#endif /* VEHICLE_H_ */
