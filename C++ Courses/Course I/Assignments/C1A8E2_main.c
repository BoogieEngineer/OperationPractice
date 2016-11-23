/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/22/2016.
 * Name of this file: C1A8E2_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program display the content of a text file in a number of groups separated by
 * a user-prompt Enter to continue viewing the file content.
 */

#include <stdio.h>
#include <stdlib.h>

#define LAST_COMMAND_LINE_INPUT_RIGHT_INDEX 1
#define SECOND_LAST_COMMAND_LINE_INPUT_RIGHT_INDEX 2

int main(int argc, char *argv[])
{
    // Declare necessary variables
    char *fileName = argv[argc - SECOND_LAST_COMMAND_LINE_INPUT_RIGHT_INDEX];
    int lineGroupLength = atoi(argv[argc - LAST_COMMAND_LINE_INPUT_RIGHT_INDEX]),
        inputValue, lineCount = 0;

    // Open the file
    FILE *file = fopen(fileName, "r");

    // Check if the open file action is successful
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open the required file\n");
        exit(EXIT_FAILURE);
    }

    // Print one char at a time until getting EOF
    while ((inputValue = getc(file)) != EOF)
    {
        printf("%c", inputValue);
        // If encountering newline, increase the line count. Once line count reaches
        // the required line group length, stop and wait for user to press Enter
        if (inputValue == '\n')
        {    
            lineCount++;
            if (lineCount == lineGroupLength)
            {
                // If user presses anything rather than Enter, break
                if (getchar() != '\n')
                    break;
                lineCount = 0;
            }
        }
    }

    fclose(file);

    return(EXIT_SUCCESS);
}
