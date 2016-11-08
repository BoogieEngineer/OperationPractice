/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/07/2016.
 * Name of this file: C1A6E2_MyStrcmp.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This function compares 2 strings by stepping through them one char
 * at a time to spot the first unequal pair of characters.
 */

int MyStrcmp(const char *s1, const char *s2)
{
    // Simultaneously step through both strings one char at a time to
    // find the first unequal corresponding characters.
    while ((*s1) || (*s2))
    {
        /* If 2 corresponding chars are unequal, return the difference between them
         * This also applies for cases where one of the two chars being compared
         * is the null terminator.
         */
        if (*s1 != *s2)
        {
            return (int)(*s1 - *s2);
        }
        else if (*s1 == '\0' && *s2 != '\0')
            return(-*s2);
        else if (*s1 != '\0' && *s2 == '\0')
            return(*s1);
        s1++;
        s2++;
    }
    return 0;
}
