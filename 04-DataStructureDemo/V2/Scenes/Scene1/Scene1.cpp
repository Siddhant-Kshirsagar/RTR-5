
#include"Scene1.h"

#pragma region CAMERA

int bazierPointsIndex = 0; // use as iterative variable for accessing BazierPoints elements

std::vector<float> BazierPoints;

BOOL bVector = FALSE;

float upVector_X = 0.5f;
float upVector_Y = 1.0f;
float upVector_Z = 0.0f;

float controlPoints[4][3] = {
	{-5.0f,2.0f,160.0f},
	{0.0f,20.0f,200.0f},
	{-150.0f,2.0f,280.0f},
	{50.0f,20.0f,430.0f},
};


int bazierPointsIndex_1 = 0; // use as iterative variable for accessing BazierPoints elements

std::vector<float> BazierPoints_1;

float controlPoints_1[4][3] = {
	{-5.0f,6.0f,200.0f},
	{-65.0f,-2.0f,180.0f},
	{-5.0f,4.0f,80.0f},
	{30.0f,0.0f,155.0f},
};

GLfloat zAxis = 150.0f;
GLfloat xAxis = -25.0f;
GLfloat yAxis = 2.0f;

int bazierPointsIndex_2 = 0; // use as iterative variable for accessing BazierPoints elements

std::vector<float> BazierPoints_2;

float controlPoints_2[4][3] = {
	{-8.0f,-4.0f,150.0f},
	{-7.0f,-4.0f,155.0f},
	{-6.0f,-5.0f,160.0f},
	{-5.0f,-5.5f,165.0f},
};

BOOL scene1_One = TRUE;
struct Node *first_sceneOne = NULL;
struct Node *temp_sceneOne = NULL;
BOOL scene1_Two = FALSE;
struct Node *first_sceneTwo = NULL;
struct Node *temp_sceneTwo = NULL;
BOOL scene1_Three = FALSE;
struct Node *first_sceneThree = NULL;
struct Node *temp_sceneThree = NULL;

#pragma endregion

GLuint texture_leaves = 0;


#pragma region Particle Effect

#define MAX_PARTICLES 100 // number of particles to create
float slowDown = 5.0f; // slow down particles speed factor, low value means fast movement
float xSpeed; // base speed - which side the particle is moving left or right
float ySpeed; // base speed - which direction the particle is moving up or down
float zoom = 0.0f; // used for zooming out, start la particles la aat dhaklun mag te baher z axis var yenaret, so -40 start position

GLuint loop; // loop counter(iterative variables to used in loops)
GLuint col; // color counter (color counter - color chya array la iterate kartana use karaycha) - particles la different color denyasathi
GLuint delay; // cycle throught the color (array) - randomly color select karaila use karnar

typedef struct
{
	bool active; // decides whether the particle is to be used or not used(screen var particle dakhawaycha ka nahi he tharavto)
	float life; // kiti vel particle jagnar? mjanjech  particle chi alpha value in glColor4f(), different particle different time la alpha kami hot jail, mhanjech jevha alpha 0.0f hoil tyanch life sample as mhanta yeil
	float fade; // fade hi value present karto ki, particle chi life kitine kami hote in each iteration

	float r; // particle red color component
	float g; // particle green color component
	float b; // particle blue color component

	float x; // particle's x position
	float y; // particle's y position
	float z; // particle's z position

	float xi; // particle direction (left or right), increment factor (mhanje particle kiti speed ni x axis var halnar)
	float yi; // particle direction (up or down), increment factor(mhanje particle kiti speed ni y axis var halnar)
	float zi; // particle direction ( inward or outward), increment factor ( mhanje particle kiti speed ni z axis var halnar)

	float xg; // gravitational pull (left or right), particle chi movement slow karnar x axis var
	float yg; // gravitational pull (up or down), particle chi movement slow karnar y axis var
	float zg; // gravitational pull (inward or outward), particle chi movement slow karnar z axis var
}PARTICLES;

PARTICLES particle[MAX_PARTICLES];

// color array for particles - assigned random pair of rgb to particles
static GLfloat colors[12][3] = {
	{1.0f,0.5f,0.5f}, {1.0f,0.75f,0.5f}, {1.0f,1.0f,0.5f}, {0.75f,1.0f,0.5f},
	{0.5f,1.0f,0.5f}, {0.5f,1.0f,0.75f}, {0.5f,1.0f,1.0f}, {1.0f,0.5f,0.5f},
	{0.5f,0.5f,1.0f}, {0.75f,0.5f,1.0f}, {1.0f,0.5f,1.0f}, {1.0f,0.5f,0.75f}
};

//  global value for redisplay particle effect after one cycle completed
bool reinit = false;

#pragma endregion

GLfloat leaf_rotate = 0.0f;

int initializeScene1(void)
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

	// texture
	bResult = loadGLPNGTexture(&texture_leaves, "./Assets/Texture/yellowLeaves.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLPNGTexture() failed\n");
		return(-5);
	}

	// calculate bazier curve coordinates

	for (float t = 0.0f; t <= 1.0f; t = t + 0.0014f)
	{
		float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
		float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
		float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

		BazierPoints.push_back(x);
		BazierPoints.push_back(y);
		BazierPoints.push_back(z);

		MYPOSITION position = { x,y,z };

		if (first_sceneOne == NULL)
		{
			first_sceneOne = create_List(position);
		}
		first_sceneOne = insert_node_AtLastPosition(first_sceneOne, position);
	}

	temp_sceneOne = first_sceneOne;

	for (float t = 0.0f; t <= 1.0f; t = t + 0.00175f)
	{
		float x = pow((1 - t), 3) * controlPoints_1[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][0] + pow(t, 3) * controlPoints_1[3][0];
		float y = pow((1 - t), 3) * controlPoints_1[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][1] + pow(t, 3) * controlPoints_1[3][1];
		float z = pow((1 - t), 3) * controlPoints_1[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][2] + pow(t, 3) * controlPoints_1[3][2];

		BazierPoints_1.push_back(x);
		BazierPoints_1.push_back(y);
		BazierPoints_1.push_back(z);

		MYPOSITION position = { x,y,z };
		
		if (first_sceneTwo == NULL)
		{
			first_sceneTwo = create_List(position);
		}
		first_sceneTwo = insert_node_AtLastPosition(first_sceneTwo, position);
	}

	temp_sceneTwo = first_sceneTwo;

	for (float t = 0.0f; t <= 1.0f; t = t + 0.0011f)
	{
		float x = pow((1 - t), 3) * controlPoints_2[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][0] + pow(t, 3) * controlPoints_2[3][0];
		float y = pow((1 - t), 3) * controlPoints_2[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][1] + pow(t, 3) * controlPoints_2[3][1];
		float z = pow((1 - t), 3) * controlPoints_2[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][2] + pow(t, 3) * controlPoints_2[3][2];

		BazierPoints_2.push_back(x);
		BazierPoints_2.push_back(y);
		BazierPoints_2.push_back(z);

		MYPOSITION position = { x,y,z };

		if (first_sceneThree == NULL)
		{
			first_sceneThree = create_List(position);
		}
		first_sceneThree = insert_node_AtLastPosition(first_sceneThree, position);

	}

	temp_sceneThree = first_sceneThree;

	// for particles
	for (loop = 0; loop < MAX_PARTICLES; loop++)
	{
		particle[loop].active = true; // jar ka particle active nasel tar screen var dakhwaycha nahi
		particle[loop].life = 1.0f; // particle life atta full ahe

		// particle life kitne ne kami karat jaychi, saglya particle la randomly different value assign kar ne
		//             random value 0 - 99 madhey aananyasathi | start lach life hi 1.0f ali tar particle chi lagech life sampel tar tya sathi thode increment waprl ahe
		particle[loop].fade = (float)(rand() % 100) / 1000.0f + 0.003f;

		// assigned color to particle (we are using white color to all element this time)
		particle[loop].r = 1.0f;
		particle[loop].g = 1.0f;
		particle[loop].b = 1.0f;

		//speed and direction in which the particle should move(trajectory of particle)
		particle[loop].xi = (float)((rand() % 40) - 20.0f) * 5.0f;
		particle[loop].yi = (float)((rand() % 50) - 50.0f) * 5.0f;
		if (particle[loop].yi > 0.0f)
		{
			particle[loop].yi = -particle[loop].yi;
		}
		particle[loop].zi = (float)((rand() % 40) - 20.0f) * 5.0f;

		// gravity - sadhya gravity effect nahi use karycha mhanun 0 set kela ahe
		particle[loop].xg = 0.0f;
		particle[loop].yg = 0.0f;
		particle[loop].zg = 0.0f;
	}

	return(0);
}

void displayScene1(void)
{
	//function declaration
	void drawLeaf(void);

	// variable declaration
	float x, y, z;
	bool reset = true; // flag for display particle component

	// code
	//glLoadIdentity();

	if (scene1_One)
	{
		if (temp_sceneOne->next !=NULL && ELLAPSED_TIME < 42.0f)
		{
			temp_sceneOne = temp_sceneOne->next;
		}
		else
		{
			//bazierPointsIndex = 0;
			//bEnableBazierCamera = FALSE;
			scene1_Two = TRUE;
			scene1_One = FALSE;
			resetFadingFactors();
			sceneCompletedTime = ELLAPSED_TIME;

		}

	}

	if (scene1_Two)
	{
		if (temp_sceneTwo->next->next != NULL && ELLAPSED_TIME < 60.0f)
		{
			temp_sceneTwo = temp_sceneTwo->next;
		}
		else
		{
			scene1_Three = TRUE;
			scene1_Two = FALSE;
			resetFadingFactors();
			sceneCompletedTime = ELLAPSED_TIME;
		}

	}

	if (scene1_Three)
	{
		if (temp_sceneThree->next != NULL)
		{
			temp_sceneThree = temp_sceneThree->next;
		}
		else
		{
			//bazierPointsIndex = 0;
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
			if (scene1_One)
			{
				gluLookAt(temp_sceneOne->myPosition.xAxis, temp_sceneOne->myPosition.yAxis, temp_sceneOne->myPosition.zAxis, -5.0f, -3.0f, 150.0f, upVector_X, upVector_Y, upVector_Z);
			}

			if (scene1_Two)
			{
				gluLookAt(temp_sceneTwo->myPosition.xAxis, temp_sceneTwo->myPosition.yAxis, temp_sceneTwo->myPosition.zAxis, xAxis, yAxis, zAxis, 0.0f, 1.0f, 0.0f);
			}

			if (scene1_Three)
			{
				gluLookAt(temp_sceneThree->myPosition.xAxis, temp_sceneThree->myPosition.yAxis, temp_sceneThree->myPosition.zAxis, -5.0f, -4.0f, 150.0f, 0.0f, 1.0f, 0.0f);
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

		if (scene1_Two)
		{
			glPushMatrix();
			{
				glTranslatef(-25.0f, 10.0f, 150.0f);
				//glScalef(0.5f, 1.0f, 0.5f);
				//glDisable(GL_LIGHTING);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				for (loop = 0; loop < MAX_PARTICLES; loop++)
				{
					if (particle[loop].active)
					{
						// temporary copy value of position of particles
						float x = particle[loop].x;
						float y = particle[loop].y;
						float z = particle[loop].z + zoom;

						// give color to the particle
						/*glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);*/
						glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, 1.0f);


						glPushMatrix();
						{
							
							if (y <= -16.9f)
							{
								glTranslatef(x, -16.9f, z);
								glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
								drawLeaf();
							}
							else
							{
								glTranslatef(x, y, z);
								glRotatef(leaf_rotate, 0.0f, 1.0f, 1.0f);
								drawLeaf();
							}
							
						}
						glPopMatrix();
						
							// ***** animation part *****
							// slow down particle value 
						particle[loop].x = particle[loop].x + (particle[loop].xi / (slowDown * 1000));
						particle[loop].y = particle[loop].y + (particle[loop].yi / (slowDown * 1000));
						particle[loop].z = particle[loop].z + (particle[loop].zi / (slowDown * 1000));

						// add gravity(resistance) in movement of particle 
						particle[loop].xi = particle[loop].xi + particle[loop].xg;
						particle[loop].yi = particle[loop].yi + particle[loop].yg;
						particle[loop].zi = particle[loop].zi + particle[loop].zg;

						particle[loop].life = particle[loop].life - particle[loop].fade;

						//reset = reset && (particle[loop].life < 0.0f);
					}
					//reinit = reset;
				}

				glDisable(GL_BLEND);
			}
			glPopMatrix();
		}

		if (ELLAPSED_TIME < sceneCompletedTime + FADE_IN_TIME_DURATION)
		{
			fadeInEnabled = TRUE;
			fadeIn();
			//fprintf(gpFILE, "inside scene 1\t time = %f\n",ELLAPSED_TIME);
		}

	}
	glPopMatrix();
}

void drawLeaf(void)
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, texture_leaves);
		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(- 0.5f, 0.5f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(- 0.5f, - 0.5f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.5f, - 0.5f, 0.0f);

		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
}
void updateScene1(void)
{
	//updateCommonScene();

	// for terrain
	updateScene();

	// for temple
	TempleUpdate();

	// for farm
	FarmUpdate();

	if(scene1_Two)
	{
		leaf_rotate = leaf_rotate + 5.0f;
		if (leaf_rotate >= 360.0f)
		{
			leaf_rotate = 0.0f;
		}
	}
	
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

void uninitializeScene1(void)
{
	//uninitializeCommonScene();
	// for farm
	FarmUninitialize();

	// for temple
	TempleUninitialize();

	// for terrain
	uninitializeScene();

	destroy_list(first_sceneTwo);
	destroy_list(first_sceneThree);
	destroy_list(first_sceneOne);

	if (texture_leaves)
	{
		glDeleteTextures(1, &texture_leaves);
		texture_leaves = 0;
	}


}
