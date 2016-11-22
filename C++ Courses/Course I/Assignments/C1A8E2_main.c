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

int main(int argc, char *argv[])
{
    int numberOfLines = atoi(argv[2]);
    FILE *textFile = fopen(argv[1], "r");

    if (textFile == NULL)
    {
        fprintf(stderr, "Cannot open the required file\n");
        exit(EXIT_FAILURE);
    }

    fclose(textFile);

    return(EXIT_SUCCESS);
}
