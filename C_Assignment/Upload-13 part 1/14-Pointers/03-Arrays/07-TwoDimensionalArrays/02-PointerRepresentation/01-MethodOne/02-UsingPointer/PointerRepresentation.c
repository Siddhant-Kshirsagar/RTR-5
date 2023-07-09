#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variable declarations
    int SGK_iArray[NUM_ROWS][NUM_COLUMNS];
    int SGK_i,SGK_j;

    int *SGK_ptr_iArray_Row = NULL;

    //code
    // *** Every Row Of a 2D Array is an array itself comprising of 'NUM_COLUMNS' Integer Elements ***
    // *** There Are 5 Rows and 3 Columns In a 2D integer array. each of the 5 Rows is a 1D Array of 3 Integers.
    // *** Hence, Each Of These 5 Rows Themselves begin arrays, will be the base addresses of their respective Rows.

    for(SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
    {
        SGK_ptr_iArray_Row = SGK_iArray[SGK_i]; // 'iArray[i]' is the base address of ith Row...
        for(SGK_j = 0; SGK_j< NUM_COLUMNS; SGK_j++)
        {
            *(SGK_ptr_iArray_Row + SGK_j) = (SGK_i + 1) * (SGK_j + 1);// 'ptr_iArray_Row' (That is, 'iArray[i]' Can be Treated As 1D Array Using Pointers)...
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for(SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
    {
        SGK_ptr_iArray_Row = SGK_iArray[SGK_i];
        for(SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
        {
            printf("*(ptr_iArray_Row + %d) = %d \t \t At Address (ptr_iArray_Row + j) : %p\n",SGK_j,*(SGK_ptr_iArray_Row + SGK_j),(SGK_ptr_iArray_Row + SGK_j));
        }
        printf("\n\n");
    }

    return(0);
}

