#include <libpynq.h>
#include "distance.h"
#include "IR_sensor.h"
#include "embeddedInitialImp.h"
#include <stepper.h>
#define THRESHOLD 263

/**
 * Method to detect Cliff or Boundary  
*/
char detectCliffBoundary(struct IRSensors IR_data ){
    int IR_1 = IR_data.sensor1Val;
        int IR_2 = IR_data.sensor2Val;
        int IR_3 = IR_data.sensor3Val;
        int IR_4 = IR_data.sensor4Val;


        printf("IR 1 value: %d ",IR_1 );
        if (IR_1 < THRESHOLD){
            printf("Black\n");
        } else{
            printf("White\n");
        }

        printf("IR 2 value: %d ",IR_2);  
        if (IR_2 < THRESHOLD){
            printf("Black\n");
        } else{
            printf("White\n");
        }

        printf("IR 3 value: %d ",IR_3);   
        if (IR_3 < THRESHOLD){
            printf("Black\n");
        } else{
            printf("White\n");
        }

        printf("IR 4 value: %d ",IR_4);  
        if (IR_4 < THRESHOLD){
            printf("Black\n");
        } else{
            printf("White\n");
        }

        if ((IR_1 < THRESHOLD && IR_2 < THRESHOLD && IR_3 < THRESHOLD)||(IR_1 < THRESHOLD && IR_3 < THRESHOLD && IR_4 < THRESHOLD) ){
            printf("Cliff\n");
            return 'c';
        } else if (IR_1 < THRESHOLD || IR_2 < THRESHOLD || IR_3 < THRESHOLD || IR_4 < THRESHOLD){
            printf("Boundary\n");
            return 'b';
        } else{
            printf("Ground\n");
            return 'g';
        }
}

/**
 * Program to move robot and stop when rock sample is detected
 *            detect liffs and boundaries  
*/



int main(){
    pynq_init();
    embeddedInit();
    stepper_init();
    stepper_enable();
    stepper_set_speed(65535,65535);
    
    while (1){
        stepper_set_speed(50000,50000);

        detectCliffBoundary(measureIRData());
        if(downwardDistanceData() > 100 && forwardDistanceData() > 30){
            stepper_set_speed(50000,50000);
            stepper_steps(-100,-100);
            sleep_msec(1000);
        }
        else if (downwardDistanceData()>70){
            printf("3x3 Box detected\n");
        } else if (forwardDistanceData() <= 30){
            printf("Hill detected\n");
        } else {
            printf("6x6 Box detected\n");
        }

        detectCliffBoundary(measureIRData());
        if(downwardDistanceData()>100 && forwardDistanceData() > 30){
            stepper_set_speed(65533,65533);
            stepper_steps(-50,50);
            sleep_msec(1000);
        }
        else if (downwardDistanceData()>70){
            printf("3x3 Box detected\n");
        } else if (forwardDistanceData() <= 30){
            printf("Hill detected\n");
        } else {
            printf("6x6 Box detected\n");
        }

        detectCliffBoundary(measureIRData());
        if(downwardDistanceData()>100 && forwardDistanceData() > 30){
            stepper_steps(100,-100);
            sleep_msec(1000);
        }
        else if (downwardDistanceData()>70){
            printf("3x3 Box detected\n");
        } else if (forwardDistanceData() <= 30){
            printf("Hill detected\n");
        } else {
            printf("6x6 Box detected\n");
        }

        detectCliffBoundary(measureIRData());
        if(downwardDistanceData()>100 && forwardDistanceData() > 30){
            stepper_steps(-50,50);
            sleep_msec(1000);
        }
        else if (downwardDistanceData()>70){
            printf("3x3 Box detected\n");
        } else if (forwardDistanceData() <= 30){
            printf("Hill detected\n");
        } else if  {
            printf("6x6 Box detected\n");
        }

    }
    return 0;
}


// #include <libpynq.h>
// #include "distance.h"
// #include "IR_sensor.h"
// #include "embeddedInitialImp.h"

// int main(){
//     pynq_init();
//     embeddedInit();
    
// //    tcs3472 sensor = color_init(integration_time_ms,IO_AR9, IO_AR8);

//     while (1){
//         sleep_msec(100);
//         int downward_distance =  downwardDistanceData();
//         int forward_distance = forwardDistanceData();
//         if (downward_distance < 50){
//             downward_distance+=8;
//         }
//         struct IRSensors IR_data = measureIRData();
//         int IR_1 = IR_data.sensor1Val;
//         int IR_2 = IR_data.sensor2Val;
//         int IR_3 = IR_data.sensor3Val;
//         int IR_4 = IR_data.sensor4Val;
//         printf("Downard distance: %d\nForward distance: %d\nIR 1 value: %d\nIR 2 value: %d\nIR 3 value: %d\nIR 4 value: %d\n", downward_distance, forward_distance, IR_1, IR_2, IR_3, IR_4);

//         struct color color = colorSensor();
// 		printf("\033[48;2;%hhu;%hhu;%hhum      \033[0m", color.red, color.green , color.blue) ;//print uint8_t's
// 		printf("RED value: %d, GREEN value: %d, BLUE value: %d\n",color.red, color.green , color.blue);
    
//     }
//     return 0;
// }