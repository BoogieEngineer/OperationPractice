/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/24/2016.
 * Name of this file: C1A4E3_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program takes 3 user inputs, a character and 2 numbers, and calls a 
 * function that produces 4 different results depending on the number of inputs.
 * The function PrintLines is using default function arguments to be able to produce
 * 4 kinds of results depending on the number of inputs.
 */

#include <iostream>

using namespace std;

/* Function prototype below includes default arguments.
 * Here numberOfCharacters and numberOfLines are initialized both to 1,
 * and characterToDisplay is initialized to Z to display 'Z' in case the function is
 * called without any arguments.
 * This way, the progams works exactly the same as the previous program that calls
 * an overloading function.
 */
void PrintLines(int characterToDisplay = 'Z', int numberOfCharacters = 1, int numberOfLines = 1);

const int REQUIRED_ITERATIONS = 5;

int main()
{
    // Loop through REQUIRED_ITERATIONS iterations as required:
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
