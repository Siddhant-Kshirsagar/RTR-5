#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_num;
	int	*SGK_ptr = NULL;
	int *SGK_copy_ptr = NULL;

	//code 
	SGK_num = 5;
	SGK_ptr = &SGK_num;

	printf("\n\n");
	printf(" ***** BEFORE copy_ptr = ptr ***** \n\n");
	printf(" num			= %d\n", SGK_num);
	printf(" &num			= %p\n", &SGK_num);
	printf(" *(&num)		= %d\n", *(&SGK_num));
	printf(" ptr			= %p\n", SGK_ptr);
	printf(" *ptr			= %d\n", *SGK_ptr);

	//'ptr' is an interger pointer variable... that it can hold the address of any integer variable only
	//'copy_ptr' is another integer pointer variable 
	// If ptr = &num ... 'ptr' will contain address of integer variable 'num'
	// If 'ptr' is assigned to 'copy_ptr', 'copy_ptr' will also contain address of integer variable 'num'
	// Hence, now, both 'ptr' and copy_ptr' will point to 'num' ...

	SGK_copy_ptr = SGK_ptr; // copy_ptr = ptr = &num

	printf("\n\n");
	printf(" ***** After copy_ptr = ptr ***** \n\n");
	printf(" num			= %d\n", SGK_num);
	printf(" &num			= %p\n", &SGK_num);
	printf(" *(&num)		= %d\n", *(&SGK_num));
	printf(" ptr			= %p\n", SGK_ptr);
	printf(" *ptr			= %d\n", *SGK_ptr);
	printf(" copy_ptr		= %p\n", SGK_copy_ptr);
	printf(" *copy_ptr		= %d\n", *SGK_copy_ptr);
	return(0);
}
