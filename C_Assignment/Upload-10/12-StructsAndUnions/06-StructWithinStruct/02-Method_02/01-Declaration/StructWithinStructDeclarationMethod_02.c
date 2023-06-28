#include<stdio.h>

struct MyPoint
{
	int SGK_x;
	int SGK_y;
};

struct Rectangle
{
	struct MyPoint SGK_point_01, SGK_point_02;
};

int main(void)
{
	//variable declaration
	int SGK_lenght, SGK_breadth, SGK_area;
	struct Rectangle SGK_rect;

	//code
	printf("\n\n");
	printf("Enter LeftMost X-Coordinate Of Rectangle : ");
	scanf("%d", &SGK_rect.SGK_point_01.SGK_x);

	printf("\n\n");
	printf("Enter Bottommost Y-Coordinate of Rectangle : ");
	scanf("%d", &SGK_rect.SGK_point_01.SGK_y);

	printf("\n\n");
	printf("Enter RightMost X-Coordinate Of Rectangle : ");
	scanf("%d", &SGK_rect.SGK_point_02.SGK_x);

	printf("\n\n");
	printf("Enter Topmost Y-Coordinate Of Rectangle : ");
	scanf("%d", &SGK_rect.SGK_point_02.SGK_y);

	SGK_lenght = SGK_rect.SGK_point_02.SGK_y - SGK_rect.SGK_point_01.SGK_y;
	if (SGK_lenght < 0)
		SGK_lenght = SGK_lenght * -1;

	SGK_breadth = SGK_rect.SGK_point_02.SGK_x - SGK_rect.SGK_point_01.SGK_x;
	if (SGK_breadth < 0)
		SGK_breadth = SGK_breadth * -1;

	SGK_area = SGK_lenght * SGK_breadth;

	printf("\n\n");
	printf("Length Of Rectangle = %d\n\n",SGK_lenght);
	printf("Breadth Of Rectangle = %d\n\n",SGK_breadth);
	printf("Area Of Rectangle = %d\n\n",SGK_area);

	return(0);
}
