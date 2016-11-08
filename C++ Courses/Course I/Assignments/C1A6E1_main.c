/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/07/2016.
 * Name of this file: C1A6E1_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program print the results of the standard strlen and of another function that
 * does the same thing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Set up string buffer length
#define LENGTH 129

// Function prototype for MyStrlen
size_t MyStrlen(const char *s1);

int main(void)
{
    char inputString[LENGTH];
    size_t length;

    // Getting user input and overwrite newline character with null terminator
    printf("Enter a string: ");
    fgets(inputString, LENGTH, stdin);
    length = strlen(inputString);
    if (length != 0 && inputString[length - 1] == '\n')
        inputString[--length] = '\0';

    // Print the results
    printf("strlen(\"%s\") returned %u\n", inputString, (unsigned int)length);
    printf("MyStrlen(\"%s\") returned %u\n", inputString, (unsigned int)MyStrlen(inputString));

    return(EXIT_SUCCESS);
}
