#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_age;

	//code 
	printf("\n\n");
	printf("Enter Age : ");
	scanf("%d", &SGK_age);
	if (SGK_age >= 18)
	{
		printf("You Are Eligible For Voting !!!\n\n");
	}
	return(0);
}
