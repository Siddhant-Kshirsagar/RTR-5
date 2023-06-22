#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_iArray[5][3] = { {1,2,3},{2,4,6},{3,6,9},{4,8,12},{5,10,15} };//In-Line Initialization
	int SGK_int_size;
	int SGK_iArray_size;
	int SGK_iArray_num_elements, SGK_iArray_num_rows, SGK_iArray_num_columns;
	int SGK_i, SGK_j;

	//code
	printf("\n\n");

	SGK_int_size = sizeof(int);

	SGK_iArray_size = sizeof(SGK_iArray);
	printf("Size Of Two Dimensional (2D) Integer Array Is = %d\n\n",SGK_iArray_size);

	SGK_iArray_num_rows = SGK_iArray_size / sizeof(SGK_iArray[0]);
	printf("Number of Rows In Two Dimensional ( 2D) Integer Array Is = %d \n\n", SGK_iArray_num_rows);

	SGK_iArray_num_columns = sizeof(SGK_iArray[0]) / SGK_int_size;
	printf("Number Of Column In Two Dimensional ( 2D ) Integer Array Is = %d \n\n", SGK_iArray_num_columns);

	SGK_iArray_num_elements = SGK_iArray_num_rows * SGK_iArray_num_columns;
	printf("Number of Elements In Two Dimensional (2D) Integer Array Is = %d \n\n", SGK_iArray_num_elements);

	printf("\n\n");
	printf("Elements In The 2D Array : \n\n");

	//*** Array Indices Begin From 0, Hence, 1st Row Is Actually 0th Row And 1st Column Is Actually 0th Column ***
	for (SGK_i = 0; SGK_i < SGK_iArray_num_rows; SGK_i++)
	{
		printf("************ ROW %d ************* \n", (SGK_i + 1));
		for (SGK_j = 0; SGK_j < SGK_iArray_num_columns; SGK_j++)
		{
			printf("iArray[%d][%d] = %d\n", SGK_i, SGK_j, SGK_iArray[SGK_i][SGK_j]);
		}
		printf("\n\n");
	}

	return(0);
}
