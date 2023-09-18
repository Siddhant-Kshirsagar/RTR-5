#include<stdio.h>//'stdio.h' contains declaration of 'printf()'

// ***** USER DEFINED FUNCTIONS : METHOD OF CALLING FUNCTION 1 *****
// ***** CALLING ALL FUNCTIONS IN main() DIRECTLY *****

// Entry-point function => main() => Valid Return Type (int) and 3 Parameters (int argc, char *argv[], char *envp[])
int main(int argc, char* argv[], char* envp[])
{
	//function prototypes or declarations
	void MyAddition(void);
	int MySubtraction(void);
	void MyMultiplication(int,int);
	int MyDivision(int,int);

	//variable declarations
	int SGK_result_subtraction;
	int SGK_a_multiplication, SGK_b_multiplication;
	int SGK_a_division, SGK_b_division, SGK_result_division;

	//code 

	//***** ADDITION *****
	MyAddition();//function call

	// ***** SUBTRACTION *****
	SGK_result_subtraction = MySubtraction();//function call
	printf("\n\n");
	printf("Subtraction Yields Result = %d\n", SGK_result_subtraction);

	// *****Multiplication *****
	printf("\n\n");
	printf("Enter Integer Value For 'A' For Multiplication : ");
	scanf("%d", &SGK_a_multiplication);

	printf("\n\n");
	printf("Enter Integer value For 'B' For Multiplication : ");
	scanf("%d", &SGK_b_multiplication);

	MyMultiplication(SGK_a_multiplication, SGK_b_multiplication);//function call

	//***** DIVISION *****
	printf("\n\n");
	printf("Enter Integer Value For 'A' For Division : ");
	scanf("%d", &SGK_a_division);

	printf("\n\n");
	printf("Enter Integer Value For 'B' For Division : ");
	scanf("%d", &SGK_b_division);
	
	SGK_result_division = MyDivision(SGK_a_division, SGK_b_division);//function call
	printf("\n\n");
	printf("Division Of %d and %d Gives = %d (Quotient)\n", SGK_a_division, SGK_b_division, SGK_result_division);

	printf("\n\n");

	return(0);
}

//*****	Function Definition Of MyAddition() *****
void MyAddition(void)//function definition
{
	//variable declarations : local variables to MyAddition()
	int SGK_a, SGK_b, SGK_sum;
	//code 
	printf("\n\n");
	printf("Enter Integer Value For 'A' For Addition : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Integer Value For 'B' For Addition : ");
	scanf("%d", &SGK_b);

	SGK_sum = SGK_a + SGK_b;

	printf("\n\n");
	printf("Sum Of %d And %d = %d \n\n", SGK_a, SGK_b, SGK_sum);
}

//***** Function Definition Of MySubtraction() *****
int MySubtraction(void)//function definition
{
	//variable declarations : local variables to MySubtraction()
	int SGK_a, SGK_b, SGK_subtraction;

	//code 
	printf("\n\n");
	printf("Enter Integer Value For 'A' For Subtraction : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Integer Value For 'B' For Subraction : ");
	scanf("%d", &SGK_b);

	SGK_subtraction = SGK_a - SGK_b;
	return(SGK_subtraction);
}

// ***** Function Definition Of MyMultiplication() *****
void MyMultiplication(int SGK_a, int SGK_b) //function definition
{
	//variable declarations : local variable to MyMultiplication()
	int SGK_multiplication;

	//code 
	SGK_multiplication = SGK_a * SGK_b;

	printf("\n\n");
	printf("Multiplication Of %d And %d = %d \n\n", SGK_a, SGK_b, SGK_multiplication);
}

// ***** Function Definition Of MyDivision() *****
int MyDivision(int SGK_a, int SGK_b)//function definition
{
	//variable declarations : local variables to MyDivision()
	int SGK_division_quotient;

	//code 
	if (SGK_a > SGK_b)
		SGK_division_quotient = SGK_a / SGK_b;
	else
		SGK_division_quotient = SGK_b / SGK_a;

	return(SGK_division_quotient);
}

