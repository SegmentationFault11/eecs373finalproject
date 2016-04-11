#ifndef STEERING_H_
#define STEERING_H_

#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_ace/mss_ace.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include "vehicle.h"
#include "settings.h"
#include "motor.h"
#include "controller.h"

// Steering functions
inline void init_steering();

inline void start_steer();

inline void kill_steer();

inline void set_steer(int angle);

inline void center_steer();

inline void adjust_center(uint8_t amt);

void GPIO4_IRQHandler();

#endif /* STEERING_H_ */
