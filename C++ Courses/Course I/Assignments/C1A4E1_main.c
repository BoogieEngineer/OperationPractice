/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/23/2016.
 * Name of this file: C1A4E1_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program takes 2 numbers from user and print out the result of 
 * C1A4E1_ComputeMaximum.c and C1A4E1_ComputeMinimum.c that find the maximum and
 * minimum of the two input numbers, respectively.
 */

#include <stdio.h>
#include <stdlib.h>

// Declare function prototypes:
double ComputeMinimum(double firstNumber, double secondNumber);
double ComputeMaximum(double firstNumber, double secondNumber);

int main(void)
{
    double firstNumber, secondNumber;

    // Taking inputs from user:
    printf("Enter 2 decimal numbers (separated by a space): ");
    scanf("%lf %lf", &firstNumber, &secondNumber);

    // Print the main result:
    printf("ComputeMinimum(%f, %f) returned %f\n",
        firstNumber, secondNumber, ComputeMinimum(firstNumber, secondNumber));
    printf("ComputeMaximum(%f, %f) returned %f\n",
        firstNumber, secondNumber, ComputeMaximum(firstNumber, secondNumber));

    return(EXIT_SUCCESS);
}
