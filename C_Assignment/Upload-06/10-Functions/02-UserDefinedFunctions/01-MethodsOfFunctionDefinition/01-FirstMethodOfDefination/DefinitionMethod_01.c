#include<stdio.h>//'stdio.h' contains declaration of 'printf()'

//Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char* argv[], char* envp[])
{
	//function prototype /declaration / signature 
	void MyAddition(void);

	//code 
	MyAddition();//function call
	return(0);
}

//***** User Defined Function : Method Of Definition 1 *****
//***** No Return Value, No Parameters *****

void MyAddition(void)//function definition
{
	//variable declarations : local variable to MyAddition()
	int SGK_a, SGK_b, SGK_sum;

	//code 
	printf("\n\n");
	printf("Enter Integer Value For 'A' : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Integer Value For 'B' : ");
	scanf("%d", &SGK_b);
	
	SGK_sum = SGK_a + SGK_b;

	printf("\n\n");
	printf("Sum Of %d And %d = %d \n\n", SGK_a, SGK_b, SGK_sum);
}	
