/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/24/2016.
 * Name of this file: C1A4E2_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program takes 3 user inputs, a character and 2 numbers, and calls a overloading
 * function that produces 4 different results depending on the number of inputs.
 *
 */

#include <iostream>

using namespace std;

// Function prototypes:
void PrintLines(int characterToDisplay, int numberOfCharacters, int numberOfLines);
void PrintLines(int characterToDisplay, int numberOfCharacters);
void PrintLines(int characterToDisplay);
void PrintLines();

const int REQUIRED_ITERATIONS = 5;

int main()
{
    // Loop through requiredIteration iterations as required:
    for (int numberIterations = 1; numberIterations <= REQUIRED_ITERATIONS; ++numberIterations)
    {
        char characterToDisplay;
        int numberOfCharacters, numberOfLines;

        // Getting user inputs:
        cout << "Enter a character and 2 numbers: ";

        cin >> characterToDisplay >> numberOfCharacters >> numberOfLines;

        PrintLines(characterToDisplay, numberOfCharacters, numberOfLines);
        PrintLines(characterToDisplay, numberOfCharacters);
        PrintLines(characterToDisplay);
        PrintLines();
    }

    return 0;
}
