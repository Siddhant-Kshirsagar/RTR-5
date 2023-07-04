#include<stdio.h>

int main(void)
{
	//variable declarations
	float SGK_fArray[10];
	float *SGK_ptr_fArray = NULL;
	int SGK_i;

	//code
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_fArray[SGK_i] = (float)(SGK_i + 1) * 1.5f;

	// *** Name Of Any Array Is Its Base Address ***
	// *** Hence, 'fArray' Is The Base Address Of Array fArray[] or 'fArray' Is The Address Of Elements fArray[0] ***
	// *** Assigning Base Address of Array 'fArray[]' To Float Pointer 'ptr_fArray'

	SGK_ptr_fArray = SGK_fArray; // SAME as  ptr_fArray = &fArray[0];

	printf("\n\n");
	printf("Elementes Of The 'float' Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("fArray[%d] = %f\n", SGK_i, *(SGK_ptr_fArray+SGK_i));

	printf("\n\n");
	printf("Elements Of The 'float' Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("fArray[%d] = %f \t\t Address = %p\n", SGK_i, *(SGK_ptr_fArray + SGK_i),(SGK_ptr_fArray + SGK_i));

	printf("\n\n");

	return(0);
}
