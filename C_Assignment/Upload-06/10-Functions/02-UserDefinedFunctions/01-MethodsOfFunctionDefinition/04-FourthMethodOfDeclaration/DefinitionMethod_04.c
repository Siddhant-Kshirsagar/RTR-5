#include<stdio.h>//'stdio.h' contains declaration of 'printf()'

// Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char* argv[], char* envp[])
{
	//function prototype / declaration / signature
	int MyAddition(int, int);

	//variable declarations : local variables to main()
	int SGK_a, SGK_b, SGK_result;

	//code 
	printf("\n\n");
	printf("Enter Integer Value For 'A' : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Integer Value For 'B' : ");
	scanf("%d", &SGK_b);

	SGK_result = MyAddition(SGK_a, SGK_b);//function call 
	printf("\n\n");
	printf("Sum Of %d And %d = %d \n\n", SGK_a, SGK_b, SGK_result);

	return(0);
}

// ***** USER DEFINED FUNCTION : METHOD OF DEFINITION 4 *****
// ***** VALID (int) RETURN VALUE, VALID PARAMETERS (int,int) *****

int MyAddition(int SGK_a, int SGK_b)//function definition
{
	//variable declarations : local variables to MyAddition()
	int SGK_sum;

	//code 
	SGK_sum = SGK_a + SGK_b;
	return(SGK_sum);
}
