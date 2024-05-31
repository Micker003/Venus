#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Method to return the distance reading from the forward TOF sensor 
*/
int forwardDistanceData() {
    
}
/**
 * Method to return distance between ground and robot in order to determine height of block 
*/

int downwardDistanceData() {
  
}
/**
 *  Method to return the color of the block 
*/
int colorSensor() {
    int color = 0; //0 for unknown
    //TODO Embedded color sensor implementation
    return color;
}

struct IRSensors {
    int sensor1Val; 
    int sensor2Val;
    int sensor3Val;
    int sensor4Val;
};

/**
 * measure the values for all three IR sensors
*/
struct IRSensors measureIRData() {
    struct IRSensors IRData;
    IRData.sensor1Val = 100;
    IRData.sensor2Val = 100;
    IRData.sensor3Val = 100;
    IRData.sensor4Val = 100;
    return IRData;
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


/**
 * move the robot forward or backward, takes as input 0 or 1, 0 indicates backwards and 1 indicates forward. 
*/
void moveRobotForwardOrBackward(int direction) {
    //TODO
    if (direction == 0) {
        rotateRobot(2);
        moveRobotForwardOrBackward(1);
    } else {
 
    }
}


