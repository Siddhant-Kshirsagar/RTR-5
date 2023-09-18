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

	switch (SGK_num_month)
	{
	case 1://like 'if'
		printf("Month Number %d Is JANUARY !!! \n\n", SGK_num_month);
		break;
	
	case 2://like 'else if'
		printf("Month Number %d Is FEBRUARY !!! \n\n", SGK_num_month);
		break;
		
	case 3://like 'else if'
		printf("Month Number %d Is MARCH !!! \n\n", SGK_num_month);
		break;

	case 4://like 'else if'
		printf("Month Number %d Is APRIL !!! \n\n", SGK_num_month);
		break;

	case 5://like 'else if'
		printf("Month Number %d Is MAY !!! \n\n ", SGK_num_month);
		break;

	case 6://like 'else if'
		printf("Month Number %d Is JUNE !!! \n\n", SGK_num_month);
		break;

	case 7://like 'else if'
		printf("Month Number %d Is JULY !!! \n\n", SGK_num_month);
		break;

	case 8://like 'else if'
		printf("Month Number %d Is AUGUST !!! \n\n", SGK_num_month);
		break;

	case 9://like 'else if'
		printf("Month Number %d Is SEPTEMBER !!! \n\n", SGK_num_month);
		break;

	case 10://like 'else if'
		printf("Month Number %d Is OCTOBER !!! \n\n", SGK_num_month);
		break;

	case 11://like 'else if'
		printf("Month Number %d Is NOVEMBER !!! \n\n", SGK_num_month);
		break;

	case 12://like 'else if'
		printf("Month Number %d Is DECEMBER !!! \n\n", SGK_num_month);
		break;

	default://like ending optional 'else' ...just like terminating 'else' is optional in if-else if-else ladder, so is the 'default' case optional in switch-case
		printf("Invalid Month Number %d Entered !!! Please Try Again ...\n\n", SGK_num_month);
		break;
	}

	printf("Switch Case Block Complete !!! \n");

	return(0);
}
