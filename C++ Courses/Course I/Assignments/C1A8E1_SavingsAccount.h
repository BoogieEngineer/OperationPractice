/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/22/2016.
 * Name of this file: C1A8E1_SavingsAccount.h
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This header contains a class that is used to manage savings accounts.
 */

// Include guard
#ifndef C1A8E1_SAVINGSACCOUNT_H
#define C1A8E1_SAVINGSACCOUNT_H

#include <iostream>
#include <string>

// Define a constant to account for percentage conversion
const double PERCENT = 0.01;

class SavingsAccount
{
    int accountType;
    std::string ownerName;
    long IDnbr;
    double accountBalance, accountClosurePenaltyPercent;

public:
    // Define required functions as public for use later in main function
    void GetInitialValues();
    inline void DisplayValues() const;

    inline double CalculatePenalty() const
    {
        return(accountBalance * accountClosurePenaltyPercent * PERCENT);
    }
};

inline void SavingsAccount::DisplayValues() const
{
    std::cout << "Account type is: " << accountType
        << "\nOwner name is: " << ownerName
        << "\nID number is: " << IDnbr
        << "\nAccount balance is: " << accountBalance
        << "\n Account closure penalty percent is: "
        << accountClosurePenaltyPercent << "\n";
}

#endif
