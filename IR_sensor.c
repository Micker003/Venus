
#include <libpynq.h>
#include "IR_sensor.h"

#define ROLLING_AVERAGE  10
#define ADC_VOLTAGE_5V  65535

void IR_init(){
  adc_init();
}

int IR_read(adc_channel_t adc_channel){
  int count = 0; //counter for rolling average
  int ADC_plus,ADC_minus,average_IR_value,IR_value,total_IR_value = 0;
  
  //read IR values and add to total
  while (count != ROLLING_AVERAGE){    
      ADC_plus = ADC_VOLTAGE_5V;
      ADC_minus = adc_read_channel_raw(adc_channel);
      IR_value = ADC_plus - ADC_minus;
      total_IR_value +=IR_value;
      count ++;
      sleep_msec(1);
  }
  //calculate average IR value
  average_IR_value = total_IR_value/ROLLING_AVERAGE;

  return average_IR_value;    
}