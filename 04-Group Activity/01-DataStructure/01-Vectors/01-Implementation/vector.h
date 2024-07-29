#pragma once

/* HEADER FILES*/
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define SUCCESS 1
#define FAILED -1

typedef int RET;

struct VECTOR
{
    float *p_arr;
    int size;
};

typedef struct VECTOR VECTOR_T;

/* GLOBAL FUNCTION DECLARATION*/
VECTOR_T *create_vector(void);
RET push_back(VECTOR_T *,float);
RET pop_back(VECTOR_T *);
void show_vector(VECTOR_T *,const char *);
RET destory_vector(VECTOR_T *);

