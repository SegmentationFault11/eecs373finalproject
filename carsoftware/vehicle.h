#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <inttypes.h>

typedef enum {Tank, F1, Toyota, LOL, Script} vehicleT_t;
typedef enum {single, burst, full_auto, laser} gunT_t;

typedef struct Status {
	uint32_t HP;
	uint16_t lives;
	uint8_t alive;
	uint8_t hit_pending;
} status_t;

typedef struct Weapon {
	gunT_t type;
	uint32_t damage;
	uint32_t reload_rate;
} weapon_t;

typedef struct Performance {
	float power_multiplier;
	float steer_multiplier;
	uint32_t weight;
} performance_t;

typedef struct Controller {
	uint8_t connection;
} controller_t;


typedef struct Vehicle {
	uint8_t id;
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

void tmnt_vehicle();


#endif /* VEHICLE_H_ */
