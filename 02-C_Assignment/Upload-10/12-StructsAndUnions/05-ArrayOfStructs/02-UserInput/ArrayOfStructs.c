#include<stdio.h>
#include<ctype.h>

#define NUM_EMPLOYEES 3 //Simply Change this constant value to have as many number of employee records as you please ...

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
	char SGK_name[NAME_LENGTH];
	int SGK_age;
	char SGK_sex;
	float SGK_salary;
	char SGK_marital_status;
};

int main(void)
{
	//function prototype
	void MyGetString(char[], int);

	//variable declarations
	struct Employee SGK_EmployeeRecord[NUM_EMPLOYEES]; //An Array of <NUM_EMPLOYEES> structs - Each being type 'struct Employee'
	int SGK_i;

	//code 
	// ***** USER INPUT INITIALIZATION OF ARRAY OF 'struct Employee' *****
	for (SGK_i = 0; SGK_i < NUM_EMPLOYEES; SGK_i++)
	{
		printf("\n\n\n\n");
		printf("***** DATA ENTRY FOR EMPLOYEE NUMBER %d *****\n",(SGK_i+1));
		printf("\n\n");
		printf("Enter Employee Name : ");
		MyGetString(SGK_EmployeeRecord[SGK_i].SGK_name, NAME_LENGTH);

		printf("\n\n\n");
		printf("Enter Employee's Age (in Years) : ");
		scanf("%d", &SGK_EmployeeRecord[SGK_i].SGK_age);

		printf("\n\n");
		printf("Enter Emplyee's Sex (M/m for Male, F/f for Female) :");
		SGK_EmployeeRecord[SGK_i].SGK_sex = getch();
		printf("%c", SGK_EmployeeRecord[SGK_i].SGK_sex);
		SGK_EmployeeRecord[SGK_i].SGK_sex = toupper(SGK_EmployeeRecord[SGK_i].SGK_sex);

		printf("\n\n\n");
		printf("Enter Employee's Salary (in Indian Rupees) : ");
		scanf("%f",&SGK_EmployeeRecord[SGK_i].SGK_salary);

		printf("\n\n");
		printf("Is The Employee Married? (Y/y For Yes, N/n For No) : ");
		SGK_EmployeeRecord[SGK_i].SGK_marital_status = getch();
		printf("%c", SGK_EmployeeRecord[SGK_i].SGK_marital_status);
		SGK_EmployeeRecord[SGK_i].SGK_marital_status = toupper(SGK_EmployeeRecord[SGK_i].SGK_marital_status);
	}

	// ***** DISPLAY *****
	printf("\n\n\n\n");
	printf("***** DISPLAY EMPLOYEE RECORDS *****\n\n");
	for (SGK_i = 0; SGK_i < NUM_EMPLOYEES; SGK_i++)
	{
		printf("***** EMPLOYEE NUMBER %d *****\n\n",(SGK_i + 1));
		printf("Name				: %s\n", SGK_EmployeeRecord[SGK_i].SGK_name);
		printf("Age				: %d years\n", SGK_EmployeeRecord[SGK_i].SGK_age);

		if (SGK_EmployeeRecord[SGK_i].SGK_sex == 'M')
			printf("Sex				: Male\n");
		else if (SGK_EmployeeRecord[SGK_i].SGK_sex == 'F')
			printf("Sex				: Female\n");
		else
			printf("Sex				: Invalid Data Entered\n");

		printf("Salaray				: Rs. %f\n", SGK_EmployeeRecord[SGK_i].SGK_salary);

		if (SGK_EmployeeRecord[SGK_i].SGK_marital_status == 'Y')
			printf("Marital Status	:Married\n");
		else if (SGK_EmployeeRecord[SGK_i].SGK_marital_status == 'N')
			printf("Marital Status	:Unmarried\n");
		else
			printf("Marital Status	: Invalid Data Entered \n");

		printf("\n\n");
	}

	return(0);
}

// *** Simple Rudimentary Implementation of gets_s() ***
// *** Implemented Due To Different Behaviour of gets_s() / fgets() / fscanf() On Different Platforms ***
// *** Backspace / Character Deletion And Arrow Key Cursor Movement Not Implemented ***

void MyGetString(char SGK_str[], int SGK_str_size)
{
	//variable declarations
	int SGK_i;
	char SGK_ch = '\0';

	//code
	SGK_i = 0;
	do
	{
		SGK_ch = getch();
		SGK_str[SGK_i] = SGK_ch;
		printf("%c", SGK_str[SGK_i]);
		SGK_i++;
	} while ((SGK_ch != '\r') && (SGK_i < SGK_str_size));

	if (SGK_i == SGK_str_size)
		SGK_str[SGK_i - 1] = '\0';
	else
		SGK_str[SGK_i] = '\0';
}
