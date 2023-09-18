#include<stdio.h>

struct Employee
{
	char SGK_name[100];
	int SGK_age;
	float SGK_salary;
	char sex;
	char marital_status;
};

int main(void)
{
	//code
	printf("\n\n");
	printf("Sizes of data types and pointer to those respective data types are : \n\n");
	printf("Size of (int)				: %zd \t\t\t Size of pointer to int (int*)					: %zd\n\n", sizeof(int), sizeof(int*));
	printf("Size of (float)				: %zd \t\t\t Size of pointer to float (float*)				: %zd\n\n", sizeof(float), sizeof(float*));
	printf("Size of (double)			: %zd \t\t\t Size of pointer to double (double*)				: %zd\n\n", sizeof(double), sizeof(double*));
	printf("Size of (char)				: %zd \t\t\t Size of pointer to char(char*)					: %zd\n\n", sizeof(char), sizeof(char*));
	printf("Size of (struct Employee)		: %zd \t\t\t Size of pointer to struct Employee (struct Employee*)		: %zd\n\n", sizeof(struct Employee), sizeof(struct Employee*));

	return(0);
}
