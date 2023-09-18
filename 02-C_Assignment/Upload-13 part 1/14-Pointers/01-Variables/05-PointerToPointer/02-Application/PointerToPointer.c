#include<stdio.h>

int main(void)
{
    //variable declarations
    int SGK_num;
    int *SGK_ptr = NULL;
    int **SGK_pptr = NULL;// Declaration Method 1 :- **pptr is a variable of type 'int'

    //code
    SGK_num = 10;

    printf("\n\n");
    printf(" ***** BEFORE ptr = &num ***** \n\n");
    printf("Value of 'num'      = %d\n\n",SGK_num);
    printf("Address of 'num'        = %p\n\n",&SGK_num);
    printf("Value at Address of 'num'   = %d\n\n",*(&SGK_num));

    //Assigning address of variable 'num' to pointer variable 'ptr'
    //'ptr' now contains address of 'num' ...hence, 'ptr' is Same as '&num'
    SGK_ptr = &SGK_num;

    printf("\n\n");

    printf(" ***** AFTER ptr = &num *****\n\n");
    printf("Value Of 'num'          = %d\n\n", SGK_num);
    printf("Address of 'num'            = %p \n\n", SGK_ptr);
    printf("Value At Address of 'num'       = %d\n\n",*SGK_ptr);

    // Assigning address of variable 'ptr' to pointer-to-pointer variable 'pptr'
    // 'pptr' now contains the address of 'ptr' which in turn contains the address of 'num'
    // Hence, 'pptr' is SAME as '&ptr'
    // 'ptr' is SAME as '&num'
    // Hence, pptr = &ptr = &(&num)
    // If ptr = &num and *ptr = *(&num) = value at address of 'num'
    // Then, pptr = &ptr and *pptr = *(&ptr) = value at address of 'ptr' i.e: 'ptr' : address of 'num'
    // Then, **pptr = **(&ptr) = *(*(&ptr)) = *ptr = *(&num) = num = 10
    // Hence, num = *(&num) = *ptr = *(*pptr) = **pptr

    SGK_pptr = &SGK_ptr;

    printf("\n\n");
    
    printf(" ***** AFTER pptr = &ptr *****\n\n");
    printf("Value of 'num'                  = %d\n\n", SGK_num);
    printf("Address of 'num' (ptr)          = %p\n\n", SGK_ptr);
    printf("Address of 'ptr' (pptr)         = %p\n\n",SGK_pptr);
    printf("Value At Address of 'ptr' (*pptr)       = %p\n\n", *SGK_pptr);
    printf("Value At Address of 'num' (*ptr) (*pptr)    = %d\n\n", **SGK_pptr);

    return(0);
}
