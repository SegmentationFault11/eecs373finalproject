#include "gun.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

inline void shoot(){
	printf("fire ");
	volatile int count;
	for (count = 0; count < 10; ++count)
		*settings.gun_loc = (2 << 9) | (vehicle.weapon.damage << 4) | (vehicle.id << 2) | 2;
	printf("\r\n");
}
