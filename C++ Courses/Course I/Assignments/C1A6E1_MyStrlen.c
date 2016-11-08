/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/07/2016.
 * Name of this file: C1A6E1_MyStrlen.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This function finds the length of a string.
 */

#include <stddef.h>

size_t MyStrlen(const char *s1)
{
    // Declare an extra pointer to be equal to the function parameter
    const char *lengthFinder = s1;

    // Step through the string to find "\0" character
    while (*lengthFinder)
        lengthFinder++;

    /* Return the string length (the number of objects between the last character
     * of the string found by the above while loop and the first character in the
     * original string). This can be found by taking the difference of the 2 pointers
     * now pointing to the last and first character of the original string, 
     * respectively
     */
    return (size_t)(lengthFinder - s1);
}
