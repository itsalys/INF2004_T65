#include <stdio.h>
#include "pico/stdlib.h"

// #include "barcode_scanner/barcode_scanner.c"
#include "encoder/encoder.c"
// #include "magnetometer/magnetometer.c"
#include "motor/motor.c"
// #include "ultrasonic/ultrasonic.c"

// Constants for the PID controller
float KP = 1.0; // Proportional gain
float KI = 0.1; // Integral gain
float KD = 0.01; // Derivative gain
    
float target_speed = 3.0; // Set desired speed in m/s (adjust as needed)

float integral[2] = {0.0, 0.0}; // Integral term for left and right wheels
float last_error[2] = {0.0, 0.0}; // Last error for left and right wheels

void update_pid(float actual_speed[2]) {
    float error[2];
    float derivative[2];

    for (int i = 0; i < 2; i++) {
        error[i] = target_speed - actual_speed[i]; // Calculate error

        integral[i] += error[i]; // Update integral term
        derivative[i] = error[i] - last_error[i]; // Calculate derivative term

        if (i == 0) printf("LEFT\t");
        else if (i == 1) printf("RIGHT\t");
        
        printf("Speed: %.5f m/s, Total Distance Traveled: %.5f m\n", speed[i], distance[i]);

        // Calculate PID output
        float output = KP * error[i] + KI * integral[i] + KD * derivative[i];
        
        // Print debug information
        printf("Wheel %d - Error: %.5f, Integral: %.5f, Derivative: %.5f, Output: %.5f\n\n", i, error[i], integral[i], derivative[i], output);

        // Adjust motor speeds based on PID output
        if (i == 0) {
            // Left wheel
            // moveForward(100 + output); // Adjust the 100 as needed
            pwm_set_chan_level(slice_num, PWM_CHAN_A, DEFAULT_SPEED * (output / 100) ); 
        } else if (i == 1) {
            // Right wheel
            // moveForward(100 - output); // Adjust the 100 as needed
            pwm_set_chan_level(slice_num, PWM_CHAN_B, DEFAULT_SPEED * (output / 100) ); 
        }

        last_error[i] = error[i]; // Update last error
    }
}

int main() {
    stdio_init_all();

    initEncoder();
    initMotor();

    sleep_ms(3000);

    moveForward(70);

    for (;;) {
        // // Obtain actual speed from encoder
        // float leftWheelSpeed = getLeftWheelSpeed();
        // float rightWheelSpeed = getRightWheelSpeed();

        // float actual_speed[2] = {leftWheelSpeed, rightWheelSpeed};

        // // Perform PID control using the obtained speed values
        // update_pid(speed);
        sleep_ms(100);

    }
}
