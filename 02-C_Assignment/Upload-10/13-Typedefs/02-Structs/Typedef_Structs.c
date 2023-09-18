#include<stdio.h>

#define MAX_NAME_LENGTH 100

struct Employee
{
	char SGK_name[MAX_NAME_LENGTH];
	unsigned int SGK_age;
	char SGK_gender;
	double SGK_salary;
};

struct MyData
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_c;
};

int main(void)
{
	//Typedefs
	typedef struct Employee MY_EMPLOYEE_TYPE;
	typedef struct MyData MY_DATA_TYPE;

	//variable declarations
	struct Employee SGK_emp = { "Funny",25,'M',10000.00 };
	MY_EMPLOYEE_TYPE SGK_emp_typedef = { "Bunny", 23, 'F', 20400.00 };

	struct MyData SGK_md = { 30, 11.45f, 26.122017, 'X'};
	MY_DATA_TYPE SGK_md_typedef;

	//code 
	SGK_md_typedef.SGK_i = 9;
	SGK_md_typedef.SGK_f = 1.5f;
	SGK_md_typedef.SGK_d = 8.041997;
	SGK_md_typedef.SGK_c = 'P';

	printf("\n\n");
	printf("struct Employee : \n\n");
	printf("emp.name		= %s\n", SGK_emp.SGK_name);
	printf("emp.age			= %d\n", SGK_emp.SGK_age);
	printf("emp.gender		= %c\n", SGK_emp.SGK_gender);
	printf("emp.salary		= %lf\n", SGK_emp.SGK_salary);

	printf("\n\n");
	printf("MY_EMPLOYEE_TYPE : \n\n");
	printf("emp_typedef.name	= %s\n", SGK_emp_typedef.SGK_name);
	printf("emp_typedef.age		= %d\n", SGK_emp_typedef.SGK_age);
	printf("emp_typedef.gender  = %c\n", SGK_emp_typedef.SGK_gender);
	printf("emp_typedef.salary  = %lf\n", SGK_emp_typedef.SGK_salary);

	printf("\n\n");
	printf("struct MyData : \n\n");
	printf("md.i		= %d\n", SGK_md.SGK_i);
	printf("md.f		= %f\n", SGK_md.SGK_f);
	printf("md.d		= %lf\n", SGK_md.SGK_d);
	printf("md.c		= %c\n", SGK_md.SGK_c);

	printf("\n\n");
	printf("MY_DATA_TYPE : \n\n");
	printf("md_typedef.i		= %d\n", SGK_md_typedef.SGK_i);
	printf("md_typedef.f		= %f\n", SGK_md_typedef.SGK_f);
	printf("md_typedef.d		= %lf\n", SGK_md_typedef.SGK_d);
	printf("md_typedef.c		= %c\n", SGK_md_typedef.SGK_c);

	printf("\n\n");
	return(0);
}

