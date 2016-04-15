#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "headers.h"

inline void sample_acc();

inline void center_acc();

// Helper
inline uint32_t get_hypotenuse(uint16_t x, uint16_t y);

#endif /* ACCELEROMETER_H_ */
