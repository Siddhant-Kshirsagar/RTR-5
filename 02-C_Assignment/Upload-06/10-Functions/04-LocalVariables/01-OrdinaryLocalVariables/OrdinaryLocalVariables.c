#include<stdio.h>

// ***** GLOBAL SCOPE *****

int main(void)
{
	// ***** LOCAL SCOPE OF main() begins *****
	//variable declarations
	//'SGK_a' is a local variable. It is local to main() only.
	int SGK_a = 5;

	//function prototypes
	void change_count(void);

	//code 
	printf("\n");
	printf("A = %d \n\n",SGK_a);

	//SGK_local_count is initialized to 0.
	//SGK_local_count = SGK_local_count + 1 = 0 +1 =1
	change_count();

	//Since,'SGK_local_count' is an ordinary local variable of change_count(), it will NOT retain it's value from previous call to change_count().
	//so SGK_local_count is AGAIN initialized to 0 
	//SGK_local_count  = SGK_local_count + 1 = 0 + 1 = 1
	change_count();

	//Since, 'SGK_local_count' is an ordinary local variable of change_count(), it will NOT retain it's value from previous call to change_count().
	//so SGK_local_count is AGAIN is initialized to 0
	//SGK_local_count = SGK_local_count + 1 = 0 + 1 = 1
	change_count();

	return(0);

	// ***** LOCAL SCOPE OF main() ends *****
}

// ***** GLOBAL SCOPE *****

void change_count(void)
{
	//*****LOCAL SCOPE OF change_count() begins *****
	//variable declarations
	//'local_count' is a Local Variable.It is local to change_count() only.
	int SGK_local_count = 0;

	//code 
	SGK_local_count = SGK_local_count + 1;
	printf("Local Count = %d\n", SGK_local_count);

	// ***** LOCAL SCOPE OF change_count() ends *****
}

// ***** GLOBAL SCOPE *****