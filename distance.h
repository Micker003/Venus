#ifndef VL53L0X_H
#define VL53L0X_H

#include <stdbool.h>
#include <stdint.h>
#include <libpynq.h>

#define VL53L0X_OUT_OF_RANGE (8190)

bool vl53l0x_init(const iic_index_t iic);

/**
 * Does a single range measurement
 * @param range contains the measured range or VL53L0X_OUT_OF_RANGE
 *        if out of range.
 * @return True if success, False if error
 * @note   Polling-based
 */
bool vl53l0x_read_range_single(uint16_t *range, const iic_index_t iic);

#endif /* VL53L0X_H */