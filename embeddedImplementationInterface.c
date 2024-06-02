#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int distancedwd = 60;
int distancefwd = 100;

/**
 * Method to return distance between ground and robot in order to determine height of block 
*/
int dwdDistanceData() {
  return distancedwd; //downward distance is set to always return 60 which would be interpreted by the algorithm as no block being present
}

/**
 * Method to return the distance reading from the forward TOF sensor 
*/
int fwdDistanceData() {
    return distancefwd;
}

struct color {
    int R;
    int G;
    int B;
};

//prototype for colorSensor method
struct color colorSensor(void);

/**
 *  Method to return the color of the block 
*/
struct color colorSensor() {
    struct color color; 
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
            printf("rotate right 90 \n");
        //TODO
        break;
        
        case 1:
        //rotate left by 90 degrees
            printf("rotate left 90 \n");
        //TODO
        break;

        case 2:
        //rotate robot 180 degrees (backwards)
            printf("rotate 180 degrees \n");
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
        printf("move robot forward 5 cm \n");
    }
}


