// header files
#include<stdio.h>

// function declaration
bool binarySearch(const int data[], const int searchKey, int end);
bool recursiveBinarySearch(const int data[], const int searchKey, const int start, const int end);

// entry point function
int main(void)
{
	// for binary search to work, the array must be sorted (either ascending or descending order)
	int data[] = { 10, 23, 33, 38, 68, 82, 94, 100 };
	int end = (sizeof(data) / sizeof(data[0])) - 1;
	int searchKey;

	searchKey = 10;
	printf("\n\tBinary Search\n");
	if (binarySearch(data, searchKey,end))
	{
		printf("\n\tFound : %d\n", searchKey);
	}
	else
	{
		printf("\n\tNot found : %d\n", searchKey);
	}

	searchKey = 100;
	printf("\n\tBinary Search\n");
	if (binarySearch(data, searchKey, end))
	{
		printf("\n\tFound : %d\n", searchKey);
	}
	else
	{
		printf("\n\tNot found : %d\n", searchKey);
	}

	searchKey = 38;
	printf("\n\tBinary Search\n");
	if (binarySearch(data, searchKey, end))
	{
		printf("\n\tFound : %d\n", searchKey);
	}
	else
	{
		printf("\n\tNot found : %d\n", searchKey);
	}


	searchKey = 38;
	printf("\n\tRecursive Binary Search\n");
	if (recursiveBinarySearch(data, searchKey,0,end))
	{
		printf("\n\tFound : %d\n", searchKey);
	}
	else
	{
		printf("\n\tNot found : %d\n", searchKey);
	}


	searchKey = 100;
	printf("\n\tRecursive Binary Search\n");
	if (recursiveBinarySearch(data, searchKey, 0, end))
	{
		
		printf("\n\tFound : %d\n", searchKey);
	}
	else
	{
		printf("\n\tNot found : %d\n", searchKey);
	}

	searchKey = 53;
	printf("\n\tRecursive Binary Search\n");
	if (recursiveBinarySearch(data, searchKey, 0, end))
	{
		
		printf("\n\tFound : %d\n", searchKey);
	}
	else
	{
		printf("\n\tNot found : %d\n", searchKey);
	}

	searchKey = 94;
	printf("\n\tRecursive Binary Search\n");
	if (recursiveBinarySearch(data, searchKey, 0, end))
	{

		printf("\n\tFound : %d\n", searchKey);
	}
	else
	{
		printf("\n\tNot found : %d\n", searchKey);
	}

}

bool binarySearch(const int data[], const int key,int end)
{
	int start = 0;

	//int end = (sizeof(data) / sizeof(data[0])) - 1;  // some error occured does not return correct number of data present in array
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		if (data[mid] == key)
		{
			return(true);
		}

		if (data[mid] < key)
		{
			start = mid + 1;
		}
		else
		{
			end = mid - 1;
		}
	}

	return(false);
}

bool recursiveBinarySearch(const int data[], const int searchKey, const int start, const int end)
{
	// end check
	if (start > end)
	{
		return(false);
	}

	// variable declaration
	int	mid = (start + end) / 2;

	// code

	if (data[mid] == searchKey)
	{
		return(true);
	}

	if (data[mid] < searchKey)
	{
		return(recursiveBinarySearch(data, searchKey, mid + 1, end));
	}
	else
	{
		return(recursiveBinarySearch(data, searchKey, start, mid - 1));
	}

}
