#include<stdio.h>

// ***** GLOBAL SCOPE *****

int main(void)
{
	// ***** LOCAL SCOPE OF main() begins *****

	//variable declarations 
	//'SGK_a' is a Local Variable. It is local to main() only.
	int SGK_a = 5;

	//function prototypes
	void change_count(void);

	//code 
	printf("\n");
	printf("A = %d\n\n", SGK_a);

	//SGK_local_count is initialized to 0.
	//SGK_local_count = local_count + 1 = 0 + 1 = 1
	change_count();

	//Since, 'SGK_local_count' is a local static variable of change_count(), it will retain its value from previous call to change_count().
	//So SGK_local_count is 1
	//SGK_local_count = local_count + 1 = 2 + 1 = 3
	change_count();

	//Since, 'SGK_local_count' is a local static variable of change_count(), it will retain so its value from previous call to change_count().
	//So SGK_local_count is 2
	//SGK_local_count = SGK_local_count + 1 = 2 + 2 = 3
	change_count();

	return(0);

	// ***** LOCAL SCOPE OF main() ends *****
}

// ***** GLOBAL SCOPE *****

void change_count(void)
{
	//***** LOCAL SCOPE OF change_count() begins *****

	//variable declarations
	//'SGK_local_count' is a Local Static variable. It is local to change_count() only.
	//It ill retain its value between calls to change_count()
	static int SGK_local_count = 0;

	//code 
	SGK_local_count = SGK_local_count + 1;
	printf("Local Count = %d \n", SGK_local_count);

	// *****	LOCAL SCOPE OF	change_count() ends *****
}

// ***** GLOBAL SCOPE *****
