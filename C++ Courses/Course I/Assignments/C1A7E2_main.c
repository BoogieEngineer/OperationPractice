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

#define LUNCH_ITEMS 3
#define FOOD_NAME_LENGTH 128

int main (void)
{
    struct Food
    {
        char *name;
        int weight, calories;
    } lunch[LUNCH_ITEMS] = { { 'Apple', 4, 100 }, { 'Salad', 2, 80 } };

    for (int iteration = 2; iteration < LUNCH_ITEMS; iteration++)
    {
        char foodName[FOOD_NAME_LENGTH];

        printf("Enter food name, weight and calories: ");
        fgets(foodName, FOOD_NAME_LENGTH, stdin);
        scanf("%i %i", &lunch[iteration].weight, &lunch[iteration].calories);
        
    }

    return(EXIT_SUCCESS)
}
