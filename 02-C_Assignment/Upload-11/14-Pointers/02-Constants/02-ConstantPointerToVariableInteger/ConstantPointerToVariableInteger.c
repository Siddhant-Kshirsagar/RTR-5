#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_num = 5;
	int* const SGK_ptr = &SGK_num; //Read this line from right to left => "ptr is a constant (const) pointer (*) to integer (int) variable".

	//code
	printf("\n");
	printf("Current Value of 'num' = %d\n",SGK_num);
	printf("Current 'ptr' (Address of 'num') = %p\n", SGK_ptr);

	// The following line dose NOT give error ... as we are modifying the value of the variable individually
	SGK_num++;
	printf("\n\n");
	printf("After num++, value of 'num' = %d\n", SGK_num);


	// The Following line gives error and is hence commented out.
	// We Cannot alter the 'ptr' value as 'ptr' is "a constant pointer to integer".
	// with respect to the pointer, the value it points to is not constant but the pointer itself is constant.
	// Uncomment it and see the error.
	
	 //ptr++;

	// The following line does not give error 
	// We do not get error because we are changing the value at a constant pointer (address).
	// The pointer is constant. The Value to which the pointer points is NOT constant.
	(*SGK_ptr)++;

	printf("\n\n");
	printf("After (*ptr)++, value of 'ptr' = %p\n", SGK_ptr);
	printf("Value at this 'ptr' = %d \n", *SGK_ptr);
	printf("\n");
	return(0);
}
