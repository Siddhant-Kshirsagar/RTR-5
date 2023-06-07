#include<stdio.h>
int main(void)
{
	//variable declarations
	float SGK_f;
	float SGK_f_num = 1.7f; //simply change this ONLY to get different outputs...

	//code 
	printf("\n\n");

	printf("Printing Numbers %f to %f : \n\n", SGK_f_num, (SGK_f_num * 10.0f));

	for (SGK_f = SGK_f_num; SGK_f <= (SGK_f_num * 10.0f); SGK_f = SGK_f + SGK_f_num)
	{
		printf("\t%f\n", SGK_f);
	}

	printf("\n\n");

	return(0);
}
