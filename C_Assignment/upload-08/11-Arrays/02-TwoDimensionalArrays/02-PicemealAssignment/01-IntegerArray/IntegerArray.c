#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_iArray[3][5]; // 3 ROWS (0,1,2) AND 5 COLUMNS (0,1,2,3,4)
	int SGK_int_size;
	int SGK_iArray_size;
	int SGK_iArray_num_elements, SGK_iArray_num_rows, SGK_iArray_num_columns;
	int SGK_i, SGK_j;

	//code 
	printf("\n\n");

	SGK_int_size = sizeof(int);

	SGK_iArray_size = sizeof(SGK_iArray);
	printf("Size of Two Dimensional (2D) Integer Array Is = %d\n\n",SGK_iArray_size);

	SGK_iArray_num_rows = SGK_iArray_size / sizeof(SGK_iArray[0]);
	printf("Number of Rows In Two Dimensional (2D) Integer Array Is = %d\n\n",SGK_iArray_num_rows);

	SGK_iArray_num_columns = sizeof(SGK_iArray[0]) / SGK_int_size;
	printf("Number of Columns In Two Dimensional (2D) Integer Array Is = %d\n\n", SGK_iArray_num_columns);

	SGK_iArray_num_elements = SGK_iArray_num_rows * SGK_iArray_num_columns;
	printf("Number of Elements In Two Dimensional (2D) Integer Array Is = %d\n\n",SGK_iArray_num_elements);

	printf("\n\n");
	printf("Elements In The 2D Array : \n\n");

	// *** PIECE-Meal Assignment ***
	// *** ROW 1 ***
	SGK_iArray[0][0] = 21;
	SGK_iArray[0][1] = 42;
	SGK_iArray[0][2] = 63;
	SGK_iArray[0][3] = 84;
	SGK_iArray[0][4] = 105;

	// *** ROW 2 ***
	SGK_iArray[1][0] = 22;
	SGK_iArray[1][1] = 44;
	SGK_iArray[1][2] = 66;
	SGK_iArray[1][3] = 88;
	SGK_iArray[1][4] = 110;

	//*** ROW 3 ***
	SGK_iArray[2][0] = 23;
	SGK_iArray[2][1] = 46;
	SGK_iArray[2][2] = 69;
	SGK_iArray[2][3] = 92;
	SGK_iArray[2][4] = 115;

	// *** Display ***
	for (SGK_i = 0; SGK_i < SGK_iArray_num_rows; SGK_i++)
	{
		printf(" ********** ROW %d ***********\n", (SGK_i + 1));
		for (SGK_j = 0 ; SGK_j < SGK_iArray_num_columns; SGK_j++)
		{
			printf("iArray[%d][%d] = %d\n", SGK_i, SGK_j, SGK_iArray[SGK_i][SGK_j]);
		}
		printf("\n\n");
	}
	
	return(0);
}
