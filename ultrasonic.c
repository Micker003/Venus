

#include <stdio.h>
#include <libpynq.h>
#define AS_SECONDS(time) (time / 1.0e8) // Timer runs at ~100MHZ


int main()
{
    pynq_init();
    switchbox_set_pin(IO_AR7, SWB_TIMER_IC0); // Pulsecounter 0 for rising edge
    pulsecounter_init(PULSECOUNTER0);
    pulsecounter_set_edge(PULSECOUNTER0, GPIO_LEVEL_HIGH); // Use Rising edge triggering
    pulsecounter_reset_count(PULSECOUNTER0);

    switchbox_set_pin(IO_AR6, SWB_TIMER_IC1); // Pulsecounter 1 for falling edge
    pulsecounter_init(PULSECOUNTER1);
    pulsecounter_set_edge(PULSECOUNTER1, GPIO_LEVEL_LOW); // Use Falling edge triggering
    pulsecounter_reset_count(PULSECOUNTER1);

    switchbox_set_pin( IO_AR5, SWB_PWM0);//freqDutySend
    pwm_init(PWM0, 10000000);
    pwm_set_duty_cycle(PWM0,1000000);
    uint32_t past_count_rise = 0;
    uint32_t past_time_rise = 0;
    uint32_t past_count_fall = 0;

    while(1)
    {
        uint32_t time_rise;
        uint32_t count_rise = pulsecounter_get_count(PULSECOUNTER0, &time_rise);

        uint32_t time_fall;
        uint32_t count_fall = pulsecounter_get_count(PULSECOUNTER1, &time_fall);

        if(count_rise != past_count_rise)
        {
            printf("Count rise: %u -- Time rise: %f\n", count_rise, AS_SECONDS(time_rise));
            past_time_rise = time_rise;
            past_count_rise = count_rise;
        }

        if(count_fall != past_count_fall)
        {
            printf("Count fall: %u -- Time fall: %f\n", count_fall, AS_SECONDS(time_fall));
            past_count_fall = count_fall;

            // Calculate the duration the square wave is ON
            if (past_time_rise != 0) // Ensure a rising edge has been detected before falling edge
            {
                uint32_t duration = time_fall - past_time_rise;
                printf("Duration ON: %f seconds\n", AS_SECONDS(duration));
                printf("Distance = %d\n", (int)(AS_SECONDS(duration)/2 * 340 * 100));
            }
        }
    }
    
    pulsecounter_destroy(PULSECOUNTER0);
    pulsecounter_destroy(PULSECOUNTER1);
    pynq_destroy();
    return 0;
}
