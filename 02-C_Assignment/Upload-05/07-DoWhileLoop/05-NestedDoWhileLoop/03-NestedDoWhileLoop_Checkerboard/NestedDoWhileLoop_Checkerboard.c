#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j, SGK_c;

	//code 
	printf("\n\n");

	SGK_i = 0;
	do
	{
		SGK_j = 0;
		do
		{
			SGK_c = ((SGK_i & 0x8) == 0) ^ ((SGK_j & 0x8) == 0);

			if (SGK_c == 0)
				printf("  ");

			if (SGK_c == 1)
				printf("* ");

			SGK_j++;
		} while (SGK_j < 64);
		printf("\n\n");
		SGK_i++;
	} while (SGK_i < 64);
	return(0);
}
