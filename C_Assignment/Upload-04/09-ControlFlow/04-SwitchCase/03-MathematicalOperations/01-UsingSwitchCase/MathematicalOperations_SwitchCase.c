#include<stdio.h>//for printf()
#include<conio.h>//for getch()

int main(void)
{
	//variable declarations
	int SGK_a, SGK_b;
	int SGK_result;

	char SGK_option, SGK_option_division;

	//code 
	printf("\n\n");
	printf("Enter Value For 'A' : ");
	scanf("%d",&SGK_a);

	printf("Enter Value For 'B' : ");
	scanf("%d", &SGK_b);

	printf("Enter Option In Character : \n\n");
	printf("'A' or 'a' For Addition : \n ");
	printf("'S' or 's' For Subtraction : \n");
	printf("'M' or 'm' For Multiplication : \n");
	printf("'D' or 'd' For Division : \n\n");

	printf("Enter Options : ");
	SGK_option = getch();

	printf("\n\n");

	switch (SGK_option)
	{
		//FALL Throught CONSITION FOR 'A' and 'a'
	case 'A':
	case 'a':
		SGK_result = SGK_a + SGK_b;
		printf("Addition Of A = %d And B = %d Gives Result = %d !!! \n\n", SGK_a, SGK_b, SGK_result);
		break;

		//Fall Through CONSITION For 'S' and 's;
	case 'S':
	case 's':
		if (SGK_a > SGK_b)
		{
			SGK_result = SGK_a - SGK_b;
			printf("Subtraction Of B = %d From A = %d Gives %d !!! \n\n", SGK_b, SGK_a, SGK_result);
		}
		else
		{
			SGK_result = SGK_b - SGK_a;
			printf("Subtraction Of A = %d From B = %d Gives Result %d !!! \n\n", SGK_a, SGK_b, SGK_result);
		}
		break;

		//Fall Throught CONSITION For 'M' and 'm'
	case 'M':
	case 'm':
		SGK_result = SGK_a * SGK_b;
		printf("Multiplication Of A = %d And B = %d Gives Result %d !!\n\n", SGK_a, SGK_b, SGK_result);
		break;

		//Fall Through CONSITION For 'D' and 'd' 
		
	case 'D':
	case 'd':
		printf("Enter Option In Character : \n\n");
		printf("'Q' or 'q' or '/' For Quotient Upon Division : \n");
		printf("'R' or 'r' or '%%' FOr Remainder Upon Division : \n");

		printf("Enter Option : ");
		SGK_option_division = getch();

		printf("\n\n");

		switch (SGK_option_division)
		{
			//FALL Through CONSITION FOR 'Q' And 'q' and '/'
		case 'Q':
		case 'q':
		case '/':
			if (SGK_a > SGK_b)
			{
				SGK_result = SGK_a / SGK_b;
				printf("Division Of A = %d By B = %d Gives Quotient = %d !!! \n\n", SGK_a, SGK_b, SGK_result);
			}
			else
			{
				SGK_result = SGK_b / SGK_a;
				printf("Division Of B = %d By A = %d Gives Quotient = %d !!! \n\n", SGK_b, SGK_a, SGK_result);
			}
			break;// 'break' of case 'Q' or case 'q' or case '/'

			//FALL Through CONSITION FOR 'R' and 'r' and '%'
		case 'R':
		case 'r':
		case '%':
			if (SGK_a >= SGK_b)
			{
				SGK_result = SGK_a % SGK_b;
				printf("Division Of A = %d By B = %d Remainder = %d !!! \n\n", SGK_a, SGK_b, SGK_result);
			}
			else
			{
				SGK_result = SGK_b % SGK_a;
				printf("Division Of B = %d By A = %d Gives Remainder = %d !!!\n\n", SGK_b, SGK_a, SGK_result);
			}
			break;// 'break' of case 'R' or case 'r' or case '%'
		default :// 'default'case for switch (option_division)
			printf("Invalid Character %c Entered For Division !!! Please Try Again...\n\n", SGK_option_division);
			break;// 'break' of 'default' of switch(option_division)

		}//ending curly brace of switch (option_division)

		break;//'break' of case 'D' or case 'd'
	default : // 'default' case for switch(option)
		printf("Invalid Character %c Entered !!! Please Try Again ...\n\n", SGK_option);
		break;
	}//ending curly brace of switch (option)
	printf("Switch Case Block Complete !!! \n");
	return(0);
}

