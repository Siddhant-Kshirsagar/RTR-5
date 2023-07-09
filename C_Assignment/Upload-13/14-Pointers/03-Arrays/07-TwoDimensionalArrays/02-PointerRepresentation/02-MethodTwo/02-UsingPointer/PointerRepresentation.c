#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
	//variable declarations
	int SGK_i, SGK_j;
	int **SGK_ptr_iArray = NULL;

	//code 
	// *** Every Row Of A 2D Array Is An Integer Array Itself Comprising of 'NUM_COLUMNS' Integer Elements ***
	// *** There Are 5 ROWS AND 3 COLUMNS In A 2D Integer Array. Each of the 5 Rows Is A 1D Array of 3 Integers. In a 2D integer Array. Each of the 5 Rows Is a 1D Array Of 3 Integers.
	// *** Hence, Each of these 5 rows themselves begin arrays, will be the base addresses of their respective rows ***
	printf("\n\n");

	// *** Memory allocation ***
	SGK_ptr_iArray = (int **)malloc(NUM_ROWS * sizeof(int *));// ptr_iArray is the name and base address of 1D Array Containing 5 integer pointers to 5 integer arrays ...so it is an array containing elements of data type (int *)
	if (SGK_ptr_iArray == NULL)
	{
		printf("Memory Allocation To The 1D Array Of Base Addresses of %d Rows Failed !!! Exitting Now ...\n\n", NUM_ROWS);
		exit(0);
	}
	else
	{
		printf("Memory Allocation To The 1D Array of Base Address of %d Rows Has Succeeded !!!\n\n", NUM_ROWS);
	}

	// *** Allocating Memory To Each Row ***
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		SGK_ptr_iArray[SGK_i] = (int *)malloc(NUM_COLUMNS * sizeof(int)); // ptr_iArray[i] is the base address of ith row ...
		if (SGK_ptr_iArray == NULL)
		{
			printf("Memory Allocation To The Columns Of Row %d Failed !!! Exitting Now...\n\n", SGK_i);
			exit(0);
		}
		else
		{
			printf("Memory Allocation To The Columns Of Row %d Has Succeeded !!!\n\n", SGK_i);
		}
	}

	// *** Assigning Values ***
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
		{
			*(*(SGK_ptr_iArray + SGK_i) + SGK_j) = (SGK_i + 1) * (SGK_j + 1); // SAME AS ptr_iArray[i][j] =(i + 1 ) * (j + 1);
		}
	}

	// *** Displaying Values ***
	printf("\n\n");
	printf("2D Integer Array Elements Along With Addresses : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
		{
			printf("ptr_iArrray[%d][%d] = %d \t\t At Address &ptr_iArray[%d][%d] : %p\n", SGK_i, SGK_j, SGK_ptr_iArray[SGK_i][SGK_j], SGK_i, SGK_j, &SGK_ptr_iArray[SGK_i][SGK_j]);
		}
		printf("\n\n");
	}

	// *** Freeing Allocated Memory ***
	// *** Freeing Memory Of Each Row ***
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		if (*(SGK_ptr_iArray + SGK_i)) // if(ptr_iArray[i])
		{
			free(*(SGK_ptr_iArray + SGK_i)); // free(ptr_iArray[i]);
			*(SGK_ptr_iArray+SGK_i) = NULL;// ptr_iArray[i] = NULL;
			printf("Memory Allocation To Row %d Has Been Successfully Freed !!!\n\n", SGK_i);
		}

	}

	// *** Freeing Memory of ptr_iArray which is the array of 5 integer pointers ... That it, it is an array having 5 integer Addresses (Type int*) ***
	if (SGK_ptr_iArray)
	{
		free(SGK_ptr_iArray);
		SGK_ptr_iArray = NULL;
		printf("Memory Allocated To ptr_iArray has been successfully freed !!!\n\n");
	}

	return(0);
}
