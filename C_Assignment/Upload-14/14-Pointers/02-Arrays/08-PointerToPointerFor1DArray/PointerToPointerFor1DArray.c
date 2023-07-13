#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	void MyAlloc(int **SGK_ptr, unsigned int SGK_numberOfElements);

	//variable declarations
	int *SGK_piArray = NULL;
	unsigned int SGK_num_elements;
	int SGK_i;

	//code
	printf("\n\n");
	printf("How Many Elements You Want In Integer Array ? \n\n");
	scanf("%u", &SGK_num_elements);

	printf("\n\n");
	MyAlloc(&SGK_piArray, SGK_num_elements);

	printf("Enter %u Elements To Fill Up Your Integer Array :\n\n", SGK_num_elements);
	for (SGK_i = 0; SGK_i < SGK_num_elements; SGK_i++)
	{
		printf("Enter %d Element of array : ",SGK_i);
		scanf("%d", &SGK_piArray[SGK_i]);
	}

	printf("\n\n");
	printf("The %u Elements Entered By You In The Integer Array : \n\n", SGK_num_elements);
	for (SGK_i = 0; SGK_i < SGK_num_elements; SGK_i++)
		printf("%u\n", SGK_piArray[SGK_i]);

	printf("\n\n");
	if (SGK_piArray)
	{
		free(SGK_piArray);
		SGK_piArray = NULL;
		printf("Memory Allocated Has Now Been Successfully Freed !!!\n\n");
	}

	return(0);
}

void MyAlloc(int **SGK_ptr, unsigned int SGK_numberOfElements)
{
	//code
	*SGK_ptr = (int *)malloc(SGK_numberOfElements * sizeof(int));
	if (*SGK_ptr == NULL)
	{
		printf("Could Not Allocate Memory !!! Exitting Now ... \n\n");
		exit(0);
	}
	printf("MyAlloc() Has Successfully Allocated %zu Bytes For Integer Array !!!\n\n", (SGK_numberOfElements * sizeof(int)));
}
