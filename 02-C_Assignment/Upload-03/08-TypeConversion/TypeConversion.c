#include<stdio.h>

int main(void) 
{
	//variable declarations
	int SGK_i,SGK_j;
	char SGK_ch_01, SGK_ch_02;

	int SGK_a, SGK_result_int;
	float SGK_f, SGK_result_float;

	int SGK_i_explicit;
	float SGK_f_explicit;

	//code 
	printf("\n\n");

	//INTERCOVERSION AND IMPLICIT TYPE-CASTING BETWEEN 'char' AND 'int' Types....
	SGK_i = 70;
	SGK_ch_01 = SGK_i;
	printf("I = %d\n", SGK_i);
	printf("Character 1 (after ch_01 = i) = %c\n\n", SGK_ch_01);

	SGK_ch_02 = 'Q';
	SGK_j = SGK_ch_02;
	printf("Character 2 = %c \n", SGK_ch_02);
	printf("J (after j = ch_02) = %d\n\n", SGK_j);

	//IMPLICIT CONVERSION OF 'int' TO 'float'...
	SGK_a = 5;
	SGK_f = 7.8f;
	SGK_result_float = SGK_a + SGK_f;
	printf("Integer a = %d And Floating-Point Number %f Added Gives Floating-Point Sum = %f\n\n", SGK_a, SGK_f, SGK_result_float);

	SGK_result_int = SGK_a + SGK_f;
	printf("Integer a = %d And Floating-Point Number %f Added Gives Integer Sum = %d\n\n", SGK_a, SGK_f, SGK_result_int);

	//EXPLICIT TYPE-CASTING USING CAST OPERATOR...
	SGK_f_explicit = 30.121995;
	SGK_i_explicit = (int)SGK_f_explicit;
	printf("Floating Point Number Which Will Be Type Casted Explicitly = %f\n", SGK_f_explicit);
	printf("Resultant Integer After Explicit Type Casting Of %f = %d\n\n", SGK_f_explicit, SGK_i_explicit);

	return(0);
}

