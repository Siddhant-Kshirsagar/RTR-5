#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_iArray[5][3] = { {1,2,3},{2,4,6},{3,6,9},{4,8,12},{5,10,15} }; //In-Line Initialization
	int SGK_int_size;
	int SGK_iArray_size;
	int SGK_iArray_num_elements, SGK_iArray_num_rows, SGK_iArray_num_columns;

	//code
	printf("\n\n");

	SGK_int_size = sizeof(int);

	SGK_iArray_size = sizeof(SGK_iArray);
	printf("Size Of Two Dimensional (2D) Integer Array Is = %d\n\n", SGK_iArray_size);

	SGK_iArray_num_rows = SGK_iArray_size / sizeof(SGK_iArray[0]);
	printf("Number Of Rows In Two Dimensional (2D) Integer Array Is = %d \n\n",SGK_iArray_num_rows);

	SGK_iArray_num_columns = sizeof(SGK_iArray[0]) / SGK_int_size;
	printf("Number Of Columns In Two Dimensional (2D) Integer Array Is = %d\n\n",SGK_iArray_num_columns);

	SGK_iArray_num_elements = SGK_iArray_num_rows * SGK_iArray_num_columns;
	printf("Number of Elements In Two Dimensional ( 2D ) Integer Array Is = %d\n\n",SGK_iArray_num_elements);

	printf("\n\n");
	printf("Elements In The 2D Array : \n\n");

	// ***** Array Indices Begin From 0, Hence, 1st Row Is Actually Oth Row And 1st Column Is Actually oth Column *****

	// ***** ROW 1 *****
	printf("************** ROW 1 ****************\n");
	printf("iArray[0][0] = %d\n",SGK_iArray[0][0]); // *** Column 1 *** (0th Element) => 1
	printf("iArray[0][1] = %d\n",SGK_iArray[0][1]); // *** Column 2 *** (1st Element) => 2
	printf("iArray[0][2] = %d\n",SGK_iArray[0][2]); // *** Column 3 *** (2nd Element) => 3

	printf("\n\n");

	//*** ROW 2 ***
	printf("************* ROW 2 *****************\n");
	printf("iArray[1][0] = %d\n",SGK_iArray[1][0]); // *** Column 1 *** (0th Element) => 2
	printf("iArray[1][1] = %d\n",SGK_iArray[1][1]); // *** Column 2 *** (1st Element) => 4
	printf("iArray[1][2] = %d\n",SGK_iArray[1][2]); // *** Column 3 *** (2nd Element) => 6

	printf("\n\n");

	//*** ROW 3 ***
	printf("************* ROW 3 *****************\n");
	printf("iArray[2][0] = %d\n",SGK_iArray[2][0]); // *** Column 1 *** (0th Element) => 3
	printf("iArray[2][1] = %d\n",SGK_iArray[2][1]); // *** Column 2 *** (1st Element) => 6
	printf("iArray[2][2] = %d\n",SGK_iArray[2][2]); // *** Column 3 *** (2nd Element) => 9

	printf("\n\n");

	//*** ROW 4 ***
	printf("************* ROW 4 *****************\n");
	printf("iArray[3][0] = %d\n",SGK_iArray[3][0]); // *** Column 1 *** (0th Element) => 4
	printf("iArray[3][1] = %d\n",SGK_iArray[3][1]); // *** Column 2 *** (1st Element) => 8
	printf("iArray[3][2] = %d\n",SGK_iArray[3][2]); // *** Column 3 *** (2nd Element) => 12

	printf("\n\n");

	//*** ROW 5 ***
	printf("************ ROW 5 ******************\n");
	printf("iArray[4][0] = %d\n",SGK_iArray[4][0]);// *** Column 1 *** (0th Element) => 5
	printf("iArray[4][1] = %d\n",SGK_iArray[4][1]);// *** Column 2 *** (1st Element) => 10
	printf("iArray[4][2] = %d\n",SGK_iArray[4][2]);// *** Column 3 *** (2nd Element) => 15

	printf("\n\n");
	
	return(0);
}
