#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_num_month;

	//code 
	printf("\n\n");

	printf("Enter Number Of Month (1 to 12) : ");
	scanf("%d", &SGK_num_month);

	printf("\n\n");

	//IF - ELSE - IF LADDER BEGINS FROM HERE
	if (SGK_num_month == 1)//like 'case 1'
		printf("Month Number %d Is January !!!\n\n", SGK_num_month);

	else if (SGK_num_month == 2) //like 'case 2'
		printf("Month Number %d Is FEBRUARY !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 3) // like 'case 3'
		printf("Month Number %d Is MARCH !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 4) //like 'case 4'
		printf("Month Number %d Is APRIL !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 5) //like 'case 5'
		printf("Montn Number %d Is MAY !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 6) //like 'case 6'
		printf("Month Number = %d Is JUNE !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 7) //like 'case 7'
		printf("Month Number = %d Is JULY !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 8) //like 'case 8'
		printf("Month Number = %d Is AUGUST !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 9) //like 'case 9'
		printf("Month Number = %d Is SEPTEMBER !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 10) //like 'case 10'
		printf("Month Number = %d Is OCTOBER !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 11) // like 'case 11'
		printf("Month Number = %d Is NOVEMBER !!! \n\n", SGK_num_month);

	else if (SGK_num_month == 12) // like 'case 12'
		printf("Month Number = %d Is DECEMBER !!!\n\n", SGK_num_month);

	else //like 'default' ... just like 'default' is optional in swith - case , so is 'else' in the if-else if-else ladder...
		printf("Invalid Month Number %d Entered !!! Please Try Again ... \n\n", SGK_num_month);

	printf("If - Else If - Else Ladder Complete !!! \n");

	return(0);
}
