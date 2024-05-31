#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Method to return the distance reading from the forward TOF sensor 
*/
int forwardDistanceData() {
    return 100; //algorithm testing value, 100 means that no action will be taken by the robot to avoid a certain coordinate
}
/**
 * Method to return distance between ground and robot in order to determine height of block 
*/

int downwardDistanceData() {
  return 60; //downward distance is set to always return 60 which would be interpreted by the algorithm as no block being present
}
/**
 *  Method to return the color of the block 
*/

struct color_value {
    int R;
    int G;
    int B;
};

//prototype for colorSensor method
struct color_value colorSensor(void);

struct color_value colorSensor() {
    struct color_value color; 
    color.R = 0;
    color.G = 0;
    color.B = 0;
    //TODO Embedded color sensor implementation
    return color;   // we return color value of 0 which will be interpreted by the algorithm as no reading on the color sensor
}

struct IRSensors {
    int sensor1Val; 
    int sensor2Val;
    int sensor3Val;
    int sensor4Val;
};


/**
 * measure the values for all four IR sensors
*/
struct IRSensors measureIRData() {
    //all four sensors having a value of above 280 should be interpreted by the algorithm as no hold or boundary being present 
    struct IRSensors IRData;
    IRData.sensor1Val = 280; //above 268 is white 
    IRData.sensor2Val = 280; //below 268 is black 
    IRData.sensor3Val = 280;
    IRData.sensor4Val = 280;
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
            printf("rotate right 90");
        //TODO
        break;
        
        case 1:
        //rotate left by 90 degrees
            printf("rotate left 90");
        //TODO
        break;

        case 2:
        //rotate robot 180 degrees (backwards)
            printf("rotate 180 degrees");
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
        printf("move robot forward 5 cm ");
    }
}


