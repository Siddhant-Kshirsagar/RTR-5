#include<stdio.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
	char SGK_name[NAME_LENGTH];
	int SGK_age;
	float SGK_salary;
	char SGK_sex;
	char SGK_marital_status[MARITAL_STATUS];
};

int main(void)
{
	//variable declarations
	struct Employee SGK_EmployeeRecord[5];//An array of 5 structs - Each being type 'struct Employee'

	char SGK_employee_rajesh[] = "Rajesh";
	char SGK_employee_sameer[] = "Sameer";
	char SGK_employee_kalyani[] = "Kalyani";
	char SGK_employee_sonali[] = "Sonali";
	char SGK_employee_shantanu[] = "Shantanu";

	int SGK_i;

	//code 
	// ***** HARD-CODED INITIALIZATION OF ARRAY OF 'struct Employee' *****

	// ***** EMPLOYEE 1 *****
	strcpy(SGK_EmployeeRecord[0].SGK_name, SGK_employee_rajesh);
	SGK_EmployeeRecord[0].SGK_age = 30;
	SGK_EmployeeRecord[0].SGK_sex = 'M';
	SGK_EmployeeRecord[0].SGK_salary = 50000.0f;
	strcpy(SGK_EmployeeRecord[0].SGK_marital_status, "Unmarried");

	// ***** EMPLOYEE 2 *****
	strcpy(SGK_EmployeeRecord[1].SGK_name, SGK_employee_sameer);
	SGK_EmployeeRecord[1].SGK_age = 32;
	SGK_EmployeeRecord[1].SGK_sex = 'M';
	SGK_EmployeeRecord[1].SGK_salary = 60000.0f;
	strcpy(SGK_EmployeeRecord[1].SGK_marital_status, "Married");

	// ***** EMPLOYEE 3 *****
	strcpy(SGK_EmployeeRecord[2].SGK_name, SGK_employee_kalyani);
	SGK_EmployeeRecord[2].SGK_age = 29;
	SGK_EmployeeRecord[2].SGK_sex = 'F';
	SGK_EmployeeRecord[2].SGK_salary = 62000.0f;
	strcpy(SGK_EmployeeRecord[2].SGK_marital_status, "Unmarried");

	// ***** EMPLOYEE 4 *****
	strcpy(SGK_EmployeeRecord[3].SGK_name, SGK_employee_sonali);
	SGK_EmployeeRecord[3].SGK_age = 33;
	SGK_EmployeeRecord[3].SGK_sex = 'F';
	SGK_EmployeeRecord[3].SGK_salary = 50000.0f;
	strcpy(SGK_EmployeeRecord[3].SGK_marital_status, "Married");

	// ***** EMPLOYEE 5 *****
	strcpy(SGK_EmployeeRecord[4].SGK_name, SGK_employee_shantanu);
	SGK_EmployeeRecord[4].SGK_age = 35;
	SGK_EmployeeRecord[4].SGK_sex = 'M';
	SGK_EmployeeRecord[4].SGK_salary = 55000.0f;
	strcpy(SGK_EmployeeRecord[4].SGK_marital_status, "Married");

	// *** DISPLAY ***
	printf("\n\n");
	printf("***** DISPLAYING EMPLOYEE RECORDS *****\n\n");
	for (SGK_i = 0; SGK_i < 5; SGK_i++)
	{
		printf("***** EMPLOYEE NUMBER %d ***** \n\n",(SGK_i+1));
		printf("Name				: %s\n", SGK_EmployeeRecord[SGK_i].SGK_name);
		printf("Age				: %d\n", SGK_EmployeeRecord[SGK_i].SGK_age);
		if (SGK_EmployeeRecord[SGK_i].SGK_sex == 'M' || SGK_EmployeeRecord[SGK_i].SGK_sex == 'm')
			printf("Sex				: Male\n");
		else
			printf("Sex				: Female\n");
		printf("Salary				: Rs. %f\n", SGK_EmployeeRecord[SGK_i].SGK_salary);
		printf("Marital Status			: %s\n", SGK_EmployeeRecord[SGK_i].SGK_marital_status);
		printf("\n\n");
	}
	
	return(0);
}
