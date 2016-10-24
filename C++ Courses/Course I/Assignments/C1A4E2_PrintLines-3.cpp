/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/24/2016.
 * Name of this file: C1A4E2_PrintLines-3.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * 
 */

#include <iostream>

using namespace std;

void PrintLines(int characterToDisplay, int numberOfCharacters, int numberOfLines)
{
    // This function takes 3 inputs and print numberOfLines lines of 
    // numberOfCharacters counts of the characterToDisplay character.
    for (int lineCount = 1; lineCount <= numberOfLines; ++lineCount)
    {
        for (int characterCount = 1; characterCount <= numberOfCharacters; ++characterCount)
        {
            cout << static_cast<char>(characterToDisplay);
        }
        cout << "\n";
    }
}
