#include<stdio.h>

#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRINGS 10
#define MAX_CHARACTERS_PER_STRING 20

#define ALPHABET_BEGINNING 65 // 'A'

struct MyDataOne
{
	int SGK_iArray[INT_ARRAY_SIZE];
	float SGK_fArray[FLOAT_ARRAY_SIZE];
};

struct MyDataTwo
{
	char SGK_cArray[CHAR_ARRAY_SIZE];
	char SGK_strArray[NUM_STRINGS][MAX_CHARACTERS_PER_STRING];
};

int main(void)
{
	//variable declarations
	struct MyDataOne SGK_data_one;
	struct MyDataTwo SGK_data_two;
	int SGK_i;

	//code
	// *** PIECE-MEAL ASSIGNMENT (HARD-CODED) ***
	SGK_data_one.SGK_fArray[0] = 0.1f;
	SGK_data_one.SGK_fArray[1] = 1.2f;
	SGK_data_one.SGK_fArray[2] = 2.3f;
	SGK_data_one.SGK_fArray[3] = 3.4f;
	SGK_data_one.SGK_fArray[4] = 4.5f;

	//*** LOOP Assignment (User Input) ***
	printf("\n\n");
	printf("Enter %d Integers : \n\n", INT_ARRAY_SIZE);
	for (SGK_i = 0; SGK_i < INT_ARRAY_SIZE; SGK_i++)
		scanf("%d", &SGK_data_one.SGK_iArray[SGK_i]);

	// *** LOOP ASSIGNMENT (HARD-CODED) ***
	for (SGK_i = 0; SGK_i < CHAR_ARRAY_SIZE; SGK_i++)
		SGK_data_two.SGK_cArray[SGK_i] = (char)(SGK_i + ALPHABET_BEGINNING);

	// *** PIECE-MEAL Assignment (HARD-CODED) ***
	strcpy(SGK_data_two.SGK_strArray[0], "Welcome !!!");
	strcpy(SGK_data_two.SGK_strArray[1], "This");
	strcpy(SGK_data_two.SGK_strArray[2], "Is");
	strcpy(SGK_data_two.SGK_strArray[3], "ASTROMEDICOMP'S");
	strcpy(SGK_data_two.SGK_strArray[4], "Real");
	strcpy(SGK_data_two.SGK_strArray[5], "Time");
	strcpy(SGK_data_two.SGK_strArray[6], "Rendering");
	strcpy(SGK_data_two.SGK_strArray[7], "Batch");
	strcpy(SGK_data_two.SGK_strArray[8], "Of");
	strcpy(SGK_data_two.SGK_strArray[9], "2023-2024 !!!");

	// *** Displaying Data Members Of 'struct DataOne' AND THEIR Values ***
	printf("\n\n");
	printf("Members Of 'struct DataOne' Along with Their Assigned Values Are : \n\n");

	printf("\n\n");
	printf("Integer Array (data_one.iArray[]) : \n\n");
	for (SGK_i = 0; SGK_i < INT_ARRAY_SIZE; SGK_i++)
		printf("data_one.iArray[%d] = %d\n", SGK_i, SGK_data_one.SGK_iArray[SGK_i]);

	printf("\n\n");
	printf("Floating-Point Array (data_one.fArray[]) : \n\n");
	for (SGK_i = 0; SGK_i < FLOAT_ARRAY_SIZE; SGK_i++)
		printf("data_one.fArray[%d] = %f \n", SGK_i, SGK_data_one.SGK_fArray[SGK_i]);

	// *** Displaying Data Members Of 'struct DataTwo' AND THEIR Values ***
	printf("\n\n");
	printf("Members Of 'struct DataTwo' Along With Their Assigned Values Are : \n\n");

	printf("\n\n");
	printf("Character Array (data_two.cArray[]) : \n\n");
	for (SGK_i = 0; SGK_i < CHAR_ARRAY_SIZE; SGK_i++)
		printf("data_two.cArray[%d] = %c\n", SGK_i, SGK_data_two.SGK_cArray[SGK_i]);

	printf("\n\n");
	printf("String Array (data_two.strArray[] ) : \n\n");
	for (SGK_i = 0; SGK_i < NUM_STRINGS; SGK_i++)
		printf("%s ", SGK_data_two.SGK_strArray[SGK_i]);

	printf("\n\n");

	return(0);
}
