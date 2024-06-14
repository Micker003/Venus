
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

int main(void) {
    //initializing stuff
    pynq_init();
    stepper_init();
    stepper_enable();
    embeddedInit();
    uart_init(UART0);
    send_information(0,2,2);
    send_information(2,3,3);
    send_information(3,4,4);
    send_information(4,-5,5);
    send_information(20,-4,2);
    return 0;
}
