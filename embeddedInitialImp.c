#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "distance.h"
#include "IR_sensor.h"
#include "color.h"
#include "embeddedInitialImp.h"



int multipleInit(vl53x *sensorA, vl53x *sensorB){
	switchbox_set_pin(IO_AR_SCL, SWB_IIC0_SCL);
	switchbox_set_pin(IO_AR_SDA, SWB_IIC0_SDA);
	iic_init(IIC0);
		int i;
	//Setup Sensor A
	printf("Initialising Sensor A:\n");

	//Change the Address of the VL53L0X
	uint8_t addrA = 0x69;
	i = tofSetAddress(IIC0, 0x29, addrA);
	printf("---Address Change: ");
	if(i != 0)
	{
		printf("Fail\n");
		return 1;
	}
	printf("Succes\n");
	
	i = tofPing(IIC0, addrA);
	printf("---Sensor Ping: ");
	if(i != 0)
	{
		printf("Fail\n");
		return 1;
	}
	printf("Succes\n");

	//Create a sensor struct
	//vl53x sensorA;

	//Initialize the sensor

	i = tofInit(sensorA, IIC0, addrA, 0);
	if (i != 0)
	{
		printf("---Init: Fail\n");
		return 1;
	}

	uint8_t model, revision;

	tofGetModel(sensorA, &model, &revision);
	printf("---Model ID - %d\n", model);
	printf("---Revision ID - %d\n", revision);
	printf("---Init: Succes\n");
	fflush(NULL);

	printf("\n\nNow Power Sensor B!!\nPress \"Enter\" to continue...\n");
	getchar();

	//Setup Sensor B
	printf("Initialising Sensor B:\n");

	//Use the base addr of 0x29 for sensor B
	//It no longer conflicts with sensor A.
	uint8_t addrB = 0x29;	
	i = tofPing(IIC0, addrB);
	printf("---Sensor Ping: ");
	if(i != 0)
	{
		printf("Fail\n");
		return 1;
	}
	printf("Succes\n");


	//Initialize the sensor

	i = tofInit(sensorB, IIC0, addrB, 0);
	if (i != 0)
	{
		printf("---Init: Fail\n");
		return 1;
	}

	tofGetModel(sensorB, &model, &revision);
	printf("---Model ID - %d\n", model);
	printf("---Revision ID - %d\n", revision);
	printf("---Init: Succes\n");
	fflush(NULL); //Get some output even if the distance readings hang
	printf("\n");
	return EXIT_SUCCESS;
}

vl53x sensorA; 
vl53x sensorB;
tcs3472 sensor;

//Method to initialize all sensors before usage
bool embeddedInit(){
	int integration_time_ms = 60;
	multipleInit(&sensorA, &sensorB);
	sensor = color_init(integration_time_ms, IO_AR9, IO_AR8);
    IR_init();
    return true;
}


/**
 * Method to return the distance reading from the forward TOF sensor 
*/
int forwardDistanceData() {
    uint32_t iDistance;
	iDistance = tofReadDistance(&sensorB);
    printf("Distance Forward: %d\n", iDistance);
    return iDistance;
}
/**
 * Method to return distance between ground and robot in order to determine height of block 
*/

int downwardDistanceData() {
    uint32_t iDistance;
	iDistance = tofReadDistance(&sensorA);
    printf("Distance Forward: %d\n", iDistance);
    return iDistance;
}
/**
 *  Method to return the color of the block 
*/
struct color colorSensor() {
	int integration_time_ms = 60;
	tcsReading color_reading = color_read(sensor, integration_time_ms);
    struct color color;
	color.red = color_reading.red; 
	color.green = color_reading.green; 
	color.blue = color_reading.blue; 
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