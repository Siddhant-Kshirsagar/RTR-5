#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

int main(void)
{
	//function prototypes
	int CalculateSum(int, ...);

	//variable declarations
	int SGK_answer;

	//code
	printf("\n\n");
	SGK_answer = CalculateSum(5, 10, 20, 30, 40, 50);
	printf("Answer = %d\n\n", SGK_answer);

	SGK_answer = CalculateSum(10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	printf("Answer = %d\n\n", SGK_answer);

	SGK_answer = CalculateSum(0);
	printf("Answer = %d\n\n", SGK_answer);

	return(0);
}

int CalculateSum(int SGK_num, ...) // variadic function
{
	//variable declarations
	int SGK_sum_total = 0;
	int SGK_n;
	
	va_list SGK_number_list;
	//code
	va_start(SGK_number_list,SGK_num);

	while (SGK_num)
	{
		SGK_n = va_arg(SGK_number_list, int);
		SGK_sum_total = SGK_sum_total + SGK_n;
		SGK_num--;
	}

	va_end(SGK_number_list);
	return(SGK_sum_total);
}

