#ifndef STEERING_H_
#define STEERING_H_

#include "headers.h"

// Steering functions
inline void init_steering();

inline void sample_steer();

inline void start_steer();

inline void kill_steer();

inline void set_steer(int angle);

inline void center_steer();

inline void adjust_center(uint8_t amt);

#endif /* STEERING_H_ */
