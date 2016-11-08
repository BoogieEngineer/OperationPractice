/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/07/2016.
 * Name of this file: C1A6E2_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This function finds the length of a string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Set up string buffer length
#define LENGTH 129

int MyStrcmp(const char *s1, const char *s2);

int main(void)
{
    char s1[LENGTH], s2[LENGTH];
    size_t length;

    // Getting user input and overwrite newline character with null terminator
    printf("Enter first string: ");
    fgets(s1, LENGTH, stdin);
    length = strlen(s1);
    if (length != 0 && s1[length - 1] == '\n')
        s1[--length] = '\0';

    printf("Enter second string: ");
    fgets(s2, LENGTH, stdin);
    length = strlen(s2);
    if (length != 0 && s2[length - 1] == '\n')
        s2[--length] = '\0';

    // Print the results
    printf("strcmp(\"%s\", \"%s\") returned %i\n", s1, s2, strcmp(s1, s2));
    printf("MyStrcmp(\"%s\", \"%s\") returned %i\n", s1, s2, MyStrcmp(s1, s2));

    return(EXIT_SUCCESS);
}
