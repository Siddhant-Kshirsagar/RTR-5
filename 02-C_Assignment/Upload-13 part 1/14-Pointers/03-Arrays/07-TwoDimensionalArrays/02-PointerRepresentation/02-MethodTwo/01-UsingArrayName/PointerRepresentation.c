#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variable declarations
    int SGK_iArray[NUM_ROWS][NUM_COLUMNS];
    int SGK_i, SGK_j;

    //code
    // *** Name Of An Array Itself Is Its Base Address ***
    // *** Hence, 'iArray' Is Base Address of 2D Integer Array iArray[][]

    // iArray[5][3] => iArray Is A 2D Array Having 5 Rows And 3 Columns. Each Of These 5 Rows Is A 1D Integer Array Of 3 Integers ***
    // iArray[0] => is the 0th Row ... Hence, is the Base Address of Row 0 
    // iArray[1] => is the 1st Row ... Hence, is the Base Address of Row 1
    // iArray[2] => is the 4th Row ... Hence, is the Base Address of Row 4

    //(iArray[0] + 0) => Address of 0th Integer From Base Address of 0th Row (iArray[0][0])
    //(iArray[0] + 1) => Address of 1st Integer From Base Address of 0th Row (iArray[0][1])
    //(iArray[0] + 2) => Address of 2nd Integer From Base Address of 0th Row (iArray[0][2])

    //(iArray[1] + 0) => Address of 0th Integer From Base Address of 1st Row (iArray[1][0])
    //(iArray[1] + 1) => Address of 1st Integer From Base Address of 1st Row (iArray[1][1])
    //(iArray[1] + 2) => Address of 2nd Integer From Base Address of 1st Row (iArray[1][2])

    // iArray[0], iArray[1] ... ARE 1D Integer Arrays AND Hence Can Be Treated As 1D Integer Array Using Pointers ...
    // 'iArray' Is The Name and Base Address of 2D Integer Array ***
    // (*(iArray + 0) + 0) = (iArray[0] + 0) = Address of 0th Element From Base Address Of 0th Row = (iArray[0] + 0) = (iArray[0][0])
    // (*(iArray + 0) + 1) = (iArray[0] + 1) = Address of 1st Element From Base Address of 0th Row = (iArray[0] + 1) = (iArray[0][1])
    // (*(iArray + 0) + 2) = (iArray[0] + 2) = Address of 2nd Element From Base Address of 0th Row = (iArray[0] + 2) = (iArray[0][2])

    // (*(iArray + 1) + 0) = (iArray[1] + 0) = Address of 0th Element From Base Address of 1st Row = (iArray[1] + 0) = (iArray[1][0])
    // (*(iArray + 1) + 1) = (iArray[1] + 1) = Address of 1st Element From Base Address of 1st Row = (iArray[1] + 1) = (iArray[1][1])
    // (*(iArray + 1) + 2) = (iArray[1] + 2) = Address of 2nd Element From Base Address of 1st Row = (iArray[1] + 2) = (iArray[1][2])

    for(SGK_i =0; SGK_i< NUM_ROWS; SGK_i++)
    {
        for(SGK_j = 0; SGK_j <NUM_COLUMNS; SGK_j++)
        {
            *(*(SGK_iArray + SGK_i) + SGK_j) = (SGK_i + 1) * (SGK_j + 1); // 'iArray[i]' Can Be Treated As 1D Array Using Pointers ...
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for(SGK_i = 0; SGK_i< NUM_ROWS; SGK_i++)
    {
        for(SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
        {
            printf("*(*(iArray + %d ) + %d) = %d \t \t At Address (*(iArray + %d) + %d) : %p\n", SGK_i,SGK_j,*(*(SGK_iArray + SGK_i)+SGK_j),SGK_i,SGK_j,(*(SGK_iArray + SGK_i) + SGK_j));
        }
        printf("\n\n");
    }

    return(0);
}
