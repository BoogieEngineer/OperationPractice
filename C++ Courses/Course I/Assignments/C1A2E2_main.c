/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/11/2016.
 * Name of this file: C1A2E2_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This programs takes a user input (a positive integer) and print that number of 
 * lines, consisting of diagonal lines of a certain leader character and diagonal
 * character.
 */

#include <stdio.h>
#include <stdlib.h>

#define LEADER_CHAR     '.' // define leader character
#define DIAGONAL_CHAR   '%' // define diagonal character

int main(void)
{
    // Define necessary variables:
    int numberOfLines, row; 

    // Taking user input:
    printf("Enter number of lines (a positive decimal integer): ");
    scanf("%d", &numberOfLines);

    // Use nested "for" loops to print the result
    // "row" keeps track of number of lines
    // "column" keeps track of number of leader characters in a line
    for (row = 0; row <= numberOfLines; ++row)
    {
        int column;
        for (column = 0; column < row; ++column)
        {
            printf("%c", LEADER_CHAR);
        }
        printf("%c\n", DIAGONAL_CHAR);
    }

    return(EXIT_SUCCESS);
}
