/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/25/2016.
 * Name of this file: C1A4E4_MaxOf.h
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This header file contains macros and inline functions that both determine the
 * maximum value of any 3 numbers of type long double.
 */

#ifndef C1A4E4_MAXOF_H // Include guard begins
#define C1A4E4_MAXOF_H

// Function-like macros to determine the maximum of any 3 numbers
#define mMaxOf2(firstNumber, secondNumber) ((firstNumber) > (secondNumber) ? \
    (firstNumber) : (secondNumber))
#define mMaxOf3(firstNumber, secondNumber, thirdNumber) \
    (mMaxOf2(mMaxOf2((firstNumber), (secondNumber)), (thirdNumber)))

// Inline functions to determine the maximum of any 3 numbers
inline long double fMaxOf2(long double firstNumber, long double secondNumber)
{
    return firstNumber > secondNumber ? firstNumber : secondNumber;
}

inline long double fMaxOf3(long double firstNumber, long double secondNumber,
    long double thirdNumber)
{
    return fMaxOf2(fMaxOf2(firstNumber, secondNumber), thirdNumber);
}

#endif // Inlcude guard ends
