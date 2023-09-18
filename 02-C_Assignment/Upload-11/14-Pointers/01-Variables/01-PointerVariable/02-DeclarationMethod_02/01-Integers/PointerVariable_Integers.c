#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_num;
	int* SGK_ptr = NULL; //Declaration Method 2 :- 'ptr' is a variable of type 'int*'

	//code
	SGK_num = 10;
	
	printf("\n\n");

	printf(" ***** BEFORE ptr = &num *****\n\n");
	printf("Value Of 'num'			= %d\n\n", SGK_num);
	printf("Address of 'num'		=%p\n\n", &SGK_num);
	printf("Value At Address of 'num'	=%d\n\n", *(&SGK_num));

	//Assigning address of variable 'num' to pointer variable 'ptr'
	//'ptr' now contains address of 'num' ...hence, 'ptr' is same as '&num'
	SGK_ptr = &SGK_num;

	printf("\n\n");

	printf(" ***** AFTER ptr = &num *****\n\n");
	printf("Value Of 'num'			=%d\n\n", SGK_num);
	printf("Address  of 'num'		=%p\n\n", SGK_ptr);
	printf("Value At Address of 'num'	=%d\n\n", *SGK_ptr);

	return(0);
}
