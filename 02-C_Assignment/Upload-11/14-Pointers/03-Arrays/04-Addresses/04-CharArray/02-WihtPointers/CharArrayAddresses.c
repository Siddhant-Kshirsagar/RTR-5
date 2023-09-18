#include<stdio.h>

int main(void)
{
	//variable declarations
	char SGK_cArray[10];
	char *SGK_ptr_cArray = NULL;
	int SGK_i;

	//code
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_cArray[SGK_i] = (char)(SGK_i + 65);

	// *** Name Of Any Array Is Its Base Address ***
	// *** Hence, 'cArray' Is The Base Address of Array cArray[] or 'cArray' is the address of element cArray[0] ***
	// *** Assigning base address of array 'cArray[]' to char pointer 'ptr_cArray'

	SGK_ptr_cArray = SGK_cArray; // same as ptr_cArray = &cArray[0];

	printf("\n\n");
	printf("Elements of the character array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("cArray[%d] = %c\n", SGK_i, *(SGK_ptr_cArray + SGK_i));

	printf("\n\n");
	printf("Elements of the character array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("cArray[%d] = %c \t \t Address = %p\n", SGK_i, *(SGK_ptr_cArray + SGK_i),(SGK_ptr_cArray + SGK_i));

	printf("\n\n");

	return(0);
}
