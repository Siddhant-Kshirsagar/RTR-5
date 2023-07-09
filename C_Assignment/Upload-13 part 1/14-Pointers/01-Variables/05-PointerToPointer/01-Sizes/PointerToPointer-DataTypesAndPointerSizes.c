#include<stdio.h>

struct Employee
{
    char SGK_name[100];
    int SGK_age;
    float SGK_salary;
    char SGK_sex;
    char SGK_marital_status;
};

int main(void)
{
    //code
    printf("\n\n");
    printf("Sizes of Data Types And Pointers To Those Respective Data Types Are : \n\n");
    printf("Size of (int)           : %zd \t size of pointer to int(int*)        : %zd \t Size of pointer to pointer to int (int**)           : %zd\n\n",sizeof(int),sizeof(int*),sizeof(int**));
    printf("Size of (float)         : %zd \t size of pointer to float(float *)         : %zd\t Size of pointer to pointer to float(float**)   : %zd\n\n",sizeof(float),sizeof(float*),sizeof(float**));
    printf("Size of (double)        : %zd \t Size of pointer to double(double*)        : %zd \t Size of pointer to pointer to double (double**)       : %zd\n\n",sizeof(double),sizeof(double*),sizeof(double**));
    printf("Size of (char)          : %zd \t size of pointer to char(char*)      : %zd \t  Size of pointer to pointer to char (char**)         : %zd\n\n",sizeof(char),sizeof(char*),sizeof(char**));
    printf("Size of (struct Employee)       : %zd \t size of pointer to strut Employee (struct Employee*)      : %zd \t Size of pointer to pointer to struct Employee (struct Employee**)         : %zd \n\n ",sizeof(struct Employee),sizeof(struct Employee*),sizeof(struct Employee**));
    
    return(0);
}
