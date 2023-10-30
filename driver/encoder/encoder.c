// #include <stdio.h>
// #include "pico/stdlib.h"

uint32_t current_time;
uint32_t last_edge_time = 0;

// Calculating speed & distance
int counter_pulse = 0;
float wheel_circumference = 0.2042035225; // in m estimated, distance of 1 rotation
float pulse_distance = 0.2042035225 / 20; // in m estimated, distance of 1 pulse/notch
float distance = 0.0;

// Debounce Control
const int delayTime = 10; // Delay for every push button may vary

void gpio_callback(uint gpio, uint32_t events)
{
    if ((to_ms_since_boot(get_absolute_time()) - current_time) > delayTime) { // handle debounce

        current_time = to_ms_since_boot(get_absolute_time());
        counter_pulse++;
        // printf("\nCounter: %i", counter_pulse);

        if (counter_pulse == 20) { // runs every full rotation
            counter_pulse = 0; 

            // determining pulse width - the time taken for one notch in s 
            float pulse_width = (current_time - last_edge_time) / 1000.0;
            last_edge_time = current_time;
            // printf("\nPulse Width: %.5fs ", pulse_width);

            // Calculate speed and update distance
            float speed = wheel_circumference / pulse_width; // Speed in m/s
            distance += wheel_circumference;                 // Update total distance

            printf("\nSpeed: %.5f m/s, Total Distance Traveled: %.5f m", speed, distance);
        }
    }
}

void enableEncoder( ) {
    gpio_set_irq_enabled_with_callback(15, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
}

void disableEncoder( ) {
    gpio_set_irq_enabled_with_callback(15, GPIO_IRQ_EDGE_RISE, false, &gpio_callback);
}

// INITIALISE

void initEncoder( )
{
    current_time = to_ms_since_boot(get_absolute_time());
    gpio_set_irq_enabled_with_callback(15, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    return;
}

// TEST - run with motor test

void testEncoder( ) {
    
}

// int main() {
//     stdio_init_all();

//     initEncoder();
//     initMotor();

//     sleep_ms(4000);
//     moveForward(50);


//     for (;;)
//     {
//     //     printf("fuck");
//         // moveForward(50);
//         // sleep_ms(4000);
//         // moveForward(50);
//         // sleep_ms(4000);
//     }
// }