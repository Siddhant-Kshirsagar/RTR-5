#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_iArray[10];
	int *SGK_ptr_iArray = NULL;
	int SGK_i;

	//code 
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_iArray[SGK_i] = (SGK_i + 1) * 3;

	// *** Name Of Any Array Is It's Base Address ***
	// *** Hence, 'iArray' Is The Base Address of Array iArray[] or 'iArray[]' is the address of element of iArray[0] ***
	// *** Assigning  Base Address of Array 'iArray[]' To Integer pointer 'ptr_iArray'

	SGK_ptr_iArray = SGK_iArray;// ptr_iArray = &iArray[0];

	printf("\n\n");
	printf("Elements Of The Integer Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("iArray[%d] = %d\n", SGK_i, *(SGK_ptr_iArray + SGK_i));

	printf("\n\n");
	printf("Elements Of The Integer Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("iArray[%d] = %d \t \t Address = %p\n", SGK_i, *(SGK_ptr_iArray + SGK_i), (SGK_ptr_iArray + SGK_i));

	printf("\n\n");

	return(0);
}
