#include<stdio.h>
#include<stdlib.h>

// Defining Struct 
struct MyData
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
};

int main(void)
{
	//function prototypes 
	void ChangeValues(struct MyData *);

	//variable declarations
	struct MyData *SGK_pData = NULL;

	//code
	printf("\n\n");

	SGK_pData = (struct MyData *)malloc(sizeof(struct MyData));
	if (SGK_pData == NULL)
	{
		printf("Failed To Allocate Memory To 'struct MyData' !!! Exitting Now \n\n");
		exit(0);
	}
	else
	{
		printf("Successfully Allocated Memory To 'struct MyData' !!!\n\n");
	}

	// Assigning Data Values To The Data Members of 'struct MyData' 
	SGK_pData->SGK_i = 30;
	SGK_pData->SGK_f = 11.45f;
	SGK_pData->SGK_d = 1.2995;

	// Displaying Values Of The Data Members Of 'struct MyData' 
	printf("\n\n");
	printf("Data Members Of 'struct MyData' Are : \n\n");
	printf("i = %d\n", SGK_pData->SGK_i);
	printf("f = %f\n", SGK_pData->SGK_f);
	printf("d = %lf\n", SGK_pData->SGK_d);

	ChangeValues(SGK_pData);

	//Displaying Values Of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("Data Members Of 'struct MyData' Are : \n\n");
	printf("i = %d \n", SGK_pData->SGK_i);
	printf("f = %f \n", SGK_pData->SGK_f);
	printf("d = %lf \n", SGK_pData->SGK_d);

	if (SGK_pData)
	{
		free(SGK_pData);
		SGK_pData = NULL;
		printf("Memory Allocated  To 'struct MyData' Has Been Successfully Freed !!!\n\n");
	}

	return(0);
}

void ChangeValues(struct MyData *SGK_pParam_Data)
{
	//code
	SGK_pParam_Data->SGK_i = 9;
	SGK_pParam_Data->SGK_f = 8.2f;
	SGK_pParam_Data->SGK_d = 6.1998;

	// Can Also Do This As ...
	/*
	* (*pParam_Data).i = 9;
	* (*pParam_Data).f = 8.2f;
	* (*pParam_Data).d = 6.1998;
	*/
}

