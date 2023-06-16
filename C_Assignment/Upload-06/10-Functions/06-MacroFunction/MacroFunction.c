#include<stdio.h>

#define MAX_NUMBER(SGK_a, SGK_b) ((SGK_a> SGK_b)? SGK_a : SGK_b)

int main(int argc, char* argv[], char* envp[])
{
	//variable declarations
	int SGK_iNum_01;
	int SGK_iNum_02;
	int SGK_iResult;

	float SGK_fNum_01;
	float SGK_fNum_02;
	float SGK_fResult;

	//code 
	//****** COMPARING INTEGER VALUES ******
	printf("\n\n");
	printf("Enter An Integer Number : \n\n");
	scanf("%d", &SGK_iNum_01);

	printf("\n\n");
	printf("Enter Another Integer Number : \n\n");
	scanf("%d", &SGK_iNum_02);

	SGK_iResult = MAX_NUMBER(SGK_iNum_01, SGK_iNum_02);
	printf("\n\n");
	printf("Result Of Macro Function MAX_NUMBER() = %d", SGK_iResult);

	printf("\n\n");

	// ***** COMPARING FLOATING-POINT VALUES *****
	printf("\n\n");
	printf("Enter A Floating Point Number : \n\n");
	scanf("%f", &SGK_fNum_01);

	printf("\n\n");
	printf("Enter Another Floating Point Number : \n\n");
	scanf("%f", &SGK_fNum_02);

	SGK_fResult = MAX_NUMBER(SGK_fNum_01, SGK_fNum_02);

	printf("\n\n");
	printf("Result Of Macro Function MAX_NUMBER() = %f \n", SGK_fResult);

	printf("\n\n");

	return(0);
}

