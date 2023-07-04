#include<stdio.h>

int main(void)
{
	//variable declarations
	double SGK_num;
	double *SGK_ptr = NULL; //Declaration Method 1 :- '*ptr' is a variable of type double

	//code
	SGK_num = 2.34343434f;
	printf("\n\n");
	
	printf(" ***** BEFORE ptr = &num *****\n\n");
	printf("Value Of 'num'			= %lf\n\n", SGK_num);;
	printf("Address of 'num'		= %p\n\n", &SGK_num);
	printf("Value At Address of 'num'	= %lf", *(&SGK_num));

	//Assigning address of variable 'num' to pointer variable 'ptr'
	//'ptr' now contains address of 'num' ...hence, 'ptr' is SAME as '&num'
	SGK_ptr = &SGK_num;

	printf("\n\n");

	printf(" ***** After ptr = &num ***** \n\n");
	printf("Value Of 'num'			= %lf\n\n", SGK_num);
	printf("Address of 'num'		= %p\n\n", SGK_ptr);
	printf("Value At Address of 'num'	= %lf\n\n", *SGK_ptr);

	return(0);
}
