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

char *GetSubstring(const char source[], int start, int count, char result[])
{
    // Create a copy of "result" string to return later
    char *resultCopy = result;

    // 1st loop to determine the chracter of "source" to start copying
    while (*source && start)
    {
        source++;
        start--;
    }

    // 2nd loop to copy the characters over to "result"
    while (*source && count)
    {
        *result++ = *source++;
        count--;
    }

    // Append the null terminator to the end of result
    // After the 2nd loop above, *result should now be pointing the end of "result"
    *result = '\0';

    // Return the copy of "result"
    return(resultCopy);
}
