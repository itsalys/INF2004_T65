#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define LEFT_IR_CHANNEL 0
#define LEFT_IR_PIN 26
#define LEFT_IR_GND 20
#define LEFT_IR_VCC 21

#define RIGHT_IR_CHANNEL 2
#define RIGHT_IR_PIN 27
// #define RIGHT_IR_GND built-in
#define RIGHT_IR_VCC 22

#define MID_IR_CHANNEL 1
#define MID_IR_PIN 28  
// #define MID_IR_GND built=in
// #define MID_IR_VCC built-in



const float THRESHOLD = 0.18;
const float conversion_factor = 3.3f / (1 << 12);


bool is_surface_black(int adc_channel) {

    // Read analog values
    adc_select_input(adc_channel);
    uint16_t adc_result = adc_read();
    float converted_results = adc_result * conversion_factor;
    // printf("\n0x%03x -> %f V\n", adc_result, adc_result * conversion_factor);

    return (converted_results > THRESHOLD);  // return 1 if black, else return 0 if white
}

void initIRLine () {
    adc_init();

    // init LEFT PINS
    gpio_init(LEFT_IR_PIN);
    gpio_set_dir(LEFT_IR_PIN, GPIO_IN);
    gpio_set_function(LEFT_IR_PIN, GPIO_FUNC_SIO);

    // init LEFT IR SENSOR GND 
    gpio_init(LEFT_IR_GND);             
    gpio_set_dir(LEFT_IR_GND, GPIO_OUT);
    gpio_put(LEFT_IR_GND, 0);

    // init LEFT IR SENSOR VCC 
    gpio_init(LEFT_IR_VCC);             
    gpio_set_dir(LEFT_IR_VCC, GPIO_OUT);
    gpio_put(LEFT_IR_VCC, 1);

    // init RIGHT PINS
    gpio_init(RIGHT_IR_PIN);
    gpio_set_dir(RIGHT_IR_PIN, GPIO_IN);
    gpio_set_function(RIGHT_IR_PIN, GPIO_FUNC_SIO);

    // init RIGHT IR SENSOR VCC 
    gpio_init(RIGHT_IR_VCC);             
    gpio_set_dir(RIGHT_IR_VCC, GPIO_OUT);
    gpio_put(RIGHT_IR_VCC, 1);

}

// int main(void) {
//     stdio_init_all();

//     initIRLine ();
//     // IR sensor
//     while (1) {
//         printf("%d\n", is_surface_black(LEFT_IR_CHANNEL));
//         sleep_ms(1000);
//     }
// }
