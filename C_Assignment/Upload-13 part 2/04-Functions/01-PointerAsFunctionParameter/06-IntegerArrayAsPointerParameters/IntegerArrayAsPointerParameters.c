#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	//function declarations
	void MultiplyArrayElementsByNumber(int *, int, int);

	//variable declaration
	int *SGK_iArray = NULL;
	int SGK_num_elements;
	int SGK_i, SGK_num;

	//code
	printf("\n\n");
	printf("Enter How Many Elements You Want In The Integer Array : ");
	scanf("%d", &SGK_num_elements);

	SGK_iArray = (int *)malloc(SGK_num_elements * sizeof(int));
	if (SGK_iArray == NULL)
	{
		printf("Memory Allocation To 'iArray' Has Failed !!! Exitting Now..\n\n");
		exit(0);
	}

	printf("\n\n");
	printf("Enter %d Elements For The Integer Array : \n\n", SGK_num_elements);
	for (SGK_i = 0; SGK_i < SGK_num_elements; SGK_i++)
	{
		printf("Enter %d Element Of Array : ", SGK_i);
		scanf("%d", &SGK_iArray[SGK_i]);
	}

	//  ***** ONE *****
	printf("\n\n");
	printf("Array Before Passing To Function MultiplyArrayElementsByNumber() : \n\n");
	for (SGK_i = 0; SGK_i < SGK_num_elements; SGK_i++)
		printf(" iArray[%d] = %d \n", SGK_i, SGK_iArray[SGK_i]);

	printf("\n\n");
	printf("Enter The Number By Which You Want To Multiple Each Array Element : ");
	scanf("%d", &SGK_num);

	MultiplyArrayElementsByNumber(SGK_iArray, SGK_num_elements, SGK_num);

	printf("\n\n");
	printf("Array Returned By Function MultiplyArrayElementsByNumber() : \n\n");
	for (SGK_i = 0; SGK_i < SGK_num_elements; SGK_i++)
		printf("iArray[%d] = %d\n", SGK_i, SGK_iArray[SGK_i]);

	if (SGK_iArray)
	{
		free(SGK_iArray);
		SGK_iArray = NULL;
		printf("\n\n");
		printf("Memory Allocated To 'iArray' Has Been Successfully Freed !!!\n\n");
	}

	return(0);
}

void MultiplyArrayElementsByNumber(int *SGK_arr, int SGK_iNumElements, int SGK_n)
{
	//variable declarations
	int SGK_i;

	//code
	for (SGK_i = 0; SGK_i < SGK_iNumElements; SGK_i++)
		SGK_arr[SGK_i] = SGK_arr[SGK_i] * SGK_n;
}
