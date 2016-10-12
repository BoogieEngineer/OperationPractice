/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/11/2016.
 * Name of this file: C1A2E2_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 */

#include <iostream>

using namespace std;



int main(void)
{
    const char LEADER_CHAR    = '.'; // define leader character
    const char DIAGONAL_CHAR  = '%'; // define diagonal character
    // Define necessary variables:
    int numberOfLines; 

    // Taking user input:
    cout << "Enter number of lines (a positive decimal integer): ";
    cin >> numberOfLines;

    // Use nested "for" loops to print the result
    // "row" keeps track of number of lines
    // "column" keeps track of number of leader characters in a line
    for (int row = 1; row <= numberOfLines; ++row)
    {
        for (int column = 1; column < row; ++column)
        {
            cout << LEADER_CHAR;
        }
        cout << DIAGONAL_CHAR << "\n";
    }

    return 0;
}
