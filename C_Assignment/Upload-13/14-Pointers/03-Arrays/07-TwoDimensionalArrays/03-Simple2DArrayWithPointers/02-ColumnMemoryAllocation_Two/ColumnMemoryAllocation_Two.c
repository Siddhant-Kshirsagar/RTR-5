#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS_ONE 3
#define NUM_COLUMNS_TWO 8

int main(void)
{
	//variable declarations
	int *SGK_iArray[NUM_ROWS]; // A 2D array which will have 5 rows and number of columns can be decided later on...
	int SGK_i, SGK_j;

	//code
	// assign value to each integer pointer to NULL as good programming practice
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		SGK_iArray[SGK_i] = NULL;
	}

	// *** One(Allocating Memory For An Array Of 3 Integers Per Row) ***
	printf("\n\n");
	printf("***** First Memory Allocation To 2D Integer Array *****\n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		SGK_iArray[SGK_i] = (int *)malloc(NUM_COLUMNS_ONE * sizeof(int));
		if (SGK_iArray[SGK_i] == NULL)
		{
			printf("Failed To Allocate Memory To Row %d Of 2D Integer Array !!! Exitting Now ...\n\n", SGK_i);
			exit(0);
		}
		else
		{
			printf("Memory Allocation To Row %d Of 2D Integer Array Succeeded !!! \n\n", SGK_i);
		}
	}

	// Assigning values to 2D Array ...
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_COLUMNS_ONE; SGK_j++)
		{
			SGK_iArray[SGK_i][SGK_j] = (SGK_i + 1) * (SGK_j + 1);
		}
	}

	// Displaying 2D Array ...
	printf("\n\n");
	printf("Displaying 2D Array : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_COLUMNS_ONE; SGK_j++)
		{
			printf("iArray[%d][%d] = %d\n", SGK_i, SGK_j, SGK_iArray[SGK_i][SGK_j]);
		}
		printf("\n\n");
	}
	printf("\n\n");

	// Freeing Memory Assigned To 2D Array(Must Be Done In Reverse Order)
	for (SGK_i = (NUM_ROWS - 1); SGK_i >= 0; SGK_i--)
	{
		if (SGK_iArray[SGK_i])
		{
			free(SGK_iArray[SGK_i]);
			SGK_iArray[SGK_i] = NULL;
			printf("Memory Allocated To Row %d of 2D integer Array Has Been Successfully Freed !!!\n\n", SGK_i);
		}
	}

	// *** Two (Allocating Memory For An Array Of 8 Integer Per Row) ***
	printf("\n\n");
	printf(" ***** Second Memory Allocation To 2D Integer Array ***** \n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		SGK_iArray[SGK_i] = (int *)malloc(NUM_COLUMNS_TWO * sizeof(int));
		if (SGK_iArray[SGK_i] == NULL)
		{
			printf("Failed To Allocate Memory to row %d of 2D integer array !!! Exitting Now ... \n\n", SGK_i);
			exit(0);
		}
		else
		{
			printf("Memory Allocation To Row %d Of 2D Integer Array Succeeded !!!\n\n", SGK_i);
		}
	}

	// Assigning Values to 2D Array...
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_COLUMNS_TWO; SGK_j++)
		{
			SGK_iArray[SGK_i][SGK_j] = (SGK_i + 1) * (SGK_j + 1);
		}
	}

	// Displaying 2D Array ...
	printf("\n\n");
	printf("Displaying 2D Array : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_COLUMNS_TWO; SGK_j++)
		{
			printf("iArray[%d][%d] = %d \n", SGK_i, SGK_j, SGK_iArray[SGK_i][SGK_j]);
		}
		printf("\n\n");
	}
	printf("\n\n");

	//Freeing Memory Assigned To 2D Array (Must Be Done In Reverse Order )
	for (SGK_i = (NUM_ROWS - 1); SGK_i >= 0; SGK_i--)
	{
		if (SGK_iArray[SGK_i])
		{
			free(SGK_iArray[SGK_i]);
			SGK_iArray[SGK_i] = NULL;
			printf("Memory ALlocated To Row %d of 2D Integer Array Has Been Successfully Freed !!! \n\n", SGK_i);
		}
	}

	return(0);
}


