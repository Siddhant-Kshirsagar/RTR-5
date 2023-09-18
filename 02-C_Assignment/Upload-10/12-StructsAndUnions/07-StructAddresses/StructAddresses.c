#include<stdio.h>

struct MyData
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_c;
};

int main(void)
{
	//variable declarations
	struct MyData SGK_data;

	//code
	//Assigning Data Values To The Data Members Of 'struct MyData'
	SGK_data.SGK_i = 30;
	SGK_data.SGK_f = 11.45f;
	SGK_data.SGK_d = 1.2995;
	SGK_data.SGK_c = 'A';

	//Displaying Values Of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
	printf("i = %d\n", SGK_data.SGK_i);
	printf("f = %f\n", SGK_data.SGK_f);
	printf("d = %lf\n", SGK_data.SGK_d);
	printf("c = %c\n", SGK_data.SGK_c);

	printf("\n\n");
	printf("ADDRESSES OF DATA MEMBER OF 'struct MyData' ARE : \n\n");
	printf("'i' Occupies Addresses From %p\n", &SGK_data.SGK_i);
	printf("'f' Occupies Address From %p\n", &SGK_data.SGK_f);
	printf("'d' Occupies Address From %p\n", &SGK_data.SGK_d);
	printf("'c' Occupies Address From %p\n", &SGK_data.SGK_c);

	printf("Starting Address Of 'struct MyData' variable 'data' = %p\n\n", &SGK_data);

	return(0);
}

