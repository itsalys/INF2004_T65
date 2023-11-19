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
    
float target_speed = 1.0; // Set desired speed in m/s (adjust as needed)

float integral[2] = {0.0, 0.0}; // Integral term for left and right wheels
float last_error[2] = {0.0, 0.0}; // Last error for left and right wheels

void update_pid(float actual_speed[2]) {
    float error[2];
    float derivative[2];

    for (int i = 0; i < 2; i++) {
        error[i] = target_speed - actual_speed[i]; // Calculate error

        integral[i] += error[i]; // Update integral term
        derivative[i] = error[i] - last_error[i]; // Calculate derivative term

        // Calculate PID output
        int output = KP * error[i] + KI * integral[i] + KD * derivative[i];
        
        // Print debug information
        printf("Wheel %d - Error: %.5f, Integral: %.5f, Derivative: %.5f, Output: %.5f\n", i, error[i], integral[i], derivative[i], output);

        // Adjust motor speeds based on PID output
        if (i == 0) {
            // Left wheel
            moveForward(100 + output); // Adjust the 100 as needed
        } else if (i == 1) {
            // Right wheel
            moveForward(100 - output); // Adjust the 100 as needed
        }

        last_error[i] = error[i]; // Update last error
    }
}

int main() {
    stdio_init_all();

    initEncoder();
    initMotor();

    sleep_ms(100);

    testMotor();

    for (;;) {
        // Obtain actual speed from encoder
        float leftWheelSpeed = getLeftWheelSpeed();
        float rightWheelSpeed = getRightWheelSpeed();

        float actual_speed[2] = {leftWheelSpeed, rightWheelSpeed};

        // Perform PID control using the obtained speed values
        update_pid(actual_speed);

        busy_wait_ms(INT8_MAX);
    }
}
