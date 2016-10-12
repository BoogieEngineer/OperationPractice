/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/11/2016.
 * Name of this file: C1A2E1_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program converts an uppercase character to lowercase.
 * It works by adding an offset of case numeral difference to an uppercase character
 * What it prints out is according to the ASCII codes.
 * 1. When the user enters anything other than an uppercase character, the offset
 * (case numeral difference) will still be added to the input character, and the
 * program still prints out the output according to the ASCII code. So, for example,
 * if the input is 0 (048 in ASCII code), the output will be 048 + 32 = 80, which
 * is the letter P.
 * 2. If the user precedes the input with a white space, the function cin.get will 
 * take the white space as its input. That is, the input will be the very first 
 * character that the user enters. The result will be @, according to the ASCII codes.
 */

#include <iostream>

using namespace std;

int main()
{
    // Define case numeral difference: 'a' - 'A':
    const char CASE_NUMERAL_DIFFERENCE = 'a' - 'A';

    // Declare the variables:
    char inputChar, outputChar;

    // Take user input:
    cout << "Enter a character: ";
    cin.get(inputChar);

    // Calculate the ouput:
    outputChar = (char)(inputChar + CASE_NUMERAL_DIFFERENCE);

    // Print out the result:
    cout << "The lowercase equivalent of " << inputChar << " is " << outputChar << "\n";

    return 0;
}
