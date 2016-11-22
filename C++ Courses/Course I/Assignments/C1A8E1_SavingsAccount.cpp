/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/22/2016.
 * Name of this file: C1A8E1_SavingsAccount.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This implementation file contains the function definition for GetInitialValues()
 * that belongs to the SavingsAccount class. This function takes user inputs and write
 * them to various variables in the SavingsAccount class.
 */

#include <iostream>
#include "C1A8E1_SavingsAccount.h"

using namespace std;

void SavingsAccount::GetInitialValues()
{
    // Prompt the user for inputs and write them to variables in the SavingsAccount class
    cout << "Enter the account type: ";
    cin >> accountType;
    cout << "Enter the owner's name: ";
    // Get rid of whitespaces
    cin >> ws;
    getline(cin, ownerName);
    cout << "Enter the ID number: ";
    cin >> IDnbr;
    cout << "Enter the account balance: ";
    cin >> accountBalance;
    cout << "Enter the account closure penalty percent: ";
    cin >> accountClosurePenaltyPercent;
}
