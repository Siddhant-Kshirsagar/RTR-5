#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	//function prototypes OR declarations
	int MyAddition(int, int);

	//variable declarations
	int SGK_r;
	int SGK_num_01, SGK_num_02;
	int SGK_num_03, SGK_num_04;

	//code
	SGK_num_01 = 10;
	SGK_num_02 = 20;
	SGK_num_03 = 30;
	SGK_num_04 = 40;

	SGK_r = MyAddition(MyAddition(SGK_num_01, SGK_num_02), MyAddition(SGK_num_03, SGK_num_04));//return value of MyAddition() is sent as parameter to another call to MyAddition()

	printf("\n\n");
	printf("%d + %d + %d + %d = %d\n", SGK_num_01, SGK_num_02, SGK_num_03, SGK_num_04, SGK_r);
	printf("\n\n");

	return(0);
}

//***** Function Definition Of MyAddition() *****
int MyAddition(int SGK_a, int SGK_b) //function definition
{
	//variable declaration
	int SGK_sum;

	//code 
	SGK_sum = SGK_a + SGK_b;
	return(SGK_sum);
}

