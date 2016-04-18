#include "accelerometer.h"

extern struct Settings settings;
extern struct Vehicle vehicle;

int prev_acc = 0;

inline void sample_acc() {
	uint16_t x_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_3"))>>5);
	uint16_t y_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_4"))>>5);

	int acc = abs(x_acc - settings.center_acc_x) + abs(y_acc - settings.center_acc_y);

	if (prev_acc > acc && acc > 100) {
		uint32_t damage = 10;
		if (damage) take_damage(damage);
	}
	prev_acc = acc;
}

inline void center_acc() {
	uint16_t x_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_3"))>>5);
	uint16_t y_acc = (ACE_get_ppe_sample(ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_4"))>>5);

	settings.center_acc_x = x_acc;
	settings.center_acc_y = y_acc;
}

inline uint32_t get_hypotenuse(uint16_t x, uint16_t y) {
	return (uint32_t)sqrt((x*x)+(y*y));
}
