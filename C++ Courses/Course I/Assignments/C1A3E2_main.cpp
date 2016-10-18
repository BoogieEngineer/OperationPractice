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
 * the minus sign if the input is a negative integer, cutting off any trailing 0s
 */

#include <iostream>

using namespace std;

int main()
{
    int inputNumber;

    cout << "Enter any integer value: ";
    cin >> inputNumber;

    // Use a new var to test if the input is positive
    bool inputIsPositive = inputNumber > 0;

    // If input number is negative, convert it to positive
    if (!inputIsPositive)
        inputNumber = -inputNumber;

    int reversedInput = (inputNumber % 10);
    float temp = reversedInput / 10;

    while (temp != 0)
    {
        reversedInput  = reversedInput % 10;
        temp = reversedInput / 10;
    }
        
    cout << "\"" << inputNumber << "\" in reverse is \"" << reversedInput << "\"\n";

    return 0;
}
