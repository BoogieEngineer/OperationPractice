/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/17/2016.
 * Name of this file: C1A3E1_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This programs takes a user input (an positive integer), and builds a table that
 * contains the factorial of numbers from the input number down to 0.
 * This will not work above 12! because we choose to represent the result by a
 * long integer (not unsigned long), whose upper bound is (2^32-1), which is greater
 * than 12! but less than 13!.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int factorialMultiplier, inputNumber;
    long factorialResult = 1;

    printf("Enter a positive decimal integer: ");
    scanf("%d", &inputNumber);
    printf("nbr           nbr!\n");
    printf("------------------\n");

    for (factorialMultiplier = 1; factorialMultiplier <= inputNumber; ++factorialMultiplier)
    {
        factorialResult *= factorialMultiplier;
        printf("%2d    %12li\n", factorialMultiplier, factorialResult);
        /* As explained above, this will not work above 12! because we choose to 
         * represent the result by a long integer (not unsigned long), whose upper 
         * bound is (2^32-1), which is greater than 12! but less than 13!.
         */
    }

    return(EXIT_SUCCESS);
}
