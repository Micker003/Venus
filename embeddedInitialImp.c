#include <stdio.h>
#include <stdlib.h>
/**
 * Method to return the distance reading from the forward ultrasonic sensor 
*/
int forwardDistanceData() {
    //TODO
}

/**
 * Method to return distance between ground and robot in order to determine height of block 
*/
int downwardDistanceData() {
    //TODO
}

/**
 *  Method to return the color of the block 
*/
char colorSensor() {
    //TODO
}

//struct IR sensor storing the color for all three IR sensors
struct IRSensors {
    int sensor1Val; 
    int sensor2Val;
    int sensor3Val;
};

/**
 * measure the values for all three IR sensors
*/
struct IRSensors measureIRData() {
    //TODO
}

/**
 * move the robot forward or backward, takes as input 0 or 1, 0 indicates backwards and 1 indicates forward. 
*/
void moveRobotForwardOrBackward(int direction) {
    //TODO
    if (direction = 0) {
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