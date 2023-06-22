#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3
#define DEPTH 2

int main(void)
{
	//variable declarations

	//IN-LINE INITIALIZATION 
	int SGK_iArray[NUM_ROWS][NUM_COLUMNS][DEPTH] = { {{9,18},{27,36},{45,54}},
													 {{8,16},{24,32},{40,48}},
													 {{7,14},{21,28},{35,42}},
													 {{6,12},{18,24},{30,36}},
													 {{5,10},{15,20},{25,30}} };
	int SGK_i, SGK_j, SGK_k;
	int SGK_iArray_1D[NUM_ROWS * NUM_COLUMNS * DEPTH]; // 5 * 3 * 2 Elements => 30 Elements In 1D Array

	//code 

	// ***** DISPLAY 3D Array *****
	printf("\n\n");
	printf("Elements In The 3D Array : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		printf("***** ROW %d *****\n",(SGK_i + 1));
		for (SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
		{
			printf("***** COLUMN %d *****\n", (SGK_j + 1));
			for (SGK_k = 0; SGK_k < DEPTH; SGK_k++)
			{
				printf("iArray[%d][%d][%d] = %d\n", SGK_i, SGK_j, SGK_k, SGK_iArray[SGK_i][SGK_j][SGK_k]);
			}
			printf("\n");
		}
		printf("\n");
	}

	// ***** CONVERTING 3D TO 1D *****
	for (SGK_i = 0; SGK_i < NUM_ROWS; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_COLUMNS; SGK_j++)
		{
			for (SGK_k = 0; SGK_k < DEPTH; SGK_k++)
			{
				SGK_iArray_1D[(SGK_i * NUM_COLUMNS * DEPTH) + (SGK_j * DEPTH) + SGK_k] = SGK_iArray[SGK_i][SGK_j][SGK_k];
			}
		}
	}

	// ***** DISPLAY 1D Array *****
	printf("\n\n\n\n");
	printf("Elements In The 1D Array : \n\n");
	for (SGK_i = 0; SGK_i < (NUM_ROWS * NUM_COLUMNS * DEPTH); SGK_i++)
	{
		printf("iArray_1D[%d] = %d \n", SGK_i, SGK_iArray_1D[SGK_i]);
	}

	return(0);
}
