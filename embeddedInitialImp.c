#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "distance.h"
#include "IR_sensor.h"
#include "embeddedInitialImp.h"

//Method to initialize all sensors before usage
bool embeddedInit(){
    
    IR_init();

    switchbox_set_pin(IO_AR4, SWB_IIC0_SCL);
    switchbox_set_pin(IO_AR5 , SWB_IIC0_SDA);
    switchbox_set_pin(IO_AR_SCL , SWB_IIC1_SCL);
    switchbox_set_pin(IO_AR_SDA , SWB_IIC1_SDA);

    iic_init(IIC1);
    iic_reset(IIC1);
    iic_init(IIC0);
    iic_reset(IIC0);

    if (!forward_distance_init()){
        return false;
    }

    if (!downward_distance_init()){
        return false;
    }
    return true;
}


/**
 * Method to return the distance reading from the forward TOF sensor 
*/
int forwardDistanceData() {
    uint16_t range = 0;
    vl53l0x_read_range_single(&range,IIC0);
    return range;
}

/**
 * Method to return distance between ground and robot in order to determine height of block 
*/
int downwardDistanceData() {
    uint16_t range = 0;
    vl53l0x_read_range_single(&range,IIC1);
    return range;
}

/**
 *  Method to return the color of the block 
*/
char colorSensor() {
    char color = 'u'; //u for unknown
    return color;
}

/**
 * measure the values for all three IR sensors
*/
struct IRSensors measureIRData() {
    struct IRSensors IRData;
    IRData.sensor1Val = IR_read(ADC5);
    IRData.sensor2Val = IR_read(ADC4);
    IRData.sensor3Val = IR_read(ADC3);
    IRData.sensor4Val = IR_read(ADC2);
    return IRData;
}

/**
 * move the robot forward or backward, takes as input 0 or 1, 0 indicates backwards and 1 indicates forward. 
*/
void moveRobotForwardOrBackward(int direction) {
    //TODO
    if (direction == 0) {
        rotateRobot(2);
        moveRobotForwardOrBackward(1);
    } else {
        //add code here to rotate the robot a certain distance to move the robot forwards.
    }
}

/**
 * Change the direction of the robot
*/
void rotateRobot(int direction) {
    int d = direction; 
    switch(d) {
        case 0:
        //rotate right by 90 degrees
        //TODO
        break;
        
        case 1:
        //rotate left by 90 degrees
        //TODO
        break;

        case 2:
        //rotate robot 180 degrees (backwards)
        //TODO
        break;
    }
}