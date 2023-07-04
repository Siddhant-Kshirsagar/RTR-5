#include<stdio.h>
#include<stdlib.h>

#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)
#define DOUBLE_SIZE sizeof(double)
#define CHAR_SIZE sizeof(char)

int main(void)
{
	//variable declarations
	int *SGK_ptr_iArray = NULL;
	unsigned int SGK_intArrayLength = 0;
	
	float *SGK_ptr_fArray = NULL;
	unsigned int SGK_floatArrayLength = 0;

	double *SGK_ptr_dArray = NULL;
	unsigned int SGK_doubleArrayLength = 0;

	char *SGK_ptr_cArray = NULL;
	unsigned int SGK_charArrayLength = 0;

	int SGK_i;

	//code

	// ***** INTEGER ARRAY *****
	printf("\n\n");
	printf("Enter The Number Of ELements You Want In The Integer Array : ");
	scanf("%u", &SGK_intArrayLength);

	SGK_ptr_iArray = (int *)malloc(INT_SIZE * SGK_intArrayLength);
	if (SGK_ptr_iArray == NULL)
	{
		printf("\n\n");
		printf("Memory Allocation For Integer Array Failed !!! Exitting Now ...\n\n");
		exit(0);
	}
	else
	{
		printf("\n\n");
		printf("Memory Allocation For Integer Array Succeeded !!!\n\n");
	}

	printf("\n\n");
	printf("Enter The %d Integer Elements To Fill Up The Integer Array : \n\n", SGK_intArrayLength);
	for (SGK_i = 0; SGK_i < SGK_intArrayLength; SGK_i++)
	{
		printf("Enter %d Element of Array  : ", SGK_i);
		scanf("%d", (SGK_ptr_iArray + SGK_i));
	}
	// ***** FLOAT ARRAY *****
	printf("\n\n");
	printf("Enter The Number Of Elements you want in the 'float' Array : ");
	scanf("%u", &SGK_floatArrayLength);

	SGK_ptr_fArray = (float *)malloc(FLOAT_SIZE * SGK_floatArrayLength);
	if (SGK_ptr_fArray == NULL)
	{
		printf("\n\n");
		printf("Memory Allocation For Floating-Point Array Failed !!! Exitting Now...\n\n");
		exit(0);
	}
	else
	{
		printf("\n\n");
		printf("Memory Allocation For Floating-Point Array Succeeded !!!\n\n");
	}

	printf("\n\n");
	printf("Enter The %d Floating-Point Elements To Fill Up The 'float' Array : \n\n", SGK_floatArrayLength);
	for (SGK_i = 0; SGK_i < SGK_floatArrayLength; SGK_i++)
	{
		printf("Enter %d Element of Array : ", SGK_i);
		scanf("%f", (SGK_ptr_fArray + SGK_i));
	}

	// ***** DOUBLE ARRAY *****
	printf("\n\n");
	printf("Enter The Number Of Elements You Want In The 'double' Array : ");
	scanf("%u", &SGK_doubleArrayLength);

	SGK_ptr_dArray = (double *)malloc(DOUBLE_SIZE * SGK_doubleArrayLength);
	if (SGK_ptr_dArray == NULL)
	{
		printf("\n\n");
		printf("Memory Allocation For 'DOUBLE' Array Failed !!! Exitting Now...\n\n");
		exit(0);
	}
	else
	{
		printf("\n\n");
		printf("Memory Allocation For 'DOUBLE' Array SUCCEEDED !!!\n\n");
	}

	printf("\n\n");
	printf("Enter The %d Double Elements To Fill Up The 'double' Array : \n\n", SGK_doubleArrayLength);
	for (SGK_i = 0; SGK_i < SGK_doubleArrayLength; SGK_i++)
	{
		printf("Enter %d Element Of Array : ", SGK_i);
		scanf("%lf", (SGK_ptr_dArray + SGK_i));
	}

	// ***** CHAR ARRAY *****
	printf("\n\n");
	printf("Enter The Number Of Elements You Want In The Character Array : ");
	scanf("%u", &SGK_charArrayLength);

	SGK_ptr_cArray = (char *)malloc(CHAR_SIZE * SGK_charArrayLength);
	if (SGK_ptr_cArray == NULL)
	{
		printf("\n\n");
		printf("Memory Allocation For Character Array Failed !!! Exitting Now...\n\n");
		exit(0);
	}
	else
	{
		printf("\n\n");
		printf("Memory Allocation For Character Array Succeeded !!!\n\n");
	}

	printf("\n\n");
	printf("Enter The %d Character Element To Fill Up The Character Array : \n\n", SGK_charArrayLength);
	for (SGK_i = 0; SGK_i < SGK_charArrayLength; SGK_i++)
	{
		printf("Enter %d Element Of Array : ", SGK_i);
		*(SGK_ptr_cArray + SGK_i) = getch();
		printf("%c\n", *(SGK_ptr_cArray + SGK_i));
	}

	// ***** Display Of Arrays *****

	// ***** INTEGER ARRAY *****
	printf("\n\n");
	printf("The Integer Array Entered By You And  Consisting of %d Elements Is As Follows : \n\n", SGK_intArrayLength);
	for(SGK_i = 0; SGK_i < SGK_intArrayLength; SGK_i++)
		printf("%d \t \t At Address : %p\n", *(SGK_ptr_iArray + SGK_i), (SGK_ptr_iArray + SGK_i));

	// ***** Float Array *****
	printf("\n\n");
	printf("The Float Array Entered By You And Consisting Of %d Elements Is As Follows : \n\n", SGK_floatArrayLength);
	for (SGK_i = 0; SGK_i < SGK_floatArrayLength; SGK_i++)
		printf("%f \t \t At Address : %p\n", *(SGK_ptr_fArray + SGK_i), (SGK_ptr_fArray + SGK_i));

	// ***** Double Array *****
	printf("\n\n");
	printf("The Double Array Entered By You And Consisting Of %d Elements Is As Follows : \n\n", SGK_doubleArrayLength);
	for (SGK_i = 0; SGK_i < SGK_doubleArrayLength; SGK_i++)
		printf("%lf \t \t At Address : %p\n", *(SGK_ptr_dArray + SGK_i), (SGK_ptr_dArray + SGK_i));

	// ***** Character Array *****
	printf("\n\n");
	printf("The Character Array Entered By You And Consisting of %d Elements Is As Follows : \n\n", SGK_charArrayLength);
	for (SGK_i = 0; SGK_i < SGK_charArrayLength; SGK_i++)
		printf("%c \t \t At Address : %p\n", *(SGK_ptr_cArray + SGK_i), (SGK_ptr_cArray + SGK_i));

	// ***** Freeing Memory Occupied By Arrays (In Reverse Order Of Allocation) *****
	if (SGK_ptr_cArray)
	{
		free(SGK_ptr_cArray);
		SGK_ptr_cArray = NULL;
		
		printf("\n\n");
		printf("Memory Occupied By Character Array Has Been Successfully Freed !!!\n\n");
	}

	if (SGK_ptr_dArray)
	{
		free(SGK_ptr_dArray);
		SGK_ptr_dArray = NULL;

		printf("\n\n");
		printf("Memory Occupied By 'DOUBLE' Array Has Been Successfully Freed !!!\n\n");
	}

	if (SGK_ptr_fArray)
	{
		free(SGK_ptr_fArray);
		SGK_ptr_fArray = NULL;

		printf("\n\n");
		printf("Memory Occupied By Floating-Point Array Has Been Successfully Freed !!!\n\n");
	}

	if (SGK_ptr_iArray)
	{
		free(SGK_ptr_iArray);
		SGK_ptr_iArray = NULL;

		printf("\n\n");
		printf("Memory Occupied By Integer Array Has Been Successfully Freed !!!\n\n");
	}

	return(0);
}
