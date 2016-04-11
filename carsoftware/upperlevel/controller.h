#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <inttypes.h>

#include "vehicle.h"
#include "steering.h"
#include "settings.h"
#include "motor.h"

#include "drivers/mss_ace/mss_ace.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

void init_controller();

void decode_controller();

#endif /* CONTROLLER_H_ */
