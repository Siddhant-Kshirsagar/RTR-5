#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_iArray[] = { 10,20,30,40,50,60,70,80,90,100 }; //Integer Array
	int *SGK_ptr_iArray = NULL;// Integer Pointer 

	// code

	// ****** Using Array Name As A Array i.e: value of xth element of iArray : iArray[x] and address of xth element of iArray : &iArray[x] *****
	printf("\n\n");
	printf(" *** Using Array Name As A Array i.e : Value of xth elemenet of iArray of iArray : iArray[x] and Address of xth element of iArray : &iArray[x] ***\n\n");
	printf("Integer Array Elements And Their Addresses : \n\n");
	printf("iArray[0]	= %d \t \t At Address &iArray[0] : %p\n", SGK_iArray[0], &SGK_iArray[0]);
	printf("iArray[1]	= %d \t \t At Address &iArray[1] : %p\n", SGK_iArray[1], &SGK_iArray[1]);
	printf("iArray[2]	= %d \t \t At Address &iArray[2] : %p\n", SGK_iArray[2], &SGK_iArray[2]);
	printf("iArray[3]	= %d \t \t At Address &iArray[3] : %p\n", SGK_iArray[3], &SGK_iArray[3]);
	printf("iArray[4]	= %d \t \t At Address &iArray[4] : %p\n", SGK_iArray[4], &SGK_iArray[4]);
	printf("iArray[5]	= %d \t \t At Address &iArray[5] : %p\n", SGK_iArray[5], &SGK_iArray[5]);
	printf("iArray[6]	= %d \t \t At Address &iArray[6] : %p\n", SGK_iArray[6], &SGK_iArray[6]);
	printf("iArray[7]	= %d \t \t At Address &iArray[7] : %p\n", SGK_iArray[7], &SGK_iArray[7]);
	printf("iArray[8]	= %d \t \t At Address &iArray[8] : %p\n", SGK_iArray[8], &SGK_iArray[8]);
	printf("iArray[9]	= %d \t At Address &iArray[9] : %p\n", SGK_iArray[9], &SGK_iArray[9]);

	// Assigning Base Address of Integer Array 'iArray' To Integer Pointer 'ptr_iArray'
	// Name Of Any Array Is Its Own Base Address
	SGK_ptr_iArray = SGK_iArray; //Same As... ptr_iArray = & iArray[0]

		// ***** Using Pointer As Pointer i.e : Value Of xth Element of iArray : *(ptr_iArray + x) And Address of xth Element of iArray : (ptr_iArray + x) *****
	printf("\n\n");
	printf(" *** Using Pointer As Pointer i.e : value of xth Element of iArray : *(ptr_iArray + x) And Address of xth Element of iArray : (ptr_iArray + x) ***\n\n");
	printf("Integer Array Element And Their Addresses : \n\n");
	printf(" *(ptr_iArray + 0) = %d \t \t At Address (ptr_iArray + 0) : %p\n", *(SGK_ptr_iArray + 0), (SGK_ptr_iArray + 0));
	printf(" *(ptr_iArray + 1) = %d \t \t At Address (ptr_iArray + 1) : %p\n", *(SGK_ptr_iArray + 1), (SGK_ptr_iArray + 1));
	printf(" *(ptr_iArray + 2) = %d \t \t At Address (ptr_iArray + 2) : %p\n", *(SGK_ptr_iArray + 2), (SGK_ptr_iArray + 2));
	printf(" *(ptr_iArray + 3) = %d \t \t At Address (ptr_iArray + 3) : %p\n", *(SGK_ptr_iArray + 3), (SGK_ptr_iArray + 3));
	printf(" *(ptr_iArray + 4) = %d \t \t At Address (ptr_iArray + 4) : %p\n", *(SGK_ptr_iArray + 4), (SGK_ptr_iArray + 4));
	printf(" *(ptr_iArray + 5) = %d \t \t At Address (ptr_iArray + 5) : %p\n", *(SGK_ptr_iArray + 5), (SGK_ptr_iArray + 5));
	printf(" *(ptr_iArray + 6) = %d \t \t At Address (ptr_iArray + 6) : %p\n", *(SGK_ptr_iArray + 6), (SGK_ptr_iArray + 6));
	printf(" *(ptr_iArray + 7) = %d \t \t At Address (ptr_iArray + 7) : %p\n", *(SGK_ptr_iArray + 7), (SGK_ptr_iArray + 7));
	printf(" *(ptr_iArray + 8) = %d \t \t At Address (ptr_iArray + 8) : %p\n", *(SGK_ptr_iArray + 8), (SGK_ptr_iArray + 8));
	printf(" *(ptr_iArray + 9) = %d \t At Address (ptr_iArray + 9) : %p\n", *(SGK_ptr_iArray + 9), (SGK_ptr_iArray + 9));
	return(0);
}
