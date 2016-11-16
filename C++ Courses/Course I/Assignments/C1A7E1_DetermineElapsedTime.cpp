/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/15/2016.
 * Name of this file: C1A7E1_DetermineElapsedTime.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This file contains a function that computes the elapsed time between 2 different
 * timestamps, the start time and the stop time. If the stop time is less than or
 * equal to the start time, the function will determine that the stop time is for
 * the next day.
 */

#include "C1A7E1_MyTime.h"

// Define some constants
const int SECONDS_PER_MINUTE = 60;
const int MINUTES_PER_HOUR = 60;
const int HOURS_PER_DAY = 24;

MyTime *DetermineElapsedTime(const MyTime *StartTime, const MyTime *StopTime)
{
    // Declare necessary variables
    MyTime startTime = *StartTime, stopTime = *StopTime;
    static MyTime elapsedTime;

    /* Calculate the elapsed time from seconds to minutes to hours. If in the result,
     * the elapsed hours is negative, it means that the stop time is for the next day,
     * which means the negative elapsed time should get an addition of 24 hours.
     */
    elapsedTime.seconds = stopTime.seconds - startTime.seconds;
    if (elapsedTime.seconds < 0)
    {
        elapsedTime.seconds += SECONDS_PER_MINUTE;
        stopTime.minutes--;
    }

    elapsedTime.minutes = stopTime.minutes - startTime.minutes;
    if (elapsedTime.minutes < 0)
    {
        elapsedTime.minutes += MINUTES_PER_HOUR;
        stopTime.hours--;
    }

    elapsedTime.hours = stopTime.hours - startTime.hours;
    if (elapsedTime.hours < 0)
    {
        elapsedTime.hours += HOURS_PER_DAY;
    }

    if (elapsedTime.hours == 0 && elapsedTime.minutes == 0 && elapsedTime.seconds == 0)
        elapsedTime.hours = HOURS_PER_DAY;
    
    // Return the elapsed time
    return(&elapsedTime);
}
