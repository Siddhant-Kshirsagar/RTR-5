// RecursiveFunction.c

#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	//variable declarations 
	unsigned int SGK_num;

	//function prototype
	void recursive(unsigned int);

	//code 
	printf("\n\n");
	printf("Enter Any Number : \n\n");
	scanf("%u", &SGK_num);

	printf("\n\n");
	printf("Output of Recursive Function : \n\n");

	recursive(SGK_num);

	printf("\n\n");
	return(0);
}

void recursive(unsigned int SGK_n)
{
	//code 
	printf("n = %d\n", SGK_n);

	if (SGK_n > 0)
	{
		recursive(SGK_n - 1);
	}
}
