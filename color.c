#include <libpynq.h>
#include <inttypes.h>
#include <sys/time.h>
#include "color.h"

int map(int x, int in_min, int in_max, int out_min, int out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int count_pulses(const io_t pin_out){
    struct timeval last_time;
    gettimeofday(&last_time, NULL);
    int last_level = gpio_get_level(pin_out);
    int pulse_count = 0;
    int time_count = 0;

    while (time_count < 1000) {
        int current_level = gpio_get_level(pin_out);

        if (current_level == GPIO_LEVEL_HIGH && last_level == GPIO_LEVEL_LOW) {
            pulse_count++;
        }

        last_level = current_level;
        sleep_msec(1);
        time_count++;
    }
    return pulse_count;
}

void color_init(const io_t pin_out, const io_t pin_s0, const io_t pin_s1,const io_t pin_s2, const io_t pin_s3 ){
    switchbox_init();
    gpio_init();

    gpio_set_direction(pin_out, GPIO_DIR_INPUT);//OUT ON THE SENSOR
    pwm_init(PWM0, 0);
    gpio_set_direction(pin_s0, GPIO_DIR_OUTPUT);//S0
    gpio_set_direction(pin_s1, GPIO_DIR_OUTPUT);//S1
    gpio_set_direction(pin_s2, GPIO_DIR_OUTPUT);//S2
    gpio_set_direction(pin_s3, GPIO_DIR_OUTPUT);//S3

    gpio_set_level(pin_s0, GPIO_LEVEL_LOW);
    gpio_set_level(pin_s1, GPIO_LEVEL_HIGH);
}

char color_read(const io_t pin_out, const io_t pin_s2, const io_t pin_s3 ){
        // RED Color
        gpio_set_level(pin_s2, GPIO_LEVEL_LOW);
        gpio_set_level(pin_s3, GPIO_LEVEL_LOW);
        sleep_msec(200);
        int pulseRED = count_pulses();
        int R_value = map(pulseRED, 10, 100, 0, 255); // Mapping based on empirical data
        printf("R value: %d\n", R_value);

        // GREEN Color
        gpio_set_level(pin_s2, GPIO_LEVEL_HIGH);
        gpio_set_level(pin_s3, GPIO_LEVEL_HIGH);
        sleep_msec(200);
        int pulseGREEN = count_pulses();
        int G_value = map(pulseGREEN, 10, 100, 0, 255);
        printf("G value: %d\n", G_value);

        // BLUE Color
        gpio_set_level(pin_s2, GPIO_LEVEL_LOW);
        gpio_set_level(pin_s3, GPIO_LEVEL_HIGH);
        sleep_msec(200);
        int pulseBLUE = count_pulses();
        int B_value = map(pulseBLUE, 10, 100, 0, 255);
        printf("B value: %d\n", B_value);

        // Color Detection 
        if (R_value > 850 && R_value < 1200 && G_value < 1400 && G_value > 1100 && B_value < 1400 && B_value > 1150) {
            return 'r'; //RED
        } else if (G_value > 580 && G_value < 800 && R_value < 950 && R_value > 650 && B_value < 1400 && B_value > 1150) {
            return 'g'; //GREEN
        } else if (B_value > 600 && B_value < 800 && R_value < 1000 && R_value > 700 && G_value < 1400 && G_value > 1200) {
            return 'b'; //BLUE
        } else if (R_value > 180 && G_value > 180 && B_value > 180) {
            return 'w'; //WHITE
        } else if (R_value < 1000 && G_value < 600 && B_value < 600) {
            return 'B'; //BLACK
        } else {
            return 'u'; //UNKNOWN
        }
}
