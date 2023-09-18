#include<stdio.h>//'stdio.h' contains declaration of 'printf()'

//Entry-Point Function => main() => Valid Return Type (int) and 2 Parameters (int argc, char *argv[])
int main(int argc, char* argv[])
{
	//variable declarations
	int SGK_i;

	//code 
	printf("\n\n");
	printf("Hello World !!!\n\n");//Library Function
	printf("Number of Command Line Arguments = %d\n\n", argc);

	printf("Command Line Arguments Passed To This Program Are :\n\n");
	for (SGK_i = 0; SGK_i < argc; SGK_i++)
	{
		printf("Command Line Argument Number %d = %s\n", (SGK_i + 1), argv[SGK_i]);
	}
	printf("\n\n");
	return(0);
}
