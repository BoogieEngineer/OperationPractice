/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/22/2016.
 * Name of this file: C1A8E3_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program searches for all occurrences of a pre-specified string in a text file
 * and replace them with another replacement string.
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

const int REQUIRED_ARGUMENTS = 4;
const int BUFFER_LENGTH = 1024;

// Enum to access command line arguments
enum CommandLineArgID
{
    IN_FILE = 1,
    OUT_FILE,
    STRING_TO_SEARCH,
    STRING_TO_REPLACE
};

int main(int argc, char *argv[])
{
    // Check if there's enough arguments
    if (argc < REQUIRED_ARGUMENTS + 1)
    {
        cerr << "Not enough command line arguments\n";
        exit(EXIT_FAILURE);
    }

    // Open the source file to read from
    ifstream sourceFile(argv[IN_FILE]);
    if (!sourceFile.is_open())
    {
        cerr << "Cannot open the source file " << argv[IN_FILE] << "\n";
        exit(EXIT_FAILURE);
    }

    // Open the destination file to write to
    fstream destinationFile(argv[OUT_FILE], ios_base::in | ios_base::in | ios_base::app);
    if (!destinationFile.is_open())
    {
        cerr << "Cannot open the destination file " << argv[OUT_FILE] << "\n";
        exit(EXIT_FAILURE);
    }

    // Perform the find and replace all algorithm per instructions
    char searchStringLength = (char)strlen(argv[STRING_TO_SEARCH]);
    while (!sourceFile.eof())
    {
        char *cp1, line[BUFFER_LENGTH];
        sourceFile.getline(line, sizeof(line), '\n');
        for (cp1 = line; char *cp2 = strstr(cp1, argv[STRING_TO_SEARCH]);)
        {
            destinationFile.write(cp1, cp2 - cp1);
            destinationFile << argv[STRING_TO_REPLACE];
            cp1 = cp2 + searchStringLength;
        }
        destinationFile << cp1 << '\n';
    }

    // Close the files
    sourceFile.close();
    destinationFile.close();

    return 0;
}
