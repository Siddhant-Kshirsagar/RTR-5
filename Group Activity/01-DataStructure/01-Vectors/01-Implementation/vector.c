#include"vector.h"

/* DYNAMICALLY ALLOCATE MEMORY FOR VECTOR_T AND RETURNS IT'S POINTER*/
VECTOR_T *create_vector(void)
{
    /*VARIABLE DECLARATION*/
    VECTOR_T *p_vec = NULL;

    /*CODE*/
    /* ALLOCATE MEMORY */
    p_vec = (VECTOR_T*)malloc(sizeof(VECTOR_T));

    /*ERROR CHECK*/
    if(p_vec == NULL)
    {
        printf("Memory Allocation Failed\n");
        return(NULL);
    }

    /*INITIALIZATION*/
    p_vec-> p_arr = NULL;
    p_vec-> size = 0;

    return(p_vec);
}

/*INCREASE SIZE OF ARRAY AND INSERT VALUE AT LAST INDEX*/
RET push_back(VECTOR_T *p_vec,float value)
{
    /*CODE*/
    p_vec->size = p_vec->size+1;
    p_vec->p_arr = (float*)realloc(p_vec->p_arr,p_vec->size * sizeof(float));

    /*Error Check*/
    if(p_vec->p_arr == NULL)
    {
        printf("Memory Allocation Failed\n");
        return(FAILED);
    }

    p_vec->p_arr[p_vec->size - 1] = value;

    return(SUCCESS);
}

/*DECREASE SIZE OF ARRAY (REMOVE LAST ELEMENT OF ARRAY)*/
RET pop_back(VECTOR_T *p_vec)
{
    /*CODE*/
    if(p_vec->size == 0)
    {
        printf("Array is Empty\n");
        return(FAILED);
    }

    p_vec->size = p_vec->size - 1;
    p_vec->p_arr = (float*) realloc(p_vec->p_arr,p_vec->size * sizeof(float));

    if(p_vec->p_arr == NULL)
    {
        printf("Memory Allocation Failed\n");
        return(FAILED);
    }

    return(SUCCESS);
}

/*DEALLOCATE MEMORY FROM HEAP(DESTROY VECTOR)*/
RET destory_vector(VECTOR_T *p_vec)
{
    /*CODE*/
    if(!p_vec)
    {
        return (FAILED);
    }

    if(p_vec->size == 0)
    {
        free(p_vec);
        p_vec = NULL;
        return(SUCCESS);
    }

    if(p_vec->p_arr)
    {
        free(p_vec->p_arr);
        p_vec->p_arr = NULL;
    }

    free(p_vec);
    p_vec = NULL;

    return(SUCCESS);
}

/*DISPLAY MEMBERS OF VECTOR*/
void show_vector(VECTOR_T *p_vec, const char *msg)
{
    /*CODE */

    /*Empty Check*/
    if(p_vec->size ==0)
    {
        printf("Array is Empty\n");
        return;
    }

    printf(msg);

    /*print all members of vector*/
    for(int i = 0; i< p_vec->size; i++)
    {
        printf("Index = %d : Value  = [%f]\n",i,p_vec->p_arr[i]);
    }
}
