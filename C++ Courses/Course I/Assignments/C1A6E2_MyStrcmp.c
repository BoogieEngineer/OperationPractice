/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/07/2016.
 * Name of this file: C1A6E2_MyStrcmp.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This function compares 2 strings.
 */

int MyStrcmp(const char *s1, const char *s2)
{
    // Simultaneously step through both strings one char at a time to
    // find the first unequal corresponding characters.
    while ((*s1) && (*s2))
    {
        // If 2 corresponding chars are unequal, return the difference between them
        if (*s1 != *s2)
        {
            return(*s1 - *s2);
        }
        s1++;
        s2++;
    }
    return 0;
}
