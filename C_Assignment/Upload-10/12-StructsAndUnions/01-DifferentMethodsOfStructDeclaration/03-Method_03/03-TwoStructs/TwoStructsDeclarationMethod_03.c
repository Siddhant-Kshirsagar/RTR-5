#include<stdio.h>

int main(void)
{
	//DEFINING STRUCT
	struct MyPoint
	{
		int SGK_x;
		int SGK_y;
	}SGK_point; // declaring a single variable of type 'struct MyPoint' locally ...

	//DEFINING STRUCT
	struct MyPointProperties
	{
		int SGK_quadrant;
		char SGK_axis_location[10];
	}SGK_point_properties;// declaring a single variable of type 'struct MyPointProperties' locally...

	//code
	//User Input For The Data Members Of 'struct MyPoint' variable 'point_A'
	printf("\n\n");
	printf("Enter X-Coordinate For A Point : ");
	scanf("%d", &SGK_point.SGK_x);
	printf("Enter Y-Coordinate For A Point : ");
	scanf("%d", &SGK_point.SGK_y);

	printf("\n\n");
	printf("Point Co-ordinates (x,y) Are : (%d,%d)!!!\n\n", SGK_point.SGK_x, SGK_point.SGK_y);

	if (SGK_point.SGK_x == 0 && SGK_point.SGK_y == 0)
		printf("The Point Is The Origin (%d,%d) !!!\n", SGK_point.SGK_x, SGK_point.SGK_y);
	else // Atleast One Of The Two Values (either 'X' or 'Y' or BOTH) is a non-zero value...
	{
		if (SGK_point.SGK_x == 0) // If 'X' Is ZERO... OBVIOUSLY 'Y' IS THE NON-ZERO VALUE
		{
			if (SGK_point.SGK_y < 0) // If 'Y' IS -ve
				strcpy(SGK_point_properties.SGK_axis_location, "Negative Y");

			if (SGK_point.SGK_y > 0) // If 'Y' Is +ve
				strcpy(SGK_point_properties.SGK_axis_location, "Positive Y");

			SGK_point_properties.SGK_quadrant = 0; // A Point Lying On Any Of The Co-ordinate Axes Is Not A Part Of Any Quadrant...
			printf("The Point Lies On The %s Axis!!!\n\n",SGK_point_properties.SGK_axis_location);
		}
		else if (SGK_point.SGK_y == 0) // If 'Y' Is ZERO... OBVIOUSLY 'X' Is The Non-Zero Value
		{
			if (SGK_point.SGK_x < 0) // If 'X' Is -ve
				strcpy(SGK_point_properties.SGK_axis_location, "Negative X");

			if (SGK_point.SGK_x > 0) // If 'X' Is +ve
				strcpy(SGK_point_properties.SGK_axis_location, "Positive X");

			SGK_point_properties.SGK_quadrant = 0; // A Point Lying On Any Of The Co-ordinate Axes is not a Part Of Any Quadrant...
			printf("The Point Lies On The %s Axis !!!\n\n",SGK_point_properties.SGK_axis_location);
		}
		else // BOTH 'X' AND 'Y' ARE NON-ZERO
		{
			SGK_point_properties.SGK_axis_location[0] = '\0'; // A Poitn Lying In ANy Of The Quadrant Cannot Be Lying On Any Of The Co-ordinate AXES...

				if (SGK_point.SGK_x > 0 && SGK_point.SGK_y > 0) // 'X' IS +Ve And 'Y' IS +ve
					SGK_point_properties.SGK_quadrant = 1;

				else if (SGK_point.SGK_x < 0 && SGK_point.SGK_y > 0) // 'X' Is -ve And 'Y' Is +ve 
					SGK_point_properties.SGK_quadrant = 2;

				else if (SGK_point.SGK_x < 0 && SGK_point.SGK_y < 0) // 'X' Is -ve And 'Y' Is -ve 
					SGK_point_properties.SGK_quadrant = 3;

				else												// 'X' Is +ve AND 'Y' IS -ve 							
					SGK_point_properties.SGK_quadrant = 4;

				printf("The Point Lies In Quadrant Number %d !!!\n\n", SGK_point_properties.SGK_quadrant);
		}

	} 
	
	return(0);
}
