#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_num;
	
	//code 
	printf("\n\n");

	printf("Enter Value For 'num' : ");
	scanf("%d", &SGK_num);

	// IF - ELSE - IF LADDER BEGINS FROM HERE...
	if (SGK_num < 0)
		printf("Num = %d Is Less Than 0 (NEGATIVE) !!! \n\n", SGK_num);
	else if ((SGK_num > 0) && (SGK_num <= 100))
		printf("Num = %d Is Between 0 And 100 !!! \n\n ", SGK_num);
	else if ((SGK_num > 100) && (SGK_num <= 200))
		printf("Num = %d Is Between 100 And 200 !!! \n\n", SGK_num);
	else if ((SGK_num > 200) && (SGK_num <= 300))
		printf("Num = %d Is Between 200 And 300 !!! \n\n", SGK_num);
	else if ((SGK_num > 300) && (SGK_num <= 400))
		printf("Num = %d Is Between 300 And 400 !!! \n\n", SGK_num);
	else if (SGK_num > 500)
		printf("Num = %d Is Greater Than 500 !!! \n\n", SGK_num);

	else // *** THIS IS THE TERMINATING 'ELSE' OF THIS 'IF - ELSE IF - ELSE' LADDER !!! ***	
		printf("Invalid Value Entered !!! \n\n");

	return(0);
}
