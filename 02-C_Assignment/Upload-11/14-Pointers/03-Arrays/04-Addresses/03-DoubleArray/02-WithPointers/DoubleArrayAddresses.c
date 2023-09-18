#include<stdio.h>

int main(void)
{
	//variable declarations
	double SGK_dArray[10];
	double *SGK_ptr_dArray = NULL;
	int SGK_i;

	//code
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_dArray[SGK_i] = (float)(SGK_i + 1) * 1.333333f;

	// *** Name Of Any Array Is Its BASE ADDRESS ***
	// *** HENCE, 'dArray' is the base address of array dArray[] or 'dArray' is the address of element dArray[0] ***
	// *** Assigning base address of array 'dArray[]' to 'double data type ' pointer 'ptr_dArray'

	SGK_ptr_dArray = SGK_dArray; // same as ptr_dArray = &dArray[0];

	printf("\n\n");
	printf("Elements of the 'double' Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("dArray[%d] = %lf\n", SGK_i, *(SGK_ptr_dArray + SGK_i));

	printf("\n\n");
	printf("Elements of the 'double' Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("dArray[%d] = %lf \t \t Address = %p\n", SGK_i, *(SGK_ptr_dArray + SGK_i), (SGK_ptr_dArray + SGK_i));

	printf("\n\n");

	return(0);
}
