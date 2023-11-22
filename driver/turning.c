#include <stdio.h>
#include "pico/stdlib.h"

// #include "barcode_scanner/barcode_scanner.c"
#include "irline/irline.c"
#include "encoder/encoder.c"
#include "magnetometer/magnetometer.c"
#include "motor/motor.c"
// #include "ultrasonic/ultrasonic.c"


int main() {
    stdio_init_all();

    // initEncoder();
    initMotor();
    initIRLine ();
    initMagnetometer();

    sleep_ms(2000);

    moveLeft(50);

    for (;;) {

        // HARDCODED

        if (!is_surface_black(LEFT_IR_CHANNEL)) {
            sleep_ms(1000);
            moveLeft(50);
            sleep_ms(1000);
            moveForward(50);
        }

        // if (!is_surface_black(RIGHT_IR_CHANNEL)) {
        //     sleep_ms(1000);
        //     moveRight(50);
        //     sleep_ms(1000);
        //     moveForward(50);
        // }


        // W/ MAGNETOMETER

        // if (!is_surface_black(LEFT_IR_CHANNEL)) {
        //     sleep_ms(1000);
        //     moveLeft(50);
        //     float turn_angle = getAngleChange();
        //     while((getAngleChange() - turn_angle) < 90.0);
        //     moveForward(50);
        // }

        // if (!is_surface_black(RIGHT_IR_CHANNEL)) {
        //     sleep_ms(1000);
        //     moveRight(50);
        //     float turn_angle = getAngleChange();
        //     while((turn_angle - getAngleChange()) < 90.0);
        //     moveForward(50);
        // }
    }
}
