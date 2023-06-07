#include<stdio.h>
#include<conio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j;

	//code 
	printf("\n\n");

	for (SGK_i = 1; SGK_i <= 20; SGK_i++)
	{
		for (SGK_j = 1; SGK_j <= 20; SGK_j++)
		{
			if (SGK_j > SGK_i)
			{
				break;
			}
			else
			{
				printf("* ");
			}
		}
		printf("\n");
	}
	printf("\n\n");
	return(0);
}
