/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1

#include "pico/stdlib.h"
#include "hardware/pwm.h"

// #define LEFT_S 13000
#define LEFT_S 18000
#define RIGHT_S 20000

#define LEFT_PMW 0
#define RIGHT_PMW 1

#define LEFT_F 2
#define LEFT_B 3
#define RIGHT_F 4
#define RIGHT_B 5

uint slice_num;


// FUNCTIONS TO SET DIRECTION OF MOTOR

void setLeftForward () {
    gpio_put(LEFT_F, 1);
    gpio_put(LEFT_B, 0);
}

void setLeftBackward () {
    gpio_put(LEFT_F, 0);
    gpio_put(LEFT_B, 1);
}

void setRightForward () {
    gpio_put(RIGHT_F, 1);
    gpio_put(RIGHT_B, 0);
}

void setRightBackward () {
    gpio_put(RIGHT_F, 0);
    gpio_put(RIGHT_B, 1);
}


// FUNCTIONS TO CONTROL MOTOR

void stop () {
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0); 
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0); 
}

// STATIC SPEED 

// void moveForward () {
//     setLeftForward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_A, LEFT_S); 
//     setRightForward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_B, RIGHT_S); 
// }

// void moveLeft() {
//     setLeftForward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_A, LEFT_S); 
//     setRightForward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_B, 0); 
// }

// // void moveLeft() {
// //     setLeftForward();
// //     pwm_set_chan_level(slice_num, PWM_CHAN_A, LEFT_S); 
// //     setRightBackward();
// //     pwm_set_chan_level(slice_num, PWM_CHAN_B, RIGHT_S); 
// // }

// void moveRight () {
//     setLeftForward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_A, 0); 
//     setRightForward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_B, RIGHT_S); 
// }

// // void moveRight () {
// //     setLeftBackward();
// //     pwm_set_chan_level(slice_num, PWM_CHAN_A, LEFT_S); 
// //     setRightForward();
// //     pwm_set_chan_level(slice_num, PWM_CHAN_B, RIGHT_S); 
// // }

// void moveBackward () {
//     setLeftBackward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_A, LEFT_S); 
//     setRightBackward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_B, RIGHT_S); 
// }

// SPECIFY SPEED : S/100

void moveForward (int s) {
    stop();
    setLeftForward();
    pwm_set_chan_level(slice_num, PWM_CHAN_A, (LEFT_S/100 * s)); 
    setRightForward();
    pwm_set_chan_level(slice_num, PWM_CHAN_B, (RIGHT_S/100 * s)); 
}

void moveLeft(int s) {
    stop();
    setLeftForward();
    pwm_set_chan_level(slice_num, PWM_CHAN_A, (LEFT_S/100 * s)); 
    setRightForward();
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0); 
}

// void moveLeft(int s) {
//     setLeftForward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_A, (LEFT_S/100 * s)); 
//     setRightBackward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_B, (RIGHT_S/100 * s)); 
// }

void moveRight (int s) {
    stop();
    setLeftForward();
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0); 
    setRightForward();
    pwm_set_chan_level(slice_num, PWM_CHAN_B, (RIGHT_S/100 * s)); 
}

// void moveRight (int s) {
//     setLeftBackward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_A, (LEFT_S/100 * s)); 
//     setRightForward();
//     pwm_set_chan_level(slice_num, PWM_CHAN_B, (RIGHT_S/100 * s)); 
// }

void moveBackward (int s) {
    stop();
    setLeftBackward();
    pwm_set_chan_level(slice_num, PWM_CHAN_A, (LEFT_S/100 * s)); 
    setRightBackward();
    pwm_set_chan_level(slice_num, PWM_CHAN_B, (RIGHT_S/100 * s)); 
}

// INITIALISE

void initMotor( ) {

// GPIO init - controls direction of motor

    // Left
    gpio_init(LEFT_F);
    gpio_init(LEFT_B);
    gpio_set_dir(LEFT_F, GPIO_OUT);
    gpio_set_dir(LEFT_B, GPIO_OUT);
    setLeftForward();

    // Right
    gpio_init(RIGHT_F);
    gpio_init(RIGHT_B);
    gpio_set_dir(RIGHT_F, GPIO_OUT);
    gpio_set_dir(RIGHT_B, GPIO_OUT);
    setRightForward(); 


// PMW init - controls speed of motor
    gpio_set_function(LEFT_PMW, GPIO_FUNC_PWM);
    gpio_set_function(RIGHT_PMW, GPIO_FUNC_PWM);

    slice_num = pwm_gpio_to_slice_num(0); // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    pwm_set_clkdiv(slice_num, 100); 
    pwm_set_wrap(slice_num, 20000);  // Set period

     stop(); // init stop

    pwm_set_enabled(slice_num, true); // enable pwm

}


int main() {


    initMotor();

    moveForward(60);

    // for (;;) {

    // moveForward(60);
    // sleep_ms(500);
    // stop();
    // sleep_ms(300);
    // // moveLeft();
    // // sleep_ms(500);
    // // moveRight();
    // // sleep_ms(500);
    // moveBackward(60);
    // sleep_ms(500);
    // stop();
    // sleep_ms(500);

    // }
}