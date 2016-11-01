/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/31/2016.
 * Name of this file: C1A5E2_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program takes 2 numbers from user input and prints the results of
 * two functions that compute the max and min of the numbers, respectively.
 */

#include <iostream>

using namespace std;

double &ComputeMinimum(const double &value1, const double &value2);
double &ComputeMaximum(const double &value1, const double &value2);

int main()
{
    double value1, value2;

    // Take user input:
    cout << "Enter 2 decimal numbers: ";
    cin >> value1 >> value2;

    // Print the main result:
    cout << "ComputeMinimum(" << value1 << ", " << value2
        << ") returned " << ComputeMinimum(value1, value2) << "\n";
    cout << "ComputeMaximum(" << value1 << ", " << value2
        << ") returned " << ComputeMaximum(value1, value2) << "\n";

    return 0;
}
