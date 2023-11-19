#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define RIGHT_ENCODER_PIN 15
#define LEFT_ENCODER_PIN 14

uint32_t current_time[2];
uint32_t last_edge_time[2] = {0, 0};

// Calculating speed & distance
#define WHEEL_CIRCUMFERENCE 0.200 // in m estimated, distance of 1 rotation
#define PULSE_DISTANCE (0.200 / 20) // in m estimated, distance of 1 pulse/notch
float distance[2] = {0.0 ,0.0}; // {L, R}
float speed[2] = {0.0, 0.0}; // Speed in m/s for each wheel

int counter_pulse[2] = {0,0};

// Debounce Control
const int delayTime = 10; // Delay for every push button may vary

void gpio_callback(uint gpio, uint32_t events)
{   
    int i;
    if (gpio == LEFT_ENCODER_PIN) i = 0;
    else if (gpio == RIGHT_ENCODER_PIN) i = 1;

    if ((to_ms_since_boot(get_absolute_time()) - current_time[i]) > delayTime) { // handle debounce

        current_time[i] = to_ms_since_boot(get_absolute_time());
        counter_pulse[i]++;
        // printf("\nCounter: %i", counter_pulse);

        if (counter_pulse[i] == 20) { // runs every full rotation
            counter_pulse[i] = 0; 

            // determining pulse width - the time taken for one notch in s 
            float pulse_width = (current_time[i] - last_edge_time[i]) / 1000.0;
            last_edge_time[i] = current_time[i];
            // printf("\nPulse Width: %.5fs ", pulse_width);

            // Calculate speed and update distance
            speed[i] = WHEEL_CIRCUMFERENCE / pulse_width; // Speed in m/s
            distance[i] += WHEEL_CIRCUMFERENCE;                 // Update total distance


            if (i == 0) printf("LEFT\t");
            else if (i == 1) printf("RIGHT\t");
            
            printf("Speed: %.5f m/s, Total Distance Traveled: %.5f m\n", speed[i], distance[i]);
        }
    }
}

// Function to get the speed of the left wheel
float getLeftWheelSpeed() {
    return speed[0];
}

// Function to get the speed of the right wheel
float getRightWheelSpeed() {
    return speed[1];
}

// void enableEncoder( ) {
//     gpio_set_irq_enabled_with_callback(RIGHT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
// }

// void disableEncoder( ) {
//     gpio_set_irq_enabled_with_callback(RIGHT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, false, &gpio_callback);
// }

// INITIALISE

void initEncoder( )
{

    gpio_init(RIGHT_ENCODER_PIN);
    gpio_init(LEFT_ENCODER_PIN);
    gpio_set_dir(RIGHT_ENCODER_PIN, GPIO_IN);
    gpio_set_dir(LEFT_ENCODER_PIN, GPIO_IN);

    current_time[0] = current_time[1] = to_ms_since_boot(get_absolute_time());
    sleep_ms(100);
    gpio_set_irq_enabled_with_callback(RIGHT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(LEFT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    return;
}

// TEST - run with motor test

void testEncoder( ) {
    
}
