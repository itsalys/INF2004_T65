#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0

static int addr = 0x1E;

// Initialise Accelerometer Function

void initMagnetometer() {

    // i2c_init(i2c_port defined, speed of communication in Hz) : Initialize I2C with 400KHz, MAX 1MHz
    i2c_init(I2C_PORT, 400000);

    // set GPIO Pins to I2C
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);

    // enable pull up resistors
    gpio_pull_up(4);
    gpio_pull_up(5);

    // Register MR_REG_M (02h) select operation mode to Continuous-conversion mode (0x00)
    uint8_t data[2]; // { register, data }
    data[0] = 0x02;
    data[1] = 0x00;
    i2c_write_blocking(I2C_PORT, addr, data, 2, true);

}

// int main(void){
//     stdio_init_all(); // Initialise STD I/O for printing over serial

//     initMagnetometer(); // Initialise I2C & Magnetometer 

//     uint8_t buffer[6]; // Store data from the 6 acceleration registers
//     int16_t x, y, z; // Combined 3 axis data
//     float fx, fy, fz; // Float type of acceleration data
//     uint8_t start_register = 0x03; // Start register address (Register OUT_X_H_M)

//     // Infinite Loop
//     for(;;){
//         i2c_write_blocking(I2C_PORT, addr, &start_register, 1, true); // request data from magnetor meter 
//         i2c_read_blocking(I2C_PORT, addr, buffer, 6, false); // get data from magnetometer

//         // read data

//         // x = ((buffer[1]<<8) | buffer[0]);
//         // y = ((buffer[3]<<8) | buffer[2]);
//         // z = ((buffer[5]<<8) | buffer[4]);

//         x = (buffer[0] << 8 | buffer[1]);
//         z = (buffer[2] << 8 | buffer[3]);
//         y = (buffer[4] << 8 | buffer[5]);

//         fx = x / 100.00;
//         fy = y / 100.00;
//         fz = z / 100.00;

//         printf("X: %6.2f    Y: %6.2f    Z: %6.2f", fx, fy, fz);

//         sleep_ms(300);
//     }
// }