
#include"EndCredits.h"

static GLfloat gfTranslate_Y = -8.0f;

BOOL gbPresentDone = TRUE;
BOOL gbTechnoloyDone = FALSE;
BOOL gbReferencesDone = FALSE;
BOOL gbSpecialThanksDone = FALSE;
BOOL gbKillFont = FALSE;

int currentTitle = 1;

#pragma region FLAG EFFECT

static float points[45][45][3];                    // The Array For The Points On The Grid Of Our "Wave"
static int wiggle_count = 0;                       // Counter Used To Control How Fast Flag Waves
static GLfloat hold;

GLUquadric *quadric = NULL;

static std::vector<float> BazierPoints;
static int bazierPointsIndex = 0; // use as iterative variable for accessing BazierPoints elements

static float controlPoints[4][3] = {
	{0.0f,5.0f,350.0f},
	{-70.0f,30.0f,290.0f},
	{70.0f,10.0f,240.0f},
	{-5.0f,5.0f,190.0f}
};

static GLuint texture_logo = 0;

static std::vector<float> BazierPoints_1;
static int bazierCurveIndex_1 = 0;

static float controlPoints_1[4][3] = {
	{0.0f,0.0f,3.0f},
	{0.0f,0.0f,7.0f},
	{0.0f,0.0f,9.0f},
	{0.0f,0.0f,10.0f}
};


#pragma endregion

// light related variable 
static GLfloat gfLightAmbient[] = { 1.0f,1.0f,1.0f,1.0f };
static GLfloat gfLightDiffuse[] = { 1.0f,1.0f,1.0f,0.0f };
static GLfloat gfLightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };

static GLfloat gfLightPosition[] = { 0.0f,0.0f,0.0f,1.0f };
static GLfloat gfLightSpotLightDirection[] = { 0.0f,0.0f,-1.0f,1.0f };

static GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
static GLfloat materialDiffuse[] = { 1.0f,0.0f,0.0f,1.0f };
static GLfloat materialSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
static GLfloat materialShininess = 120.0f;

static GLfloat angleCutoff = 30.0f;
static GLfloat angleExponent = 30.0f;


static GLfloat zTranslate = -1.0f;

//BOOL showTextureLogo = FALSE;
BOOL scene5_One = TRUE;
BOOL scene5_OneCompleted = FALSE;
BOOL scene5_Two = FALSE;


int initializeEndCreadits(void)
{
	BOOL bResult;

	bResult = loadGLTexture(&texture_logo, MAKEINTRESOURCE(MY_BITMAP_LOGO));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of flag texture is Failed.\n");
		return(-1);
	}

	// flag effect
	// Loop Through The X Plane
	//fprintf(gpFILE, "\n\t\t***************** Loop Start Here ****************** \n");
	for (int x = 0; x < 45; x++)
	{
		//fprintf(gpFILE, "\n\t\t***************** x = % d ****************** \n", x);
		// Loop Through The Y Plane
		for (int y = 0; y < 45; y++)
		{
			// Apply The Wave To Our Mesh
			points[x][y][0] = (float)((x / 5.0f) - 4.5f);
			points[x][y][1] = (float)((y / 5.0f) - 4.5f);
			points[x][y][2] = (float)(sin((((x / 5.0f) * 40.0f) / 360.0f) * 3.141592654 * 2.0f));
			//fprintf(gpFILE, "\n\t\t***************** y = % d ****************** \n", y);
			//fprintf(gpFILE, "\n { x = %f y = %f  z = %f }\n", points[x][y][0], points[x][y][1], points[x][y][2]);
		}
	}
	//fprintf(gpFILE, "\n\t\t***************** Loop End Here ****************** \n");

	// for temple
	bResult = TempleInitialize();
	if (bResult != 0)
	{
		fprintf(gpFILE, "TempleInitialize() temple failed.\n");
		return(-2);
	}

	// for farm
	bResult = FarmInitialize();
	if (bResult != 0)
	{
		fprintf(gpFILE, "TempleInitialize() temple failed.\n");
		return(-2);
	}

	// for terrain
	bResult = initializeScene();
	if (bResult != 0)
	{
		fprintf(gpFILE, "TempleInitialize() temple failed.\n");
		return(-2);
	}

	quadric = gluNewQuadric();

	// for bazier curve (calculate points )
	for (float t = 0.0f; t <= 1.0f; t = t + 0.00115f)
	{
		float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
		float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
		float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

		BazierPoints.push_back(x);
		BazierPoints.push_back(y);
		BazierPoints.push_back(z);
	}

	// for bazier curve (calculate points )
	for (float t = 0.0f; t <= 1.0f; t = t + 0.0006525f)
	{
		float x = pow((1 - t), 3) * controlPoints_1[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][0] + pow(t, 3) * controlPoints_1[3][0];
		float y = pow((1 - t), 3) * controlPoints_1[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][1] + pow(t, 3) * controlPoints_1[3][1];
		float z = pow((1 - t), 3) * controlPoints_1[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][2] + pow(t, 3) * controlPoints_1[3][2];

		BazierPoints_1.push_back(x);
		BazierPoints_1.push_back(y);
		BazierPoints_1.push_back(z);
	}

	BuildFont();

	return(0);
}

void displayEndCreadits(void)
{
	// variable declaration 
	int x, y;                       // Loop Variables
	float float_x, float_y, float_xb, float_yb;     // Used To Break The Flag Into Tiny Quads

	glPushMatrix();
	{
		if (scene5_One)
		{
			if ((bazierPointsIndex + 3) + 4 < BazierPoints.size() && ELLAPSED_TIME < 240.0f)
			{
				bazierPointsIndex = bazierPointsIndex + 3;
			}
			else
			{
				if (!scene5_OneCompleted)
				{
					sceneCompletedTime = ELLAPSED_TIME;
				}
				
				scene5_OneCompleted = TRUE;
			}

			glPushMatrix();
			{
				if (bEnableBazierCamera)
				{
					gluLookAt(gfHorizontal, gfVertical, gfDepth, gfCameraHorizontal, gfCameraVertical, gfCameraDepth, 0.0f, 1.0f, 0.0f);
				}
				else
				{
					gluLookAt(BazierPoints[bazierPointsIndex], BazierPoints[bazierPointsIndex + 1], BazierPoints[bazierPointsIndex + 2], -5.0f, -5.0f, 150.0f, 0.0f, 1.0f, 0.0f);
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


				GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
				GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
				//GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
				GLfloat lightPosition[] = { 0.0f,10.0f,1000.0f,1.0f };

				// light related initialization
				glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmbient);
				glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDiffuse);
				//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
				glLightfv(GL_LIGHT4, GL_POSITION, lightPosition);

				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT4);
				glEnable(GL_COLOR_MATERIAL);


				//glEnable(GL_LIGHT1);
				glColor3f(1.0f, 0.5f, 0.0f);
				glPushMatrix();
				{
					glTranslatef(-20.0f, 10.0f, 300.0f);
					glPrint("Presented by");
					glTranslatef(-3.0f, -2.0f, 0.0f);
					glPrint("Siddhant Kshirsagar");

				}
				glPopMatrix();

				glPushMatrix();
				{
					glTranslatef(-30.0f, 15.0f, 270.0f);

					glPrint("Technology used");
					glPushMatrix();
					{
						glTranslatef(-4.0f, -2.0f, 0.0f);
						glPrint("OpenGL FFP");
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(-4.0f, -4.0f, 0.0f);
						glPrint("Win32 SDK");
					}
					glPopMatrix();
				}
				glPopMatrix();


				glPushMatrix();
				{
					glTranslatef(-10.0f, 15.0f, 250.0f);

					glPrint("References");
					glPushMatrix();
					{
						glTranslatef(-2.5f, -2.0f, 0.0f);
						glPrint("RTR Assignments");
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(-2.75f, -4.0f, 0.0f);
						glPrint("Group Activity");
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(-2.5f, -6.0f, 0.0f);
						glPrint("https://nehe.gamedev.net/");
					}
					glPopMatrix();

				}
				glPopMatrix();


				glPushMatrix();
				{
					glTranslatef(10.0f, 15.0f, 230.0f);

					glPrint("Music");
					glPushMatrix();
					{
						glTranslatef(-1.0f, -2.0f, 0.0f);
						glPrint("Aadhi Kalas Mag Paya");
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(-1.5f, -4.0f, 0.0f);
						glPrint("Sudhir Phadke");
					}
					glPopMatrix();

				}
				glPopMatrix();

				glPushMatrix();
				{
					glTranslatef(15.0f, 10.0f, 200.0f);

					glPrint("Special Thanks");
					glPushMatrix();
					{
						glTranslatef(-3.5f, -2.0f, 0.0f);
						glPrint("Dr. Rama Gokhale madam");
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(-3.5f, -4.0f, 0.0f);
						glPrint("Pradnya Gokhale madam");
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(-3.5f, -6.0f, 0.0f);
						glPrint("Pratik Jagadale");
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(-3.5f, -8.0f, 0.0f);
						glPrint("Texture Group Members");
					}
					glPopMatrix();

				}
				glPopMatrix();

				glPushMatrix();
				{
					glTranslatef(-5.0f, 3.0f, 180.0f);
					glPrint("Ignited By");
					glPushMatrix();
					{
						glTranslatef(-2.5f, -2.0f, 0.0f);
						glPrint("Dr. Vijay Gokhale Sir");
					}
					glPopMatrix();

				}
				glPopMatrix();

				glDisable(GL_COLOR_MATERIAL);

				glDisable(GL_LIGHTING);

				glDisable(GL_LIGHT4);

				glPushMatrix();
				{
					// for terrain
					glTranslatef(-300.0f, 100.0f, 0.0f);
					displayScene();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		if (scene5_Two)
		{

			if ((bazierCurveIndex_1 + 3) + 4 < BazierPoints_1.size() && ELLAPSED_TIME < 249.0f)
			{
				bazierCurveIndex_1 = bazierCurveIndex_1 + 3;
			}
			else
			{
				//bazierCurveIndex = 0;
				//bEnableBazierCamera = false;
				fadeOutEnabled = TRUE;

			}

			glPushMatrix();
			{

				if (bEnableBazierCamera)
				{
					gluLookAt(gfHorizontal, gfVertical, gfDepth, gfCameraHorizontal, gfCameraVertical, gfCameraDepth, 0.0f, 1.0f, 0.0f);

				}
				else
				{
					gluLookAt(BazierPoints_1[bazierCurveIndex_1], BazierPoints_1[bazierCurveIndex_1 + 1], BazierPoints_1[bazierCurveIndex_1 + 2], 0.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f);
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

				// light related initialization
				glLightfv(GL_LIGHT2, GL_AMBIENT, gfLightAmbient);
				glLightfv(GL_LIGHT2, GL_DIFFUSE, gfLightDiffuse);
				glLightfv(GL_LIGHT2, GL_SPECULAR, gfLightSpecular);

				glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, gfLightSpotLightDirection);
				glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, angleCutoff);
				glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, angleExponent);
				glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.5f);
				glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0f);
				glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f);



				glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
				glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);


				gfLightPosition[0] = 0.0f;
				gfLightPosition[1] = 0.0f;
				gfLightPosition[2] = zTranslate + 1.0f;
				gfLightPosition[3] = 1.0f;

				glLightfv(GL_LIGHT2, GL_POSITION, gfLightPosition);

				if (gfLightPosition[2] <= 0.0f)
				{
					gfLightSpotLightDirection[0] = 0.0f;
					gfLightSpotLightDirection[1] = 0.0f;
					gfLightSpotLightDirection[2] = 1.0f;
					gfLightSpotLightDirection[3] = 1.0f;
				}
				else
				{
					gfLightSpotLightDirection[0] = 0.0f;
					gfLightSpotLightDirection[1] = 0.0f;
					gfLightSpotLightDirection[2] = -1.0f;
					gfLightSpotLightDirection[3] = 1.0f;
				}

				glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, gfLightSpotLightDirection);

				glPushMatrix();
				{
					glEnable(GL_LIGHTING);
					glEnable(GL_LIGHT2);
					
					glPushMatrix();
					{
						glTranslatef(0.05f, 0.0f, 0.1f);
						glScalef(1.35f, 1.25f, 1.0f);
						glBindTexture(GL_TEXTURE_2D, texture_logo);       // Select Our Texture
							// first column
						materialDiffuse[0] = 1.0f; // r
						materialDiffuse[1] = 1.0f; // g
						materialDiffuse[2] = 1.0f; // b
						materialDiffuse[3] = 1.0f; // a
						glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						//glColor4f(1.0f, 1.0f, 1.0f, 0.75f);

						glBegin(GL_QUADS);                  // Start Drawing Our Quads

						glNormal3f(0.0f, 0.0f, 1.0f);

						for (x = 0; x < 44; x++)                // Loop Through The X Plane (44 Points)
						{
							for (y = 0; y < 44; y++)            // Loop Through The Y Plane (44 Points)
							{
								float_x = (float)(x) / 44.0f;       // Create A Floating Point X Value
								float_y = (float)(y) / 44.0f;       // Create A Floating Point Y Value
								float_xb = (float)(x + 1) / 44.0f;        // Create A Floating Point Y Value+0.0227f
								float_yb = (float)(y + 1) / 44.0f;        // Create A Floating Point Y Value+0.0227f
								glTexCoord2f(float_x, float_y);    // First Texture Coordinate (Bottom Left)
								glVertex3f(points[x][y][0], points[x][y][1], points[x][y][2]);

								glTexCoord2f(float_x, float_yb);  // Second Texture Coordinate (Top Left)
								glVertex3f(points[x][y + 1][0], points[x][y + 1][1], points[x][y + 1][2]);

								glTexCoord2f(float_xb, float_yb); // Third Texture Coordinate (Top Right)
								glVertex3f(points[x + 1][y + 1][0], points[x + 1][y + 1][1], points[x + 1][y + 1][2]);

								glTexCoord2f(float_xb, float_y);  // Fourth Texture Coordinate (Bottom Right)
								glVertex3f(points[x + 1][y][0], points[x + 1][y][1], points[x + 1][y][2]);
							}

						}
						glEnd();                        // Done Drawing Our Quads
						glBindTexture(GL_TEXTURE_2D, 0);

						glDisable(GL_BLEND);

						glDisable(GL_LIGHT2);

						glDisable(GL_LIGHTING);

						if (wiggle_count == 2)                 // Used To Slow Down The Wave (Every 2nd Frame Only)(Every time when display is called than each time here check value of wiggle_count if we increment value of check than animation is slow down because after n number of display call only once this if block will be executed. e.g.  if we check wiggle_count == 2 this block executed once when display called twice, if we check wiggle_count == 4 this block executed once when display called 4 times)
						{
							for (y = 0; y < 45; y++)            // Loop Through The Y Plane
							{
								hold = points[0][y][2];           // Store Current Value One Left Side Of Wave
								for (x = 0; x < 44; x++)     // Loop Through The X Plane
								{
									// Current Wave Value Equals Value To The Right
									points[x][y][2] = points[x + 1][y][2];
								}
								points[44][y][2] = hold;          // Last Value Becomes The Far Left Stored Value
							}
							wiggle_count = 0;               // Set Counter Back To Zero
						}
						wiggle_count++;
					}
					glPopMatrix();
				}
				glPopMatrix();



				if (fadeOutEnabled)
				{
					//fadeOutEnabled = TRUE;
					glPushMatrix();
					{
						fadeOut();
						//fprintf(gpFILE, "in side if fade out scene 5 Ellapse time = %f \n", ELLAPSED_TIME);
					}
					glPopMatrix();
				}
			}
			glPopMatrix();

			
		}

		if (scene5_OneCompleted)
		{
			
			if (ELLAPSED_TIME < sceneCompletedTime + FADE_OUT_TIME_DURATION)
			{
				fadeOutEnabled = TRUE;
				fadeOut();
			}
			else
			{
				
				scene5_Two = TRUE;
				scene5_One = FALSE;
				scene5_OneCompleted = FALSE; // for reset 
				//sceneCompletedTime = ELLAPSED_TIME;
				resetFadingFactors();
			}
		}
	}
	glPopMatrix();
}

void updateEndCreadits(void)
{

	if (scene5_Two)
	{
		if (zTranslate <= 7.0f)
		{
			zTranslate = zTranslate + 0.04f;
		}
	}
}

void uninitializeEndCreadits(void)
{
	KillFont();
	TempleUninitialize();

	if (texture_logo)
	{
		glDeleteTextures(1, &texture_logo);
		texture_logo = 0;
	}

	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}
}
