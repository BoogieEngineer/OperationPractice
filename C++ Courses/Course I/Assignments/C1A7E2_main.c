/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 11/15/2016.
 * Name of this file: C1A7E2_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This program takes user input for nutritional information regarding some kinds of
 * foods and display these information in a table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define some necessary macros
#define LUNCH_ITEMS 3
#define FOOD_NAME_LENGTH 128
#define Elements(A) (sizeof(A)/sizeof(*(A)))

int main(void)
{
    // Define the Food struct, declare "lunch" and initialize first 2 elements of "lunch"
    struct Food
    {
        char *name;
        int weight, calories;
    } lunch[LUNCH_ITEMS] = { { "Apple", 4, 100 }, { "Salad", 2, 80 } };

    // Loop to get user information for the uninitialized foods
    int iteration;
    for (iteration = 2; iteration < LUNCH_ITEMS; iteration++)
    {
        // Create a buffer for food name
        char foodName[FOOD_NAME_LENGTH];

        printf("Enter food name, weight and calories: ");
        //fgets(foodName, FOOD_NAME_LENGTH, stdin);
        scanf("%s %i %i", foodName, &lunch[iteration].weight, &lunch[iteration].calories);
        
        // Check if malloc is successful
        if ((lunch[iteration].name = (char *)malloc(Elements(foodName) * sizeof(char))) == NULL)
        {
            fputs("Out of memory\n", stderr);
            exit(EXIT_FAILURE);
        }
        // Copy user input string to food name using memcpy
        memcpy(lunch[iteration].name, foodName, sizeof(foodName));
        //printf("%i %i %s", (int*)strlen(foodName), strlen(lunch[iteration].name), lunch[iteration].name);
    }

    // Print the result table
    for (iteration = 0; iteration < LUNCH_ITEMS; iteration++)
    {
        printf("%-12s %7i %7i\n", lunch[iteration].name, lunch[iteration].weight, lunch[iteration].calories);
    }
    return(EXIT_SUCCESS);
}
