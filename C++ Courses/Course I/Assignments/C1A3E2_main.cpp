/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/17/2016.
 * Name of this file: C1A3E2_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This programs takes a user-input number and display the number in reverse, including
 * the minus sign if the input is a negative integer, cutting off any leading 0s
 */

#include <iostream>

using namespace std;

int main()
{
    int currentNumber;

    // Take user input
    cout << "Enter any integer value: ";
    cin >> currentNumber;

    // Print first part of the text
    cout << "\"" << currentNumber << "\" in reverse is \"";

    // Use a new bool to test if the input is non-negative
    bool inputIsNonNegative = currentNumber >= 0;

    // If input number is negative, convert it to positive
    if (!inputIsNonNegative)
        currentNumber = -currentNumber;

    // Loop through the algorithm to arrange the digits in reverse order
    // At least 1 iteration is need, so "do" loop was chosen.
    do
    {
        int currentLeastSignificantDigit = currentNumber % 10;
        cout << currentLeastSignificantDigit;
        currentNumber /= 10;
    } while (currentNumber != 0);
    
    // If the original input was negative, print also a minus sign in the end
    if (!inputIsNonNegative)
        cout << "-";

    cout << "\"\n";

    return 0;
}
