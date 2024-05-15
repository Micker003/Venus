#ifndef VL53L0X_H
#define VL53L0X_H

#include <stdbool.h>
#include <stdint.h>
#include <libpynq.h>

#define VL53L0X_OUT_OF_RANGE (8190)


/**
 * Initialize forward distance sensor
 * Initialize the followings in main():
 *      iic_init(IIC0);
 *      switchbox_set_pin(IO_AR_SDA , SWB_IIC0_SDA);
 *      switchbox_set_pin(IO_AR_SCL , SWB_IIC0_SCL);
*/
bool forward_distance_init();

/**
 * Initialize downward distance sensor
 * Initialize the followings in main():
 *      iic_init(IIC1);
 *      switchbox_set_pin(IO_AR4, SWB_IIC1_SDA);
 *      switchbox_set_pin(IO_AR5, SWB_IIC1_SCL);
*/
bool downward_distance_init();

/**
 * Does a single range measurement
 * @return measured distance in front of robot in millimeters
 *         or VL53L0X_OUT_OF_RANGE if distance is out of range
 *         for distance < 10 mm, distance = 10 mm
 * @note   Polling-based
 */
int forwardDistanceData();

/**
 * Does a single range measurement
 * @return measured distance below robot arm in millimeters
 *         or VL53L0X_OUT_OF_RANGE if distance is out of range
 *         for distance < 10 mm, distance = 10 mm
 * @note   Polling-based
 */
int downwardDistanceData();


#endif /* VL53L0X_H */