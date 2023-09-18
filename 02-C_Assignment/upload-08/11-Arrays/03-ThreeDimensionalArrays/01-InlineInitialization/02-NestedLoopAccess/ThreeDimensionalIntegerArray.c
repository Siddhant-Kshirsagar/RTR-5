#include<stdio.h>
int main(void)
{
	//variable declarations

	//IN-LINE INITIALIZATION
	int SGK_iArray[5][3][2] = { {{9,18},{27,36},{45,54}},
								{{8,16},{24,32},{40,48}},
								{{7,14},{21,28},{35,42}},
								{{6,12},{18,24},{30,36}},
								{{5,10},{15,20},{25,30}}};
	int SGK_int_size;
	int SGK_iArray_size;
	int SGK_iArray_num_elements, SGK_iArray_width, SGK_iArray_height, SGK_iArray_depth;
	int SGK_i, SGK_j, SGK_k;

	//code
	printf("\n\n");

	SGK_int_size = sizeof(int);

	SGK_iArray_size = sizeof(SGK_iArray);
	printf("Size Of Three Dimensional (3D) Integer Array Is = %d\n\n",SGK_iArray_size);

	SGK_iArray_width = SGK_iArray_size / sizeof(SGK_iArray[0]);
	printf("Number Of Rows (Width) In Three Dimensional (3D) Integer Array Is %d \n\n",SGK_iArray_width);

	SGK_iArray_height = sizeof(SGK_iArray[0]) / sizeof(SGK_iArray[0][0]);
	printf("Number Of Columns (Height) In Three Dimensional (3D) Integer Array  Is = %d \n\n",SGK_iArray_height);

	SGK_iArray_depth = sizeof(SGK_iArray[0][0]) / SGK_int_size;
	printf("Depth In Three Dimensional ( 3D ) Integer Array Is = %d \n\n",SGK_iArray_depth);

	SGK_iArray_num_elements = SGK_iArray_width * SGK_iArray_height * SGK_iArray_depth;
	printf("Number of Elements In Three Dimensional ( 3D) Integer Array Is = %d\n\n", SGK_iArray_num_elements);

	printf("\n\n");
	printf("Elements In Integer 3D Array : \n\n");
	for (SGK_i = 0; SGK_i < SGK_iArray_width; SGK_i++)
	{
		printf("***** ROW %d *****\n",(SGK_i+1));
		for (SGK_j = 0; SGK_j < SGK_iArray_height; SGK_j++)
		{
			printf("***** COLUMN %d *****\n", (SGK_j + 1));
			for (SGK_k = 0; SGK_k < SGK_iArray_depth; SGK_k++)
			{
				printf("iArray[%d][%d][%d] = %d\n", SGK_i, SGK_j, SGK_k, SGK_iArray[SGK_i][SGK_j][SGK_k]);
			}
			printf("\n");
		}
		printf("\n\n");
	}

	return(0);
}

