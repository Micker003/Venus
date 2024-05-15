#include <libpynq.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include "distance.h"

#define REG_IDENTIFICATION_MODEL_ID (0xC0)
#define REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV (0x89)
#define REG_MSRC_CONFIG_CONTROL (0x60)
#define REG_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT (0x44)
#define REG_SYSTEM_SEQUENCE_CONFIG (0x01)
#define REG_DYNAMIC_SPAD_REF_EN_START_OFFSET (0x4F)
#define REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD (0x4E)
#define REG_GLOBAL_CONFIG_REF_EN_START_SELECT (0xB6)
#define REG_SYSTEM_INTERRUPT_CONFIG_GPIO (0x0A)
#define REG_GPIO_HV_MUX_ACTIVE_HIGH (0x84)
#define REG_SYSTEM_INTERRUPT_CLEAR (0x0B)
#define REG_RESULT_INTERRUPT_STATUS (0x13)
#define REG_SYSRANGE_START (0x00)
#define REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0 (0xB0)
#define REG_RESULT_RANGE_STATUS (0x14)

#define VL53L0X_EXPECTED_DEVICE_ID (0xEE)

#define RANGE_SEQUENCE_STEP_TCC (0x10) /* Target CentreCheck */
#define RANGE_SEQUENCE_STEP_MSRC (0x04) /* Minimum Signal Rate Check */
#define RANGE_SEQUENCE_STEP_DSS (0x28) /* Dynamic SPAD selection */
#define RANGE_SEQUENCE_STEP_PRE_RANGE (0x40)
#define RANGE_SEQUENCE_STEP_FINAL_RANGE (0x80)
#define IIC_DEVICE_ADDRESS (0x29)

#define VL53L0X_OUT_OF_RANGE (8190)

uint8_t zero = 0x00;
uint8_t one = 0x01;
uint8_t four = 0x04;
uint8_t FF = 0xFF;
uint8_t x2C = 0x2C;
uint8_t x20 = 0x20;
uint8_t x09 = 0x09;
uint8_t x03 = 0x03;
uint8_t x83 = 0x83;
uint8_t x25 = 0x25;
uint8_t x06 = 0x06;
uint8_t x96 = 0x96;
uint8_t x08 = 0x08;
uint8_t x30 = 0x30;
uint8_t x0A = 0x0A;
uint8_t xA0 = 0xA0;
uint8_t x32 = 0x32;
uint8_t x14 = 0x14;
uint8_t x21 = 0x21;
uint8_t x26 = 0x26;
uint8_t x05 = 0x05;
uint8_t x40 = 0x40;
uint8_t xF8 = 0xF8;
uint8_t xFE = 0xFE;
uint8_t x28 = 0x28;
uint8_t x44 = 0x44;
uint8_t x34 = 0x34;
uint8_t x1A = 0x1A;

bool device_is_booted(const iic_index_t iic){
    uint8_t device_id = 0;
     
    if (!iic_read_register	(iic, IIC_DEVICE_ADDRESS, REG_IDENTIFICATION_MODEL_ID, &device_id, 1)) {
        printf("Device ID: %d\n",device_id);
        return true;
    }
    return device_id == VL53L0X_EXPECTED_DEVICE_ID;
}

static uint8_t stop_variable = 0;

/**
 * One time device initialization
 */
static bool data_init(const iic_index_t iic){
    bool success = false;

    /* Set 2v8 mode */
    uint8_t vhv_config_scl_sda = 0;
    if (iic_read_register(iic, IIC_DEVICE_ADDRESS, REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, &vhv_config_scl_sda,1)) {
        return false;
    }
    vhv_config_scl_sda |= 0x01;
    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, &vhv_config_scl_sda,1)) {
        return false;
    }
  

    /* Set I2C standard mode */
    success = !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x88, &zero,1);

    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS,0x80, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS,0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS,0x00, &zero,1);
    success &= !iic_read_register(iic, IIC_DEVICE_ADDRESS, 0x91, &stop_variable,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS,0x00, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS,0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS,0x80, &zero,1);

    return success;
}


static bool load_default_tuning_settings(const iic_index_t iic)
{
    bool success = !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x00, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x09, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x10, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x11, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x24, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x25, &FF,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x75, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x4E, &x2C,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x48, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x30, &x20,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x30, &x09,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x54, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x31, &four,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x32, &x03,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x40, &x83,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x46, &x25,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x60, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x27, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x50, &x06,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x51, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x52, &x96,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x56, &x08,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x57, &x30,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x61, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x62, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x64, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x65, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x66, &xA0,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x22, &x32,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x47, &x14,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x49, &FF,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x4A, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x7A, &x0A,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x7B, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x78, &x21,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x23, &x34,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x42, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x44, &FF,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x45, &x26,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x46, &x05,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x40, &x40,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x0E, &x06,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x20, &x1A,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x43, &x40,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x34, &x03,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x35, &x44,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x31, &four,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x4B, &x09,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x4C, &x05,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x4D, &four,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x44, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x45, &x20,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x47, &x08,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x48, &x28,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x67, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x70, &four,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x71, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x72, &xFE,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x76, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x77, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x0D, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x80, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x01, &xF8,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x8E, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x00, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x80, &zero,1);
    return success;
}

static bool configure_interrupt(const iic_index_t iic){
    /* Interrupt on new sample ready */

    if (iic_write_register(iic, IIC_DEVICE_ADDRESS,REG_SYSTEM_INTERRUPT_CONFIG_GPIO, &four, 1)) {
        return false;
    }

    /* Configure active low since the pin is pulled-up on most breakout boards */
    uint8_t gpio_hv_mux_active_high = 0;
    if (iic_read_register(iic, IIC_DEVICE_ADDRESS,REG_GPIO_HV_MUX_ACTIVE_HIGH, &gpio_hv_mux_active_high,1)) {
        return false;
    }
    gpio_hv_mux_active_high &= ~0x10;
    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_GPIO_HV_MUX_ACTIVE_HIGH, &gpio_hv_mux_active_high,1)) {
        return false;
    }

    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_SYSTEM_INTERRUPT_CLEAR, &one, 1)) {
        return false;
    }
    return true;
}

static bool set_sequence_steps_enabled(uint8_t sequence_step, const iic_index_t iic){
    return !iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_SYSTEM_SEQUENCE_CONFIG, &sequence_step,1);
}


static bool static_init(const iic_index_t iic){
    if (!load_default_tuning_settings(iic)) {
        return false;
    }

    if (!configure_interrupt(iic)) {
        return false;
    }

    if (!set_sequence_steps_enabled(RANGE_SEQUENCE_STEP_DSS + RANGE_SEQUENCE_STEP_PRE_RANGE + RANGE_SEQUENCE_STEP_FINAL_RANGE,iic)) {
        return false;
    }

    return true;
}


typedef enum
{
    CALIBRATION_TYPE_VHV,
    CALIBRATION_TYPE_PHASE
} calibration_type_t;

static bool perform_single_ref_calibration(calibration_type_t calib_type, const iic_index_t iic)
{
    uint8_t sysrange_start = 0;
    uint8_t sequence_config = 0;
    switch (calib_type)
    {
    case CALIBRATION_TYPE_VHV:
        sequence_config = 0x01;
        sysrange_start = 0x01 | 0x40;
        break;
    case CALIBRATION_TYPE_PHASE:
        sequence_config = 0x02;
        sysrange_start = 0x01 | 0x00;
        break;
    }
    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_SYSTEM_SEQUENCE_CONFIG, &sequence_config,1)) {
        return false;
    }
    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_SYSRANGE_START, &sysrange_start,1)) {
        return false;
    }
    /* Wait for interrupt */
    uint8_t interrupt_status = 0;
    bool success = false;
    do {
        success = !iic_read_register(iic, IIC_DEVICE_ADDRESS, REG_RESULT_INTERRUPT_STATUS, &interrupt_status,1);
    } while (success && ((interrupt_status & 0x07) == 0));
    if (!success) {
        return false;
    }
    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_SYSTEM_INTERRUPT_CLEAR, &one, 1)) {
        return false;
    }

    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_SYSRANGE_START, &zero, 1)) {
        return false;
    }
    return true;
}

/**
 * Temperature calibration needs to be run again if the temperature changes by
 * more than 8 degrees according to the datasheet.
 */
static bool perform_ref_calibration(const iic_index_t iic)
{
    if (!perform_single_ref_calibration(CALIBRATION_TYPE_VHV,iic)) {
        return false;
    }
    if (!perform_single_ref_calibration(CALIBRATION_TYPE_PHASE,iic)) {
        return false;
    }
    /* Restore sequence steps enabled */
    if (!set_sequence_steps_enabled(RANGE_SEQUENCE_STEP_DSS +
                                    RANGE_SEQUENCE_STEP_PRE_RANGE +
                                    RANGE_SEQUENCE_STEP_FINAL_RANGE,iic)) {
        return false;
    }
    return true;
}

bool vl53l0x_init(const iic_index_t iic)
{
    if (!device_is_booted(iic)) {
        return false;
    }
    if (!data_init(iic)) {
        return false;
    }
    if (!static_init(iic)) {
        return false;
    }
    if (!perform_ref_calibration(iic)) {
        return false;
    }
    return true;
}

bool forward_distance_init(){
    return vl53l0x_init(IIC0);
}

bool downward_distance_init(){
    return vl53l0x_init(IIC1);
}

bool vl53l0x_read_range_single(uint16_t *range, const iic_index_t iic)
{
    bool success = !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x80, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x00, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x91, &stop_variable,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x00, &one,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0xFF, &zero,1);
    success &= !iic_write_register(iic, IIC_DEVICE_ADDRESS, 0x80, &zero,1);


    if (!success) {
        return false;
    }
 //    printf("1st done\n");


    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_SYSRANGE_START, &one, 1)) {
        return false;
    }
//    printf("2nd done\n");
    
    uint8_t sysrange_start;
    do {
        success = !iic_read_register(iic, IIC_DEVICE_ADDRESS, REG_SYSRANGE_START, &sysrange_start,1);

    } while (success && (sysrange_start & 0x01));

//     printf("3rd done\n");

    if (!success) {
        return false;
    }

//    printf("4th done\n");

    uint8_t interrupt_status = 0;

    do {
        sleep_msec(200);
        success = !iic_read_register(iic, IIC_DEVICE_ADDRESS, REG_RESULT_INTERRUPT_STATUS, &interrupt_status,1);
       
    //Use code below to investigate error if any
        // printf("Interrupt status: %d\n", interrupt_status);
        // if (success){
        //     printf("Success\n");
        // } else{
        //     printf("Failed\n");
        // }
    } while (success && ((interrupt_status & 0x07) == 0));

//    printf("5th done\n");

    if (!success) {
        //return false;
    }
  
//     printf("6th done\n");

    uint8_t tempRange[2];
    if (iic_read_register(iic, IIC_DEVICE_ADDRESS, REG_RESULT_RANGE_STATUS + 10, tempRange,2)) {
        return false;
    }
//    printf("7th done\n");
    if (iic_write_register(iic, IIC_DEVICE_ADDRESS, REG_SYSTEM_INTERRUPT_CLEAR, &one, 1)) {
        return false;
    }

    *range =  (unsigned short)((tempRange[0]<<8) + tempRange[1] - 37);
    /* 8190 or 8191 may be returned when obstacle is out of range. */
    if (*range == 8190 || *range == 8191) {
        *range = VL53L0X_OUT_OF_RANGE;
    }

    return true;
}


int forwardDistanceData(){
    uint16_t range = 0;
    vl53l0x_read_range_single(*range,IIC0);
    return range;
}

int downwardDistanceData(){
    uint16_t range = 0;
    vl53l0x_read_range_single(*range,IIC1);
    return range;
}

