#include <stdio.h>
#include "pico/stdlib.h"

// #include "barcode_scanner/barcode_scanner.c"
#include "irline/irline.c"
#include "encoder/encoder.c"
#include "magnetometer/magnetometer.c"
#include "motor/motor.c"
#include "ultrasonic/ultrasonic.c"


int main() {
    stdio_init_all();

    // initEncoder();
    initMotor();
    initIRLine ();
    initMagnetometer();
    initUltrasonic();

    sleep_ms(2000);

    moveForward(50);

    for (;;) {

        // HARDCODED
        if (getUltrasonicDistance() < 7) { // set distance in cm
            pivotLeft(50);
            sleep_ms(1350);
            moveForward(50);
            // stop();
        }

        // W/ MAGNETOMETER

        // if (getUltrasonicDistance() < 10)) {
        //     moveLeft(50);
        //     float turn_angle = getAngleChange();
        //     while((getAngleChange() - turn_angle) < 180.0);
        //     moveForward(50);
        // }


        // HARDCODED turning

        // if (!is_surface_black(LEFT_IR_CHANNEL)) {
        //     sleep_ms(1000);
        //     moveLeft(50);
        //     sleep_ms(1000);
        //     moveForward(50);
        // }

        // if (!is_surface_black(RIGHT_IR_CHANNEL)) {
        //     sleep_ms(1000);
        //     moveRight(50);
        //     sleep_ms(1000);
        //     moveForward(50);
        // }

    }
}
