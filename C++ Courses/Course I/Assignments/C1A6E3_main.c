/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/08/2016.
 * Name of this file: C1A6E3_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program print the result of the function GetSubstring that basically extracts
 * a number of characters starting from an index of a given (user input) string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define LENGTH 256 // Define input string length

// Function prototype for GetSubstring
char *GetSubstring(const char source[], int start, int count, char result[]);

int main(void)
{
    char source[LENGTH], result[LENGTH];
    int start, count;
    size_t length;

    // Get the source string and trim the newline character off of the string
    printf("Enter a string: ");
    fgets(source, LENGTH, stdin);
    length = strlen(source);
    if (length != 0 && source[length - 1] == '\n')
        source[--length] = '\0';

    // Get "start" and "count"
    printf("Enter start index and character count to copy: ");
    scanf("%i %i", &start, &count);

    // Print the results per the requirements
    printf("\"%s\", %i, %i, extracts \"%s\"\n", source, start, count,
        GetSubstring(source, start, count, result));

    return(EXIT_SUCCESS);
}
