#include<stdio.h>

int main(void)
{
	//function declarations
	int AddInteger(int, int);
	int SubtractIntegers(int, int);
	float AddFloats(float, float);

	//variable declaration
	typedef int (*AddIntsFnPtr)(int, int);
	AddIntsFnPtr SGK_ptrAddTwoIntegers = NULL;
	AddIntsFnPtr SGK_ptrFunc = NULL;

	typedef float (*AddFloatsFnPtr)(float, float);
	AddFloatsFnPtr SGK_ptrAddTwoFloats = NULL;

	int SGK_iAnswer = 0;
	float SGK_fAnswer = 0.0f;

	//code
	SGK_ptrAddTwoIntegers = AddInteger;
	SGK_iAnswer = SGK_ptrAddTwoIntegers(9, 30);
	printf("\n\n");
	printf("Sum Of Integers = %d\n\n", SGK_iAnswer);

	SGK_ptrFunc = SubtractIntegers;
	SGK_iAnswer = SGK_ptrFunc(9, 30);
	printf("\n\n");
	printf("Subtraction Of Integers = %d\n\n", SGK_iAnswer);

	SGK_ptrAddTwoFloats = AddFloats;
	SGK_fAnswer = SGK_ptrAddTwoFloats(11.45f, 8.2f);
	printf("\n\n");
	printf("Sum Of Floating-Point Numbers = %f\n\n", SGK_fAnswer);

	return(0);
}

int AddInteger(int SGK_a, int SGK_b)
{
	//variable declarations
	int SGK_c;

	//code
	SGK_c = SGK_a + SGK_b;
	return(SGK_c);
}

int SubtractIntegers(int SGK_a,int SGK_b)
{
	//variable declarations
	int SGK_c;
	
	//code
	if(SGK_a>SGK_b)
	{
		SGK_c = SGK_a - SGK_b;
	}
	else
	{
		SGK_c = SGK_b - SGK_a;
	}

	return(SGK_c);
}

float AddFloats(float SGK_f_num1, float SGK_f_num2)
{
	//variable declarations
	float SGK_ans;

	//code
	SGK_ans = SGK_f_num1 + SGK_f_num2;
	return(SGK_ans);
}
