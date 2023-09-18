#include<stdio.h>//'stdio.h' contains declarations of 'printf()'

//Entry-Point Function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char* argv[], char* envp[])
{
	//function prototype / declaration / signature
	void MyAddition(int, int);
	
	//variable declarations : local variables to main()
	int SGK_a, SGK_b;
	
	//code 
	printf("\n\n");
	printf("Enter Integer Value For 'A' : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Integer Value For 'B' : ");
	scanf("%d",&SGK_b);

	MyAddition(SGK_a, SGK_b);//function call

	return(0);
}

// ***** USER DEFINED FUNCTION : METHOD OF DEFINATION 3 *****
// ***** NO RETURN VALUE, VALID PARAMETERS (int,int) *****

void MyAddition(int SGK_a, int SGK_b)//fuction definition
{
	//variable declarations : local variables to MyAddition()
	int SGK_sum;

	//code 
	SGK_sum = SGK_a + SGK_b;
	printf("\n\n");
	printf("Sum Of %d And %d  = %d \n\n", SGK_a, SGK_b, SGK_sum);
}
