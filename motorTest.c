#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "embeddedInitialImp.h"
#include "robotAlgorithmBFSTest.h"
#include <limits.h>
#include "esp_2.h"
#include <libpynq.h>
#include <stepper.h>


#define initialx 0
#define initialy 0
#define MAX_SIZE 100



/**
* primary control method calling other mehtods in program 
* @pre nil
* @post 
* @return int   
*/
int main(void) {
   
    
    //initializing stuff
    
    pynq_init();
    stepper_init();
    embeddedInit();
    stepper_enable();

    rotateRobot(0);
    rotateRobot(1);
    rotateRobot(2);
    rotateRobot(2);
    moveRobotForwardOrBackward(1);
    rotateRobot(3);
    rotateRobot(4);
    rotateRobot(4);
    rotateRobot(3);
    moveRobotForwardOrBackward(0);

    pynq_destroy();
    stepper_destroy();
  

    return 0;
}

 