#include "ir_receiver.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

inline void init_receiver() {
	MSS_GPIO_config(MSS_GPIO_6, MSS_GPIO_INPUT_MODE);
	MSS_GPIO_enable_irq(MSS_GPIO_6);
}

inline void got_shot() {
	uint32_t data = *settings.receiver_loc;
	printf("GOT_SHOT: 0x%08x\r\n", data);
	vehicle.last_hit_id = (data & 0b1100) >> 2;
	vehicle.status.HP -= (data & 0b111110000) >> 4;
	printf("HP = %d last_hit_id = %d\r\n", vehicle.status.HP, vehicle.last_hit_id);
}
