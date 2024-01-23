//// header file inclusion
#include <windows.h> // windows header files
#include <stdio.h>   // for file IO
#include <stdlib.h>  // for exit
#define _USE_MATH_DEFINES
#include <math.h>

#include<vector>
// openGL headers
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SGK_Demo.h"

// Below is the code related to Project

int timer = 0; // use as iterative variable for accessing BazierPoints elements
bool bEnableBazierCamera = false;

std::vector<float> BazierPoints;

float controlPoints[4][3] = {
	{3.5f,0.2f,-3.0f},
	{2.2f,0.8f,1.0f},
	{-0.1f,0.2f,1.0f},
	{-0.1f,0.2f,-0.2f}
};

// for camera
GLfloat gfHorizontal = 0.0f;
GLfloat gfVertical = 0.0f;
GLfloat gfDepth = 3.0f;

// for camera
GLfloat gfCameraHorizontal = 0.0f;
GLfloat gfCameraVertical = 0.0f;
GLfloat gfCameraDepth = 0.0f;

// struct declaration

struct MyControlPoints
{
	float xAxis;
	float yAxis;
	float zAxis;
};

typedef struct MyControlPoints ControlPoints;

struct MYCOLOR
{
	float fRed;
	float fGreen;
	float fBlue;
};

typedef struct MYCOLOR myColor;

myColor skinColor = { 241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f };

BOOL loadGLTexture(GLuint *, TCHAR[]);
GLuint texture_Screen = 0;
GLuint texture_CityBuildings = 0;
GLuint texture_Road = 0;

extern FILE *gpFILE;

#pragma region MODEL

GLfloat gfRightShoulderAngle = 340.0f;
GLfloat gfRightElbowAngle = 360.0f;
GLfloat gfRightWristAngle = 0.0f;

GLfloat gfRightFingerAngle = 360.0f;

GLfloat gfLeftShoulderAngle = 340.0f;
GLfloat gfLeftElbowAngle = 360.0f;
GLfloat gfLeftWristAngle = 0.0f;

GLfloat gfLeftFingerAngle = 0.0f;

GLfloat gfRightLegAngle = 350.0f;
GLfloat gfRightKneeAngle = 0.0f;
GLfloat gfRightAnkleAngle = 360.0f;

GLfloat gfLeftLegAngle = 350.0f;
GLfloat gfLeftKneeAngle = 0.0f;
GLfloat gfLeftAnkleAngle = 360.0f;

GLfloat gfWaistAngle = 0.0f;

#pragma endregion

BOOL gbWalkAnimation = FALSE;
BOOL gbStartWalkAnimation = FALSE;


#pragma region
BOOL gbLight = FALSE;

GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 0.5f,0.2f,0.7f,1.0f };
//GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f,1.0f };
GLfloat materialShininess[] = { 128.0f };

#pragma endregion
// variable declaration


// for gluSphere
GLUquadric *quadric;

BOOL switchCameraToMovement = FALSE;


// global function declaration
//!
//! 1st param :- x coordinate of right top
//! 2nd param :- y coordinate of right top
//! 3rd param :- z coordinate of right top
//! 4th param :- depth 
//! 5th param :- width of figure
//! 6th param :- height of figure
//! 7th param :- struct MYCOLOR (typedef myColor)
//! 

void SGK_drawBox(float, float, float, float, float, float, myColor);


int initializeScene(void)
{
	BOOL bResult;

	//// create texture
	//bResult = loadGLTexture(&texture_Screen, MAKEINTRESOURCE(MY_MOBILE_SCREEN));
	//if (bResult == FALSE)
	//{
	//	fprintf(gpFILE, "load of screen texture is Failed.\n");
	//	return(-6);
	//}

	//bResult = loadGLTexture(&texture_CityBuildings, MAKEINTRESOURCE(CITY_BUILDINGS));
	//if (bResult == FALSE)
	//{
	//	fprintf(gpFILE, "load of screen texture is Failed.\n");
	//	return(-7);
	//}

	//bResult = loadGLTexture(&texture_Road, MAKEINTRESOURCE(NIGHT_ROAD));
	//if (bResult == FALSE)
	//{
	//	fprintf(gpFILE, "load of screen texture is Failed.\n");
	//	return(-9);
	//}

	// enable texture
	glEnable(GL_TEXTURE_2D);

	quadric = gluNewQuadric();

	// light related initialization
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

	// by default GL_LIGHT0 is enable but we still use this because good programming practice to understand it's enalble(if we don't use this then when we enalbe lighting after pressing 'L' key than we see sphere is invisible becuase of state machine we are setting properties of light after light is enalbe that's why it is not consider so "always use this")
	glEnable(GL_LIGHT0);


	for (float t = 0.0f; t <= 1.0f; t = t + 0.002f)
	{
		float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
		float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
		float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

		BazierPoints.push_back(x);
		BazierPoints.push_back(y);
		BazierPoints.push_back(z);
	}


	return(0);
}

void displayScene(void)
{
	//function declaration
	// 1st param (float) center x Coordinate of body
	// 2nd param (float) center y coordinate of body
	// 3rd param (float) center z coordinate of body
	void drawBody(float, float, float);

	// code
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLoadIdentity();

	if (gbLight == FALSE)
	{
		glDisable(GL_LIGHTING);		
	}
	else
	{
		glEnable(GL_LIGHTING);
	}

	if ((timer + 3) + 4 < BazierPoints.size())
	{
		timer = timer + 3;
	}
	else
	{
		timer = 0;
		//bEnableBazierCamera = false;
	}


	if (bEnableBazierCamera)
	{
		gluLookAt(gfHorizontal, gfVertical, gfDepth, gfCameraHorizontal, gfCameraVertical, gfCameraDepth, 0.0f, 1.0f, 0.0f);
	}
	else
	{
		gluLookAt(BazierPoints[timer], BazierPoints[timer + 1], BazierPoints[timer + 2], BazierPoints[(timer + 3)], BazierPoints[(timer + 3) + 1], BazierPoints[(timer + 3) + 2], 0.0f, 1.0f, 0.0f);
	}


	if (switchCameraToMovement)
	{
		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(gfCameraHorizontal, gfCameraVertical, gfCameraDepth);
		glEnd();
		glPointSize(1.0f);
	}

	drawBody(0.0f, 0.0f, 0.0f);
}
void drawBody(float xAxis, float yAxis, float zAxis)
{
	// 1st param x coordinate of center of face
	// 2nd param y coordinate of center of face
	// 3rd param z coordinate of center of face
	void drawFace(float, float, float);

	// 1st param x coordinate of top of trunk
	// 2nd param y coordinate of top of trunk
	// 3rd param z coordinate of top of trunk
	void drawTrunk(float, float, float);

	

	

	drawFace(0.0f, 0.0f, 0.0f);

	drawTrunk(0.0f, -0.5f, 0.0f);
}

void updateScene(void)
{
	if (gbStartWalkAnimation)
	{
		if (gbWalkAnimation == FALSE)
		{
			// for right hand
			if (gfRightShoulderAngle <= 360.0f)
			{
				gfRightShoulderAngle = gfRightShoulderAngle + 1.0f;

				if (gfRightElbowAngle <= 360.0f)
				{
					gfRightElbowAngle = gfRightElbowAngle + 1.0f;
				}
			}

			// for left hand
			if (gfLeftShoulderAngle >= 250.0f)
			{
				gfLeftShoulderAngle = gfLeftShoulderAngle - 1.0f;
				if (gfLeftElbowAngle >= 350.0f)
				{
					gfLeftElbowAngle = gfLeftElbowAngle - 1.0f;
				}
			}

			// for left leg
			if (gfLeftLegAngle >= 330.0f)
			{
				gfLeftLegAngle = gfLeftLegAngle - 1.0f;
				if (gfLeftKneeAngle >= 0.0f)
				{
					gfLeftKneeAngle = gfLeftKneeAngle - 1.0f;
				}
			}

			// for right leg
			if (gfRightLegAngle <= 360.0f)
			{
				gfRightLegAngle = gfRightLegAngle + 1.0f;

				if (gfRightKneeAngle <= 70.0f)
				{
					gfRightKneeAngle = gfRightKneeAngle + 1.0f;
				}
			}
			else
			{
				gbWalkAnimation = TRUE;
			}
		}
		else
		{
			// for right hand
			if (gfRightShoulderAngle >= 250.0f)
			{
				gfRightShoulderAngle = gfRightShoulderAngle - 1.0f;
				if (gfRightElbowAngle >= 350.0f)
				{
					gfRightElbowAngle = gfRightElbowAngle - 1.0f;
				}
			}

			// for left hand
			if (gfLeftShoulderAngle <= 360.0f)
			{
				gfLeftShoulderAngle = gfLeftShoulderAngle + 1.0f;
				if (gfLeftElbowAngle <= 360.0f)
				{
					gfLeftElbowAngle = gfLeftElbowAngle + 1.0f;
				}
			}

			// for left leg
			if (gfLeftLegAngle <= 360.0f)
			{
				gfLeftLegAngle = gfLeftLegAngle + 1.0f;
				if (gfLeftKneeAngle <= 70.0f)
				{
					gfLeftKneeAngle = gfLeftKneeAngle + 1.0f;
				}
			}

			// for right leg
			if (gfRightLegAngle >= 330.0f)
			{
				gfRightLegAngle = gfRightLegAngle - 1.0f;
				if (gfRightKneeAngle >= 0.0f)
				{
					gfRightKneeAngle = gfRightKneeAngle - 1.0f;
				}
			}
			else
			{
				gbWalkAnimation = FALSE;
			}
		}
	}

}

void uninitializeScene(void)
{
	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}

	//if (texture_Screen)
	//{
	//	glDeleteTextures(1, &texture_Screen);
	//	texture_Screen = 0;
	//}

}

BOOL loadGLTexture(GLuint *texture, TCHAR imageResourceID[])
{
	//local variable declaration
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap == NULL)
	{
		fprintf(gpFILE, "LoadImage() Failed.\n");
		return(FALSE);
	}

	// get Image Data
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	// create openGL texture
	glGenTextures(1, texture);

	// bind to the generated texture
	glBindTexture(GL_TEXTURE_2D, *texture);

	// decide image pixel alignment and unpacking 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// set texture parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// create multiple mipmap images
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

	// unBind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// delete bitmap object
	DeleteObject(hBitmap);

	hBitmap = NULL;

	return(TRUE);
}


void drawFace(float xAxis, float yAxis, float zAxis)
{
	// 1st param x coordinate of center of nose
	// 2nd param y coordinate of center of nose
	// 3rd param z coordinate of center of nose
	void drawNoseChinCheekForehead(float, float, float);

	////draw main head part(sphere for round head)
	//glPushMatrix();
	//glTranslatef(0.0f, 0.1f, -0.43f);
	//glPushMatrix();
	//glScalef(0.9f, 1.0f, 0.7f);
	//glColor3f(1.0f, 175.0f/255.0f, 100.0f/255.0f);
	//gluSphere(quadric, 0.4f, 30, 30);
	//glPopMatrix();
	//glPopMatrix();

	//glPushMatrix();
	//glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glTranslatef(0.0f, -0.4f, -0.27f);
	//glPushMatrix();
	//glScalef(2.0f,2.0f,2.8f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//gluCylinder(quadric, 0.1f, 0.1f, 0.2f, 30, 30);
	////glColor3f(1.0f, 1.0f, 1.0f);
	////gluDisk(quadric, 0.0f, 0.1f, 30, 30);
	//glPopMatrix();
	//glPopMatrix();

	//glPushMatrix();
	//glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glTranslatef(0.0f, -0.4f, 0.29f);
	//glPushMatrix();
	//glScalef(2.0f, 2.0f, 2.8f);
	////glColor3f(1.0f, 0.0f, 0.0f);
	////gluCylinder(quadric, 0.1f, 0.1f, 0.2f, 30, 30);
	//glColor3f(1.0f, 1.0f, 1.0f);
	//gluDisk(quadric, 0.0f, 0.1f, 30, 30);
	//glPopMatrix();
	//glPopMatrix();

	//// draw neck
	//glPushMatrix();
	//glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // by default inside glu Library x axis is 'Z' axis and y axis is 'Y' axis
	//glPushMatrix();
	//glTranslatef(0.0f, -0.15f, 0.3f); // after rotate x = 'X' Axis, y = 'Z' Axis, z = 'Y' Axis
	//glScalef(1.2f, 1.0f, 1.0f);
	//glColor3f(1.0f,1.0f,1.0f);
	//gluCylinder(quadric, 0.1f, 0.1f, 0.2f, 30, 30);
	//glPopMatrix();
	//glPopMatrix();

	//// draw front face part
	//glPushMatrix();
	//glTranslatef(0.0f, -0.1f, 0.1f);
	//drawNoseChinCheekForehead(xAxis, yAxis, zAxis);
	//glPopMatrix();


}

void drawNoseChinCheekForehead(float xAxis, float yAxis, float zAxis)
{
	// draw front face part
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, 0.225f);
		glBegin(GL_QUADS);

		// center part of nose
		glColor3f(1.0f, 175.0f / 255.0f, 100.0f / 255.0f);
		glVertex3f(xAxis + 0.02f, yAxis + 0.2f, zAxis - 0.12f);

		glVertex3f(xAxis - 0.02f, yAxis + 0.2f, zAxis - 0.12f);
		glColor3f(0.8f, 155.0f / 255.0f, 80.0f / 255.0f);
		glVertex3f(xAxis - 0.05f, yAxis - 0.05f, zAxis - 0.08f);
		glVertex3f(xAxis + 0.05f, yAxis - 0.05f, zAxis - 0.08f);

		// nostril part of nose
		glColor3f(0.8f, 155.0f / 255.0f, 80.0f / 255.0f);
		glVertex3f(xAxis + 0.05f, yAxis - 0.05f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.05f, yAxis - 0.05f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.05f, yAxis - 0.05f, zAxis - 0.08f);
		glVertex3f(xAxis + 0.05f, yAxis - 0.05f, zAxis - 0.08f);


		// left side cheek part
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(xAxis - 0.175f, yAxis + 0.22f, zAxis - 0.14f);
		glVertex3f(xAxis - 0.02f, yAxis + 0.2f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.05f, yAxis - 0.05f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.175f, yAxis - 0.08f, zAxis - 0.14f);

		// left deep cheek part
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(xAxis - 0.23f, yAxis + 0.2f, zAxis - 0.3f);
		glVertex3f(xAxis - 0.175f, yAxis + 0.22f, zAxis - 0.14f);
		glVertex3f(xAxis - 0.175f, yAxis - 0.08f, zAxis - 0.14f);
		glVertex3f(xAxis - 0.15f, yAxis - 0.15f, zAxis - 0.3f);

		// right side cheek part
		glColor3f(1.0f, 0.5f, 1.0f);
		glVertex3f(xAxis + 0.175f, yAxis + 0.22f, zAxis - 0.14f);
		glVertex3f(xAxis + 0.02f, yAxis + 0.2f, zAxis - 0.12f);
		glVertex3f(xAxis + 0.05f, yAxis - 0.05f, zAxis - 0.12f);
		glVertex3f(xAxis + 0.175f, yAxis - 0.08f, zAxis - 0.14f);

		// right deep cheek part
		glColor3f(1.0f, 0.5f, 1.0f);
		glVertex3f(xAxis + 0.23f, yAxis + 0.2f, zAxis - 0.3f);
		glVertex3f(xAxis + 0.175f, yAxis + 0.22f, zAxis - 0.14f);
		glVertex3f(xAxis + 0.175f, yAxis - 0.08f, zAxis - 0.14f);
		glVertex3f(xAxis + 0.15f, yAxis - 0.15f, zAxis - 0.3f);

		// part between nose top and forehead
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(xAxis + 0.175f, yAxis + 0.22f, zAxis - 0.14f);
		glVertex3f(xAxis + 0.05f, yAxis + 0.2f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.05f, yAxis + 0.2f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.175f, yAxis + 0.22f, zAxis - 0.14f);

		// part between nose and lips
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(xAxis + 0.175f, yAxis - 0.08f, zAxis - 0.14f);
		glVertex3f(xAxis + 0.05f, yAxis - 0.05f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.05f, yAxis - 0.05f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.175f, yAxis - 0.08f, zAxis - 0.14f);

		// chin part
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(xAxis + 0.175f, yAxis - 0.08f, zAxis - 0.14f);
		glVertex3f(xAxis - 0.175f, yAxis - 0.08f, zAxis - 0.14f);
		glVertex3f(xAxis - 0.1f, yAxis - 0.2f, zAxis - 0.15f);
		glVertex3f(xAxis + 0.1f, yAxis - 0.2f, zAxis - 0.15f);

		// bottom part of chin
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(xAxis + 0.15f, yAxis - 0.15f, zAxis - 0.3f);
		glVertex3f(xAxis - 0.15f, yAxis - 0.15f, zAxis - 0.3f);
		glVertex3f(xAxis - 0.1f, yAxis - 0.2f, zAxis - 0.15f);
		glVertex3f(xAxis + 0.1f, yAxis - 0.2f, zAxis - 0.15f);


		//forehead part
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(xAxis + 0.15f, yAxis + 0.35f, zAxis - 0.16f);
		glVertex3f(xAxis - 0.15f, yAxis + 0.35f, zAxis - 0.16f);
		glVertex3f(xAxis - 0.175f, yAxis + 0.22f, zAxis - 0.14f);
		glVertex3f(xAxis + 0.175f, yAxis + 0.22f, zAxis - 0.14f);

		// top part of forehead 
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(xAxis + 0.15f, yAxis + 0.35f, zAxis - 0.16f);
		glVertex3f(xAxis - 0.15f, yAxis + 0.35f, zAxis - 0.16f);
		glVertex3f(xAxis - 0.08f, yAxis + 0.425f, zAxis - 0.35f);
		glVertex3f(xAxis + 0.08f, yAxis + 0.425f, zAxis - 0.35f);

		glEnd();


		glBegin(GL_TRIANGLES);

		// right part of nose
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(xAxis + 0.02f, yAxis + 0.2f, zAxis - 0.12f);
		glVertex3f(xAxis + 0.05f, yAxis - 0.05f, zAxis - 0.08f);
		glVertex3f(xAxis + 0.05f, yAxis - 0.05f, zAxis - 0.12f);

		// left part of nose
		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(xAxis - 0.02f, yAxis + 0.2f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.05f, yAxis - 0.05f, zAxis - 0.12f);
		glVertex3f(xAxis - 0.05f, yAxis - 0.05f, zAxis - 0.08f);

		// right side deep part of chin
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(xAxis + 0.15f, yAxis - 0.15f, zAxis - 0.3f);
		glVertex3f(xAxis + 0.175f, yAxis - 0.08f, zAxis - 0.14f);
		glVertex3f(xAxis + 0.1f, yAxis - 0.2f, zAxis - 0.15f);


		// left side deep part of chin
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(xAxis - 0.15f, yAxis - 0.15f, zAxis - 0.3f);
		glVertex3f(xAxis - 0.175f, yAxis - 0.08f, zAxis - 0.14f);
		glVertex3f(xAxis - 0.1f, yAxis - 0.2f, zAxis - 0.15f);

		//right side forehead part
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(xAxis + 0.15f, yAxis + 0.35f, zAxis - 0.16f);
		glVertex3f(xAxis + 0.175f, yAxis + 0.22f, zAxis - 0.14f);
		glVertex3f(xAxis + 0.23f, yAxis + 0.2f, zAxis - 0.3f);

		//left side forehead part
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(xAxis - 0.15f, yAxis + 0.35f, zAxis - 0.16f);
		glVertex3f(xAxis - 0.175f, yAxis + 0.22f, zAxis - 0.14f);
		glVertex3f(xAxis - 0.23f, yAxis + 0.2f, zAxis - 0.3f);

		// left side of top part of forehead
		glVertex3f(xAxis - 0.08f, yAxis + 0.425f, zAxis - 0.35f);
		glVertex3f(xAxis - 0.15f, yAxis + 0.35f, zAxis - 0.16f);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(xAxis - 0.23f, yAxis + 0.2f, zAxis - 0.3f);

		// right side of top part of forehead
		glVertex3f(xAxis + 0.08f, yAxis + 0.425f, zAxis - 0.35f);
		glVertex3f(xAxis + 0.15f, yAxis + 0.35f, zAxis - 0.16f);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(xAxis + 0.23f, yAxis + 0.2f, zAxis - 0.3f);

		glEnd();
	}
	glPopMatrix();

	// face back side sphere
	glPushMatrix();
	{
		glTranslatef(0.0f, 0.095f, -0.15f);
		glScalef(0.6f, 0.82f, 0.6f);
		gluSphere(quadric, 0.4f, 25, 25);
	}
	glPopMatrix();

	// draw neck
	glPushMatrix();
	{
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // by default inside glu Library x axis is 'Z' axis and y axis is 'Y' axis
		glTranslatef(0.0f, -0.15f, 0.2f); // after rotate x = 'X' Axis, y = 'Z' Axis, z = 'Y' Axis
		glScalef(1.2f, 1.0f, 1.0f);
		glColor3f(1.0f, 1.0f, 1.0f);

		gluCylinder(quadric, 0.1f, 0.1f, 0.2f, 30, 1);

	}
	glPopMatrix();
}

void drawTrunk(float xAxis, float yAxis, float zAxis)
{
	// function declaration
	
	// 1st param x coordinate of top of right hand
	// 2nd param y coordinate of top of right hand
	// 3rd param z coordinate of top of right hand
	void drawRightHand(float, float, float);

	// 1st param x coordinate of top of left hand
	// 2nd param y coordinate of top of left hand
	// 3rd param z coordinate of top of left hand
	void drawLeftHand(float, float, float);

	// 1st param x coordinate of top of legs 
	// 2nd param y coordinate of top of legs 
	// 3rd param z coordinate of top of legs 
	void drawLegs(float, float, float);

	// 1st param x coordinate of center of nose
	// 2nd param y coordinate of center of nose
	// 3rd param z coordinate of center of nose
	void drawNoseChinCheekForehead(float, float, float);


	// code
	
	glPushMatrix();
	{
		glTranslatef(xAxis, yAxis-1.4f, zAxis-0.15);
		glPushMatrix();
		{
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

			glPushMatrix(); // Waist base sphere
			{
				glScalef(1.2f, 0.6f, 0.4f);
				gluSphere(quadric, 0.4f, 20, 20);
			}
			glPopMatrix();

			
			glRotatef(gfWaistAngle, 1.0f, 0.0f, 0.0f); // Waist rotation
			
			glPushMatrix(); // trunk 
			{	
				glScalef(1.2f, 0.6f, 1.0f);
				gluCylinder(quadric, 0.4f, 0.5f, 1.4f, 20, 1); 
			}
			glPopMatrix();

			glPushMatrix(); // top trunk part(sholder) sphere
			{
				glTranslatef(0.0f, 0.0f, zAxis + 1.4f);
				glScalef(1.2f, 0.6f, 0.4f);
				gluSphere(quadric, 0.5f, 20, 20);
			}
			glPopMatrix();
			
			// for face
			glPushMatrix();
			{	
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, yAxis + 2.4f, zAxis + 0.15f);
				
				drawNoseChinCheekForehead(0.0f, 0.0f, 0.0f);
			}
			glPopMatrix();
		}
		glPopMatrix();

		// for hands
		glPushMatrix(); 
		{
			glRotatef(gfWaistAngle, 1.0f, 0.0f, 0.0f); // waist rotation required to move upper body part accordingly

			glPushMatrix();
			{
				glTranslatef(xAxis + 0.6f, yAxis + 1.8f, zAxis);
				drawRightHand(0.0f, 0.0f, 0.0f);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(xAxis - 0.6f, yAxis + 1.8f, zAxis);
				drawLeftHand(0.0f, 0.0f, 0.0f);
			}
			glPopMatrix();
		}
		glPopMatrix();

	}
	glPopMatrix();

	

	drawLegs(xAxis, yAxis - 1.55f, zAxis - 0.15f);

}

void drawRightHand(float xAxis, float yAxis, float zAxis)
{
	// function declaration
	void drawPalmRightHand(float xAxis, float yAxis, float zAxis, float depth, float width, float height, myColor color);

	//variable declaration
	myColor palmColor = { 1.0f,1.0f,0.0f };
	// code
	glPushMatrix();
	{
		glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // adjustment made for walk animation 
		glPushMatrix();
		{
			glTranslatef(xAxis, yAxis, zAxis);
			gluSphere(quadric, 0.15f, 30, 30); // sholder start sphere

			glRotatef(gfRightShoulderAngle, 1.0f, 0.0f, 0.0f); // sholder angle handle on 'R'/'r' key press

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.15f, 0.1f, 0.7f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -0.7f, 0.0f);
			gluSphere(quadric, 0.1f, 30, 30); // elbow start sphere
			glRotatef(gfRightElbowAngle, 1.0f, 0.0f, 0.0f); // elbow angle handle on 'T'/'t' key press

			glPushMatrix();
			{
				//glColor3f(1.0f, 0.0f, 0.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.1f, 0.07f, 0.7f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -0.7f, 0.0f); // wrist start here

			glPushMatrix(); // palm start here
			{

				//glColor3f(1.0f, 1.0f, 0.0f);
				//glScalef(0.8f, 1.5f, 1.2f);
				gluSphere(quadric, 0.07f, 15, 15); // palm start sphere
				drawPalmRightHand(0.05f, 0.0f, 0.09f, 0.18f, 0.08f, 0.15f, skinColor); // palm

			}
			glPopMatrix();

			glTranslatef(0.0f, -0.04f, 0.0f);
			glPushMatrix(); // finger start here
			{
				// first finger 
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.12f, 0.07f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 1 start sphere base part
					glRotatef(gfRightFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 1 start sphere middle part
					glRotatef(gfRightFingerAngle - 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part
					glRotatef(gfRightFingerAngle - 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.055f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.055f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// second finger
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.12f, 0.03f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 2 start sphere base part
					glRotatef(gfRightFingerAngle - 0.0, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.06f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.06f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 2 start sphere middle part
					glRotatef(gfRightFingerAngle - 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.06f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.06f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 2 start sphere tip/end part
					glRotatef(gfRightFingerAngle - 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.06f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.06f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Third finger
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.12f, -0.01f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 3 start sphere base part
					glRotatef(gfRightFingerAngle - 0.0, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 3 start sphere middle part
					glRotatef(gfRightFingerAngle - 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 3 start sphere tip/end part
					glRotatef(gfRightFingerAngle - 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.055f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.055f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Forth finger
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.12f, -0.05f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 4 start sphere base part
					glRotatef(gfRightFingerAngle - 0.0, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 4 start sphere middle part
					glRotatef(gfRightFingerAngle - 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 4 start sphere tip/end part
					glRotatef(gfRightFingerAngle - 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.04f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.04f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Thumb
				glPushMatrix();
				{
					glTranslatef(0.01f, -0.08f, 0.08f);
					glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
					gluSphere(quadric, 0.03f, 15, 15); // Thumb start sphere base part
					glRotatef(gfRightFingerAngle - 5.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.03f, 0.024f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					//glRotatef(30.0f, 1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.024f, 15, 15); // Thumb start sphere middle part
					glRotatef(gfRightFingerAngle - 12.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.024f, 0.018f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					//glRotatef(15.0f, 1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.018f, 15, 15); // Thumb start sphere tip/end part
					glRotatef(gfRightFingerAngle - 16.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.018f, 0.014f, 0.04f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.04f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.014f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLeftHand(float xAxis, float yAxis, float zAxis)
{
	// function declaration
	void drawPalmLeftHand(float xAxis, float yAxis, float zAxis, float depth, float width, float height, myColor color);

	//variable declaration
	myColor palmColor = { 1.0f,1.0f,0.0f };

	// code
	glPushMatrix();
	{
		glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // adjustment made for walk animation 
		glPushMatrix();
		{
			glTranslatef(xAxis, yAxis, zAxis);
			gluSphere(quadric, 0.15f, 30, 30); // sholder start sphere

			glRotatef(gfLeftShoulderAngle, 1.0f, 0.0f, 0.0f); // sholder angle handle on 'R'/'r' key press

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.15f, 0.1f, 0.7f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -0.7f, 0.0f);
			gluSphere(quadric, 0.1f, 30, 30); // elbow start sphere
			glRotatef(gfLeftElbowAngle, 1.0f, 0.0f, 0.0f); // elbow angle handle on 'T'/'t' key press

			glPushMatrix();
			{
				//glColor3f(1.0f, 0.0f, 0.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.1f, 0.07f, 0.7f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -0.7f, 0.0f); // wrist start here

			glPushMatrix(); // palm start here
			{

				//glColor3f(1.0f, 1.0f, 0.0f);
				//glScalef(0.8f, 1.5f, 1.2f);
				gluSphere(quadric, 0.07f, 15, 15); // palm start sphere
				drawPalmLeftHand(0.05f, 0.0f, 0.09f, 0.18f, 0.08f, 0.15f, skinColor); // palm

			}
			glPopMatrix();

			glTranslatef(0.0f, -0.04f, 0.0f);
			glPushMatrix(); // finger start here
			{
				// first finger 
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.12f, 0.07f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 1 start sphere base part
					glRotatef(gfLeftFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 1 start sphere middle part
					glRotatef(gfLeftFingerAngle + 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part
					glRotatef(gfLeftFingerAngle + 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.055f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.055f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// second finger
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.12f, 0.03f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 2 start sphere base part
					glRotatef(gfLeftFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.06f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.06f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 2 start sphere middle part
					glRotatef(gfLeftFingerAngle + 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.06f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.06f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 2 start sphere tip/end part
					glRotatef(gfLeftFingerAngle + 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.06f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.06f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Third finger
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.12f, -0.01f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 3 start sphere base part
					glRotatef(gfLeftFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 3 start sphere middle part
					glRotatef(gfLeftFingerAngle + 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 3 start sphere tip/end part
					glRotatef(gfLeftFingerAngle + 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.055f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.055f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Forth finger
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.12f, -0.05f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 4 start sphere base part
					glRotatef(gfLeftFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 4 start sphere middle part
					glRotatef(gfLeftFingerAngle + 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 4 start sphere tip/end part
					glRotatef(gfLeftFingerAngle + 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.04f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.04f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Thumb
				glPushMatrix();
				{
					glTranslatef(0.03f, -0.08f, 0.08f);
					glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
					gluSphere(quadric, 0.03f, 15, 15); // Thumb start sphere base part
					glRotatef(gfLeftFingerAngle + 5.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.03f, 0.024f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					//glRotatef(30.0f, 1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.024f, 15, 15); // Thumb start sphere middle part
					glRotatef(gfLeftFingerAngle + 12.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.024f, 0.018f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					//glRotatef(15.0f, 1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.018f, 15, 15); // Thumb start sphere tip/end part
					glRotatef(gfLeftFingerAngle + 16.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.018f, 0.014f, 0.04f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.04f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.014f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawPalmRightHand(float xAxis, float yAxis, float zAxis, float depth, float width, float height, myColor color)
{
	glBegin(GL_QUADS);
	//front // thumb start face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);
	glVertex3f(xAxis, yAxis, zAxis-0.02f);//right top
	glVertex3f(xAxis - width, yAxis, zAxis-0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);
	glVertex3f((xAxis - width) + 0.02f, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, zAxis);//right bottom

	//right // palm back side face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis, yAxis, (zAxis - depth)+0.02f);//right top
	glVertex3f(xAxis, yAxis, zAxis-0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, (zAxis - depth)+0.02f);//right bottom

	//back // little finger down side face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis - width, yAxis, (zAxis - depth)+0.02f);//right top
	glVertex3f(xAxis, yAxis, (zAxis - depth)+0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis, yAxis - height, (zAxis - depth)+0.02f);//left bottom
	glVertex3f((xAxis - width) + 0.02f,yAxis - height, (zAxis - depth)+0.02f);//right bottom

	//left // palm face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis - width, yAxis, zAxis-0.02f);//right top
	glVertex3f(xAxis - width, yAxis, (zAxis - depth)+0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f((xAxis - width) + 0.02f, yAxis - height, (zAxis - depth)+0.02f);//left bottom
	glVertex3f((xAxis - width) + 0.02f, yAxis - height, zAxis);//right bottom

	//top // wrist side face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis, yAxis, (zAxis - depth) + 0.02f);//right top
	glVertex3f(xAxis - width, yAxis, (zAxis - depth) + 0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis - width, yAxis, zAxis-0.02f);//left bottom
	glVertex3f(xAxis, yAxis, zAxis-0.02f);//right bottom

	//bottom // finger start side face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis, yAxis - height, (zAxis - depth)+0.02f);//right top
	glVertex3f((xAxis - width)+0.02f, yAxis - height, (zAxis - depth)+0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f((xAxis - width) + 0.02f, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, zAxis);//right bottom

	glEnd();
}

void drawPalmLeftHand(float xAxis, float yAxis, float zAxis, float depth, float width, float height, myColor color)
{
	glBegin(GL_QUADS);
	//front // thumb start face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);
	
	glVertex3f(xAxis, yAxis, zAxis - 0.02f);//right top
	glVertex3f(xAxis - width, yAxis, zAxis - 0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);
	glVertex3f((xAxis - width) + 0.025f, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, zAxis);//right bottom

	//right // palm back side face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis, yAxis, (zAxis - depth) + 0.02f);//right top
	glVertex3f(xAxis, yAxis, zAxis - 0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, (zAxis - depth) + 0.02f);//right bottom

	//back // little finger down side face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis - width, yAxis, (zAxis - depth) + 0.02f);//right top
	glVertex3f(xAxis, yAxis, (zAxis - depth) + 0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis, yAxis - height, (zAxis - depth) + 0.02f);//left bottom
	glVertex3f((xAxis - width) + 0.025f, yAxis - height, (zAxis - depth) + 0.02f);//right bottom

	//left // palm face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis - width, yAxis, zAxis - 0.02f);//right top
	glVertex3f(xAxis - width, yAxis, (zAxis - depth) + 0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f((xAxis - width) + 0.025f, yAxis - height, (zAxis - depth) + 0.02f);//left bottom
	glVertex3f((xAxis - width) + 0.025f, yAxis - height, zAxis);//right bottom

	//top // wrist side face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis, yAxis, (zAxis - depth) + 0.02f);//right top
	glVertex3f(xAxis - width, yAxis, (zAxis - depth) + 0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis - width, yAxis, zAxis - 0.02f);//left bottom
	glVertex3f(xAxis, yAxis, zAxis - 0.02f);//right bottom

	//bottom // finger start side face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glColor3f(color.fRed, color.fGreen, color.fBlue);

	glVertex3f(xAxis, yAxis - height, (zAxis - depth) + 0.02f);//right top
	glVertex3f((xAxis - width) + 0.025f, yAxis - height, (zAxis - depth) + 0.02f);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f((xAxis - width) + 0.025f, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, zAxis);//right bottom

	glEnd();
}

void drawLegs(float xAxis, float yAxis, float zAxis)
{
	// 1st param x coordinate of top of right legs 
	// 2nd param y coordinate of top of right legs 
	// 3rd param z coordinate of top of right legs 
	void drawRightLeg(float, float, float);
	// 1st param x coordinate of top of left legs 
	// 2nd param y coordinate of top of left legs 
	// 3rd param z coordinate of top of left legs
	void drawLeftLeg(float, float, float);

	glPushMatrix();
	{	
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glTranslatef(xAxis, yAxis + 0.1f, zAxis);
		glPushMatrix();
		{
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glScalef(1.2f, 0.6f, 1.0f);
			glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
			gluCylinder(quadric, 0.4f, 0.38f, 0.2f, 20, 1);
		}
		glPopMatrix();
		glTranslatef(0.0f, -0.2f, 0.0f);
		glScalef(1.2f, 0.52f, 0.5f);
		gluSphere(quadric, 0.38f, 20, 20);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(xAxis + 0.225f, yAxis, zAxis);
		drawRightLeg(0.0f, 0.0f, 0.0f);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(xAxis - 0.225f, yAxis, zAxis);
		drawLeftLeg(0.0f, 0.0f, 0.0f);
	}
	glPopMatrix();
}

void drawRightLeg(float xAxis, float yAxis, float zAxis)
{
	// code
	glPushMatrix();
	{
		glRotatef(10.0f, 1.0f, 0.0f, 0.0f);// adjustment made for walk animation 
		glPushMatrix();
		{
			glTranslatef(xAxis, yAxis - 0.1f, zAxis);
			gluSphere(quadric, 0.22f, 30, 30); // hip start sphere

			glRotatef(gfRightLegAngle, 1.0f, 0.0f, 0.0f); // leg angle handle on 'P'/'p' key press

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.22f, 0.15f, 1.1f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -1.1f, 0.0f);
			gluSphere(quadric, 0.15f, 30, 30); // knee start sphere
			glRotatef(gfRightKneeAngle, 1.0f, 0.0f, 0.0f); // knee angle handle on 'O'/'o' key press

			glPushMatrix();
			{
				//glColor3f(1.0f, 0.0f, 0.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.15f, 0.12f, 1.0f, 30, 1); // shin part
			}
			glPopMatrix();

			glPushMatrix();// ankle start here
			{
				glTranslatef(0.0f, -1.0f, 0.0f); 
				glPushMatrix();
				{
					glScalef(1.0f, 0.6f, 1.0f);
					gluSphere(quadric, 0.12f, 15, 15); // ankle start sphere
				}
				glPopMatrix();

				glRotatef(gfRightAnkleAngle, 1.0f, 0.0f, 0.0f); // not handle yet use if required implemented relative changes in wndproc()

				glPushMatrix(); // ankle start here
				{
					glColor3f(1.0f, 0.0f, 0.0f);
					glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
					glScalef(1.0f, 0.6f, 1.0f);
					gluCylinder(quadric, 0.12f, 0.1f, 0.3f, 10, 1); // foot
					

				}
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, 0.3f);// foot ends(fingers start sphere)
				
				glPushMatrix();
				{
					glPushMatrix();
					{
						glScalef(1.0f, 0.6f, 1.0f);
						gluSphere(quadric, 0.1f, 10, 10); // (fingers start sphere)
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(0.0f, -0.015f, 0.0f);
						glPushMatrix(); // foot thumb
						{
							glTranslatef(-0.0625f, 0.0f, 0.06f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.02f, 0.022f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.022f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot index finger
						{
							glTranslatef(-0.025f, 0.0f, 0.09f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot middle finger
						{
							glTranslatef(0.005f, 0.0f, 0.085f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot forth finger
						{
							glTranslatef(0.035f, 0.0f, 0.075f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.07f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.07f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot little finger
						{
							glTranslatef(0.065f, 0.0f, 0.06f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.06f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.06f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();
					}
					glPopMatrix();

				}
				glPopMatrix();
			}
			glPopMatrix();

			
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLeftLeg(float xAxis, float yAxis, float zAxis)
{
	// code
	glPushMatrix();
	{
		glRotatef(10.0f, 1.0f, 0.0f, 0.0f);// adjustment made for walk animation 
		glPushMatrix();
		{
			glTranslatef(xAxis, yAxis - 0.1f, zAxis);
			gluSphere(quadric, 0.22f, 30, 30); // hip start sphere

			glRotatef(gfLeftLegAngle, 1.0f, 0.0f, 0.0f); // leg angle handle on 'U'/'u' key press

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.22f, 0.15f, 1.1f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -1.1f, 0.0f);
			gluSphere(quadric, 0.15f, 30, 30); // knee start sphere
			glRotatef(gfLeftKneeAngle, 1.0f, 0.0f, 0.0f); // knee angle handle on 'H'/'h' key press

			glPushMatrix();
			{
				//glColor3f(1.0f, 0.0f, 0.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.15f, 0.12f, 1.0f, 30, 1); // shin part
			}
			glPopMatrix();

			glPushMatrix();// ankle start here
			{
				glTranslatef(0.0f, -1.0f, 0.0f);
				glPushMatrix();
				{
					glScalef(1.0f, 0.6f, 1.0f);
					gluSphere(quadric, 0.12f, 15, 15); // ankle start sphere
				}
				glPopMatrix();

				glRotatef(gfLeftAnkleAngle, 1.0f, 0.0f, 0.0f); // not handle yet use if required implemented relative changes in wndproc()

				glPushMatrix(); // ankle start here
				{
					glColor3f(1.0f, 0.0f, 0.0f);
					glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
					glScalef(1.0f, 0.6f, 1.0f);
					gluCylinder(quadric, 0.12f, 0.1f, 0.3f, 10, 1); // foot


				}
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, 0.3f);// foot ends(fingers start sphere)

				glPushMatrix();
				{
					glPushMatrix();
					{
						glScalef(1.0f, 0.6f, 1.0f);
						gluSphere(quadric, 0.1f, 10, 10); // (fingers start sphere)
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(0.0f, -0.015f, 0.0f);
						glPushMatrix(); // foot thumb
						{
							glTranslatef(0.0625f, 0.0f, 0.06f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.02f, 0.022f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.022f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot index finger
						{
							glTranslatef(0.025f, 0.0f, 0.09f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot middle finger
						{
							glTranslatef(-0.005f, 0.0f, 0.085f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot forth finger
						{
							glTranslatef(-0.035f, 0.0f, 0.075f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.07f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.07f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot little finger
						{
							glTranslatef(-0.065f, 0.0f, 0.06f);
							glColor3f(1.0f, 1.0f, 0.0f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.06f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.06f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();
					}
					glPopMatrix();

				}
				glPopMatrix();
			}
			glPopMatrix();


		}
		glPopMatrix();
	}
	glPopMatrix();
}

void SGK_drawBox(float xAxis, float yAxis, float zAxis, float depth, float width, float height, myColor color)
{
	glBegin(GL_QUADS);
	//front
	glColor3f(color.fRed, color.fGreen, color.fBlue);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(xAxis, yAxis, zAxis);//right top
	glVertex3f(xAxis - width, yAxis, zAxis);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);
	glVertex3f(xAxis - width, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, zAxis);//right bottom

	//right
	glColor3f(color.fRed, color.fGreen, color.fBlue);
	//glColor3f(1.0f, 1.0f, 0.0f);

	glVertex3f(xAxis, yAxis, zAxis - depth);//right top
	glVertex3f(xAxis, yAxis, zAxis);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, zAxis - depth);//right bottom

	//back
	glColor3f(color.fRed, color.fGreen, color.fBlue);
	//glColor3f(1.0f, 0.5f, 0.0f);

	glVertex3f(xAxis - width, yAxis, zAxis - depth);//right top
	glVertex3f(xAxis, yAxis, zAxis - depth);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis, yAxis - height, zAxis - depth);//left bottom
	glVertex3f(xAxis - width, yAxis - height, zAxis - depth);//right bottom

	//left
	glColor3f(color.fRed, color.fGreen, color.fBlue);
	//glColor3f(1.0f, 0.0f, 1.0f);

	glVertex3f(xAxis - width, yAxis, zAxis);//right top
	glVertex3f(xAxis - width, yAxis, zAxis - depth);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis - width, yAxis - height, zAxis - depth);//left bottom
	glVertex3f(xAxis - width, yAxis - height, zAxis);//right bottom

	//top
	glColor3f(color.fRed, color.fGreen, color.fBlue);
	//glColor3f(1.0f, 0.0f, 0.5f);

	glVertex3f(xAxis, yAxis, zAxis - depth);//right top
	glVertex3f(xAxis - width, yAxis, zAxis - depth);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis - width, yAxis, zAxis);//left bottom
	glVertex3f(xAxis, yAxis, zAxis);//right bottom

	//bottom
	//glColor3f(color.fRed, color.fGreen, color.fBlue);
	//glColor3f(1.0f, 0.5f, 0.5f);

	glVertex3f(xAxis, yAxis - height, zAxis - depth);//right top
	glVertex3f(xAxis - width, yAxis - height, zAxis - depth);//left top

	glColor3f(color.fRed - 0.2f, color.fGreen - 0.2f, color.fBlue - 0.2f);

	glVertex3f(xAxis - width, yAxis - height, zAxis);//left bottom
	glVertex3f(xAxis, yAxis - height, zAxis);//right bottom

	glEnd();
}


