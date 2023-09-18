#include<stdio.h>

//Defining struct
struct MyPoint
{
	int SGK_x;
	int SGK_y;
};

//Defining struct 
struct MyPointProperties
{
	int SGK_quadrant;
	char SGK_axis_location[10];
};

int main(void)
{
	//variable declarations
	struct MyPoint SGK_point; // declaring a single variable of type 'struct MyPoint' locally...
	struct MyPointProperties SGK_point_properties; // declaring a sing variable of type 'struct MyPointProperties' locally...

	//code
	//User Input For The Data Members Of 'struct MyPoint' variables 'point'
	printf("\n\n");
	printf("Enter X-Coordinate For A Point : ");
	scanf("%d", &SGK_point.SGK_x);
	printf("Enter Y-Coordinate For A Point : ");
	scanf("%d", &SGK_point.SGK_y);

	printf("\n\n");
	printf("Point Co-ordinates (x,y) Are : (%d,%d) !!!\n\n", SGK_point.SGK_x, SGK_point.SGK_y);

	if (SGK_point.SGK_x == 0 && SGK_point.SGK_y == 0)
		printf("The Point Is The Origin (%d,%d) !!!\n", SGK_point.SGK_x, SGK_point.SGK_y);
	else// Atleast One Of The Two values (either 'X' or 'Y' or BOTH) is a non-zero value...
	{
		if (SGK_point.SGK_x == 0)// If 'X' Is Zero...OBVIOUSLY 'Y' Is The Non-Zero Value
		{
			if (SGK_point.SGK_y < 0) // If 'Y' is -ve
				strcpy(SGK_point_properties, "Negative Y");

			if (SGK_point.SGK_y > 0) // If 'Y' is +ve
				strcpy(SGK_point_properties, "Positive Y");

			SGK_point_properties.SGK_quadrant = 0; // A Point Lying On Any Of The Co-ordinate Axes Is Not A Part Of Any Quadrant...
			printf("The Poin Lies On The %s Axis !!!\n\n", SGK_point_properties.SGK_axis_location);
		}
		else if (SGK_point.SGK_y == 0) // If 'Y' is Zero...OBVIOUSLY 'X' Is The Non-Zero Value
		{
			if (SGK_point.SGK_x < 0) // If 'X' Is -ve
				strcpy(SGK_point_properties, "Negative X");

			if (SGK_point.SGK_x > 0) // If 'X' Is +ve
				strcpy(SGK_point_properties, "Positive X");

			SGK_point_properties.SGK_quadrant = 0; // A Point Lying On Any Of The Co-ordinate Axes Is Not A Part Of Any Quadrant...
			printf("The Point Lies On The %s Axis !!!\n\n",SGK_point_properties.SGK_axis_location);
		}
		else // BOTH 'X' And 'Y' Are Non-Zero
		{
			SGK_point_properties.SGK_axis_location[0] = '\0';// A Point Lying In Any Of The 4 Qudrant Can Not Be Lying On Any Of The Co-ordinate Axes...

			if (SGK_point.SGK_x > 0 && SGK_point.SGK_y > 0) // 'X' Is +ve AND 'Y' Is +ve
				SGK_point_properties.SGK_quadrant = 1;

			else if (SGK_point.SGK_x < 0 && SGK_point.SGK_y > 0)  // 'X' Is -ve AND 'Y' Is +ve
				SGK_point_properties.SGK_quadrant = 2;

			else if (SGK_point.SGK_x < 0 && SGK_point.SGK_y < 0) // 'X' Is -ve AND 'Y' Is -ve
				SGK_point_properties.SGK_quadrant = 3;

			else												// 'X' Is +Ve AND 'Y' is -ve		
				SGK_point_properties.SGK_quadrant = 4;

			printf("The Point Lies In Quadrant Number %d !!!\n\n", SGK_point_properties.SGK_quadrant);
		}
	}

	return(0);
}	
