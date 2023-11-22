#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <math.h>

#define I2C_PORT i2c0
#define SCA_PIN 8
#define SCL_PIN 9
// #define MAG_GND built-int
#define MAG_VCC 10

static int addr = 0x1E;

uint8_t buffer[6]; // Store data from the 6 acceleration registers
int16_t x, y, z; // Combined 3 axis data
float fx, fy, fz; // Float type of acceleration data
uint8_t start_register = 0x03; // Start register address (Register OUT_X_H_M)

float initial_angle = 0.0; // Initialize initial angle to 0 degrees
float current_angle = 0.0;
float angle_change = 0.0;
int16_t prev_x = 0;
int16_t prev_y = 0;

void initMagnetometer() {

    // init MAD VCC 
    gpio_init(MAG_VCC);             
    gpio_set_dir(MAG_VCC, GPIO_OUT);
    gpio_put(MAG_VCC, 1);

    // i2c_init(i2c_port defined, speed of communication in Hz) : Initialize I2C with 400KHz, MAX 1MHz
    i2c_init(I2C_PORT, 400000);

    // set GPIO Pins to I2C
    gpio_set_function(SCA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // enable pull up resistors
    gpio_pull_up(SCA_PIN);
    gpio_pull_up(SCL_PIN);

    // Register MR_REG_M (02h) select operation mode to Continuous-conversion mode (0x00)
    uint8_t data[2]; // { register, data }
    data[0] = 0x02;
    data[1] = 0x00;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);

}

float getAngleChange() {
        uint8_t data[2]; // { register, data }
        data[0] = 0x02;
        data[1] = 0x00;
        i2c_write_blocking(I2C_PORT, addr, data, 2, true);

        i2c_write_blocking(I2C_PORT, addr, &start_register, 1, true); // request data from magnetor meter 
        i2c_read_blocking(I2C_PORT, addr, buffer, 6, false); // get data from magnetometer

        // read data


        x = (buffer[0] << 8 | buffer[1]);
        z = (buffer[2] << 8 | buffer[3]);
        y = (buffer[4] << 8 | buffer[5]);

        // fx = x / 100.00;
        // fy = y / 100.00;
        // fz = z / 100.00;

        printf("X: %6.2hd    Y: %6.2hd    Z: %6.2hd", x, y, z);

        // Calculate the angle change. Tangent of (y / x) in radians. 
        current_angle = atan2(y, x) * (180.0 / M_PI); // Converts the angle from radians to degrees. M_PI is a constant represents the value of π from math library.
        angle_change = current_angle - initial_angle; // Calculate change in angle from initial

        if (angle_change < 0) angle_change += 360;
        
        printf("\tAngle change: %.2f degrees\n", angle_change);

        // Calculate change in x and y axes
        int16_t x_change = x - prev_x;
        int16_t y_change = y - prev_y;
        // printf("\tX Change: %d, Y Change: %d\n\n", x_change, y_change);

        prev_x = x;
        prev_y = y;

        return angle_change;
} 

// int main(void){
//     stdio_init_all(); // Initialise STD I/O for printing over serial

//     initMagnetometer(); // Initialise I2C & Magnetometer 

//     // Infinite Loop
//     for(;;){
//         getAngleChange();
//     }
// }