
#include"Scene2.h"

#pragma region CAMERA

static int bazierPointsIndex = 0; // use as iterative variable for accessing BazierPoints elements

static std::vector<float> BazierPoints;

static BOOL bVector = FALSE;

static float upVector_X = 0.5f;
static float upVector_Y = 1.0f;
static float upVector_Z = 0.0f;

static float controlPoints[4][3] = {
	{-350.0f,7.0f,400.0f},
	{-300.0f,20.0f,-200.0f},
	{270.0f,40.0f,-80.0f},
	{250.0f,40.0f,250.0f},
};

static int bazierPointsIndex_1 = 0; // use as iterative variable for accessing BazierPoints elements

static std::vector<float> BazierPoints_1;

static float controlPoints_1[4][3] = {
	{-5.0f,-5.0f,149.0f},
	{-5.0f,-5.6f,151.0f},
	{-5.0f,-5.7f,152.0f},
	{-5.0f,-5.8f,153.0f},
};

BOOL scene2_ONE = TRUE;
BOOL scene2_TWO = FALSE;
BOOL scene2_THREE = FALSE;

#pragma endregion



int initializeScene2(void)
{
	// variable declaration
	int result;
	BOOL bResult = FALSE;

	// code
	// for terrain
	result = initializeScene();
	if (result != 0)
	{
		fprintf(gpFILE, "initializeScene() terrain failed.\n");
		return(-1);
	}

	// for temple
	result = TempleInitialize();
	if (result != 0)
	{
		fprintf(gpFILE, "TempleInitialize() temple failed.\n");
		return(-2);
	}

	// for farm
	result = FarmInitialize();
	if (result != 0)
	{
		fprintf(gpFILE, "FarmInitialize() farm failed.\n");
		return(-3);
	}

	// calculate bazier curve coordinates
	for (float t = 0.0f; t <= 1.0f; t = t + 0.002985f)
	{
		float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
		float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
		float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

		BazierPoints.push_back(x);
		BazierPoints.push_back(y);
		BazierPoints.push_back(z);
	}

	// calculate bazier curve coordinates
	for (float t = 0.0f; t <= 1.0f; t = t + 0.001225f)
	{
		float x = pow((1 - t), 3) * controlPoints_1[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][0] + pow(t, 3) * controlPoints_1[3][0];
		float y = pow((1 - t), 3) * controlPoints_1[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][1] + pow(t, 3) * controlPoints_1[3][1];
		float z = pow((1 - t), 3) * controlPoints_1[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][2] + pow(t, 3) * controlPoints_1[3][2];

		BazierPoints_1.push_back(x);
		BazierPoints_1.push_back(y);
		BazierPoints_1.push_back(z);
	}

	return(0);
}

void displayScene2(void)
{
	// variable declaration
	float x, y, z;


	// code
	//glLoadIdentity();

	if (scene2_ONE)
	{
		if ((bazierPointsIndex + 3) + 4 < BazierPoints.size() && ELLAPSED_TIME < 91.0f)
		{
			bazierPointsIndex = bazierPointsIndex + 3;
		}
		else
		{
			bazierPointsIndex = 0;
			scene2_ONE = FALSE;
			scene2_TWO = TRUE;
			resetFadingFactors();
			sceneCompletedTime = ELLAPSED_TIME;
		}
	}

	if (scene2_TWO)
	{


		if ((bazierPointsIndex + 3) + 4 < BazierPoints.size() && ELLAPSED_TIME < 101.0f)
		{
			bazierPointsIndex = bazierPointsIndex + 3;	

		}
		else
		{
			bazierPointsIndex = 0;
			scene2_TWO = FALSE;
			scene2_THREE = TRUE;
			sceneCompletedTime = ELLAPSED_TIME;
			resetFadingFactors();
		}

	}

	if (scene2_THREE)
	{
		if ((bazierPointsIndex_1 + 3) + 4 < BazierPoints_1.size())
		{
			bazierPointsIndex_1 = bazierPointsIndex_1 + 3;

		}
		else
		{
			//bazierPointsIndex_1 = 0;

		}
	}

	if (scene2_TWO == TRUE)
	{
		gbFogEnable = TRUE;
	}
	else
	{
		gbFogEnable = FALSE;
	}
	

	if (gbFogEnable)
	{
		glEnable(GL_FOG); // enable GL_FOG

		// for fog effect
		glFogi(GL_FOG_MODE, fogMode[fogFilter]); // Fog mode
		glFogfv(GL_FOG_COLOR, fogColor); // Sets the color of the fog
		glFogf(GL_FOG_DENSITY, 0.30f); // Increase the number and fog will be more dense or  decrease it will be less dense(thick)
		glHint(GL_FOG_HINT, GL_DONT_CARE);	// Used to give hint to OpenGL about fog
											// GL_DONT_CARE -> let openGL chosse the kind of fog, i dont care how you do per pixel calculation of fog
											//	GL_NICEST -> show highest quality fog, but for this complex and length per pixel calculation need to be done
											// GL_FASTEST -> the most efficient option in terms of time or makes the fog faster, but quality of fog will not be good as GL_NICEST..

		glFogf(GL_FOG_START, 0.1f); // fog start depth

		glFogf(GL_FOG_END, 800.0f); // fog end depth
		
	}
	else
	{
		glDisable(GL_FOG); // Disable GL_FOG
	}

	
	

	glPushMatrix();
	{

		if (bEnableBazierCamera)
		{
			gluLookAt(gfHorizontal, gfVertical, gfDepth, gfCameraHorizontal, gfCameraVertical, gfCameraDepth, 0.0f, 1.0f, 0.0f);
		}
		else
		{
			if (scene2_ONE)
			{
				gluLookAt(-80.0f,0.0f, 200.0f, BazierPoints[bazierPointsIndex], BazierPoints[bazierPointsIndex + 1], BazierPoints[bazierPointsIndex + 2], 0.0f, 1.0f, 0.0f);

			}
			else if(scene2_TWO)
			{
				gluLookAt(BazierPoints[bazierPointsIndex], BazierPoints[bazierPointsIndex + 1], BazierPoints[bazierPointsIndex + 2], -100.0f, 10.0f, 200.0f, 0.0f, 1.0f, 0.0f);
			}
			else if (scene2_THREE)
			{
				gluLookAt(BazierPoints_1[bazierPointsIndex_1], BazierPoints_1[bazierPointsIndex_1 + 1], BazierPoints_1[bazierPointsIndex_1 + 2], -5.0f, -6.0f, 144.0f, 0.0f, 1.0f, 0.0f);
			}
		}
		// to display center of camera looking at(4,5,6 parameters of gluLookAt())
		if (switchCameraToMovement)
		{
			glPointSize(10.0f);
			glBegin(GL_POINTS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(gfCameraHorizontal, gfCameraVertical, gfCameraDepth);
			glEnd();
			glPointSize(1.0f);
		}

		glPushMatrix();
		{
			glTranslatef(-5.0f, -6.9f, 150.0f);
			// for temple
			TempleDisplay();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-350.0f, -6.9f, 150.0f);
			// for farm
			FarmDisplay();
		}
		glPopMatrix();


		glPushMatrix();
		{
			// for terrain
			glTranslatef(-300.0f, 100.0f, 0.0f);
			displayScene();
		}
		glPopMatrix();


		if (ELLAPSED_TIME < sceneCompletedTime + FADE_IN_TIME_DURATION)
		{
			fadeInEnabled = TRUE;
			fadeIn();
		}

	}
	glPopMatrix();
}

void updateScene2(void)
{

	// for terrain
	updateScene();

	// for temple
	TempleUpdate();

	// for farm
	FarmUpdate();

	//updateCommonScene();

	if (bVector == FALSE)
	{
		if (upVector_Y >= 0.5f)
		{
			upVector_Y = upVector_Y - 0.001f;
		}

		if (upVector_X >= -0.5f)
		{
			upVector_X = upVector_X - 0.001f;
		}
		else
		{
			bVector = TRUE;
		}


	}
	else
	{
		if (upVector_Y <= 1.0f)
		{
			upVector_Y = upVector_Y + 0.001f;
		}
		if (upVector_X <= 0.5f)
		{
			upVector_X = upVector_X + 0.001f;
		}
		else
		{
			bVector = FALSE;
		}


	}

}

void uninitializeScene2(void)
{
	// for farm
	FarmUninitialize();

	// for temple
	TempleUninitialize();

	// for terrain
	uninitializeScene();
}

