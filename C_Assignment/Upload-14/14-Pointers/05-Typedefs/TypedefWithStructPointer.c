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
	//variable declarations
	int SGK_i_size;
	int SGK_f_size;
	int SGK_d_size;
	int SGK_MyData_size;
	int SGK_pointer_to_struct_MyData_size;

	typedef struct MyData *MyDataPtr;

	MyDataPtr SGK_pData;

	//code
	printf("\n\n");

	SGK_pData = (MyDataPtr)malloc(sizeof(struct MyData));
	if (SGK_pData == NULL)
	{
		printf("Failed To Allocate Memory To 'struct MyData' !!! Exitting Now...\n\n");
		exit(0);
	}
	else
	{
		printf("Successfully Allocated Memory To 'struct MyData' !!!\n\n");
	}

	//Assigning Data Values To The Data Members Of 'struct MyData'
	SGK_pData->SGK_i = 30;
	SGK_pData->SGK_f = 11.45f;
	SGK_pData->SGK_d = 1.2995;

	//Displaying Values Of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("Data Members Of 'struct MyData' Are : \n\n");
	printf("i = %d \n", SGK_pData->SGK_i);
	printf("f = %f \n", SGK_pData->SGK_f);
	printf("d = % lf \n", SGK_pData->SGK_d);

	// Calculating Sizes (in Bytes) Of The  Data Members Of 'struct MyData'
	SGK_i_size = sizeof(SGK_pData->SGK_i);
	SGK_f_size = sizeof(SGK_pData->SGK_f);
	SGK_d_size = sizeof(SGK_pData->SGK_d);

	//Displaying Sizes (in Bytes) Of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("Sizes (in Bytes) Of Data Members Of 'struct MyData' Are : \n\n");
	printf("Size of 'i' = %d bytes\n", SGK_i_size);
	printf("Size of 'f' = %d bytes\n", SGK_f_size);
	printf("Size of 'd' = %d bytes\n", SGK_d_size);

	//Calculating  Size (in Bytes) Of The entire 'struct MyData'
	SGK_MyData_size = sizeof(struct MyData);
	SGK_pointer_to_struct_MyData_size = sizeof(MyDataPtr);

	//Displaying Sizes (in Bytes) Of the entire 'struct MyData'
	printf("\n\n");
	printf("Size of 'struct MyData' :  %d bytes\n\n", SGK_MyData_size);
	printf("Size of pointer to 'struct MyData' : %d bytes\n\n", SGK_pointer_to_struct_MyData_size);

	if (SGK_pData)
	{
		free(SGK_pData);
		SGK_pData = NULL;
		printf("Memory Allocated To 'struct MyData' Has Been Successfully Freed !!!\n\n");
	}

	return(0);
}

