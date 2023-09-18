#include<stdio.h>
#include<stdlib.h>

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

	//variable delcarations
	struct Employee *SGK_pEmployeeRecord = NULL;
	int SGK_num_employees, SGK_i;

	//code
	printf("\n\n");
	printf("Enter Number Of Employees Whose Details You Want To Record : ");
	scanf("%d", &SGK_num_employees);

	printf("\n\n");
	SGK_pEmployeeRecord = (struct Employee *)malloc(sizeof(struct Employee) * SGK_num_employees);
	if (SGK_pEmployeeRecord == NULL)
	{
		printf("Failed To Allocated Memory For %d Employees !!! Exitting Now...\n\n", SGK_num_employees);
		exit(0);
	}
	else
	{
		printf("Successfully Allocated Memory For %d Employees !!!\n\n", SGK_num_employees);
	}

	// ***** User Input Initialization Of Array Of 'struct Employee' *****
	for (SGK_i = 0; SGK_i < SGK_num_employees; SGK_i++)
	{
		printf("\n\n\n\n");
		printf("***** Data Entry For Employee Number %d *****\n", (SGK_i + 1));
		printf("\n\n");
		printf("Enter Employee Name : ");
		MyGetString(SGK_pEmployeeRecord[SGK_i].SGK_name, NAME_LENGTH);

		printf("\n\n\n");
		printf("Enter Employee's Age (in Years) : ");
		scanf("%d", &SGK_pEmployeeRecord[SGK_i].SGK_age);
		
		printf("\n\n");
		printf("Enter Employee's Sex (M/m for Male, F/f For Female) : ");
		SGK_pEmployeeRecord[SGK_i].SGK_sex = getch();
		printf("%c", SGK_pEmployeeRecord[SGK_i].SGK_sex);
		SGK_pEmployeeRecord[SGK_i].SGK_sex = toupper(SGK_pEmployeeRecord[SGK_i].SGK_sex);

		printf("\n\n");
		printf("Enter Employee's Salary (in Indian Rupees) : ");
		scanf("%f", &SGK_pEmployeeRecord[SGK_i].SGK_salary);

		printf("\n\n");
		printf("Is The Employee Married ? (Y/y For Yes, N/n For No) : ");
		SGK_pEmployeeRecord[SGK_i].SGK_marital_status = getch();
		printf("%c", SGK_pEmployeeRecord[SGK_i].SGK_marital_status);
		SGK_pEmployeeRecord[SGK_i].SGK_marital_status = toupper(SGK_pEmployeeRecord[SGK_i].SGK_marital_status);
	}

	// *** Display *** 
	printf("\n\n\n\n");
	printf("***** Displaying Employee Records *****\n\n");
	for (SGK_i = 0; SGK_i < SGK_num_employees; SGK_i++)
	{
		printf(" ***** Employee Number %d *****\n\n", (SGK_i + 1));
		printf(" Name			: %s\n", SGK_pEmployeeRecord[SGK_i].SGK_name);
		printf(" Age			: %d years\n", SGK_pEmployeeRecord[SGK_i].SGK_age);

		if (SGK_pEmployeeRecord[SGK_i].SGK_sex == 'M')
			printf(" Sex			: Male\n");
		else if (SGK_pEmployeeRecord[SGK_i].SGK_sex == 'F')
			printf(" Sex			: Female\n");
		else
			printf(" Sex			: Invalid Data Entered\n");

		printf("Salary			: Rs. %f\n", SGK_pEmployeeRecord[SGK_i].SGK_salary);

		if (SGK_pEmployeeRecord[SGK_i].SGK_marital_status == 'Y')
			printf("Marital Status : Married \n");
		else if (SGK_pEmployeeRecord[SGK_i].SGK_marital_status == 'N')
			printf("Marital Status : Unmarried \n");
		else
			printf("Marital Status : Invalid Data Entered\n");

		printf("\n\n");
	}

	if (SGK_pEmployeeRecord)
	{
		free(SGK_pEmployeeRecord);
		SGK_pEmployeeRecord = NULL;
		printf("Memory Allocated To %d Employees Has Been Successfully Freed !!!\n\n", SGK_num_employees);
	}

	return(0);
}

// *** Simple Rudimentary Implementation Of gets_s() ***
// *** Implemented Due To Different Behaviour Of gets_s() / fgets()/ fscanf() On Different Platform ***
// *** Backspace / Character Deletion And Arrow Key Movemement Not Implemented ***

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
	} while ((SGK_ch != '\r') && (SGK_i < SGK_str_size));

	if (SGK_i == SGK_str_size)
		SGK_str[SGK_i - 1] = '\0';
	else
		SGK_str[SGK_i] = '\0';
}
