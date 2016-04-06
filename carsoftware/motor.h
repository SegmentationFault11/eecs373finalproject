#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdio.h>
#include <inttypes.h>
#include "settings.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "drivers/mss_ace/mss_ace.h"


// Steering functions
void init_motor();

void start_motor();

void kill_motor();

void set_motor(int speed);


#endif /* MOTOR_H_ */
