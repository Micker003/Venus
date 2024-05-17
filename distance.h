#ifndef VL53L0X_H
#define VL53L0X_H

#include <stdbool.h>
#include <stdint.h>
#include <libpynq.h>

#define VL53L0X_OUT_OF_RANGE (8190)


/**
 * Initialize forward distance sensor
 * Initialize the followings before usage:
 *      iic_init(IIC0);
 *      switchbox_set_pin(IO_AR_SDA , SWB_IIC0_SDA);
 *      switchbox_set_pin(IO_AR_SCL , SWB_IIC0_SCL);
*/
bool forward_distance_init();

/**
 * Initialize downward distance sensor
 * Initialize the followings before usage:
 *      iic_init(IIC1);
 *      switchbox_set_pin(IO_AR4, SWB_IIC1_SDA);
 *      switchbox_set_pin(IO_AR5, SWB_IIC1_SCL);
*/
bool downward_distance_init();

/**
 * Does a single range measurement
 * @param iic selects specific iic channel
 * @param range contains the measured range or VL53L0X_OUT_OF_RANGE
 *        if out of range.
 * @return True if success, False if error
 * @note   Polling-based
 */
bool vl53l0x_read_range_single(uint16_t *range, const iic_index_t iic);

#endif /* VL53L0X_H */