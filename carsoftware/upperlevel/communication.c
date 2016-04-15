#include "communication.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

//inline void init_comms() {
//
//}

inline void send_event() {
	unsigned char message[14] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	message[0]  = (unsigned char)0;
	message[1]  = (unsigned char)0;
	message[2]  = (unsigned char)'e';
	message[3]  = (unsigned char)vehicle.id;
	message[4]  = (unsigned char)vehicle.status.HP;
	message[5]  = (unsigned char)vehicle.status.lives;
	message[6]  = (unsigned char)vehicle.last_hit_id;
	message[13] = (unsigned char)255;
}
