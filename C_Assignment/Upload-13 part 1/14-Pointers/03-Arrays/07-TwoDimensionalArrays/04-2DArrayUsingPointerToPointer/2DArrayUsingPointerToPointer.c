#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	//variable declarations
	int **SGK_ptr_iArray = NULL; // A pointer-to-pointer to integer ... but can also hold the base address of a 2D array which will can have any number of rows and any number of columns...
	int SGK_i, SGK_j;
	int SGK_num_rows, SGK_num_columns;

	//code

	// *** Accept Number of Rows 'num_rows' from user ***
	printf("\n\n");
	printf("Enter Number of Rows : ");
	scanf("%d", &SGK_num_rows);

	// *** Accept Number Of Columns 'num_columns' from user ***
	printf("\n\n");
	printf("Enter Number of Columns : ");
	scanf("%d", &SGK_num_columns);

	// *** Allocating Memory To 1D Array Consisting of Base Address of Rows ***
	printf("\n\n");
	printf("***** Memory Allocation To 2D Integer Array ***** \n\n");
	SGK_ptr_iArray = (int **)malloc(SGK_num_rows * sizeof(int *));
	if (SGK_ptr_iArray == NULL)
	{
		printf("Failed to allocate memory to %d Rows of 2D Integer Array !!! Exitting Now ... \n\n", SGK_num_rows);
		exit(0);
	}
	else
	{
		printf("Memory Allocation To %d Rows Of 2D Integer Array Succeeded \n\n", SGK_num_rows);
	}

	// assign value to each integer pointer to NULL as good programming practice
	for (SGK_i = 0; SGK_i < SGK_num_rows; SGK_i++)
	{
		SGK_ptr_iArray[SGK_i] = NULL;
	}

	// *** Allocating memory to each row which is a 1D Array Containing of columns  which contain the actual integers ***
	for (SGK_i = 0; SGK_i < SGK_num_rows; SGK_i++)
	{
		SGK_ptr_iArray[SGK_i] = (int *)malloc(SGK_num_columns * sizeof(int)); // allocating memory (number of columns * size of 'int') to row 'i'
		if (SGK_ptr_iArray[SGK_i] == NULL) // Row 'i' memory allocated ? 
		{
			printf("Failed To Allocate Memory To Columns Of Row %d Of 2D Integer Array !!! Exitting Now...\n\n", SGK_i);
			exit(0);
		}
		else
		{
			printf("Memory Allocation To Columns Of Row %d Of 2D Integer Array Succeeded !!! \n\n", SGK_i);
		}
	}	

	// *** Assign Values *** 
	for (SGK_i = 0; SGK_i < SGK_num_rows; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < SGK_num_columns; SGK_j++)
		{
			SGK_ptr_iArray[SGK_i][SGK_j] = (SGK_i * 1) + (SGK_j * 1); // can also use : *(*(ptr_iArray + i)+j) = (i * 1) + (j * 1)
		}
	}

	// *** Displaying Values *** 
	for (SGK_i = 0; SGK_i < SGK_num_rows; SGK_i++)
	{
		printf("Base Address Of Row %d : ptr_iArray[%d] = %p \t At Address : %p\n", SGK_i, SGK_i, SGK_ptr_iArray[SGK_i], &SGK_ptr_iArray[SGK_i]);
	}

	printf("\n\n");

	for (SGK_i = 0; SGK_i < SGK_num_rows; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < SGK_num_columns; SGK_j++)
		{
			printf("ptr_iArray[%d][%d] = %d \t At Address : %p\n", SGK_i, SGK_j, SGK_ptr_iArray[SGK_i][SGK_j], &SGK_ptr_iArray[SGK_i][SGK_j]); // can also use *(*(ptr_iArray + i) + j) for value and *(ptr_iArray + i) + j for address ...
		}
		printf("\n");
	}
	printf("\n\n");

	// *** Freeing memory allocated to each row ***
	for (SGK_i = (SGK_num_rows - 1); SGK_i >= 0; SGK_i--)
	{
		if (SGK_ptr_iArray[SGK_i])
		{
			free(SGK_ptr_iArray[SGK_i]);
			SGK_ptr_iArray[SGK_i] = NULL;
			printf("Memory Allocated To Row %d Has been successfully freed !!!\n\n", SGK_i);
		}
	}
	
	// *** Freeing Memory allocated to 1D Array Consisting Of Base Addresses of rows ***
	if (SGK_ptr_iArray)
	{
		free(SGK_ptr_iArray);
		SGK_ptr_iArray = NULL;
		printf("Memory Allocated To ptr_iArray Has Been Successfully Freed !!! \n\n");
	}

	return(0);
}
