#include<stdio.h>

int main(void)
{
	//variable declarations
	float SGK_num;
	float* SGK_ptr;//declaration Method 2 :- 'ptr' is a variable of type 'float*'

	//code 
	SGK_num = 6.9f;

	printf("\n\n");

	printf(" ***** BEFORE ptr = &num ***** \n\n");
	printf("Value of 'num'			= %f\n\n", SGK_num);
	printf("Address of 'num'		= %p\n\n", &SGK_num);
	printf("Value At Address of 'num'	= %f\n\n", *(&SGK_num));

	//Assigning address of variable 'num' to pointer variable 'ptr'
	//'ptr' now contains address of 'num' ...hence, 'ptr' is SAME as '&num'
	SGK_ptr = &SGK_num;

	printf("\n\n");

	printf(" ***** AFTER ptr = &num ***** \n\n");
	printf("Value of 'num'			= %f\n\n", SGK_num);
	printf("Address of 'num'		= %p\n\n", SGK_ptr);
	printf("Value At Address of 'num'	= %f\n\n", *SGK_ptr);

	return(0);
}
