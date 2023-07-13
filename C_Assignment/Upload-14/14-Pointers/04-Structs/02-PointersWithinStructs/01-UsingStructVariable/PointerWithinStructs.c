#include<stdio.h>

// Defining Struct 
struct MyData
{
	int *SGK_ptr_i;
	int SGK_i;
	
	float *SGK_ptr_f;
	float SGK_f;

	double *SGK_ptr_d;
	double SGK_d;
};

int main(void)
{
	//variable declarations
	struct MyData SGK_data;

	//code
	SGK_data.SGK_i = 9;
	SGK_data.SGK_ptr_i = &SGK_data.SGK_i;

	SGK_data.SGK_f = 11.45f;
	SGK_data.SGK_ptr_f = &SGK_data.SGK_f;

	SGK_data.SGK_d = 30.121995;
	SGK_data.SGK_ptr_d = &SGK_data.SGK_d;

	printf("\n\n");
	printf("i = %d\n", *(SGK_data.SGK_ptr_i));
	printf("Address Of 'i' = %p\n", SGK_data.SGK_ptr_i);

	printf("\n\n");
	printf("f = %f\n", *(SGK_data.SGK_ptr_f));
	printf("Address Of 'f' = %p\n", SGK_data.SGK_ptr_f);

	printf("\n\n");
	printf("d = %lf\n", *(SGK_data.SGK_ptr_d));
	printf("Address Of 'd' = %p\n", SGK_data.SGK_ptr_d);

	return(0);
}

