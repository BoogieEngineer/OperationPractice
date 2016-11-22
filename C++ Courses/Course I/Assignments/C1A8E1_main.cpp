/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/22/2016.
 * Name of this file: C1A8E1_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program use the SavingsAccount class to calculate the account closure penalty
 * and displays the result.
 */

#include <iostream>
#include "C1A8E1_SavingsAccount.h"

using namespace std;

int main()
{
    // Declare an object of the SavingsAccount class
    SavingsAccount Accounts;

    // Call the object's function and return the result
    Accounts.GetInitialValues();
    Accounts.DisplayValues();
    cout << "Account closure penalty is: " << Accounts.CalculatePenalty() << "\n";

    return 0;
}
