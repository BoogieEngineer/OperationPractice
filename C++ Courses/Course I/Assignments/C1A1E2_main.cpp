/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/03/2016.
 * Name of this file: C1A1E2_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * Names of functions defined in the file with a general overview of what
 * they do; descriptions of anything else in the file you consider relevant.
 * PLEASE BE ORIGINAL - PLEASE DO NOT COPY OR RESTATE MY INSTRUCTIONS.
 */

#include <iostream>
using namespace std;

int main()
{
    int userInputNumber; // define variable that will take value from user-input
    cout << "Please provide an integer: "; //prompting the user to provide input
    cin >> userInputNumber; // read user input

    // print the results
    cout << userInputNumber << " decimal = " 
        << oct << userInputNumber << " octal = " 
        << hex << userInputNumber << " hexadecimal\n"; 

    return 0;
}
