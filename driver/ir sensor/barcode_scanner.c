#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#define SENSOR_PIN       28
#define CHAR_ARR_SIZE       10
#define CONCATENATED        31
#define MIN_THRESHOLD      1500                
#define MAX_THRESHOLD      5000               
#define THIN_WHITE_BAR      "1"
#define WIDE_WHITE_BAR      "111"
#define THIN_BLACK_BAR      "0"
#define WIDE_BLACK_BAR      "000"

volatile bool white_surface_detected = false;   //Idk if needed.
absolute_time_t white_surface_start_time;       // Needed.
absolute_time_t black_surface_start_time;       // Needed.
uint32_t elapsed_seconds = 0;
uint32_t last_elapsed_seconds = 0;

// Creating a Array Pointer to Store a Single Character.
static char *charArr[CHAR_ARR_SIZE];

// Creating a Variable to Store Character Array.
static char charCode[CONCATENATED];

// Creating a static Position counter.
static int currentPosition = 0;

// Creating a Variable to Store the Character String.
static char result;

// Creating a Variable to Store the Last Interrupt Event (no event/ edge rise/ edge fall)
typedef enum {
    NONE,
    EDGE_RISE,
    EDGE_FALL
} LastInterruptEvent;
static LastInterruptEvent last_event = NONE;

// Defining a Struct for decrpyting Barcode39 Characters.
typedef struct {
    char character;
    char pattern[CONCATENATED];
} Barcode39Chars;

// Defining the Barcode39 Characters and their individual Pattern.
static const Barcode39Chars Barcode39Table[] = {
    {'*' , "1000101110111010"},
    {'*' , "1011101110100010"},
    {'A' , "1110101000101110"},
    {'A' , "1110100010101110"},
    {'B' , "1011101000101110"},
    {'B' , "1110100010111010"},
    {'C' , "1110111010001010"},
    {'c' , "1010001011101110"},
    {'D' , "1010111000101110"},
    {'D' , "1110100011101010"},
    {'E' , "1110101110001010"},
    {'E' , "1010001110101110"},
    {'F' , "1011101110001010"},
    {'F' , "1010001110111010"},
    {'G' , "1010100011101110"},
    {'G' , "1110111000101010"},
    {'H' , "1110101000111010"},
    {'H' , "1011100010101110"},
    {'I' , "1011101000111010"},
    {'I' , "1011100010111010"},
    {'J' , "1010111000111010"},
    {'J' , "1011100011101010"},
    {'K' , "1110101010001110"},
    {'K' , "1110001010101110"},
    {'L' , "1011101010001110"},
    {'L' , "1110001010111010"},
    {'M' , "1110111010100010"},
    {'M' , "1000101011101110"},
    {'N' , "1010111010001110"},
    {'N' , "1110001011101010"},
    {'O' , "1110101110100010"},
    {'O' , "1000101110101110"},
    {'P' , "1011101110100010"},
    {'P' , "1000101110111010"},
    {'Q' , "1010101110001110"},
    {'Q' , "1110001110101010"},
    {'R' , "1110101011100010"},
    {'R' , "1000111010101110"},
    {'S' , "1011101011100010"},
    {'S' , "1000111010111010"},
    {'T' , "1010111011100010"},
    {'T' , "1000111011101010"},
    {'U' , "1110001010101110"},
    {'U' , "1110101010001110"},
    {'V' , "1000111010101110"},
    {'V' , "1110101011100010"},
    {'W' , "1110001110101010"},
    {'W' , "1010101110001110"},
    {'X' , "1000101110101110"},
    {'X' , "1110101110100010"},
    {'Y' , "1110001011101010"},
    {'Y' , "1010111010001110"},
    {'Z' , "1000111011101010"},
    {'Z' , "1010111011100010"},
    {'-' , "1000101011101110"},
    {'.' , "1110001010111010"},
    {'$' , "1000100010001010"},
    {'/' , "1000100010100010"},
    {'+' , "1000101000100010"},
    {'%' , "1010001000100010"},
    {' ' , "1000111010111010"},
};

// Defining the TABLE_SIZE of the Barcode39Table.
#define TABLE_SIZE  (sizeof(Barcode39Table) / sizeof(Barcode39Table[0])) //sizeofTable/sizeofElement

// Test Function to Print Array.
static void printArray(char **arr) 
{
    printf("\nArray Contents: ");
    for (int i = 0; i < CHAR_ARR_SIZE; i++) {
        if (arr[i]) {
            printf("%s ", arr[i]);
        } else {
            printf("(null) ");  // for null pointers
        }
    }
    printf("\n");
}

// Function to Append Values to a Pointer Array.
void appendArrayValue(char **arr, const char *value) 
{
    // Shift all elements down by one
    for (int i = 0; i < CHAR_ARR_SIZE - 1; i++) {
        arr[i] = arr[i + 1];
    }

    // Insert new value at the latest index
    arr[CHAR_ARR_SIZE - 1] = (char *)value;

    // printf("Inserted.\n");
    printArray(arr);
}

// Function to Concatenate the Array Values to a single String.
char* concatenateArrayValue(char **arr)
{
    // Initialize the string as empty.
    charCode[0] = '\0';

    for (int i = 0; i < CHAR_ARR_SIZE; i++)
    {
        if (arr[i])
        {
            strcat(charCode, arr[i]);
        }
    }
}

// Function to check Character String with the Barcode List.
char checkPattern(const char *pattern)
{
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        if (strcmp(Barcode39Table[i].pattern, pattern) == 0) {
            return Barcode39Table[i].character;
        }
    }
    return '\0';  // Return null character if not found
}


// Interrupt! Edge Rise / Edge Fall.
void white_surface_detected_handler(uint gpio, uint32_t events)
{
    // Check for consecutive Edge Falls
    if ((events & GPIO_IRQ_EDGE_FALL) && last_event == EDGE_FALL) {
        return; // Ignore this interrupt
    }

    // Check for consecutive Edge Rises
    if ((events & GPIO_IRQ_EDGE_RISE) && last_event == EDGE_RISE) {
        return; // Ignore this interrupt
    }

    // This Event detects a White Bar.
    if (events & GPIO_IRQ_EDGE_FALL)
    {
        last_event = EDGE_FALL;
        /*  If it detects White as the 1st Position of the new Array, 
            It should be ignored as the Barcode39 starts with a Black at All Times. */
        if(currentPosition == 0)
        {
            // Do nothing.
        }
        else
        {
            // Check if the Value is a Wide or Thin Bar, based on the running average and thresholds.
            if ( (elapsed_seconds > MIN_THRESHOLD) && (elapsed_seconds < MAX_THRESHOLD) )
            {
                // Append the Wide Value into the charArr.
                appendArrayValue(charArr, WIDE_WHITE_BAR); //Wide white bar
            }
            else if (elapsed_seconds < MIN_THRESHOLD)
            {
                // Append the Thin Value into the charArr.
                appendArrayValue(charArr, THIN_WHITE_BAR); //Thin white bar
            }
            else
            {
                // Append a ? Value into the charArr to show running average is not accurate.
                appendArrayValue(charArr, "?");
            }

            white_surface_detected = true;
            white_surface_start_time = get_absolute_time();

            // Increment the current position.
            currentPosition++;

            concatenateArrayValue(charArr); //concatenate array into one string
            // printf("\nCharacter Code is: %s\n\n", charCode);
            result = checkPattern(charCode);
            if (result != '\0')
            {
                printf("Pattern Detected %s! The Character is %c\n", charCode, result);
                currentPosition = 0;
            }

            // Check if we've reached the end of the array
            if (currentPosition >= CHAR_ARR_SIZE)
            {
                currentPosition = 0;  // Wrap around to the start
            }

        }
    }
    // This Event detects a Black Bar.
    else if (events & GPIO_IRQ_EDGE_RISE)
    {
        last_event = EDGE_RISE;
        /*  If it detects Black as the last Position of the Array, 
            It should be ignored as the Barcode39 ends with a White at All Times. */
        if (currentPosition == 10)
        {
            // Do nothing.
        }
        else
        {
            // Check if the Value is a Wide or Thin Bar, based on the running average and thresholds.
            if ( (elapsed_seconds > MIN_THRESHOLD) && (elapsed_seconds < MAX_THRESHOLD) )
            {
                // Append the Wide Value into the charArr.
                appendArrayValue(charArr, WIDE_BLACK_BAR);
            }
            else if (elapsed_seconds < MIN_THRESHOLD)
            {
                // Append the Thin Value into the charArr.
                appendArrayValue(charArr, THIN_BLACK_BAR);
            }
            else
            {
                // Append a ? Value into the charArr to show running average is not accurate.
                appendArrayValue(charArr, "?");
            }

            white_surface_detected = false;
            black_surface_start_time = get_absolute_time();

            // Increment the current position.
            currentPosition++;

            concatenateArrayValue(charArr);
            // printf("\nCharacter Code is: %s\n\n", charCode);
            result = checkPattern(charCode);
            if (result != '\0')
            {
                printf("Pattern Detected %s! The Character is %c\n", charCode, result);
                currentPosition = 0;
            }

            // Check if we've reached the end of the array
            if (currentPosition >= CHAR_ARR_SIZE)
            {
                currentPosition = 0;  // Wrap around to the start
            }
        }
    }
}

int main()
{
    stdio_init_all();

    gpio_init(SENSOR_PIN);
    gpio_set_dir(SENSOR_PIN, GPIO_IN);
    // FALL or RISE will trigger the interrupt, true = enable interrupt
    gpio_set_irq_enabled_with_callback(SENSOR_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &white_surface_detected_handler);

    while (1)
    {
        if (white_surface_detected)
        {
            absolute_time_t current_time = get_absolute_time();
            // absolute_time_diff_us returns difference in ms between 2 timestamps
            elapsed_seconds = absolute_time_diff_us(white_surface_start_time, current_time) / 1000;

            printf("White -- Time elapsed: %d ms\n", elapsed_seconds);
            // printf("Current Counter: %d\n", currentPosition);
        }
        else
        {
            absolute_time_t current_time = get_absolute_time();
            elapsed_seconds = absolute_time_diff_us(black_surface_start_time, current_time) / 1000;

            printf("Black -- Time elapsed: %d ms\n", elapsed_seconds);
            // printf("Current Counter: %d\n", currentPosition);
        }
        
        // Add a delay to avoid rapid readings (adjust as needed)
        sleep_ms(50); // Sleep for 100 milliseconds
    }
    return 0;
}