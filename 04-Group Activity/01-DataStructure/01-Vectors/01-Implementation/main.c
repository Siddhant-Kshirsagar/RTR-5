#include<stdio.h>
#include"vector.h"

/*Entry Point Function*/
int main(void)
{
    /*Variable declarations*/
    VECTOR_T *my_vec;

    /*code*/
    my_vec = create_vector();

    //case - 1 (filling values manually)
    push_back(my_vec,1.0f);
    push_back(my_vec,2.0f);
    push_back(my_vec,3.0f);

    show_vector(my_vec,"After Filling Values Manually\n");

    // pop array value (delete last element of array)
    pop_back(my_vec);
    pop_back(my_vec);

    // case - 2 (filling values using for loop)
    for(float i = 100.0f; i<110.0f; i = i+1.0f)
    {
        push_back(my_vec,i);
    }

    //display array element 
    show_vector(my_vec,"After Filling Values Using Loop\n");

    // destory vector 
    destory_vector(my_vec);

    return(0);
}
