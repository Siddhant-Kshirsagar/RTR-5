#include<stdio.h>
#include<stdarg.h>

#define NUM_TO_BE_FOUND 3
#define NUM_ELEMENTS 10

int main(void)
{
	//function prototypes
	void FindNumber(int, int, ...);

	//code
	printf("\n\n");

	FindNumber(NUM_TO_BE_FOUND, NUM_ELEMENTS, 3, 5, 9, 2, 3, 6, 9, 3, 1, 3);

	return(0);
}

void FindNumber(int SGK_num_to_be_found, int SGK_num, ...) // Variadic function
{
	//function prototypes
	int va_FindNumber(int, int, va_list);

	//variable declarations
	int SGK_count = 0;
	int SGK_n;
	va_list SGK_numbers_list;

	//code
	va_start(SGK_numbers_list, SGK_num);

	SGK_count = va_FindNumber(SGK_num_to_be_found, SGK_num, SGK_numbers_list);

	if (SGK_count==0)
		printf("Number %d Could Not Be Found !!!\n\n", SGK_num_to_be_found);
	else
		printf("Number %d Found %d Times !!!\n\n", SGK_num_to_be_found, SGK_count);

	va_end(SGK_numbers_list);
}

int va_FindNumber(int SGK_num_to_be_found, int SGK_num, va_list SGK_list)
{
	//variable declarations
	int SGK_count_of_num = 0;
	int SGK_n;

	//code
	while (SGK_num)
	{
		SGK_n = va_arg(SGK_list, int);
		if (SGK_n == SGK_num_to_be_found)
			SGK_count_of_num++;

		SGK_num--;
	}
	return(SGK_count_of_num);
}

