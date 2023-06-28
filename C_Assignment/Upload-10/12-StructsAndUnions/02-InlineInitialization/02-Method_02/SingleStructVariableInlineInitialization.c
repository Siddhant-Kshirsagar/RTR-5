#include<stdio.h>

//DEFINING STRUCT 
struct MyData
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_c;
};

struct MyData SGK_data = { 9,8.2f, 9.61998,'P' };

int main(void)
{
	//code 
	//Displaying Values Of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
	printf("i = %d\n", SGK_data.SGK_i);
	printf("f = %f\n", SGK_data.SGK_f);
	printf("d = %lf\n", SGK_data.SGK_d);
	printf("c = %c\n\n", SGK_data.SGK_c);
	return(0);
}
