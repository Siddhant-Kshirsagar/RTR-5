#include<stdio.h>//'stdio.h' contains declaration of 'printf()'

//Entry-Point Function => main() => Valid Return Type(int) and 3 Parametes (int argc, char *argv[], char *envp[])
int main(int argc, char* argv[], char* envp[])
{
	//function prototype / declaration / signature
	int MyAddition(void);

	//variable declarations : local variables to main()
	int SGK_result;

	//code 
	SGK_result = MyAddition();//function call

	printf("\n\n");
	printf("Sum = %d \n\n", SGK_result);
	return(0);
}

// ***** User Defined Function : Method Of Definition 2 *****
// ***** Valid (int) return value, no parameters *****

int MyAddition(void)
{
	//variable declarations : local variables to MyAddition()
	int SGK_a, SGK_b, SGK_sum;
	
	//code
	printf("\n\n");
	printf("Enter Integer Value For 'A' : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Integer Value For 'B' : ");
	scanf("%d", &SGK_b);

	SGK_sum = SGK_a + SGK_b;

	return(SGK_sum);
}
