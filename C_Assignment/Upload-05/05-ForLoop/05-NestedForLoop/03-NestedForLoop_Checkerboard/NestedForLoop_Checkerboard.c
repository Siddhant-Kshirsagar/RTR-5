#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j, SGK_c;

	//code 
	printf("\n\n");
	for (SGK_i = 0; SGK_i < 64; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < 64; SGK_j++)
		{
			SGK_c = ((SGK_i & 0x8) == 0) ^ ((SGK_j & 0x8) == 0);

			if (SGK_c == 0)
				printf("  ");

			if (SGK_c == 1)
				printf("* ");
		}
		printf("\n\n");
	}
	return(0);
}
