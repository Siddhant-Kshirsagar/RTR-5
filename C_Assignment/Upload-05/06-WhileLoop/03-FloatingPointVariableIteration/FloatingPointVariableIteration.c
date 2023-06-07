#include<stdio.h>
int main(void)
{
	//variable declarations
	float SGK_f;
	float SGK_f_num = 1.7f; // simply change this value only to get different outputs...

	//code 
	printf("\n\n");
	
	printf("Printing Numbers %f to %f : \n\n", SGK_f_num, (SGK_f_num * 10.0f));

	SGK_f = SGK_f_num;
	while (SGK_f <= (SGK_f_num * 10.0f))
	{
		printf("\t%f\n", SGK_f);
		SGK_f = SGK_f + SGK_f_num;
	}

	printf("\n\n");

	return(0);
}
