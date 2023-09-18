#include<stdio.h>//'stdio.h' contains declaration of 'printf()'
#include<stdlib.h>//'stdlib.h' contains declaration of 'exit()'

int main(int argc, char* argv[], char* envp[])
{
	//variable declations
	int SGK_i;

	//code 
	if (argc != 4)//Program name + first name + middle name + surname = 4 command line arguments are required
	{
		printf("\n\n");
		printf("Invalid Usage !!! Exitting Now ... \n\n");
		printf("Usage : CommandLineArgumentsApplication <first name> <middle name> <surname>\n\n");
		exit(0);
	}

	//***THIS PROGRAMS PRINTS YOUR FULL NAME AS ENTERED IN THE COMMAND LINE ARGUMENTS ***
	printf("\n\n");
	printf("Your Full Name Is : ");
	for (SGK_i = 1; SGK_i < argc; SGK_i++)//loop starts fro i = 1 because, i = 0 will result in 'argv[i]' = 'argv[0]' which is the name of the program itself i.e : 'commandLineArgumentsApplication.exe'
	{
		printf("%s ", argv[SGK_i]);
	}

	printf("\n\n");

	return(0);
} 
