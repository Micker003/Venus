
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

    uart_init(UART0);
    send_information(0,2,2);
    send_information(2,3,3);
    send_information(3,4,4);
    send_information(4,-5,5);
    send_information(20,-4,2);
    return 0;
}
