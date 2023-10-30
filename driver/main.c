#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "motor/motor.c"
#include "encoder/encoder.c"

int main() {
    stdio_init_all();

    initEncoder();
    initMotor();

    testMotor();


    // for (;;) {
    //     busy_wait_ms(INT8_MAX);
    // }
}
