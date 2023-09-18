#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_a, SGK_b;
	int SGK_p, SGK_q;
	char SGK_ch_result_01, SGK_ch_result_02;
	int SGK_i_result_01, SGK_i_result_02;

	//code 
	printf("\n\n");

	SGK_a = 7;
	SGK_b = 5;
	SGK_ch_result_01 = (SGK_a > SGK_b) ? 'A' : 'B';
	SGK_i_result_01 = (SGK_a > SGK_b) ? SGK_a : SGK_b;
	printf("Ternary Operator Answer 1 ------ %c and %d.\n\n", SGK_ch_result_01, SGK_i_result_01);

	SGK_p = 30;
	SGK_q = 30;
	SGK_ch_result_02 = (SGK_p != SGK_q) ? 'P' : 'Q';
	SGK_i_result_02 = (SGK_p != SGK_q) ? SGK_p : SGK_q;
	printf("Ternary Operator Answer 2 ------ %c and %d.\n\n", SGK_ch_result_02, SGK_i_result_02);

	printf("\n\n");
	return(0);
}

