/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/08/2016.
 * Name of this file: C1A6E3_GetSubstring.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This function copies some characters from an existing string (user input) and uses
 * them to create a new string.
 */

#include <string.h>

char *GetSubstring(const char source[], int start, int count, char result[])
{
    char *resultCopy = result;

    while ((*source) || (start))
    {
        source++;
        start--;
    }

    while (*source)
    {
        *result++ = *source++;
        if (*source == '\0')
            break;
    }

    result[(int)strlen(result)] = '\0';

    return(resultCopy);
}
