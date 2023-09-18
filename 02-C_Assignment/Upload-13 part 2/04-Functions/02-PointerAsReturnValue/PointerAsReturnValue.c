#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function declarations
	char *ReplaceVowelsWithHashSymbol(char *);

	//variable declaration
	char SGK_string[MAX_STRING_LENGTH];
	char *SGK_replaced_string = NULL;

	//code
	printf("\n\n");
	printf("Enter String : ");
	gets_s(SGK_string, MAX_STRING_LENGTH);

	SGK_replaced_string = ReplaceVowelsWithHashSymbol(SGK_string);
	if (SGK_replaced_string == NULL)
	{
		printf("ReplaceVowelsWithHashSymbol() Function Has Failed !!! Exitting Now...\n\n");
		exit(0);
	}

	printf("\n\n");
	printf("Replaced String Is : \n\n");
	printf("%s\n\n", SGK_replaced_string);

	if (SGK_replaced_string)
	{
		free(SGK_replaced_string);
		SGK_replaced_string = NULL;
	}

	return(0);
}

char* ReplaceVowelsWithHashSymbol(char *SGK_s)
{
	//function prototype
	void MyStrcpy(char *, char *);
	int MyStrlen(char *);

	//variable declarations
	char *SGK_new_string = NULL;
	int SGK_i;

	//code
	SGK_new_string = (char*)malloc(MyStrlen(SGK_s)*sizeof(char));
	if(SGK_new_string == NULL)
	{
		printf("Could Not Allocate Memory For New String !!!\n\n");
		return(NULL);
	}

	MyStrcpy(SGK_new_string,SGK_s);
	for(SGK_i = 0; SGK_i < MyStrlen(SGK_new_string);SGK_i++)
	{
		switch(SGK_new_string[SGK_i])
		{
			case 'A':
			case 'a':
			case 'E':
			case 'e':
			case 'I':
			case 'i':
			case 'O':
			case 'o':
			case 'U':
			case 'u':
				SGK_new_string[SGK_i] = '#';
				break;
			default:
				break;
		}
	}

	return(SGK_new_string);
}

void MyStrcpy(char *SGK_str_destination, char *SGK_str_source)
{
	//function prototype
	int MyStrlen(char *);

	//variable declaration
	int SGK_iStringLength = 0;
	int SGK_j;

	//code
	SGK_iStringLength = MyStrlen(SGK_str_source);
	for(SGK_j = 0; SGK_j < SGK_iStringLength; SGK_j++)
	{
		*(SGK_str_destination+SGK_j)= *(SGK_str_source+SGK_j);
	}
	*(SGK_str_destination+SGK_j) ='\0';
}
  
int MyStrlen(char *SGK_str)
{
	//variable declarations
	int SGK_j;
	int SGK_string_length= 0;

	//code
	// *** Determining exact length of the string, by detecting the first occurence of NULL-Terminating Character '\0' ***
	for(SGK_j =0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if(SGK_str[SGK_j] =='\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
