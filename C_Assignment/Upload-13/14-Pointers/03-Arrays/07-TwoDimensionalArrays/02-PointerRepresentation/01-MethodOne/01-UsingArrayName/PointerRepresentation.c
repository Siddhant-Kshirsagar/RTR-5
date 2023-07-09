#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variable declarations
    int SGK_iArray[NUM_ROWS][NUM_COLUMNS];
    int SGK_i,SGK_j;

    //code
    // *** Every Row of a 2D Array Is An Integer Array Itself Comprising of 'NUM_COLUMNS' Integer ELEMENTS ***
    // *** There are 5 rows and 3 columns in a 2D integer array. Each Of The 5 Rows Is a 1D Array of 3 Integers.
    // *** Hence, Each of these 5 rows themselves begin arrays, will be the base addresses of their respective rows

    for(SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
    {
        for(SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
        {
            *(SGK_iArray[SGK_i] + SGK_j) = (SGK_i + 1) * (SGK_j + 1); // 'iArray[i]' can be treated as 1d array using pointers ...
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for(SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
    {
        for(SGK_j = 0; SGK_j < NUM_COLUMNS;SGK_j++)
        {
            printf("*(iArray[%d] + %d) = %d \t \t Address (iArray[i] + j) : %p\n",SGK_i,SGK_j,*(SGK_iArray[SGK_i]+SGK_j),(SGK_iArray[SGK_i]+SGK_j));
        }
        printf("\n\n");
    }

    return(0);
}

