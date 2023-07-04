#include<stdio.h>

int main(void)
{
	//variable declaration
	char SGK_ch;
	char* SGK_ptr = NULL; //Declaration Method 2:- 'ptr' is a variable of type 'char*'

	//code 
	SGK_ch = 'A';

	printf("\n\n");

	printf(" ***** BEFORE ptr = &ch *****\n\n");
	printf("Value Of 'ch'			= %c\n\n", SGK_ch);
	printf("Address of 'ch'			= %p\n\n", &SGK_ch);
	printf("Value At Address of 'ch'	=%c\n\n",*(&SGK_ch));

	// Assigning address of variable 'ch' to pointer variable 'ptr'
	// 'ptr' now contains addresss of 'ch' ...hence, 'ptr' is SAME as '&ch'
	SGK_ptr = &SGK_ch;

	printf("\n\n");

	printf(" ***** AFTER ptr = &ch ***** \n\n");
	printf("Value Of 'ch'			= %c \n\n", SGK_ch);
	printf("Address of 'ch'			= %p\n\n", SGK_ptr);
	printf("Value At Address of 'ch'	= %c\n\n", *SGK_ptr);

	return(0);
}
