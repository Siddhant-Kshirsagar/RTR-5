#include<stdio.h>

int main(void)
{
	//function prototypes
	void PrintBinaryFormOfNumber(unsigned int);

	//variable declaration
	unsigned int SGK_a;
	unsigned int SGK_num_bits;
	unsigned int SGK_result;

	//code 
	printf("\n\n");
	printf("Enter An Integer = ");
	scanf("%u", &SGK_a);

	printf("\n\n");
	printf("By How Many Bits Do You Want To Shift A = %d To The Right ?", SGK_a);
	scanf("%u", &SGK_num_bits);

	printf("\n\n\n\n");
	SGK_result = SGK_a >> SGK_num_bits;
	printf("Bitwise RIGHT-SHIFTing A = %d By %d Bits \nGives The Result = %d (Decimal).\n\n", SGK_a, SGK_num_bits, SGK_result);

	PrintBinaryFormOfNumber(SGK_a);
	PrintBinaryFormOfNumber(SGK_result);

	return(0);
}

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
	//variable declarations 
	unsigned int quotient, remainder;
	unsigned int num;
	unsigned int binary_array[8];
	int i;

	//code
	for (i = 0; i < 8; i++)
		binary_array[i] = 0;

	printf("The Binary Form Of The Decimal Integer %d Is \t = \t", decimal_number);
	num = decimal_number;
	i = 7;
	while (num != 0)
	{
		quotient = num / 2;
		remainder = num % 2;
		binary_array[i] = remainder;
		num = quotient;
		i--;
	}

	for (i = 0; i < 8; i++)
		printf("%u", binary_array[i]);

	printf("\n\n");
}
