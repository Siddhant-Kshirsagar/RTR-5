#include<stdio.h>
#include<stdlib.h>

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
	struct MyData *SGK_pData = NULL;

	//code
	printf("\n\n");
	SGK_pData = (struct MyData *)malloc(sizeof(struct MyData));
	if (SGK_pData == NULL)
	{
		printf("Failed To Allocate Memory To 'struct  MyData' !!! Exitting Now ... \n\n");
		exit(0);
	}
	else
	{
		printf("Successfully Allocated Memory To 'struct MyData' !!!\n\n");
	}

	(*SGK_pData).SGK_i = 9;
	(*SGK_pData).SGK_ptr_i = &(*SGK_pData).SGK_i;

	(*SGK_pData).SGK_f = 11.45f;
	(*SGK_pData).SGK_ptr_f = &(*SGK_pData).SGK_f;

	(*SGK_pData).SGK_d = 30.121995;
	(*SGK_pData).SGK_ptr_d = &(*SGK_pData).SGK_d;

	printf("\n\n");
	printf("i = %d\n", *((*SGK_pData).SGK_ptr_i));
	printf("Address Of 'i'= %p\n", (*SGK_pData).SGK_ptr_i);

	printf("\n\n");
	printf("f = %f\n", *((*SGK_pData).SGK_ptr_f));
	printf("Address Of 'f' = %p\n", (*SGK_pData).SGK_ptr_f);

	printf("\n\n");
	printf("d = %lf\n", *((*SGK_pData).SGK_ptr_d));
	printf("Address Of 'd' = %p\n", (*SGK_pData).SGK_ptr_d);

	if (SGK_pData)
	{
		free(SGK_pData);
		SGK_pData = NULL;
		printf("Memory Allocated To 'struct MyData' Has Been Successfully Freed !!!\n\n");
	}

	return(0);
}
