#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_num;
	int *SGK_ptr = NULL;
	int SGK_ans;

	//code
	SGK_num = 5;
	SGK_ptr = &SGK_num;

	printf("\n\n");
	printf("num		= %d\n", SGK_num);
	printf("&num		= %p\n", &SGK_num);
	printf("*(&num)		= %d\n", *(&SGK_num));
	printf("ptr		= %p\n", SGK_ptr);
	printf("*ptr		= %d\n", *SGK_ptr);

	printf("\n\n");

	//Add 10 to 'ptr' which is the address of 'num' ...
	//Hence, 10 will be added to the address of 'num' and the resultant address will be displayed 
	printf("Answer Of (ptr + 10)	= %p\n", (SGK_ptr + 10));

	//Add 10 to 'ptr' which is the address of 'num' and give value at the new address...
	//Hence, 10 will be added to the address of 'num' and and the value at resultant address will displayed ...
	printf("Answer of *(ptr + 10)	= %d\n", *(SGK_ptr + 10));

	//Add 10 to '*ptr' which is the value at address of 'num' (i.e : 'num' i.e :5) and give new value. without any change in any address ...
	//Hence, 10 will be added to the '*ptr' (num = 5) and the resultant value will be given (*ptr + 10) = (num + 10) = (5 + 10) = 15...
	printf("Answer of (*ptr + 10)	= %d\n\n", (*SGK_ptr + 10));

	// *** Associativity of * (value At Address) And ++ And -- Operators is from right to left ***
	//  (Right To Left) Consider value *ptr ... Pre-increment *ptr ... That is, Value at address 'ptr' i.e: *ptr is pre-increment (++*ptr)
	++*SGK_ptr; // *ptr is pre-incremented ... *ptr is 5 ...after execution of this statement... *ptr = 6
	printf("Answer Of ++*ptr : %d\n", *SGK_ptr); //Brackets not necessary for pre-increment / pre-decrement
	
	// (Right To Left) Post-increment address ptr ... That is , address 'ptr' i.e: ptr is post-incremented (ptr++) and then the value at the new address is displayed (*ptr++)...
	*SGK_ptr++;//Incorrect method of post-incrementing a value using pointer ...
	printf("Answer Of *ptr++ : %d\n", *SGK_ptr);//Brackets are necessary for post-increment/ post-decrement

	//(Right To Left) Post-Increment address ptr ... That is , address 'ptr' i.e: ptr is post-incremented(*ptr)++
	SGK_ptr = &SGK_num;
	(*SGK_ptr)++;//correct method of post-incrementing a value using pointer... *ptr is 6 .. at this statement *ptr remains 6 but at next statement *ptr7(post-increment)
	printf("Answer of (*ptr)++ : %d\n\n", *SGK_ptr);// Brackets are necessary for post-increment /post-decrement

	return(0);
}
