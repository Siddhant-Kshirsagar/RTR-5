#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	//function prototypes OR declarations
	int MyAddition(int, int);

	//variable declarations
	int SGK_r;
	int SGK_num_01, SGK_num_02;

	//code 
	SGK_num_01 = 10;
	SGK_num_02 = 20;

	printf("\n\n");
	printf("%d + %d = %d\n", SGK_num_01, SGK_num_02, MyAddition(SGK_num_01, SGK_num_02));
	printf("\n\n");

	return(0);
}

//***** Function Definition Of MyAddition() *****
int MyAddition(int SGK_a, int SGK_b) //function definition
{
	//function prototype
	int Add(int, int);

	//code 
	return(Add(SGK_a, SGK_b));
}

int Add(int SGK_x, int SGK_y)
{
	//code
	return(SGK_x + SGK_y);
}

