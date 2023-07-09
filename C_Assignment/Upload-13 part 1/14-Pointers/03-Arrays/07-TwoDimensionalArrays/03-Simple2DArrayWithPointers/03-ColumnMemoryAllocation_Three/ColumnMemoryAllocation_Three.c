#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 5

int main(void)
{
	//variable declarations
	int *SGK_iArray[NUM_ROWS]; // A 2D Array which will have 5 Rows and number of columns can be decide later on ...
	int SGK_i, SGK_j;

	//code

	//assign value to each integer pointer to NULL as good programming practice
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		SGK_iArray[SGK_i] = NULL;
	}

	// Memory Allocation
	printf("\n\n");
	printf(" ***** Memory Allocation To 2D Integer Array *****\n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		//Row 0 will have (NUM_COLUMNS - 0 ) = (5-0) = 5 COLUMNS...
		//Row 1 will have (NUM_COLUMNS - 1 ) = (5-1) = 4 COLUMNS...
		//Row 2 will have (NUM_COLUMNS - 2 ) = (5-2) = 3 COLUMNS...
		//Row 3 will have (NUM_COLUMNS - 3 ) = (5-3) = 2 COLUMNS...
		//Row 4 will have (NUM_COLUMNS - 4 ) = (5-4) = 1 COLUMNS...

		// Because of this, there is no contiguous memory allocation ...hence, although we may use th data as a 2D array, it is not really a 2D array in memory...

		SGK_iArray[SGK_i] = (int *)malloc((NUM_COLUMNS - SGK_i) * sizeof(int));
		if (SGK_iArray[SGK_i] == NULL)
		{
			printf("Failed To Allocate Memory To Row %d of 2D Integer Array !!! Exitting Now... \n\n", SGK_i);
			exit(0);
		}
		else
		{
			printf("Memory Allocation To Row %d of 2D Integer Array Succeeded !!!\n\n", SGK_i);
		}
	}

	// Assign value 
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < (NUM_COLUMNS - SGK_i); SGK_j++)
		{
			SGK_iArray[SGK_i][SGK_j] = (SGK_i * 1) + (SGK_j * 1);
		}
	}

	// Displaying Value 
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < (NUM_COLUMNS - SGK_i); SGK_j++)
		{
			printf("iArray[%d][%d] = %d \t At Address : %p\n", SGK_i, SGK_j, SGK_iArray[SGK_i][SGK_j], &SGK_iArray[SGK_i][SGK_j]);
		}
		printf("\n\n");
	}
	printf("\n\n");

	for (SGK_i = (NUM_ROWS - 1); SGK_i >= 0; SGK_i--)
	{
		if (SGK_iArray[SGK_i])
		{
			free(SGK_iArray[SGK_i]);
			SGK_iArray[SGK_i] = NULL;
			printf("Memory Allocated To Row %d Has Been Successfully Freed !!!\n\n", SGK_i);
		}
	}

	return(0);
}

