
#include"Scene3.h"
#include"..\HumanModel.h"

#pragma region CAMERA

static int bazierPointsIndex = 0; // use as iterative variable for accessing BazierPoints elements

static int bazierPointsIndex_2 = 0; // use as iterative variable for accessing BazierPoints elements

static int bazierPointsIndex_3 = 0; // use as iterative variable for accessing BazierPoints elements

static std::vector<float> BazierPoints;

static std::vector<float> BazierPoints_2;

static std::vector<float> BazierPoints_3;

static BOOL bVector = FALSE;

BOOL scene3_One = TRUE;
BOOL scene3_Two = FALSE;
BOOL scene3_Three = FALSE;

static float upVector_X = 0.0f;
static float upVector_Y = 1.0f;
static float upVector_Z = 0.0f;

static float controlPoints[4][3] = {
	{-340.0f,-4.0f,220.0f},
	{-320.0f,-3.0f,180.0f},
	{-300.0f,-2.0f,150.0f},
	{-280.0f,-1.0f,140.0f},
};

static float controlPoints_2[4][3] = {
	{-210.0f,0.0f,185.0f},
	{-208.0f,-4.0f,155.0f},
	{-200.0f,-3.0f,160.0f},
	{-180.0f,-5.0f,155.0f},
};

static float controlPoints_3[4][3] = {
	{-10.0f,-4.0f,155.0f},
	{-5.0f,-4.5f,150.0f},
	{2.0f,-4.5f,155.0f},
	{-6.0f,-4.5f,157.0f},
};


#pragma endregion

#pragma region MODEL

HumanModel humanModel;
HumanModel humanModel_1;

#pragma endregion

GLfloat animation_x = -200.0f;

GLuint texture_sack = 0;
GLuint texture_cards = 0;
extern GLuint texture_whiteCloth;
GLuint texture_greenBottle = 0;
GLuint texture_brownBottle = 0;

static GLUquadric *quadric;

//GLuint textureArray[6];

int initializeScene3(void)
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

	result = initializeCommonScene();
	if (result != 0)
	{
		fprintf(gpFILE, "initializeCommonScene() Failed \n");
		return(-4);
	}



	if (humanModel.initialize())
	{
		fprintf(gpFILE, "humanModel.initialize() is Failed.\n");
		return(-5);
	}

	if (humanModel_1.initialize())
	{
		fprintf(gpFILE, "humanModel_1.initialize() is Failed.\n");
		return(-6);
	}

	bResult = loadGLPNGTexture(&texture_greenBottle, "./Assets/Texture/greenBottle.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed \n");
		return(-1);
	}

	bResult = loadGLPNGTexture(&texture_brownBottle, "./Assets/Texture/brownBottle.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed \n");
		return(-1);
	}

	bResult = loadGLTexture(&texture_cards, MAKEINTRESOURCE(MY_CARDS_TEXTURE));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed \n");
		return(-7);
	}

	bResult = loadGLTexture(&texture_sack, MAKEINTRESOURCE(MY_SACK_TEXTURE));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed \n");
		return(-8);
	}

	// calculate bazier curve coordinates
	for (float t = 0.0f; t <= 1.0f; t = t + 0.01f)
	{
		float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
		float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
		float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

		BazierPoints.push_back(x);
		BazierPoints.push_back(y);
		BazierPoints.push_back(z);
	}

	// calculate bazier curve coordinates
	for (float t = 0.0f; t <= 1.0f; t = t + 0.00385f)
	{
		float x = pow((1 - t), 3) * controlPoints_2[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][0] + pow(t, 3) * controlPoints_2[3][0];
		float y = pow((1 - t), 3) * controlPoints_2[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][1] + pow(t, 3) * controlPoints_2[3][1];
		float z = pow((1 - t), 3) * controlPoints_2[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][2] + pow(t, 3) * controlPoints_2[3][2];

		BazierPoints_2.push_back(x);
		BazierPoints_2.push_back(y);
		BazierPoints_2.push_back(z);
	}

	// calculate bazier curve coordinates
	for (float t = 0.0f; t <= 1.0f; t = t + 0.00115f)
	{
		float x = pow((1 - t), 3) * controlPoints_3[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_3[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_3[2][0] + pow(t, 3) * controlPoints_3[3][0];
		float y = pow((1 - t), 3) * controlPoints_3[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_3[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_3[2][1] + pow(t, 3) * controlPoints_3[3][1];
		float z = pow((1 - t), 3) * controlPoints_3[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_3[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_3[2][2] + pow(t, 3) * controlPoints_3[3][2];

		BazierPoints_3.push_back(x);
		BazierPoints_3.push_back(y);
		BazierPoints_3.push_back(z);
	}

	quadric = gluNewQuadric();

	return(0);
}

void displayScene3(void)
{
	//function declaration
	void drawGamble(void);

	// code
	//glLoadIdentity();

	if (scene3_One)
	{
		if ((bazierPointsIndex + 3) + 4 < BazierPoints.size())
		{
			bazierPointsIndex = bazierPointsIndex + 3;
		}
		else
		{
			bazierPointsIndex = 0;
			if (scene3_One)
			{
				scene3_One = FALSE;
				scene3_Two = TRUE;

				resetFadingFactors();
				sceneCompletedTime = ELLAPSED_TIME;					
			}
			else
			{
				//scene3_One = TRUE;
			}

			//bEnableBazierCamera = FALSE;
		}
	}

	if (scene3_Two)
	{
		if ((bazierPointsIndex_2 + 3) + 4 < BazierPoints_2.size() && ELLAPSED_TIME < 139.0f)
		{
			bazierPointsIndex_2 = bazierPointsIndex_2 + 3;
		}
		else
		{
			bazierPointsIndex_2 = 0;
			if (scene3_Two)
			{
				scene3_Two = FALSE;
				scene3_Three = TRUE;
				resetFadingFactors();
				sceneCompletedTime = ELLAPSED_TIME;
			}
			else
			{
				//scene3_One = TRUE;
			}

			//bEnableBazierCamera = FALSE;
		}
	}

	if (scene3_Three)
	{
		if ((bazierPointsIndex_3 + 3) + 4 < BazierPoints_3.size())
		{
			bazierPointsIndex_3 = bazierPointsIndex_3 + 3;
		}
		else
		{
			//bazierPointsIndex_3 = 0;
			//bEnableBazierCamera = FALSE;
		}
	}

	glDisable(GL_FOG); // Disable GL_FOG

	glPushMatrix();
	{

		if (bEnableBazierCamera)
		{
			gluLookAt(gfHorizontal, gfVertical, gfDepth, gfCameraHorizontal, gfCameraVertical, gfCameraDepth, 0.0f, 1.0f, 0.0f);
		}
		else
		{
			if (scene3_One)
			{
				gluLookAt(BazierPoints[bazierPointsIndex], BazierPoints[bazierPointsIndex + 1] + 10.0f, BazierPoints[bazierPointsIndex + 2], -330.0f, -6.0f, 150.0f, upVector_X, upVector_Y, upVector_Z);
			}
			else if (scene3_Two)
			{
				gluLookAt(BazierPoints_2[bazierPointsIndex_2], BazierPoints_2[bazierPointsIndex_2 + 1], BazierPoints_2[bazierPointsIndex_2 + 2], -202.0f, -5.0f, 155.0f, 0.0f, 1.0f, 0.0f);
			}
			else if (scene3_Three)
			{
				gluLookAt(BazierPoints_3[bazierPointsIndex_3], BazierPoints_3[bazierPointsIndex_3 + 1], BazierPoints_3[bazierPointsIndex_3 + 2], -5.0f, -4.5f, 155.0f, 0.0f, 1.0f, 0.0f);
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
			//glScalef(1.0f, 0.5f, 1.0f);
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

		if (scene3_Two)
		{
			glPushMatrix();
			{
				glPushMatrix();
				{
					glTranslatef(-202.0f, -4.8f, 154.0f);
					glScalef(0.5f, 0.5f, 0.5f);
					if (!humanModel_1.isHumanSwitch())
					{
						humanModel_1.toggleHumanSwitch();
					}
					humanModel_1.drawBody();
				}
				glPopMatrix();

				glPushMatrix();
				{
					glTranslatef(animation_x, -4.8f, 155.0f);
					glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
					glScalef(0.5f, 0.5f, 0.5f);

					if (humanModel.isNamaskarAnimationEnable())
					{
						humanModel.toggleNamaskarAnimation();
					}

					if (!humanModel.isWalkingAnimationEnable())
					{
						humanModel.toggleWalkingAnimation();
					}

					humanModel.drawBody();
				}
				glPopMatrix();

			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(-202.0f, -6.95f, 155.0f);
				drawGamble();
			}
			glPopMatrix();
		}
	

		if (scene3_Three)
		{		
			// code
			glPushMatrix();
			{
				if (!humanModel.isNamaskarAnimationEnable())
				{
					humanModel.toggleNamaskarAnimation();
				}

				glTranslatef(-5.0f, -4.0f, 155.0f);
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
				glScalef(0.5f, 0.5f, 0.5f);
				
				humanModel.drawBody();		

			}
			glPopMatrix();
		}


		if (ELLAPSED_TIME < sceneCompletedTime + FADE_IN_TIME_DURATION)
		{
			fadeInEnabled = TRUE;
			fadeIn();
		}
	}
	glPopMatrix();

}

void drawGamble(void)
{
	//code
	glPushMatrix();
	{
		// sack on ground
		glBindTexture(GL_TEXTURE_2D, texture_sack);

		glPushMatrix();
		{
			glScalef(1.5f, 1.0f, 2.0f);
			glBegin(GL_QUADS);

			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 1.0f);
			glEnd();
		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);

		glPushMatrix();
		{
			glScalef(1.25f, 1.5f, 0.8f);
			glPushMatrix();
			{
				glTranslatef(0.8f, 0.01f, 0.0f);
				glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				gluQuadricTexture(quadric, GL_TRUE);

				glBindTexture(GL_TEXTURE_2D, texture_brownBottle);

				glColor3f(119.0f / 255.0f, 168.0f / 255.0f, 219.0f / 255.0f);

				gluDisk(quadric, 0.0f, 0.1f, 10, 1);
				gluCylinder(quadric, 0.1f, 0.1f, 0.25f, 10, 1);

				glTranslatef(0.0f, 0.0f, 0.25f);
				gluCylinder(quadric, 0.1f, 0.02f, 0.05f, 10, 1);

				glTranslatef(0.0f, 0.0f, 0.05f);
				gluCylinder(quadric, 0.02f, 0.02f, 0.02f, 10, 1);

				glBindTexture(GL_TEXTURE_2D, 0);

				gluQuadricTexture(quadric, GL_FALSE);

				glDisable(GL_BLEND);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.8f, 0.01f, -0.5f);
				glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				gluQuadricTexture(quadric, GL_TRUE);

				glBindTexture(GL_TEXTURE_2D, texture_greenBottle);

				glColor3f(119.0f / 255.0f, 168.0f / 255.0f, 219.0f / 255.0f);

				gluDisk(quadric, 0.0f, 0.1f, 10, 1);
				gluCylinder(quadric, 0.1f, 0.1f, 0.25f, 10, 1);

				glTranslatef(0.0f, 0.0f, 0.25f);
				gluCylinder(quadric, 0.1f, 0.02f, 0.05f, 10, 1);

				glTranslatef(0.0f, 0.0f, 0.05f);
				gluCylinder(quadric, 0.02f, 0.02f, 0.02f, 10, 1);

				glBindTexture(GL_TEXTURE_2D, 0);

				gluQuadricTexture(quadric, GL_FALSE);

				glDisable(GL_BLEND);
			}
			glPopMatrix();
		}
		glPopMatrix();


		glPushMatrix();
		{
			glTranslatef(0.0f, 0.05f, 0.0f);
			glScalef(0.5f, 1.0f, 0.5f);


			// cards on sack
			glBindTexture(GL_TEXTURE_2D, texture_cards);

			glBegin(GL_QUADS);

			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 1.0f);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glPopMatrix();


	}
	glPopMatrix();
}

void updateScene3(void)
{
	// for terrain
	updateScene();

	// for temple
	TempleUpdate();

	// for farm
	FarmUpdate();

	

	if (scene3_One)
	{
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
	if (scene3_Two)
	{
		
		if (animation_x <= 150.0f)
		{
			animation_x = animation_x + 0.025f;
		}
		else
		{
			scene3_Two = FALSE;
		}

		humanModel.update();
	}

	if (scene3_Three)
	{
		humanModel.update();
	}
}

void uninitializeScene3(void)
{
	// for farm
	FarmUninitialize();

	// for temple
	TempleUninitialize();

	// for terrain
	uninitializeScene();

	humanModel.uninitialize();

	humanModel_1.uninitialize();

	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}

	if (texture_brownBottle)
	{
		glDeleteTextures(1, &texture_brownBottle);
		texture_brownBottle = 0;
	}

	if (texture_greenBottle)
	{
		glDeleteTextures(1, &texture_greenBottle);
		texture_greenBottle = 0;
	}

	if (texture_cards)
	{
		glDeleteTextures(1, &texture_cards);
		texture_cards = 0;
	}


	if (texture_sack)
	{
		glDeleteTextures(1, &texture_sack);
		texture_sack = 0;
	}
}
