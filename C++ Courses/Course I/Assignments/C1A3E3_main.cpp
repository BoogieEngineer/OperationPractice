/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/17/2016.
 * Name of this file: C1A3E3_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This programs converts a user-input integer into its value in words, e.g. 87 will be
 * converted to "eighty seven". Negative sign will also be read as "minus". 
 * Leading zeros from user input will be cut off.
 */

#include <iostream>

using namespace std;

const int MSD_EXTRACTOR = 10;

int main()
{
    int currentNumber;

    // Take user input
    cout << "Enter any integer value: ";
    cin >> currentNumber;

    // Print first part of the text
    cout << "\"" << currentNumber << "\" in words is \"";

    // Use a new bool to test if the input is non-negative
    bool inputIsNonNegative = currentNumber >= 0;

    /* 
     * If input number is not non-negative, convert it to non-negative
     * (0 remains 0) and print the word "minus" in the beginning of translation
     */
    if (!inputIsNonNegative)
    {
        currentNumber = -currentNumber;
        cout << "minus ";
    }

    // Implement algorithm Part 1:
    int divisor = 1; //, dividend = currentNumber;
    /*while (dividend > MSD_EXTRACTOR - 1)
    {
        divisor *= MSD_EXTRACTOR;
        dividend /= MSD_EXTRACTOR;
    }*/
    // for is better than the above while, using less var in the global scope (dividend)
    for (int dividend = currentNumber; dividend > MSD_EXTRACTOR - 1; dividend /= MSD_EXTRACTOR)
    {
        divisor *= MSD_EXTRACTOR;
    }

    // Implement algorithm Part 2: "for" loop without initial expression
    for (; divisor != 0; divisor /= MSD_EXTRACTOR)
    {
        int currentMostSignificantDigit = currentNumber / divisor;
        switch (currentMostSignificantDigit)
        {
        case 0: cout << "zero";  break;
        case 1: cout << "one";   break;
        case 2: cout << "two";   break;
        case 3: cout << "three"; break;
        case 4: cout << "four";  break;
        case 5: cout << "five";  break;
        case 6: cout << "six";   break;
        case 7: cout << "seven"; break;
        case 8: cout << "eight"; break;
        case 9: cout << "nine";  break;
        default: break;
        }

        /* 
         * If the next iteration of divisor is not 0, print a space to separate words
         * If all the digits have been translated, i.e. divisor == 0 in the next
         * interation, there'll be no space in the end of the translation.
         */
        if ((divisor / MSD_EXTRACTOR) != 0)
            cout << " ";

        currentNumber -= currentMostSignificantDigit * divisor;
    }

    cout << "\"\n";

    return 0;
}
