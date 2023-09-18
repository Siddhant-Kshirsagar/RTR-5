#include<stdio.h>// 'stdio.h' contains declaration of 'printf()'
#include<ctype.h>// 'ctype.h' contains declaration of 'atoi()'
#include<stdlib.h>// 'stdlib.h' contains declaration of 'exit()'

int main(int argc, char* argv[], char* envp[])
{
	//variable declarations
	int SGK_i;
	int SGK_num;
	int SGK_sum = 0;

	//code 
	if (argc == 1)
	{
		printf("\n\n");
		printf("No Number Given For Addition !!! Exitting now ...\n\n");
		printf("Usage : CommandLineArgumentApplication <first number> <second number> ...\n\n");
		exit(0);
	}

	// *** This Programs adds all command line arguments given in Integer form only and outputs the sum***
	// *** Due to use of atoi(), All Command line arguments of types other than 'int' are ignored ***
	printf("\n\n");
	printf("Sum Of All Integer Command Line Arguments Is : \n\n");
	for (SGK_i = 1; SGK_i < argc; SGK_i++)	//loop starts from i = 1 because, i = 0 will result in 'argv[i]' = 'argv[0]' which is the name of the program itself i.e : 'CommandLineArgumentsApplication.exe'
	{
		SGK_num = atoi(argv[SGK_i]);
		SGK_sum = SGK_sum + SGK_num;
	}

	printf("Sum = %d \n\n", SGK_sum);
	return(0);
}

