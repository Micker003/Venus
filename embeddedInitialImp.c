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
 * move the robot forward or backward
*/
void moveRobotForwardOrBackward(int distance) {
    //TODO
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