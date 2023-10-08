// header files
#include <GL/freeglut.h>

// global variable declarations
bool bIsFullScreen = false;

bool bFlameLeft = false;
bool bFlameRight = false;
bool bFlameCenter = false;
bool sKeyPress = false;

float vertexFlameLeft[] = { -0.02f,0.54f,0.0f }; // left side flame
float vertexFlameCenter[] = { 0.0f,0.56f,0.0f }; // center flame
float vertexFlameRight[] = { 0.02f,0.54f,0.0f }; // right side flame

float colorFlameLeft[] = { 1.0f,50.0f / 255.0f,0.0f }; // left flame color
float colorFlameRight[] = { 1.0f,160.0f / 255.0f,0.0f };// right flame color
float colorFlameCenter[] = { 1.0f,130.0f / 255.0f,0.0f };// center flame color

float vertexPointOne[] = { 0.0f,0.4f,0.0f };
float vertexPointTwo[] = { 0.0f,0.4f,0.0f };
float vertexPointThree[] = { 0.0f,0.4f,0.0f };
float vertexPointFour[] = { 0.0f,0.4f,0.0f };
float vertexPointFive[] = { -0.01f,0.4f,0.0f };
float vertexPointSix[] = { 0.01f,0.4f,0.0f };
float vertexPointSeven[] = { 0.0f,0.4f,0.0f };
float vertexPointEight[] = { 0.0f,0.4f,0.0f };
float vertexPointNine[] = { 0.0f,0.4f,0.0f };
float vertexPointTen[] = { 0.0f,0.4f,0.0f };
float vertexPointEleven[] = { 0.0f,0.4f,0.0f };
float vertexPointTwelve[] = { 0.0f,0.4f,0.0f };

// Entry-point function
int main(int argc, char* argv[])
{
	// function declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);
	
	// code
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("My First RTR5 Program : Siddhant Ganesh Kshirsagar");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return(0);

}

void initialize(void)
{
	// code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height)
{
	// code
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glBegin(GL_POLYGON); // bottom of cake 1

	glColor3f(1.0f, 241.0f/255.0f, 187.0f/255.0f);
	glVertex3f(-0.4f, -0.4f, 0.0f);

	glColor3f(1.0f, 241.0f / 255.0f, 187.0f / 255.0f);
	glVertex3f(0.4f, -0.4f, 0.0f);

	glColor3f(1.0f, 166.0f/255.0f, 131.0f/255.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);

	glColor3f(1.0f, 166.0f / 255.0f, 131.0f / 255.0f);
	glVertex3f(0.38f, -0.18f, 0.0f);

	glColor3f(1.0f, 166.0f / 255.0f, 131.0f / 255.0f);
	glVertex3f(-0.38f, -0.18f, 0.0f);

	glColor3f(1.0f, 166.0f / 255.0f, 131.0f / 255.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);// bottom of cake bottom white quad 1

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.38f, -0.18f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.38f, -0.18f, 0.0f);

	glEnd();

	glPointSize(4.0f);
	glBegin(GL_POINTS);

	glColor3f(1.0f, 0.0f, 0.0f);
	//glColor3f(81.0f/255.0f, 29.0f/255.0f, 16.0f/255.0f);
	glVertex3f(-0.36f, -0.19f, 0.0f);
	glVertex3f(-0.32f, -0.19f, 0.0f);
	glVertex3f(-0.28f, -0.19f, 0.0f);
	glVertex3f(-0.24f, -0.19f, 0.0f);
	glVertex3f(-0.2f, -0.19f, 0.0f);
	glVertex3f(-0.16f, -0.19f, 0.0f);
	glVertex3f(-0.12f, -0.19f, 0.0f);
	glVertex3f(-0.08f, -0.19f, 0.0f);
	glVertex3f(-0.04f, -0.19f, 0.0f);
	glVertex3f(0.0f, -0.19f, 0.0f);
	glVertex3f(0.04f, -0.19f, 0.0f);
	glVertex3f(0.08f, -0.19f, 0.0f);
	glVertex3f(0.12f, -0.19f, 0.0f);
	glVertex3f(0.16f, -0.19f, 0.0f);
	glVertex3f(0.2f, -0.19f, 0.0f);
	glVertex3f(0.24f, -0.19f, 0.0f);
	glVertex3f(0.28f, -0.19f, 0.0f);
	glVertex3f(0.32f, -0.19f, 0.0f);
	glVertex3f(0.36f, -0.19f, 0.0f);
	glEnd();

	glBegin(GL_QUADS); // bottom of cake top white quad 

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.4f, -0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, -0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, -0.38f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.4f, -0.38f, 0.0f);

	glEnd();

	glPointSize(4.0f);
	glBegin(GL_POINTS);

	glColor3f(1.0f, 0.0f, 0.0f);
	//glColor3f(81.0f/255.0f, 29.0f/255.0f, 16.0f/255.0f);
	glVertex3f(-0.36f, -0.39f, 0.0f);
	glVertex3f(-0.32f, -0.39f, 0.0f);
	glVertex3f(-0.28f, -0.39f, 0.0f);
	glVertex3f(-0.24f, -0.39f, 0.0f);
	glVertex3f(-0.2f, -0.39f, 0.0f);
	glVertex3f(-0.16f, -0.39f, 0.0f);
	glVertex3f(-0.12f, -0.39f, 0.0f);
	glVertex3f(-0.08f, -0.39f, 0.0f);
	glVertex3f(-0.04f, -0.39f, 0.0f);
	glVertex3f(0.0f, -0.39f, 0.0f);
	glVertex3f(0.04f, -0.39f, 0.0f);
	glVertex3f(0.08f, -0.39f, 0.0f);
	glVertex3f(0.12f, -0.39f, 0.0f);
	glVertex3f(0.16f, -0.39f, 0.0f);
	glVertex3f(0.2f, -0.39f, 0.0f);
	glVertex3f(0.24f, -0.39f, 0.0f);
	glVertex3f(0.28f, -0.39f, 0.0f);
	glVertex3f(0.32f, -0.39f, 0.0f);
	glVertex3f(0.36f, -0.39f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON); // middle of cake 2

	glColor3f(214.0f/255.0f, 0.0f, 1.0f);
	glVertex3f(-0.34f, -0.18f, 0.0f);

	glColor3f(214.0f / 255.0f, 0.0f, 1.0f);
	glVertex3f(0.34f, -0.18f, 0.0f);

	glColor3f(0.0f, 1.0f, 226.0f/255.0f);
	glVertex3f(0.34f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 226.0f / 255.0f);
	glVertex3f(0.32f, 0.02f, 0.0f);

	glColor3f(0.0f, 1.0f, 226.0f / 255.0f);
	glVertex3f(-0.32f, 0.02f, 0.0f);

	glColor3f(0.0f, 1.0f, 226.0f / 255.0f);
	glVertex3f(-0.34f, 0.0f, 0.0f);

	glEnd();

	glBegin(GL_QUADS); // middle of cake top white quad 

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.34f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.34f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.32f, 0.02f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.32f, 0.02f, 0.0f);

	glEnd();

	glPointSize(4.0f);
	glBegin(GL_POINTS);

	glColor3f(1.0f, 0.0f, 0.0f);
	//glColor3f(81.0f/255.0f, 29.0f/255.0f, 16.0f/255.0f);
	glVertex3f(-0.32f, 0.01f, 0.0f);
	glVertex3f(-0.28f, 0.01f, 0.0f);
	glVertex3f(-0.24f, 0.01f, 0.0f);
	glVertex3f(-0.2f, 0.01f, 0.0f);
	glVertex3f(-0.16f, 0.01f, 0.0f);
	glVertex3f(-0.12f, 0.01f, 0.0f);
	glVertex3f(-0.08f, 0.01f, 0.0f);
	glVertex3f(-0.04f, 0.01f, 0.0f);
	glVertex3f(0.0f, 0.01f, 0.0f);
	glVertex3f(0.04f, 0.01f, 0.0f);
	glVertex3f(0.08f, 0.01f, 0.0f);
	glVertex3f(0.12f, 0.01f, 0.0f);
	glVertex3f(0.16f, 0.01f, 0.0f);
	glVertex3f(0.2f, 0.01f, 0.0f);
	glVertex3f(0.24f, 0.01f, 0.0f);
	glVertex3f(0.28f, 0.01f, 0.0f);
	glVertex3f(0.32f, 0.01f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON); // top of cake 3

	glColor3f(199.0f / 255.0f, 0.0f, 57.0f / 255.0f);
	glVertex3f(-0.26f, 0.02f, 0.0f);

	glColor3f(199.0f / 255.0f, 0.0f, 57.0f / 255.0f);
	glVertex3f(0.26f, 0.02f, 0.0f);

	glColor3f(255.0f / 255.0f, 195.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(0.26f, 0.2f, 0.0f);

	glColor3f(255.0f / 255.0f, 195.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(0.24f, 0.22f, 0.0f);

	glColor3f(255.0f / 255.0f, 195.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.24f, 0.22f, 0.0f);

	glColor3f(255.0f / 255.0f, 195.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.26f, 0.2f, 0.0f);

	glEnd();

	glBegin(GL_QUADS); // top of cake top white quad 

	glColor3f(1.0f, 1.0f, 1.0f);

	glVertex3f(-0.26f, 0.2f, 0.0f);

	glVertex3f(0.26f, 0.2f, 0.0f);

	glVertex3f(0.24f, 0.22f, 0.0f);

	glVertex3f(-0.24f, 0.22f, 0.0f);

	glEnd();

	glPointSize(4.0f);
	glBegin(GL_POINTS);

	glColor3f(1.0f, 0.0f, 0.0f);
	//glColor3f(81.0f/255.0f, 29.0f/255.0f, 16.0f/255.0f);
	glVertex3f(-0.24f, 0.21f, 0.0f);
	glVertex3f(-0.2f, 0.21f, 0.0f);
	glVertex3f(-0.16f, 0.21f, 0.0f);
	glVertex3f(-0.12f, 0.21f, 0.0f);
	glVertex3f(-0.08f, 0.21f, 0.0f);
	glVertex3f(-0.04f, 0.21f, 0.0f);
	glVertex3f( 0.0f, 0.21f, 0.0f);
	glVertex3f(0.04f, 0.21f, 0.0f);
	glVertex3f(0.08f, 0.21f, 0.0f);
	glVertex3f(0.12f, 0.21f, 0.0f);
	glVertex3f(0.16f, 0.21f, 0.0f);
	glVertex3f(0.2f, 0.21f, 0.0f);
	glVertex3f(0.24f, 0.21f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);	// candle 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(-0.02f, 0.22f, 0.0f);

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.02f, 0.22f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.02f, 0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.01f, 0.41f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.01f, 0.41f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.02f, 0.4f, 0.0f);

	glEnd();



	glBegin(GL_TRIANGLES);

	//left flame
	glColor3f(1.0f, 0.8f, 0.0f);
	glVertex3f(-0.01f, 0.41f, 0.0f);

	glColor3f(1.0f, 0.7f, 0.0f);
	glVertex3f(0.01f, 0.41f, 0.0f);

	glColor3f(colorFlameLeft[0], colorFlameLeft[1], colorFlameLeft[2]);
	glVertex3f(vertexFlameLeft[0], vertexFlameLeft[1], vertexFlameLeft[0]);

	//center flame
	glColor3f(1.0f, 0.8f, 0.0f);
	glVertex3f(-0.01f, 0.41f, 0.0f);

	glColor3f(1.0f, 0.7f, 0.0f);
	glVertex3f(0.01f, 0.41f, 0.0f);

	glColor3f(colorFlameCenter[0], colorFlameCenter[1], colorFlameCenter[2]);
	glVertex3f(vertexFlameCenter[0], vertexFlameCenter[1], vertexFlameCenter[2]);

	//right flame
	glColor3f(1.0f, 0.7f, 0.0f);
	glVertex3f(-0.01f, 0.41f, 0.0f);

	glColor3f(1.0f, 0.8f, 0.0f);
	glVertex3f(0.01f, 0.41f, 0.0f);

	glColor3f(colorFlameRight[0], colorFlameRight[1], colorFlameRight[2]);
	glVertex3f(vertexFlameRight[0], vertexFlameRight[1], vertexFlameRight[2]);

	glEnd();

	if (bFlameLeft == false)//flame left side 
	{
		if (vertexFlameLeft[0] < 0.02f)
		{
			vertexFlameLeft[0] = vertexFlameLeft[0] + 0.0005f;
		}
		else
		{
			bFlameLeft = true;
		}
	}
	else
	{
		if (vertexFlameLeft[0] > -0.02f)
		{
			vertexFlameLeft[0] = vertexFlameLeft[0] - 0.0005f;
		}
		else
		{
			bFlameLeft = false;
		}
	}

	if (bFlameCenter == false) // flame center
	{
		if (vertexFlameCenter[0] < 0.02f)
		{
			vertexFlameCenter[0] = vertexFlameCenter[0] + 0.0005f;
		}
		else
		{
			bFlameCenter = true;
		}
		if (vertexFlameCenter[1] > 0.5f)
		{
			vertexFlameCenter[1] = vertexFlameCenter[1] - 0.005f;
		}
	}
	else
	{
		if (vertexFlameCenter[0] > -0.02f)
		{
			vertexFlameCenter[0] = vertexFlameCenter[0] - 0.0005f;
		}
		else
		{
			bFlameCenter = false;
		}
		if (vertexFlameCenter[1] < 0.6f)
		{
			vertexFlameCenter[1] = vertexFlameCenter[1] + 0.005f;
		}
	}

	if (bFlameRight == false) // flame right side
	{
		if (vertexFlameRight[0] > -0.02f)
		{
			vertexFlameRight[0] = vertexFlameRight[0] - 0.0005f;
		}
		else
		{
			bFlameRight = true;
		}
		if (vertexFlameRight[1] < 0.6f)
		{
			vertexFlameRight[1] = vertexFlameRight[1] + 0.005f;
		}
	}
	else
	{
		if (vertexFlameRight[0] < 0.02f)
		{
			vertexFlameRight[0] = vertexFlameRight[0] + 0.0005f;
		}
		else
		{
			bFlameRight = false;
		}
		if (vertexFlameRight[1] > 0.5f)
		{
			vertexFlameRight[1] = vertexFlameRight[1] - 0.005f;
		}
	}
	// color flame left
	if (bFlameLeft == false)
	{
		if (colorFlameLeft[1] < (160.0f / 255.0f))
		{
			colorFlameLeft[1] = colorFlameLeft[1] + 0.01f;
		}
	}
	else
	{
		if (colorFlameLeft[1] > (50.0f / 255.0f))
		{
			colorFlameLeft[1] = colorFlameLeft[1] - 0.01f;
		}
	}

	// color flame right
	if (bFlameRight == false)
	{
		if (colorFlameRight[1] > (50.0f / 255.0f))
		{
			colorFlameRight[1] = colorFlameRight[1] - 0.01f;
		}
	}
	else
	{
		if (colorFlameRight[1] < (160.0f / 255.0f))
		{
			colorFlameRight[1] = colorFlameRight[1] + 0.01f;
		}
	}
	//color flame center
	if (bFlameCenter == false)
	{
		if (colorFlameCenter[1] > (50.0f / 255.0f))
		{
			colorFlameCenter[1] = colorFlameCenter[1] - 0.01f;
		}
	}
	else
	{
		if (colorFlameCenter[1] < (160.0f / 255.0f))
		{
			colorFlameCenter[1] = colorFlameCenter[1] + 0.01f;
		}
	}

	glPointSize(2.45f);
	glBegin(GL_POINTS);

	glColor3f(colorFlameCenter[0], colorFlameCenter[1], colorFlameCenter[2]); // for dynamic color change
	glVertex3f(vertexPointOne[0], vertexPointOne[1], vertexPointOne[2]);
	glVertex3f(vertexPointTwo[0], vertexPointTwo[1], vertexPointTwo[2]);
	glVertex3f(vertexPointThree[0], vertexPointThree[1], vertexPointThree[2]);
	glVertex3f(vertexPointFour[0], vertexPointFour[1], vertexPointFour[2]);

	glPointSize(3.45f);
	glColor3f(colorFlameLeft[0], colorFlameLeft[1], colorFlameLeft[2]); // for dynamic color change
	glVertex3f(vertexPointFive[0], vertexPointFive[1], vertexPointFive[2]);
	glVertex3f(vertexPointSix[0], vertexPointSix[1], vertexPointSix[2]);
	glVertex3f(vertexPointSeven[0], vertexPointSeven[1], vertexPointSeven[2]);
	glVertex3f(vertexPointEight[0], vertexPointEight[1], vertexPointEight[2]);

	glPointSize(1.45f);
	glColor3f(colorFlameRight[0], colorFlameRight[1], colorFlameRight[2]); // for dynamic color change
	glVertex3f(vertexPointNine[0], vertexPointNine[1], vertexPointNine[2]);
	glVertex3f(vertexPointTen[0], vertexPointTen[1], vertexPointTen[2]);
	glVertex3f(vertexPointEleven[0], vertexPointEleven[1], vertexPointEleven[2]);
	glVertex3f(vertexPointTwelve[0], vertexPointTwelve[1], vertexPointTwelve[2]);

	glEnd();

		
	if (sKeyPress == false)
	{
		/*while (sKeyPress != true)
		{*/
			//for x axis animation
			if (vertexPointOne[0] > -0.05f)
			{
				vertexPointOne[0] = vertexPointOne[0] - 0.008f;
				vertexPointSix[0] = vertexPointSix[0] - 0.008f;
			}
			else
			{
				vertexPointOne[0] = 0.0f;
				vertexPointSix[0] = 0.0f;
			}
			if (vertexPointTwo[0] < 0.05f)
			{
				vertexPointTwo[0] = vertexPointTwo[0] + 0.008f;
				vertexPointTwelve[0] = vertexPointTwelve[0] + 0.008f;
			}
			else
			{
				vertexPointTwo[0] = 0.0f;
				vertexPointTwelve[0] = 0.0f;
			}
			//for x axis animation
			if (vertexPointThree[0] > -0.07f)
			{
				vertexPointThree[0] = vertexPointThree[0] - 0.008f;
				vertexPointNine[0] = vertexPointNine[0] - 0.008f;
				vertexPointEleven[0] = vertexPointEleven[0] - 0.008f;
			}
			else
			{
				vertexPointThree[0] = 0.0f;
				vertexPointNine[0] = 0.0f;
				vertexPointEleven[0] = 0.0f;
			}
			if (vertexPointFour[0] < 0.07f)
			{
				vertexPointFour[0] = vertexPointFour[0] + 0.008f;
				vertexPointEight[0] = vertexPointEight[0] + 0.008f;
				vertexPointTen[0] = vertexPointTen[0] + 0.008f;
			}
			else
			{
				vertexPointFour[0] = 0.0f;
				vertexPointEight[0] = 0.0f;
				vertexPointTen[0] = 0.0f;
			}
			//for y axis animation
			if (vertexPointOne[1] < 0.7f)
			{
				vertexPointOne[1] = vertexPointOne[1] + 0.008f;
				vertexPointTwo[1] = vertexPointTwo[1] + 0.008f;
				vertexPointEleven[1] = vertexPointEleven[1] + 0.008f;
				vertexPointTwelve[1] = vertexPointTwelve[1] + 0.008f;
			}
			else
			{
				vertexPointOne[1] = 0.4f;
				vertexPointTwo[1] = 0.4f;
				vertexPointEleven[1] = 0.4f;
				vertexPointTwelve[1] = 0.4f;
			}
			//for y axis animation
			if (vertexPointThree[1] < 0.9f)
			{
				vertexPointThree[1] = vertexPointThree[1] + 0.008f;
				vertexPointFour[1] = vertexPointFour[1] + 0.008f;
				vertexPointFive[1] = vertexPointFive[1] + 0.008f;
				vertexPointSix[1] = vertexPointSix[1] + 0.008f;
			}
			else
			{
				vertexPointThree[1] = 0.4f;
				vertexPointFour[1] = 0.4f;
				vertexPointFive[1] = 0.4f;
				vertexPointSix[1] = 0.4f;
			}

			//for y axis animation
			if (vertexPointSeven[1] < 0.8f)
			{
				vertexPointSeven[1] = vertexPointSeven[1] + 0.008f;
				vertexPointEight[1] = vertexPointEight[1] + 0.008f;
				vertexPointNine[1] = vertexPointNine[1] + 0.008f;
				vertexPointTen[1] = vertexPointTen[1] + 0.008f;
			}
			else
			{
				vertexPointSeven[1] = 0.4f;
				vertexPointEight[1] = 0.4f;
				vertexPointNine[1] = 0.4f;
				vertexPointTen[1] = 0.4f;
			}
			/*if (sKeyPress == true)
			{
				break;
			}*/

		//}
	}

	
	glutPostRedisplay();
	glutSwapBuffers();

}

void keyboard(unsigned char key, int x, int y)
{
	// code
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
	{
		if (bIsFullScreen == false)
		{
			glutFullScreen();
			bIsFullScreen = true;

		}
		else
		{
			glutLeaveFullScreen();
			bIsFullScreen = false;
		}
	}
		break;
	case 'S':
	case 's':
	{
		if (sKeyPress == false)
		{
			sKeyPress = true;
		}
		else
		{
			sKeyPress = false;
		}
	}
	break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	// code 
	switch (button)
	{
	case GLUT_RIGHT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void uninitialize(void)
{
	// code
}

