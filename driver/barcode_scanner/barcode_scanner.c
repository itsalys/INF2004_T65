#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/adc.h"

#define ADC_NUM 0
#define BARCODE_PIN 26 // GPIO pin 26 ADC0
#define ADC_VREF 3.3
#define ADC_RANGE (1 << 12)                      // 2^12
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1)) // 3.3/4097
#define NUM_BLACK_BAR 15
#define NUM_WHITE_BAR 15


void init_barcode();
void combineArray(char bars[], char spaces[], char combined[]);
void decipherBarcode(char* combined);
void printArray(char arr[], int size);
char compareArray(char a[], char b[], int size);
void decodeBarcode();
void detectBar(int adc_reading);
void barcode_IRQ();

static volatile float curatedRes;
int adcResult, adcInterrupt, startTime = 0;
int totalAdc, avgResult = 0;

int avg, currentTime, whiteCounter, blackCounter = 0;
int isBlack = 1;
int lineThreshold = 2000;
int avgBlackTime, sumBlackTime, avgWhiteTime, sumWhiteTime, i = 0;
int blackLineCount, whiteLineCount = 0;
int whiteOffSet = 1;

char black[15];
char white[15];
char barcode[10];
char combined[30];
char firstChar[10];
char thirdChar[10];
char bar[15];
char space[15];

/* line sensor */
void init_barcode()
{
    adc_init();
    adc_gpio_init(BARCODE_PIN + ADC_NUM); // make sure GPIO is high-impedance, no pullups
    adc_select_input(ADC_NUM);            // GPIO 26 ADC 0
}

/* To combine the black and white bar*/
void combineArray(char bars[], char spaces[], char combined[])
{
    int i = 0, j = 0, k = 0;

    for (i = 0; i < 30; i++)
    {
        if (i % 2 == 0)
        {
            combined[i] = bars[k];
            k++;
        }
        else
        {
            combined[i] = spaces[j];
            j++;
        }
    }
}

/* To break down the array to start, barcode, end arrays*/
void decipherBarcode(char *combined)
{
    int i = 0;
    for (i = 10; i < 20; i++)
    {
        if (i < 10)
        {
            firstChar[i - 1] = combined[i];
        }
        else if (i >= 10 && i < 19)
        {
            barcode[i - 10] = combined[i];
        }
        else if (i >= 21 && i < 30)
        {
            thirdChar[i - 21] = combined[i];
        }
    }
}

/* To print array element */
void printArray(char arr[], int size)
{
    int i = 0;

    printf("\nElements are : ");
    for (i = 0; i < size; i++)
    {
        printf("\n\tarr[%d] : %d", i, arr[i]);
    }
    printf("\n");
}

/* To compare array elements */
char compareArray(char a[], char b[], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (a[i] != b[i])
            return 1;
    }
    return 0;
}

/*decode barcode*/
void decodeBarcode()
{
    char A[16] = {1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0};
    char B[16] = {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0};
    char C[16] = {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0};
    char D[16] = {1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0};
    char E[16] = {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0};
    char F[16] = {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0};
    char G[16] = {1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0};
    char H[16] = {1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0};
    char I[16] = {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0};
    char J[16] = {1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0};
    char K[16] = {1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0};
    char L[16] = {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0};
    char M[16] = {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0};
    char N[16] = {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1 ,0};
    char O[16] = {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0};
    char P[16] = {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0};
    char Q[16] = {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0};
    char R[16] = {1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0};
    char S[16] = {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0};
    char T[16] = {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0};
    char U[16] = {1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0};
    char V[16] = {1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0};
    char W[16] = {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0};
    char X[16] = {1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0};
    char Y[16] = {1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0};
    char Z[16] = {1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0};
    char start[16] = {1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0};
    char end[16] = {1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1};

    if (compareArray(A, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is A\n");
    }
    else if (compareArray(B, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is B\n");
    }
    else if (compareArray(C, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is C\n");
    }
    else if (compareArray(D, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is D\n");
    }
    else if (compareArray(E, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is E\n");
    }
    else if (compareArray(F, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is F\n");
    }
    else if (compareArray(G, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is G\n");
    }
    else if (compareArray(H, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is H\n");
    }
    else if (compareArray(I, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is I\n");
    }
    else if (compareArray(J, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is J\n");
    }
    else if (compareArray(K, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is K\n");
    }
    else if (compareArray(L, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is L\n");
    }
    else if (compareArray(M, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is M\n");
    }
    else if (compareArray(N, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is N\n");
    }
    else if (compareArray(O, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is O\n");
    }
    else if (compareArray(P, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is P\n");
    }
    else if (compareArray(Q, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is Q\n");
    }
    else if (compareArray(R, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is R\n");
    }
    else if (compareArray(S, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is S\n");
    }
    else if (compareArray(T, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is T\n");
    }
    else if (compareArray(U, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is U\n");
    }
    else if (compareArray(V, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is V\n");
    }
    else if (compareArray(W, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is W\n");
    }
    else if (compareArray(X, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is X\n");
    }
    else if (compareArray(Y, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is Y\n");
    }
    else if (compareArray(Z, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is Z\n");
    }
    else if (compareArray(start, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is *\n");
    }
    else if (compareArray(end, barcode, 16) == 0)
    {
        printArray(barcode, 16);
        printf("Character identified is *\n");
    }
    else
    {
        printArray(barcode, 16);
        printf("Different elements, no matches\n");
    }
}

/*get barcode reading*/
void detectBar_raw(int adc_reading)
{
    // barcode
    if (adc_reading > lineThreshold && !isBlack)
    {
        currentTime = time_us_32() - startTime;
        startTime = time_us_32();
        black[blackCounter] = currentTime;
        blackCounter++;
        isBlack = 1;
        //printf("BLACK %lld\n", currentTime);
    }
    else if (adc_reading < lineThreshold && isBlack)
    {
        currentTime = time_us_32() - startTime;
        startTime = time_us_32();          // reset startTime to capture the time now
        white[whiteCounter] = currentTime; // set the white bar benchmark
        whiteCounter++;
        isBlack = 0;
        //printf("WHITE %lld\n", currentTime);
    }

    if (blackCounter == NUM_BLACK_BAR) // check if it detect all the black bar
    {
        sumBlackTime = 0;
        avgBlackTime = 0;

        for (i = 0; i < blackCounter; i++) // sum black timing
        {
            sumBlackTime += black[i]; // sum of timing
        }
        avgBlackTime = sumBlackTime / (NUM_BLACK_BAR);

        // add into the barcode array [0] to [8] -> 9 items
        for (i = 0; i < blackCounter; i++)
        {
            if (black[i] < avgBlackTime) // thin black bar
            {
                bar[i] = 0;
            }
            else // thick black bar
            {
                bar[i] = 1;
            }
        }

        // find thick / thin timing
        for (i = whiteOffSet; i < whiteCounter; i++) // sum white timing
        {
            sumWhiteTime += white[i];
        }
        avgWhiteTime = sumWhiteTime / (NUM_WHITE_BAR - 1);

        for (i = 1; i < whiteCounter; i++)
        {
            if (white[i] < avgWhiteTime) // thin white bar
            {
                space[i] = 0;
            }
            else
            { // thick white bar
                space[i] = 1;
            }
        }
        combineArray(bar, space, combined);
        decipherBarcode(combined);
        printArray(combined, 30);
        decodeBarcode();

        blackCounter = 0;
        whiteCounter = 0;
    }

    //reset variable;
    totalAdc = 0;
    adcInterrupt = 0;
    memset(barcode, 0, 9);
    memset(bar, 0, 15);
    memset(space, 0, 15);
    memset(combined, 0, 30);
}

/*barcode irq*/
void barcode_IRQ()
{
    // barcode_sampling();
    int adc_raw = adc_read(); // raw voltage from ADC

    // printf("Raw value: %u, voltage: %.2f V\n", adc_raw, curatedRes);
    totalAdc += adc_raw;
    adcInterrupt++;

    if (adcInterrupt == 1000) // take the average after 999 sampling
    {
        avgResult = totalAdc / 1000;
        curatedRes = avgResult * ADC_CONVERT; // change to voltage
        if (startTime == 0)
        {
            startTime = time_us_32();
        }

        //printf("%.2f", curatedRes);
        detectBar_raw(avgResult);
    }
} 


// int main()
// {
//     stdio_init_all();
//     init_barcode();

//     while (1)
//     {
//       barcode_IRQ();
//     }
//     return 0;
// }