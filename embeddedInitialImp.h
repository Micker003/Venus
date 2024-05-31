
#include <stdbool.h>
#include <stdint.h>
#include <libpynq.h>

struct IRSensors {
    int sensor1Val; 
    int sensor2Val;
    int sensor3Val;
    int sensor4Val;
};

struct color {
    int red; 
    int green;
    int blue;
};


//Initialize all sensors before usage (MUST HAVE)
bool embeddedInit();

/**
 * Method to return the distance reading from the forward TOF sensor 
*/
int forwardDistanceData();

/**
 * Method to return distance between ground and robot in order to determine height of block 
*/
int downwardDistanceData();

/**
 *  Method to return the color of the block 
*/
struct color colorSensor();

/**
 * measure the values for all three IR sensors
*/
struct IRSensors measureIRData();

/**
 * move the robot forward or backward, takes as input 0 or 1, 0 indicates backwards and 1 indicates forward. 
*/
void moveRobotForwardOrBackward(int direction);

/**
 * Change the direction of the robot
*/
void rotateRobot(int direction);

