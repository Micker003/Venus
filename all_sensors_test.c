#include <libpynq.h>
#include "distance.h"
#include "IR_sensor.h"
#include "embeddedInitialImp.h"

int main(){
    pynq_init();
    embeddedInit();

    while (1){
        int downward_distance =  downwardDistanceData();
        int forward_distance = forwardDistanceData();
        struct IRSensors IR_data = measureIRData();
        int IR_1 = IR_data.sensor1Val;
        int IR_2 = IR_data.sensor2Val;
        int IR_3 = IR_data.sensor3Val;
        int IR_4 = IR_data.sensor4Val;
        printf("Downard distance: %d\nForward distance: %d\nIR 1 value: %d\nIR 2 value: %d\nIR 3 value: %d\nIR 4 value: %d\n", downward_distance, forward_distance, IR_1, IR_2, IR_3, IR_4);
    }
    return 0;
}