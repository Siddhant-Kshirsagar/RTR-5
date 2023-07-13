#include<stdio.h>
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
	//function declarations
	int Sum(int, va_list);
	//variable declarations
	int SGK_sum_total = 0;
	int SGK_n;

	va_list SGK_number_list;

	//code
	va_start(SGK_number_list, SGK_num);
	SGK_sum_total = Sum(SGK_num, SGK_number_list);
	va_end(SGK_number_list);
	return(SGK_sum_total);
}

int Sum(int SGK_n, va_list SGK_list)
{
	//variable declarations
	int SGK_sum = 0;
	int SGK_num;
	
	//code
	while(SGK_n)
	{
		SGK_num = va_arg(SGK_list, int);
		SGK_sum = SGK_sum + SGK_num;
		SGK_n--;
	}
	return(SGK_sum);
}