#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
	//variable declarations
	int SGK_iArray_2d[NUM_ROWS][NUM_COLUMNS]; //TOTAL NUMBER OF ELEMENTS = NUM_ROWS * NUM_COLUMNS
	int SGK_iArray_1D[NUM_ROWS * NUM_COLUMNS];

	int SGK_i, SGK_j;
	int SGK_num;

	//code 
	printf("Enter Elements Of Your Choice to Fill Up The Integer 2D Array : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		printf("For ROW NUMBER %d : \n", (SGK_i + 1));
		for (SGK_j = 0; SGK_j < NUM_COLUMNS ; SGK_j++)
		{
			printf("Enter Element Number %d : \n", (SGK_j + 1));
			scanf("%d", &SGK_num);
			SGK_iArray_2d[SGK_i][SGK_j] = SGK_num;
		}
		printf("\n\n");
	}

	// *** Display Of 2D Array ***
	printf("\n\n");
	printf("Two - Dimensional ( 2D ) Array Of Integers : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		printf("********* ROW %d **********\n",(SGK_i+1));
		for (SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
		{
			printf("iArray_2d[%d][%d] = %d \n", SGK_i, SGK_j, SGK_iArray_2d[SGK_i][SGK_j]);
		}
		printf("\n\n");
	}
	// *** CONVERTING 2D INTEGER ARRAY TO 1 1D INTEGER ARRAY ***	
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
		{
			SGK_iArray_1D[(SGK_i * NUM_COLUMNS) + SGK_j] = SGK_iArray_2d[SGK_i][SGK_j];
		}
	}
	
	// *** PRINTING 1D Array ***
	printf("\n\n");
	printf("One-Dimensional (1D) Array Of Integers : \n\n");
	for (SGK_i = 0; SGK_i < (NUM_ROWS * NUM_COLUMNS); SGK_i++)
	{
		printf("iArray_1d[%d] = %d\n", SGK_i, SGK_iArray_1D[SGK_i]);
	}

	printf("\n\n");

	return(0);
}

