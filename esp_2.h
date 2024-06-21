#include <libpynq.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// //create a struct called coordinates which can maintain the location of the robot
// struct coordinates {
//     int x; 
//     int y;
//     char* objectAtLocation;
// };

void send_message(char* message){
    if(strlen(message) > 256){
        printf("Message contains too much chars\n");
    }
    else if(strlen(message) == 0){
        printf("Message contains no information\n");
    }
    else{
        uint32_t length = strlen(message) + 1;
        // Calculate how much bits need to be added to make the length divisible by 8
        uint32_t padding = 8 - (length % 8);
        uint32_t padded_length = length;
        if(padding < 8){
                padded_length = length + padding;
        }
        uint8_t* len_bytes = (uint8_t*)&padded_length;
        // Print the length of the message
        printf("Lengths : %d, %d\n", length, padded_length);

        fflush(NULL);

        // Send length
        for(uint32_t i = 0; i < 4; i++){
            uart_send(UART0, len_bytes[i]);
        }
        // Send message
        for(uint32_t i = 0; i < length; i++){
            uart_send(UART0, message[i]);
        }
	    if (padding - 1 > 0){
            for (uint32_t i = 0; i < padding - 1; i++){
                uart_send(UART0, ' ');
            }
	    }

        uart_send(UART0, '\0'); // Null terminator
        printf("Message is sent\n");
    }
}

void decodeMessage(const char *message, int *ID, int *x, int *y){
    // Use sscanf to parse the integers from the message string
    // coordinates->objectAtLocation = "Empty";
    sscanf(message, "%d,%d,%d", ID, x, y);
}

char* receive_message(int *ID, int *x, int *y){
    uint8_t read_len[4];
    for(uint32_t i = 0; i < 4; i++)
    {
        read_len[i] = uart_recv(UART0);
    }
    uint32_t length = *((uint32_t*)read_len);    
    printf(">> Incoming Message: Length = %d\n", length);
    fflush(NULL);
    uint32_t i = 0;    
    char* buffer = (char*) malloc(sizeof(char) * (length + 1)); // +1 for null terminator
    while(i < length)
    {
        buffer[i] = (char)uart_recv(UART0);
        i++;
    }
    buffer[length] = '\0'; // Null terminate the string
    printf("  >%s\n", buffer);
    fflush(NULL);
    decodeMessage(buffer, ID, x, y);
    return buffer;
}

char* glue(int ID, int x, int y) {
    char ID_C[12] = ""; // Increase the size to handle more than single-digit IDs
    char x_c[12] = "";
    char y_c[12] = "";
    
    sprintf(ID_C, "%d", ID);
    sprintf(x_c, "%d", x);
    sprintf(y_c, "%d", y);
    
    // Calculate the total length of the resulting string
    size_t total_length = strlen(ID_C) + strlen(x_c) + strlen(y_c) + 2 + 1; // 2 commas and 1 null terminator

    // Allocate enough memory for the resulting string
    char *result = (char *)malloc(total_length);
    if (result == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Construct the resulting string
    strcpy(result, ID_C);
    strcat(result, ",");
    strcat(result, x_c);
    strcat(result, ",");
    strcat(result, y_c);
    
    return result;
}

void send_information(int ID, int x, int y){
    char* output = glue(ID, x, y);
    printf("Message: %s\n", output);
    send_message(output);
    free(output); // Free allocated memory
    // sleep_msec(1000);
}

void receive_information(int *ID, int *x, int *y) {
    char *message = receive_message(ID, x, y);
    if (message != NULL) {
        printf("Received message: %d, %d, %d\n", *ID, *x, *y);
        free(message); // Free allocated memory
    }
}

// int main() {
//     printf("Boom\n");
//     switchbox_init();
//     switchbox_set_pin(IO_AR0, SWB_UART0_RX);
//     switchbox_set_pin(IO_AR1, SWB_UART0_TX);

//     // Initialize UART 0
//     uart_init(UART0);
//     // Flush FIFOs of UART 0
//     uart_reset_fifos(UART0);

//     fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

//     // struct coordinates currentCoordinate;
//     // struct coordinates receivedCoordinates;
//     int currentX;
//     int currentY;
//     int currentID;

//     int receivedX;
//     int receivedY;
//     // int receivedID;

//     // Create the initial coordinate which can be assumed as empty
//     currentX = 123;
//     currentY = 684;
//     currentID = 0;
//     int ID = 0;

//     bool readyToSendInfo = true;

//     while(true){
//         char ch;
//         if (read(STDIN_FILENO, &ch, 1) == 1) {
//             readyToSendInfo = true;
//         }

//         if(uart_has_data(UART0)){
//             receive_information(&ID, &receivedX, &receivedY);
//             printf("Received message: %d, %d, %d\n", ID, x, y);
//         }
//         else if (readyToSendInfo){
//             send_information(ID, currentCoordinate);
//             readyToSendInfo = false;
//         }
//     }

//     printf("Done\n");

//     fflush(NULL);
//     uart_reset_fifos(UART0);
//     uart_destroy(UART0);
//     // Clean up after use
//     pynq_destroy();
//     return EXIT_SUCCESS;
// }
