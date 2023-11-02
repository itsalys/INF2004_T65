// #include "pico/stdlib.h"
// #include "hardware/pwm.h"

#define DEFAULT_SPEED 62500
#define LEFT_S 62500
#define RIGHT_S 62500

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
//     stop();
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
//     stop();
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

// INITIALISE ------------------------------------------------------------------------------------

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
    pwm_set_wrap(slice_num, DEFAULT_SPEED);  // Set period

     stop(); // init stop

    pwm_set_enabled(slice_num, true); // enable pwm

}

// TEST ------------------------------------------------------------------------------------------

void testMotor( ) {

    for(;;) {
        moveForward(100);
        sleep_ms(800);

        moveForward(50);
        sleep_ms(800);
        

        moveLeft(70);
        sleep_ms(800);

        moveRight(70);
        sleep_ms(800);

        moveBackward(70);
        sleep_ms(1500);

        stop();
        sleep_ms(500);
    }
}



// int main() {


//     initMotor();
//     moveForward(60);

//     // testMotor();
// }