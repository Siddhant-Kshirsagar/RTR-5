#include<stdio.h>

int main(void)
{
	//variable declarations
	char SGK_chArray_01[] = { 'A','S','T','R','O','M','E','D','I','C','O','M','P','\0' };//Must Give \0 Explicitly For Proper Initialization
	char SGK_chArray_02[9] = { 'W','E','L','C','O','M','E','S','\0' };//Must give \0 Explicitly For Proper Initialization
	char SGK_chArray_03[] = { 'Y','O','U','\0' }; //Must give \0 Explicitly for proper initialization
	char SGK_chArray_04[] = "TO";// \0 is assumed, size is given as 3, although string has only 2 characters 
	char SGK_chArray_05[] = "REAL TIME RENDERING BATCH OF 2023-24"; // \0 IS assumed, size is given as 37, although string has 36 characters

	char SGK_chArray_WithoutNullTerminator[] = { 'H','e','l','l','o' };

	//code 
	printf("\n\n");
	
	printf("Size of chArray_01 : %zu\n\n", sizeof(SGK_chArray_01));
	printf("Size of chArray_02 : %zu\n\n", sizeof(SGK_chArray_02));
	printf("Size of chArray_03 : %zu\n\n", sizeof(SGK_chArray_03));
	printf("Size of chArray_04 : %zu\n\n", sizeof(SGK_chArray_04));
	printf("Size of chArray_05 : %zu\n\n", sizeof(SGK_chArray_05));

	printf("\n\n");

	printf("The String Are \n\n");
	printf("chArray_01 : %s\n\n", SGK_chArray_01);
	printf("chArray_02 : %s\n\n", SGK_chArray_02);
	printf("chArray_03 : %s\n\n", SGK_chArray_03);
	printf("chArray_04 : %s\n\n", SGK_chArray_04);
	printf("chArray_05 : %s\n\n", SGK_chArray_05);

	printf("\n\n");
	printf("Size Of chArray_WithoutNullTerminator : %zu \n\n", sizeof(SGK_chArray_WithoutNullTerminator));
	printf("chArray_WithoutNullTerminator : %s \n\n", SGK_chArray_WithoutNullTerminator); // Will display garbage value at the end of string due to absence of \0

	return(0);
}	
