#include<stdio.h> // for printf()
#include<conio.h>// for getch()

int main(void)
{
	//variable declarations
	int SGK_a, SGK_b;
	int SGK_result;

	char SGK_option, SGK_option_division;

	//code 
	printf("\n\n");

	printf("Enter Value For 'A' : ");
	scanf("%d", &SGK_a);
	
	printf("Enter Value For 'B' : ");
	scanf("%d", &SGK_b);

	printf("Enter Option In Character : \n\n");
	printf("'A' or 'a' For Addition : \n");
	printf("'S' or 's' For Subtraction : \n");
	printf("'M' or 'm' For Multiplication : \n");
	printf("'D' or 'd' For Division : \n");

	printf("Enter Option : ");
	SGK_option = getch();
	
	printf("\n\n");
	if (SGK_option == 'A' || SGK_option == 'a')
	{
		SGK_result = SGK_a + SGK_b;
		printf("Addition Of A = %d And B = %d Gives Result %d !!! \n\n", SGK_a, SGK_b, SGK_result);
	}
	else if (SGK_option == 'S' || SGK_option == 's')
	{
		if (SGK_a >= SGK_b)
		{
			SGK_result = SGK_a - SGK_b;
			printf("Subtraction Of B = %d From A = %d Gives Result %d !!! \n\n", SGK_b, SGK_a, SGK_result);
		}
		else
		{
			SGK_result = SGK_b - SGK_a;
			printf("Subtraction Of A = %d From B = %d Gives Result = %d !!!\n\n", SGK_a, SGK_b, SGK_result);
		}
	}
	else if (SGK_option == 'M' || SGK_option == 'm')
	{
		SGK_result = SGK_a * SGK_b;
		printf("Multiplication Of A = %d And B = %d Gives Result = %d !!! \n\n", SGK_a, SGK_b, SGK_result);
	}
	else if (SGK_option == 'D' || SGK_option == 'd')
	{
		printf("Enter Option In Character : \n\n");
		printf("'Q' or 'q' or '/' For Quotient Upon Division : \n");
		printf("'R' or 'r' or '%%' For Remainder Upon Division : \n\n");

		printf("Enter Option : ");
		SGK_option_division = getch();

		printf("\n\n");

		if (SGK_option_division == 'Q' || SGK_option_division == 'q' || SGK_option_division == '/')
		{
			if (SGK_a >= SGK_b)
			{
				SGK_result = SGK_a / SGK_b;
				printf("Division Of A = %d By B = %d Gives Quotient = %d !!! \n\n", SGK_a, SGK_b, SGK_result);
			}
			else
			{
				SGK_result = SGK_b / SGK_a;
				printf("Divison Of B = %d By A = %d Gives Quotient = %d !!! \n\n", SGK_b, SGK_a, SGK_result);
			}
		}
		else if (SGK_option_division == 'R' || SGK_option_division == 'r' || SGK_option_division == '%')
		{
			if (SGK_a > SGK_b)
			{
				SGK_result = SGK_a % SGK_b;
				printf("Division Of A = %d By B = %d Gives Remainder = %d !!! \n\n", SGK_a, SGK_b, SGK_result);
			}
			else
			{
				SGK_result = SGK_b % SGK_a;
				printf("Division Of B = %d By A = %d Gives Remainder = %d !!! \n\n", SGK_b, SGK_a, SGK_result);
			}
		}
		else
			printf("Invalid Character %c Entered For Division !!! Please Try Again... \n\n", SGK_option_division);
	}
	else
		printf("Invalid Character %c Entered !!! Please Try Again ...\n\n", SGK_option);

	return(0);
}
