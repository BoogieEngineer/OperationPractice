/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/15/2016.
 * Name of this file: C1A7E1_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program takes arguments from user inpue, calls using these arguments and
 * prints the results of the function DetermineElapsedTime.
 */

#include <iostream>
#include <iomanip>
#include "C1A7E1_MyTime.h"

const int REQUIRED_ITERATION = 3;

using namespace std;

// Function prototye for DetermineElapsedTime
MyTime *DetermineElapsedTime(const MyTime *StartTime, const MyTime *StopTime);

int main()
{
    cout << setfill('0');
    for (int iteration = 0; iteration < REQUIRED_ITERATION; iteration++)
    {
        MyTime StartTime, StopTime, ElapsedTime;
        char delim1, delim2;

        // Get user input and parse it to start time and stop time
        cout << "Enter start time and stop time: ";
        cin >> StartTime.hours >> delim1 >> StartTime.minutes >> delim2 >> StartTime.seconds
            >> StopTime.hours >> delim1 >> StopTime.minutes >> delim2 >> StopTime.seconds;

        // Calculate the elapsed time by calling DetermineElapsedTime
        ElapsedTime = *DetermineElapsedTime(&StartTime, &StopTime);

        // Print the results. Fill with zeros as needed
        cout << "The time elapsed from " << setw(2) << StartTime.hours << delim1
            << setw(2) << StartTime.minutes << delim2
            << setw(2) << StartTime.seconds
            << " to " << setw(2) << StopTime.hours << delim1
            << setw(2) << StopTime.minutes << delim2
            << setw(2) << StopTime.seconds << " is "
            << setw(2) << ElapsedTime.hours << delim1
            << setw(2) << ElapsedTime.minutes << delim2
            << setw(2) << ElapsedTime.seconds << "\n";
    }
    return 0;
}
