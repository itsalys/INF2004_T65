//Get readings from ultrasonic sensor

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"

int timeout = 26100;

#define ULTRA_ECHO 16
#define ULTRA_TRIG 17
// #define ULTRA_GND built-int
#define ULTRA_VCC 18

void initUltrasonic() {
    
    // init ULTRA ECO
    gpio_init(ULTRA_ECHO);             
    gpio_set_dir(ULTRA_ECHO, GPIO_IN);

    // init ULTRA TRIG
    gpio_init(ULTRA_TRIG);             
    gpio_set_dir(ULTRA_TRIG, GPIO_OUT);

    // init UTRA VCC
    gpio_init(ULTRA_VCC);             
    gpio_set_dir(ULTRA_VCC, GPIO_OUT);
    gpio_put(ULTRA_VCC, 1);

}

uint64_t getPulse(uint trigPin, uint echoPin)
{
    gpio_put(trigPin, 1);
    sleep_us(10);
    gpio_put(trigPin, 0);

    uint64_t width = 0;

    while (gpio_get(echoPin) == 0) tight_loop_contents();
    absolute_time_t startTime = get_absolute_time();
    while (gpio_get(echoPin) == 1) 
    {
        width++;
        sleep_us(1);
        if (width > timeout) return 0;
    }
    absolute_time_t endTime = get_absolute_time();
    
    return absolute_time_diff_us(startTime, endTime);
}

uint64_t getCm(uint trigPin, uint echoPin)
{
    uint64_t pulseLength = getPulse(trigPin, echoPin);
    return pulseLength / 29 / 2;
}

uint64_t getInch(uint trigPin, uint echoPin)
{
    uint64_t pulseLength = getPulse(trigPin, echoPin);
    return (long)pulseLength / 74.f / 2.f;
}

int getUltrasonicDistance() {
    int temp = getCm(ULTRA_TRIG, ULTRA_ECHO);
    return temp;
}

// int main() {
//     stdio_init_all();

//     printf("Hello GPIO IRQ\n");

//     initUltrasonic();

//     // Wait forever
//     while (1){
//         // uint64_t distance=getUltrasonicDistance();
//         printf("Distance = %lldcm \n", getUltrasonicDistance());
//         sleep_ms(1000);
//     }
// }