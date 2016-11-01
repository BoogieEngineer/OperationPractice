/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/31/2016.
 * Name of this file: C1A5E3_ComputeMaximum.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This function compares 2 doubles and returns the pointer to the greatest number.
 */

double *ComputeMaximum(const double *value1, const double *value2)
{
    // Compare the values but return the pointer pointing to the max value
    return (double *)(*value1 > *value2 ? value1 : value2);
}
