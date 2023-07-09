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
    for(SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
    {
        for(SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
        {
            SGK_iArray[SGK_i][SGK_j] = (SGK_i + 1)*(SGK_j + 1);
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");
    for(SGK_i = 0; SGK_i < NUM_ROWS;SGK_i++)
    {
        for(SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
        {
            printf("iArray[%d][%d] = %d \t \t At Address : %p\n",SGK_i,SGK_j,SGK_iArray[SGK_i][SGK_j],&SGK_iArray[SGK_i][SGK_j]);
        }
        printf("\n\n");
    }

    return(0);
}
