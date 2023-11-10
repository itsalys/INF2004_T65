#include <stdio.h>
#include "pico/stdlib.h"
#include "lwipopts.h"
#include "/lwip/apps/httpd.h"

// #include "barcode_scanner/barcode_scanner.c"
#include "encoder/encoder.c"
#include "magnetometer/magnetometer.c"
#include "motor/motor.c"
#include "ultrasonic/ultrasonic.c"
#include "wifi/wifi.c"



int main() {
//     stdio_init_all();

//     initEncoder();
//     initMotor();

//     sleep_ms(100);

//     testMotor();

    


    for (;;) {
        busy_wait_ms(INT8_MAX);
    }
}
