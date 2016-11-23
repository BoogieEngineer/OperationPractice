/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/22/2016.
 * Name of this file: C1A8E3_main.cpp
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program seaches for a string and replace it with another string.
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int REQUIRED_ARGUMENTS = 4;
const int BUFFER_LENGTH = 1024;

enum CommandLineArgID
{
    IN_FILE = 1,
    OUT_FILE,
    STRING_TO_SEARCH,
    STRING_TO_REPLACE,
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
    const char * const sourceFileName = argv[IN_FILE];
    ifstream sourceFile(sourceFileName);
    if (!sourceFile.is_open())
    {
        cerr << "Cannot open the source file " << sourceFileName << "\n";
        exit(EXIT_FAILURE);
    }

    // Open the destination file to write to
    const char * const destinationFileName = argv[OUT_FILE];
    fstream destinationFile(destinationFileName, ios_base::in | ios_base::in | ios_base::app);
    if (!destinationFile.is_open())
    {
        cerr << "Cannot open the destination file " << destinationFileName << "\n";
        exit(EXIT_FAILURE);
    }

    while(!sourceFile.eof())
    {
        char line[BUFFER_LENGTH];
        sourceFile.getline(line, sizeof(line), '\n');
        char *cp1;
        for (cp1 = line; char *cp2 = strstr(cp1, argv[STRING_TO_SEARCH]);)
        {
            destinationFile.write(cp1, cp2 - cp1);
            destinationFile << argv[STRING_TO_REPLACE];
            cp1 = cp2 + strlen(argv[STRING_TO_SEARCH]);
        }
        destinationFile << cp1 << '\n';
    }

    sourceFile.close();
    destinationFile.close();

    return 0;
}
