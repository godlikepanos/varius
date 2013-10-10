#ifndef GLF_TIME_H
#define GLF_TIME_H

#include <stdint.h>

/**
 * @brief Init the time module
 */
extern void glf_time_init();

/**
 * @brief Get the CPU ticks from a starting point
 * In ms
 */
extern uint32_t glf_get_ticks();

/**
 * @brief Nano sleep
 * In ms
 */
extern void glf_delay(uint32_t ms);

#endif
