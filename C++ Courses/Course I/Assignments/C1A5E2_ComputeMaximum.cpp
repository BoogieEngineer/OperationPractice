/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/31/2016.
 * Name of this file: C1A5E2_ComputeMaximum.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This function finds the greatest number of 2 doubles.
 */

double &ComputeMaximum(const double &value1, const double &value2)
{
    return (double &)(value1 > value2 ? value1 : value2);
}
