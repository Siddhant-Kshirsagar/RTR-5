#include<stdio.h>

int main(void)
{
	//DEFINING STRUCT 
	struct MyData
	{
		int SGK_i;
		float SGK_f;
		double SGK_d;
		char SGK_c;
	}SGK_data = { 5,9.1f, 3.78623, 'N' };

	//Displaying Values Of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("DATA MEMBERS of 'struct MyData' ARE : \n\n");
	printf("i = %d\n", SGK_data.SGK_i);
	printf("f = %f\n", SGK_data.SGK_f);
	printf("d = %lf\n", SGK_data.SGK_d);
	printf("c = %c\n\n", SGK_data.SGK_c);
	return(0);
}

