#include<stdio.h>
int main(void)
{
	//variable declarations

	//In-line Initialization
	int SGK_iArray[5][3][2] = { { {9,18},{27,36},{45,54} },
								{ {8,16},{24,32},{40,48} },
								{ {7,14},{21,28},{35,42} },
								{ {6,12},{18,24},{30,36} },
								{ {5,10},{15,20},{25,30} } };
	int SGK_int_size;
	int SGK_iArray_size;
	int SGK_iArray_num_elements, SGK_iArray_width, SGK_iArray_height, SGK_iArray_depth;

	//code 
	printf("\n\n");
	SGK_int_size = sizeof(int);

	SGK_iArray_size = sizeof(SGK_iArray);
	printf("Size of Three Dimensional (3D) Integer Array Is = %d \n\n",SGK_iArray_size);

	SGK_iArray_width = SGK_iArray_size / sizeof(SGK_iArray[0]);
	printf("Number of Rows (Width) In Three Dimensional (3D) Integer Array Is =>%d\n\n",SGK_iArray_width);

	SGK_iArray_height = sizeof(SGK_iArray[0]) / sizeof(SGK_iArray[0][0]);
	printf("Number Of Columns (Height) In Three Dimensional (3D) Integer Array Is = %d\n\n",SGK_iArray_height);

	SGK_iArray_depth = sizeof(SGK_iArray[0][0]) / SGK_int_size;
	printf("Depth In Three Dimensional (3D) Integer Array Is = %d \n\n",SGK_iArray_depth);

	SGK_iArray_num_elements = SGK_iArray_width * SGK_iArray_height * SGK_iArray_depth;
	printf("Number Of Element In Three Dimensional (3D) Integer Array Is = %d \n\n",SGK_iArray_num_elements);

	printf("\n\n");
	printf("Elements In Integer 3D Array : \n\n");

	//*** Piece-Meal Display ***
	//***** ROW 1 *****
	printf("***** ROW 1 *****\n");
	printf("***** COLUMN 1 *****\n");
	printf("iArray[0][0][0] = %d\n", SGK_iArray[0][0][0]);
	printf("iArray[0][0][1] = %d\n", SGK_iArray[0][0][1]);
	printf("\n");

	printf("***** COLUMN 2 *****\n");
	printf("iArray[0][1][0] = %d\n", SGK_iArray[0][1][0]);
	printf("iArray[0][1][1] = %d\n", SGK_iArray[0][1][1]);
	
	printf("***** COLUMN 3 *****\n");
	printf("iArray[0][2][0] = %d\n", SGK_iArray[0][2][0]);
	printf("iArray[0][2][1] = %d\n", SGK_iArray[0][2][1]);
	printf("\n");

	// ***** ROW 2 *****
	printf("***** ROW 2 *****\n");
	printf("***** COLUMN 1 *****\n");
	printf("iArray[1][0][0] = %d\n", SGK_iArray[1][0][0]);
	printf("iArray[1][0][1] = %d\n", SGK_iArray[1][0][1]);
	printf("\n");

	printf("***** COLUMN 2 *****\n");
	printf("iArray[1][1][0] = %d\n", SGK_iArray[1][1][0]);
	printf("iArray[1][1][1] = %d\n", SGK_iArray[1][1][1]);
	printf("\n");

	printf("***** COLUMN 3 *****\n");
	printf("iArray[1][2][0] = %d\n", SGK_iArray[1][2][0]);
	printf("iArray[1][2][1] = %d\n", SGK_iArray[1][2][1]);
	printf("\n");

	// ***** ROW 3 *****
	printf("***** ROW 3 *****\n");
	printf("***** COLUMN 1 *****\n");
	printf("iArray[2][0][0] = %d\n",SGK_iArray[2][0][0]);
	printf("iArray[2][0][1] = %d\n",SGK_iArray[2][0][1]);
	printf("\n");

	printf("***** COLUMN 2 *****\n");
	printf("iArray[2][1][0] = %d\n", SGK_iArray[2][1][0]);
	printf("iArray[2][1][1] = %d\n", SGK_iArray[2][1][1]);
	printf("\n");

	printf("***** COLUMN 3 *****\n");
	printf("iArray[2][2][0] = %d\n", SGK_iArray[2][2][0]);
	printf("iArray[2][2][1] = %d\n", SGK_iArray[2][2][1]);
	printf("\n");

	// ***** ROW 4 *****
	printf("***** ROW 4 *****\n");
	printf("***** COLUMN 1 *****\n");
	printf("iArray[3][0][0] = %d\n", SGK_iArray[3][0][0]);
	printf("iArray[3][0][1] = %d\n", SGK_iArray[3][0][1]);
	printf("\n");

	printf("***** COLUMN 2 *****\n");
	printf("iArray[3][1][0] = %d\n", SGK_iArray[3][1][0]);
	printf("iArray[3][1][1] = %d\n", SGK_iArray[3][1][1]);
	printf("\n");

	printf("***** COLUMN 3 *****\n");
	printf("iArray[3][2][0] = %d\n", SGK_iArray[3][2][0]);
	printf("iArray[3][2][1] = %d\n", SGK_iArray[3][2][1]);
	printf("\n");

	// ***** ROW 5 *****
	printf("***** ROW 5 *****\n");
	printf("***** COLUMN 1 *****\n");
	printf("iArray[4][0][0] = %d\n", SGK_iArray[4][0][0]);
	printf("iArray[4][0][1] = %d\n", SGK_iArray[4][0][1]);
	printf("\n");

	printf("***** COLUMN 2 *****\n");
	printf("iArray[4][1][0] = %d\n", SGK_iArray[4][1][0]);
	printf("iArray[4][1][1] = %d\n", SGK_iArray[4][1][1]);
	printf("\n");

	printf("***** COLUMN 3 *****\n");
	printf("iArray[4][2][0] = %d\n", SGK_iArray[4][2][0]);
	printf("iArray[4][2][1] = %d\n", SGK_iArray[4][2][1]);
	printf("\n");

	return(0);
}
