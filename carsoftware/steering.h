#ifndef STEERING_H_
#define STEERING_H_

#include <stdio.h>
#include <inttypes.h>
#include "settings.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "drivers/mss_ace/mss_ace.h"


// Steering functions
void init_steering();

void start_steer();

void kill_steer();

void set_steer(int angle);

// Interrupt Handling
void sample_int();

void GPIO1_IRQHandler();

// Helper functions
void angle2val(uint32_t des_pos, int angle);

#endif /* STEERING_H_ */
