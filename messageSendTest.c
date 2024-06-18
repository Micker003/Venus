
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

    switchbox_init();
    switchbox_set_pin(IO_AR0, SWB_UART0_RX);
    switchbox_set_pin(IO_AR1, SWB_UART0_TX);

    // initialize UART 0
    uart_init(UART0);
    // flush FIFOs of UART 0
    uart_reset_fifos(UART0);

    // char buf[256];
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    send_information(0,2,2);
    send_information(1,1,1);
    send_information(2,3,3);
    send_information(3,4,4);
    send_information(4,4,2);
    send_information(5,4,0);
    send_information(6,4,-2);
    send_information(7,4,-4);
    send_information(8,2,-4);
    send_information(9,0,-4);
    send_information(10,-2,-4);

    send_information(15,15,0);
    send_information(16,-15,0);
    send_information(17,0,15);
    send_information(18,0,-15);

    send_information(20,10,2);
    send_information(20,10,6);
    send_information(20,10,10);  

    send_information(25,-10,2);
    send_information(25,-10,6);
    send_information(25,-10,10);
    return 0;
}
