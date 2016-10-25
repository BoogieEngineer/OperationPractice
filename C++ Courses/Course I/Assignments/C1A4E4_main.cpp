/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/25/2016.
 * Name of this file: C1A4E4_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program takes 3 numbers from user input and print out the maximum of them using
 * the function-like macros and inline functions in the header file C1A4E4_MaxOf.h.
 */

#include <iostream>
#include "C1A4E4_MaxOf.h"

using namespace std;

int main()
{
    long double firstNumber, secondNumber, thirdNumber;

    // Taking user input numbers:
    cout << "Enter 3 space-separated decimal numeric values: ";
    cin >> firstNumber >> secondNumber >> thirdNumber;

    // Calling the macros and inline functions and print out the results:
    cout << "mMaxOf3(" << firstNumber << ", " << secondNumber << ", " << thirdNumber
        << ") returned " << mMaxOf3(firstNumber, secondNumber, thirdNumber) << "\n";
    cout << "fMaxOf3(" << firstNumber << ", " << secondNumber << ", " << thirdNumber
        << ") returned " << fMaxOf3(firstNumber, secondNumber, thirdNumber) << "\n";

    return 0;
}
