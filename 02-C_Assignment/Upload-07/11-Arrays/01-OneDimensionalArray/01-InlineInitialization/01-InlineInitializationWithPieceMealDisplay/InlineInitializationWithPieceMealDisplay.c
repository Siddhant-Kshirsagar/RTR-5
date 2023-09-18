#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_iArray[] = { 9, 30, 6, 12, 98, 95, 20, 23, 2, 45 };
	int SGK_int_size;
	int SGK_iArray_size;
	int SGK_iArray_num_elements;

	float SGK_fArray[] = { 1.2f, 2.3f, 3.4f, 4.5f, 5.6f, 6.7f, 7.8f, 8.9f };
	int SGK_float_size;
	int SGK_fArray_size;
	int SGK_fArray_num_elements;

	char SGK_cArray[] = { 'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P' };
	int SGK_char_size;
	int SGK_cArray_size;
	int SGK_cArray_num_elements;

	//code 

	// ***** SGK_iArray[] *****
	printf("\n\n");
	printf("In-line Initialization And Piece-meal Display Of Elements of Array 'SGK_iArray[]' : \n\n");
	printf("SGK_iArray[0] (1st Element) = %d\n", SGK_iArray[0]);
	printf("SGK_iArray[1] (2nd Element) = %d\n", SGK_iArray[1]);
	printf("SGK_iArray[2] (3rd Element) = %d\n", SGK_iArray[2]);
	printf("SGK_iArray[3] (4th Element) = %d\n", SGK_iArray[3]);
	printf("SGK_iArray[4] (5th Element) = %d\n", SGK_iArray[4]);
	printf("SGK_iArray[5] (6th Element) = %d\n", SGK_iArray[5]);
	printf("SGK_iArray[6] (7th Element) = %d\n", SGK_iArray[6]);
	printf("SGK_iArray[7] (8th Element) = %d\n", SGK_iArray[7]);
	printf("SGK_iArray[8] (9th Element) = %d\n", SGK_iArray[8]);
	printf("SGK_iArray[9] (10th Element) = %d\n", SGK_iArray[9]);

		
	SGK_int_size = sizeof(int);
	SGK_iArray_size = sizeof(SGK_iArray);
	SGK_iArray_num_elements = SGK_iArray_size / SGK_int_size;

	printf("Size Of Data type 'int'								= %d bytes\n", SGK_int_size);
	printf("Number Of Elements In 'int' Array 'iArray[]'		= %d Elements\n", SGK_iArray_num_elements);
	printf("Size of Array 'iArray[]' (%d Elements * %d bytes)	= %d bytes \n\n", SGK_iArray_num_elements, SGK_int_size, SGK_iArray_size);

	//***** fArray[] *****
	printf("\n\n");
	printf("In-line Initialization And Piece-meal Display Of Elements of Array 'fArray[]' : \n\n");
	printf("fArray[0] (1st Element) = %f\n", SGK_fArray[0]);
	printf("fArray[1] (2nd Element) = %f\n", SGK_fArray[1]);
	printf("fArray[2] (3rd Element) = %f\n", SGK_fArray[2]);
	printf("fArray[3] (4th Element) = %f\n", SGK_fArray[3]);
	printf("fArray[4] (5th Element) = %f\n", SGK_fArray[4]);
	printf("fArray[5] (6th Element) = %f\n", SGK_fArray[5]);
	printf("fArray[6] (7th Element) = %f\n", SGK_fArray[6]);
	printf("fArray[7] (8th Element) = %f\n", SGK_fArray[7]);
	printf("fArray[8] (9th Element) = %f\n", SGK_fArray[8]);
	printf("fArray[9] (10th Element) = %f\n", SGK_fArray[9]);

	SGK_float_size = sizeof(float);
	SGK_fArray_size = sizeof(SGK_fArray);
	SGK_fArray_num_elements = SGK_fArray_size / SGK_float_size;
	printf("Size Of Data type 'float'							= %d bytes\n", SGK_float_size);
	printf("Number Of Elements In 'float' Array 'fArray[]'		= %d Elements\n", SGK_fArray_num_elements);
	printf("Size Of Array 'fArray[]' (%d ELements * %d bytes)	= %d bytes\n\n", SGK_fArray_num_elements, SGK_float_size, SGK_fArray_size);

	// ***** cArray[] *****
	printf("\n\n");
	printf("In-line Initialization And Piece-meal Display Of Elements of Array 'cArray[]' : \n\n");
	printf("cArray[0] (1st Element) = %c\n", SGK_cArray[0]);
	printf("cArray[1] (2nd Element) = %c\n", SGK_cArray[1]);
	printf("cArray[2] (3rd Element) = %c\n", SGK_cArray[2]);
	printf("cArray[3] (4th Element) = %c\n", SGK_cArray[3]);
	printf("cArray[4] (5th Element) = %c\n", SGK_cArray[4]);
	printf("cArray[5] (6th Element) = %c\n", SGK_cArray[5]);
	printf("cArray[6] (7th Element) = %c\n", SGK_cArray[6]);
	printf("cArray[7] (8th Element) = %c\n", SGK_cArray[7]);
	printf("cArray[8] (9th Element) = %c\n", SGK_cArray[8]);
	printf("cArray[9] (10th Element) = %c\n", SGK_cArray[9]);
	printf("cArray[10] (11th Element) = %c\n", SGK_cArray[10]);
	printf("cArray[11] (12th Element) = %c\n", SGK_cArray[11]);
	printf("cArray[12] (13th Element) = %c\n", SGK_cArray[12]);

	SGK_char_size = sizeof(char);
	SGK_cArray_size = sizeof(SGK_cArray);
	SGK_cArray_num_elements = SGK_cArray_size / SGK_char_size;
	printf("Size Of Data type 'char'								= %d bytes\n",SGK_char_size);
	printf("Number Of Elements In 'char' Array 'cArray[]'			= %d Elements\n", SGK_cArray_num_elements);
	printf("Size Of Array 'cArray[]' (%d Elements * %d bytes)		= %d bytes\n\n", SGK_cArray_num_elements, SGK_char_size, SGK_cArray_size);;

	return(0);
}

