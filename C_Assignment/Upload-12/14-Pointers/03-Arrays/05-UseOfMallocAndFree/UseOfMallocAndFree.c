#include<stdio.h>
#include<stdlib.h> // contains prototypes of malloc() and free()

int main(void)
{
	//variable declarations
	int *SGK_ptr_iArray = NULL; // It is good discipline to initialize any pointer with null address to prevent any garbage value getting into it, that way it makes it easy to check for success or failure of memory allocation later on after malloc()...
	unsigned int SGK_intArrayLength = 0;
	int SGK_i;

	//code
	printf("\n\n");
	printf("Enter The Number Of Elements You Want In Your Integer Array : ");
	scanf("%d", &SGK_intArrayLength);

	// Allocating as much memory required to the integer array
	// Memory required for integer array = size in bytes of one integer * number of integers to be stored in array
	// To allocate said amout of memory, function malloc() will be used 
	// malloc() will allocate said amount of memory and will return the initial / starting / base address of the allocated memory, which must be captured in pointer variable 
	// Using this base address, the integer array can be accessed and used 

	SGK_ptr_iArray = (int *)malloc(sizeof(int) * SGK_intArrayLength);
	if (SGK_ptr_iArray == NULL)// If ptr_iArray is still null, even after call to malloc(), it means malloc() has failed to allocate memory and no address has been returned by malloc() in ptr_iArray ...
	{
		printf("\n\n");
		printf("Memory Allocation For Integer Array Has Failed !!! Exiting Now ... \n\n");
		exit(0);
	}
	else// If ptr_iArray is not NULL , it means it must contain a valid address which is returned by malloc(), hence, malloc() has succeeded in memory allocation...
	{
		printf("\n\n");
		printf("Memory Allocation for integer array has succeeded !!!\n\n");
		printf("Memory Addresses from %p to %p have been allocated to integer array !!!\n\n", SGK_ptr_iArray, (SGK_ptr_iArray + SGK_intArrayLength));
	}

	printf("\n\n");
	printf("Enter %d Elements For the integer Array : \n\n", SGK_intArrayLength);

	for (SGK_i = 0; SGK_i < SGK_intArrayLength; SGK_i++)
	{
		printf("Enter '%d' Element of Array  : ", SGK_i);
		scanf("%d", (SGK_ptr_iArray + SGK_i));
	}

	printf("\n\n");
	printf("The Integer Array Entered By You, Consisting Of %d Elements : \n\n", SGK_intArrayLength);
	for (SGK_i = 0; SGK_i < SGK_intArrayLength; SGK_i++)
	{
		printf("ptr_iArray[%d] = %d \t \t At Address &ptr_iArray[%d] : %p \n", SGK_i, SGK_ptr_iArray[SGK_i], SGK_i, &SGK_ptr_iArray[SGK_i]);
	}

	printf("\n\n");
	for (SGK_i = 0; SGK_i < SGK_intArrayLength; SGK_i++)
	{
		printf("*(ptr_iArray + %d ) = %d \t \t At Address (ptr_iArray + %d) : %p\n", SGK_i, *(SGK_ptr_iArray + SGK_i), SGK_i, (SGK_ptr_iArray + SGK_i));
	}

	// Checking if memory is still allocated by checking validity of base address 'ptr_iArray' 
	// If Address is valid, that is if 'ptr_iArray' Exists , that is, if it is not null, memory is still allocated 
	// In That Case, The Allocated Memory Must Be freed
	// Memory is allocated using malloc() and freed using free()
	// Once Memory is freed using free(), the Base Address must be cleaned, That is, it must be re - initialized to 'NULL' to keep away garbage values. this is not compulsory, but it is good coding discipline

	if (SGK_ptr_iArray)
	{
		free(SGK_ptr_iArray);
		SGK_ptr_iArray = NULL;

		printf("\n\n");
		printf("Memory Allocated for integer array has been successfully freed !!!\n\n");
	}

	return(0);
}
