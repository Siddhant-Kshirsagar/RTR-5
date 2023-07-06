
// header files
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <MATH.h>
// global variable declarations
bool bIsFullScreen = false;// for fullscreen
bool isKeyPress = false;//for door open
bool sunrise = false;//for sunrise
float movement = 0.0001; // movement speed of sunrise

float vertexCircle[] = {0.9f,0.35f,0.0f};
float sunriseColor[] = {0.0f/255.0f,0.0f/255.0f,0.0f/255.0f};
float sunColor[] = { 1.0f,108.0f / 255.0f,0.0f };
float streetLightColor[] = { 1.0f,1.0f,0.0f };

float vertexDoorTop[] = {0.0f,0.0f,0.0f}; // to handle opening of door (left side top vertex)
float vertexDoorBottom[] = {0.0f,0.0f,0.0f}; // to handle opening of door(left side bottom vertex)

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

	glBegin(GL_QUADS);

    //square top background =>top part
	glColor3f(0.0f / 255.0f, 108.0f / 255.0f, 174.0f / 255.0f);
	glVertex3f(-1.0f, 0.7f, 0.0f);

	glColor3f(0.0f / 255.0f, 108.0f / 255.0f, 174.0f / 255.0f);
	glVertex3f(1.0f, 0.7f, 0.0f);

	glColor3f(17.0f / 255.0f, 49.0f / 255.0f, 124.0f / 255.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glColor3f(17.0f / 255.0f, 49.0f / 255.0f, 124.0f / 255.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//square top background =>bottom part
	glColor3f(sunriseColor[0] , sunriseColor[1], sunriseColor[2]);
	glVertex3f(-1.0f, 0.5f, 0.0f);

	glColor3f(sunriseColor[0], sunriseColor[1], sunriseColor[2]);
	glVertex3f(1.0f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 108.0f / 255.0f, 174.0f / 255.0f);
	glVertex3f(1.0f, 0.7f, 0.0f);

	glColor3f(0.0f / 255.0f, 108.0f / 255.0f, 174.0f / 255.0f);
	glVertex3f(-1.0f, 0.7f, 0.0f);
	
	glEnd();
	
	glBegin(GL_LINES);

	float radius = 0.1f;
	for(float angle = 0.0f; angle<=360.0f;angle = angle + 0.001f)
	{
		float x = vertexCircle[0] + radius * cos(angle);
		float y = vertexCircle[1] + radius * sin(angle);

		glColor3f(sunColor[0], sunColor[1], sunColor[2]);
		glVertex3f(vertexCircle[0], vertexCircle[1], vertexCircle[2]);

		glColor3f(sunColor[0], sunColor[1], sunColor[2]);
		glVertex3f(x, y, 0.0f);

	}
		
	if (sunrise == false)
	{
		if (vertexCircle[1] < 0.45f)
		{
			vertexCircle[1] = vertexCircle[1] + movement;
			if (sunColor[1] < (166.0f / 255.0f))
			{
				sunColor[1] = sunColor[1] + 0.0009f;
			}
			if (sunriseColor[0] < 1.0f)
			{
				sunriseColor[0] = sunriseColor[0] + 0.001f;
			}
			if (sunriseColor[1] < (108.0f / 255.0f))
			{
				sunriseColor[1] = sunriseColor[1] + 0.001f;
			}
			/*if (sunriseColor[2] > (0.0f / 255.0f))
			{
				sunriseColor[2] = sunriseColor[2] - 0.0001f;
			}*/
		}
	}
	else
	{
		//float vertexCircle[] = { 0.9f,0.45f,0.0f };
		vertexCircle[0] = 0.9f;
		vertexCircle[1] = 0.35f;
		vertexCircle[2] = 0.0f;

		sunriseColor[0] = 0.0f/255.0f;
		sunriseColor[1] = 0.0f/255.0f;
		sunriseColor[2] = 0.0f/255.0f;

		sunColor[0] = 1.0f;
		sunColor[1] = 108.0f / 255.0f;
		sunColor[2] = 0.0f;
	}
	glEnd();
	glBegin(GL_QUADS);
	//square bottom background =>
	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-1.0f, 0.5f, 0.0f);

	glColor3f(sunriseColor[0], sunriseColor[1], sunriseColor[2]);
	glVertex3f(1.0f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	//square 1 left side front home 
	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(-0.6f, 0.6f, 0.0f);

	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(0.0f, 0.7f, 0.0f);

	
	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(-0.6f, -0.2f, 0.0f);

	
	//square 2 right side front home
	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(0.6f, 0.6f, 0.0f);

	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(0.0f, 0.7f, 0.0f);

	glColor3f(241.0f / 255.0f, 226.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glColor3f(241.0f / 255.0f, 226.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(0.6f, -0.2f, 0.0f);

	
	//square 3 // top part of home under roof and behind left side triangle
	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(-0.6f, 0.6f, 0.0f);

	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(0.0f, 0.4f, 0.0f);

	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(0.6f, 0.6f, 0.0f);

	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(0.0f, 0.7f, 0.0f);

	
	//square 4  shadow
	glColor3f(0.04f, 0.04f, 0.04f);
	glVertex3f(-0.6f, -0.2f, 0.0f);

	glColor3f(0.04f, 0.04f, 0.04f);
	glVertex3f(0.0f, -0.6f, 0.0f);

	glColor3f(0.12f, 0.12f, 0.12f);
	glVertex3f(-0.455f, -0.8777f, 0.0f);

	glColor3f(0.12f, 0.12f, 0.12f);
	glVertex3f(-0.999f, -0.455f, 0.0f);

	//square 5 roof 1 backside 93, 49, 25 
	glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	glVertex3f(-0.7f, 0.599f, 0.0f);

	glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	glVertex3f(-0.4f, 0.9f, 0.0f);

	glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	glVertex3f(0.3f, 0.9f, 0.0f);

	glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	glVertex3f(0.0f, 0.7f, 0.0f);

	//square 6 roof 2 frontside 94, 51, 18 

	glColor3f(70.0f / 255.0f, 38.0f / 255.0f, 13.0f / 255.0f);
	glVertex3f(-0.4f, 0.9f, 0.0f);

	glColor3f(70.0f / 255.0f, 38.0f / 255.0f, 13.0f / 255.0f);
	glVertex3f(-0.1f, 0.3f, 0.0f);

	//70, 38, 13 

	glColor3f(94.0f / 255.0f, 51.0f / 255.0f, 18.0f / 255.0f);
	glVertex3f(0.7f, 0.5f, 0.0f);

	glColor3f(94.0f / 255.0f, 51.0f / 255.0f, 18.0f / 255.0f);
	glVertex3f(0.3f, 0.9f, 0.0f);


	//square 7 door 247, 140, 56 
	glColor3f(247.0f / 255.0f, 140.0f / 255.0f, 56.0f / 255.0f);
	glVertex3f(0.3f, -0.40f, 0.0f);

	glColor3f(247.0f / 255.0f, 140.0f / 255.0f, 56.0f / 255.0f);
	glVertex3f(0.3f, 0.3f, 0.0f);
	//right top side
	glColor3f(247.0f / 255.0f, 140.0f / 255.0f, 56.0f / 255.0f);
	glVertex3f(0.5f, 0.368f, 0.0f);
	// right bottom side 
	glColor3f(247.0f / 255.0f, 140.0f / 255.0f, 56.0f / 255.0f);
	glVertex3f(0.5f, -0.266f, 0.0f);

	if(isKeyPress) // door opening on space key pressed
	{
		if(vertexDoorTop[0]<=0.6f && vertexDoorBottom[0] <= 0.6f)
		{
			vertexDoorTop[0] = vertexDoorTop[0] + 0.001f;
			vertexDoorBottom[0] = vertexDoorBottom[0] + 0.001f;
		}
		
	}
	else
	{
		vertexDoorBottom[0] = 0.3f;
		vertexDoorBottom[1] = -0.40f;
		vertexDoorBottom[2] = 0.0f;

		vertexDoorTop[0] = 0.3f;
		vertexDoorTop[1] = 0.3f;
		vertexDoorTop[2] = 0.0f;
	}
	//square 7 door1 opening door vertex
	// glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	// glVertex3f(0.4f, -0.40f, 0.0f);

	// glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	// glVertex3f(0.4f, 0.3f, 0.0f);

	glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	glVertex3f(vertexDoorBottom[0], vertexDoorBottom[1], vertexDoorBottom[2]);

	glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	glVertex3f(vertexDoorTop[0], vertexDoorTop[1], vertexDoorTop[2]);
	//right top side
	glColor3f(68.0f / 255.0f, 33.0f / 255.0f, 14.0f / 255.0f);
	glVertex3f(0.5f, 0.372f, 0.0f);
	//right bottom side
	glColor3f(68.0f / 255.0f, 33.0f / 255.0f, 14.0f / 255.0f);
	glVertex3f(0.5f, -0.266f, 0.0f);



	
	//square 8 window 249, 101, 4 
	glColor3f(249.0f / 255.0f, 101.0f / 255.0f, 4.0f / 255.0f);
	glVertex3f(-0.3f, 0.10f, 0.0f);

	glColor3f(249.0f / 255.0f, 101.0f / 255.0f, 4.0f / 255.0f);
	glVertex3f(-0.3f, 0.35f, 0.0f);

	glColor3f(247.0f / 255.0f, 140.0f / 255.0f, 56.0f / 255.0f);
	glVertex3f(-0.45f, 0.418f, 0.0f);

	glColor3f(247.0f / 255.0f, 140.0f / 255.0f, 56.0f / 255.0f);
	glVertex3f(-0.45f, 0.179f, 0.0f);
	

	//square 8 window 1
	glColor3f(68.0f / 255.0f, 33.0f / 255.0f, 14.0f / 255.0f);
	glVertex3f(-0.37f, 0.11f, 0.0f);

	glColor3f(68.0f / 255.0f, 33.0f / 255.0f, 14.0f / 255.0f);
	glVertex3f(-0.37f, 0.34f, 0.0f);

	glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	glVertex3f(-0.45f, 0.418f, 0.0f);

	glColor3f(93.0f / 255.0f, 49.0f / 255.0f, 25.0f / 255.0f);
	glVertex3f(-0.45f, 0.179f, 0.0f);


	/*
	/
	//square 8 window 2

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.10f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.24f, 0.101f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.24f, 0.31f, 0.0f);

	*/
	glEnd();


	glBegin(GL_TRIANGLES);
	//triangle 1 shadow

	glColor3f(0.12f, 0.12f, 0.12f);
	glVertex3f(-0.6f, 0.5f, 0.0f);

	glColor3f(0.04f, 0.04f, 0.04f);
	glVertex3f(-0.6f, -0.2f, 0.0f);

	glColor3f(0.12f, 0.12f, 0.12f);
	glVertex3f(-0.999f, -0.455f, 0.0f);


	
	//triangle 2 left side triangle below roof
	
	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(-0.6f, 0.6f, 0.0f);
	//4, 85, 182
	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(-0.381f, 0.870f, 0.0f);

	glColor3f(142.0f / 255.0f, 134.0f / 255.0f, 6.0f / 255.0f);
	glVertex3f(-0.1858f, 0.4649f, 0.0f);

	
	

	/*
	
	//triangle 4

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f,-0.5f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, -0.9f, 0.0f);


	
	//triangle 5

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, -0.9f, 0.0f);


	//triangle 6
	

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);


	//triangle 7
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);

	//triangle 8
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);


	//triangle 9

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, 0.0f, 0.0f);


	//triangle 10


	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, 0.0f, 0.0f);


	//triangle 11
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.9f, 0.0f, 0.0f);

	//triangle 12
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f,-0.5f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.9f, 0.0f, 0.0f);
	*/

	glEnd();
// stars 
	glPointSize(1.45f);
	glBegin(GL_POINTS);
	glColor3f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	glVertex3f(-0.9f, 0.8f, 0.0f);
	glVertex3f(-0.7f, 0.85f, 0.0f);
	glVertex3f(-0.75f, 0.93f, 0.0f);
	glVertex3f(-0.5f, 0.83f, 0.0f);
	glVertex3f(0.4f, 0.92f, 0.0f);
	glVertex3f(0.7f, 0.96f, 0.0f);
	glVertex3f(0.9f, 0.88f, 0.0f);
	glVertex3f(-0.92f, 0.71f, 0.0f);
	glVertex3f(-0.821f, 0.83f, 0.0f);
	glVertex3f(-0.951f, 0.934f, 0.0f);
	glVertex3f(0.73f, 0.92f, 0.0f);
	glVertex3f(0.44f, 0.94f, 0.0f);
	glVertex3f(0.74f, 0.962f, 0.0f);
	glVertex3f(0.914f, 0.81f, 0.0f);
	glVertex3f(0.73f, 0.62f, 0.0f);
	glVertex3f(0.84f, 0.74f, 0.0f);
	glVertex3f(0.64f, 0.862f, 0.0f);
	glVertex3f(-0.514f, 0.981f, 0.0f);
	glVertex3f(-0.33f, 0.962f, 0.0f);
	glVertex3f(-0.24f, 0.974f, 0.0f);
	glVertex3f(-0.44f, 0.862f, 0.0f);
	glVertex3f(-0.314f, 0.951f, 0.0f);
	glVertex3f(-0.214f, 0.921f, 0.0f);
	glVertex3f(-0.13f, 0.962f, 0.0f);
	glVertex3f(-0.24f, 0.974f, 0.0f);
	glVertex3f(-0.14f, 0.992f, 0.0f);
	glVertex3f(0.0f, 0.981f, 0.0f);
	glEnd();

	
	glBegin(GL_QUADS); // Buildings on background

// building 1 from left
	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-1.0f, 0.55f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-1.0f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.9f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.9f, 0.55f, 0.0f);

// building 2 from left
	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.9f, 0.6f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.9f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.85f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.85f, 0.6f, 0.0f);

	// street light from left
	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.81f, 0.55f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.81f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.815f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.815f, 0.55f, 0.0f);

// street light horizontal bar
	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.82f, 0.55f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.82f, 0.54f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.81f, 0.54f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.81f, 0.55f, 0.0f);
	
	// building 3 from left
	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.8f, 0.6f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.8f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.75f, 0.5f, 0.0f);

	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(-0.75f, 0.6f, 0.0f);

// tree stem from left
	//glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	//glVertex3f(0.83f, 0.55f, 0.0f);

	//glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	//glVertex3f(0.82f, 0.5f, 0.0f);

	//glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	//glVertex3f(0.84f, 0.5f, 0.0f);

	//glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	//glVertex3f(0.83f, 0.55f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	// street light color from left
	if (vertexCircle[1] >= 0.45f)
	{
		streetLightColor[0] = sunriseColor[0];
		streetLightColor[1] = sunriseColor[1];
		streetLightColor[2] = sunriseColor[2];
	}
	else
	{
		streetLightColor[0] = 1.0f;
		streetLightColor[1] = 1.0f;
		streetLightColor[2] = 0.0f;
	}
	glColor3f(streetLightColor[0], streetLightColor[1], streetLightColor[2]);
	glVertex3f(-0.82f, 0.55f, 0.0f);

	glColor3f(streetLightColor[0], streetLightColor[1], streetLightColor[2]);
	glVertex3f(-0.85f, 0.5f, 0.0f);

	glColor3f(streetLightColor[0], streetLightColor[1], streetLightColor[2]);
	glVertex3f(-0.82f, 0.5f, 0.0f);
	glEnd();
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
	case ' ':
	{
		if(isKeyPress == false)
		{
			isKeyPress = true;
		}
		else
		{
			isKeyPress = false;
		}
	}
	break;
	case 'S':
	case 's':
	{
		if (sunrise == false)
		{
			sunrise = true;
		}
		else
		{
			sunrise = false;
		}
	}
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

