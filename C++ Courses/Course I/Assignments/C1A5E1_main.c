/*
 * Student name: Minh Ha, UCSD student ID U07470714
 * Email address: hahongminh@gmail.com
 * Course: C/C++ Programming I, Section ID 118011, instructor: Raymond Mitchell.
 * Date: 10/31/2016.
 * Name of this file: C1A5E1_main.c
 * Operating system: Windows 7.
 * Compiler & version: Visual C++ 2010.
 *
 * This programs prompts the user to enter a number of responses to rate a product.
 * The response must be in range. If it is not, the user will be prompted to enter
 * the response again. If the user consecutively enters an out-of-range response
 * for more than a limit, the program will end.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_RESPONDENTS     20
#define MIN_RESPONSE_VALUE  3
#define MAX_RESPONSE_VALUE  15
#define OUT_OF_RANGE_LIMIT  2
#define RESPONSE_VALUES     (MAX_RESPONSE_VALUE - MIN_RESPONSE_VALUE + 1)

int main(void)
{
    // Declare necessary variables:
    int loopCount, ratingCounters[RESPONSE_VALUES] = {0};
    int consecutiveRangeErrors = 0;
    
    // Carry out the algorithm
    for (loopCount = 0; loopCount < MAX_RESPONDENTS;)
    {
        int response;

        // Geting user input:
        printf("Enter the rating of respondent #%i: ", loopCount + 1);
        scanf("%i", &response);

        /* If response is in range, set range error counter to 0, else increment it
         * Then if it is greater than the limit, break out of the loop, 
         * else continue on to the next iteration.
         */
        if (response >= MIN_RESPONSE_VALUE && response <= MAX_RESPONSE_VALUE)
        {
            consecutiveRangeErrors = 0;
        }
        else
        {
            ++consecutiveRangeErrors;
            if (consecutiveRangeErrors >= OUT_OF_RANGE_LIMIT)
                break;
            else
                continue;
        }

        // Increment the correct index for rating counter:
        ++ratingCounters[response - MIN_RESPONSE_VALUE];
        ++loopCount;
    }

    // Print table headers:
    printf("   Rating      Responses\n");
    printf("   ------      ---------\n");

    // Print the survey results:
    for (loopCount = 0; loopCount < RESPONSE_VALUES; ++loopCount)
    {
        printf("%7i        %5d\n", loopCount + MIN_RESPONSE_VALUE, ratingCounters[loopCount]);
    }
    
    return(EXIT_SUCCESS);
}
