#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_num = 5;
	const int* SGK_ptr = NULL; //Read this line from right to left => "ptr is a variable pointer(*) to integer (int) constant (const)"

	//code
	SGK_ptr = &SGK_num;
	printf("\n");
	printf("Current Value Of 'num' = %d\n", SGK_num);
	printf("Current 'ptr' (Address of 'num') = %p\n", SGK_ptr);

	//The Following Line Does not give error .. as we are modifying the value of the variable individually
	SGK_num++;
	printf("\n\n");
	printf("After num++, Value of 'num' = %d\n", SGK_num);

	// The Following line gives error and is hence commented out.
	// We cannot alter the value stored in "A pointer to constant integer"
	// with respect to the pointer, the value it points to should be constant.
	// Uncomment it and see the error.

	// (*ptr)++;

	// The Following lines does NOT give error 
	// We do not get error because we are changing the pointer (address).
	// The pointer is not constant. The value to which the pointer points is constant.

	SGK_ptr++;

	printf("\n\n");
	printf("After ptr++, value of 'ptr' = %p\n", SGK_ptr);
	printf("Value at this new 'ptr' = %d\n", *SGK_ptr);
	printf("\n");
	return(0);
}
