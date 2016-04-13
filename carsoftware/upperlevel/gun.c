#include "gun.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

inline void shoot(){
	*settings.gun_loc = (2 << 9) | (vehicle.weapon.damage << 4) | (vehicle.id << 2) | 2;
}
