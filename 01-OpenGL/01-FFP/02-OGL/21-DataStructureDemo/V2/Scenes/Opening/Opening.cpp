
#include"Opening.h"

// light related variable 
GLfloat gfLightAmbient[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat gfLightDiffuse[] = { 1.0f,1.0f,1.0f,0.0f };
GLfloat gfLightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };

GLfloat gfLightPosition[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat gfLightSpotLightDirection[] = { 0.0f,0.0f,-1.0f,1.0f };

static GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
static GLfloat materialDiffuse[] = { 1.0f,0.0f,0.0f,1.0f };
static GLfloat materialSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
static GLfloat materialShininess = 120.0f;

GLfloat angleCutoff = 50.0f;
GLfloat angleExponent = 30.0f;


GLfloat zTranslate = 0.0f;


#pragma region FLAG EFFECT

static float points[45][45][3];                    // The Array For The Points On The Grid Of Our "Wave"
static int wiggle_count = 0;                       // Counter Used To Control How Fast Flag Waves
static GLfloat hold;

static GLUquadric *quadric = NULL;


static GLfloat gfAlpha = 0.0f;


static BOOL bEnableBazierCamera = FALSE;
static std::vector<float> BazierPoints;
static BOOL switchCameraToMovement = FALSE;
static GLfloat changeFactor = 0.5f;

static GLuint texture_logo = 0;

static int bazierCurveIndex = 0;

static float controlPoints[4][3] = {
	{0.0f,0.0f,-50.0f},
	{0.0f,0.0f,-10.0f},
	{0.0f,0.0f,0.0f},
	{0.0f,0.0f,15.0f}
};

#pragma endregion

GLuint texture_earth = 0;
GLuint texture_earthCloud = 0;
GLuint texture_space = 0;

GLfloat gfTranslateTitle_z = -30.0f;
static int currentTitle = 1;

//static float sceneCompletedTime = 0.0f;

GLfloat yRotate = 0.0f;


int initializeOpening(void)
{
	BOOL bResult;

	bResult = loadGLTexture(&texture_logo, MAKEINTRESOURCE(MY_BITMAP_LOGO));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of flag texture is Failed.\n");
		return(-7);
	}

	bResult = loadGLTexture(&texture_earth, MAKEINTRESOURCE(MY_EARTH_BITMAP));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of texture is Failed.\n");
		return(-7);
	}

	bResult = loadGLPNGTexture(&texture_earthCloud, "./Assets/Texture/earth_clouds.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLPNGTexture() failed\n");
		return(-9);
	}

	bResult = loadGLTexture(&texture_space, MAKEINTRESOURCE(MY_SPACE_BITMAP));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of texture is Failed.\n");
		return(-7);
	}

	// light related initialization
	glLightfv(GL_LIGHT1, GL_AMBIENT, gfLightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, gfLightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, gfLightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, gfLightPosition);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, gfLightSpotLightDirection);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angleCutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, angleExponent);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);



	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);


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

	quadric = gluNewQuadric();

	// for bazier curve (calculate points )
	for (float t = 0.0f; t <= 1.0f; t = t + 0.001f)
	{
		float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
		float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
		float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

		BazierPoints.push_back(x);
		BazierPoints.push_back(y);
		BazierPoints.push_back(z);
	}

	BuildFont();

	return(0);
}

void displayOpening(void)
{
	// variable declaration 

	int x, y;                       // Loop Variables
	float float_x, float_y, float_xb, float_yb;     // Used To Break The Flag Into Tiny Quads

	// code
	glDisable(GL_FOG);

	glPushMatrix();
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glTranslatef(0.0f, 0.0f, -10.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(quadric, GL_TRUE);

		glBindTexture(GL_TEXTURE_2D, texture_space);
		glColor3f(1.0f, 1.0f, 1.0f);
		gluSphere(quadric, 50.0f, 50, 50);
		glBindTexture(GL_TEXTURE_2D, 0);

		glTranslatef(0.0f, 10.0f, 0.0f);
		glRotatef(yRotate * 1.0f, 0.0f, 0.0f, 1.0f);

		glBindTexture(GL_TEXTURE_2D, texture_earth);
		glColor3f(1.0f, 1.0f, 1.0f);
		gluSphere(quadric, 4.5f, 50, 50);
		glBindTexture(GL_TEXTURE_2D, 0);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, texture_earthCloud);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glRotatef(yRotate * 0.75f, 0.0f, 0.0f, 1.0f);
		gluSphere(quadric, 4.55f, 50, 50);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);

		gluQuadricTexture(quadric, GL_FALSE);
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

	}
	glPopMatrix();
	glPushMatrix();
	{
		glEnable(GL_LIGHT1);

		glEnable(GL_LIGHTING);

		glEnable(GL_COLOR_MATERIAL);

		glColor3f(1.0f, 0.5f, 0.0f);

		glTranslatef(0.0f, -0.5f, gfTranslateTitle_z);
		switch (currentTitle)
		{
		case TITLE_ONE:
			glPrint("ASTROMEDICOMP");
			break;
		case TITLE_TWO:
			glPrint("TEXTURE GROUP");
			break;
		case TITLE_THREE:
			glPrint("PRESENTS");
			break;
		default:
			break;
		}
		glDisable(GL_COLOR_MATERIAL);

		glDisable(GL_LIGHT1);

		glDisable(GL_LIGHTING);

	}
	glPopMatrix();

}

void updateOpening(void)
{
	gfTranslateTitle_z = gfTranslateTitle_z + 0.0335f;
	if (gfTranslateTitle_z >= 5.0f)
	{
		gfTranslateTitle_z = -30.0f;
		if (currentTitle < 3)
		{
			currentTitle = currentTitle + 1;
		}
	}

	if (zTranslate <= 10.0f)
	{
		zTranslate = zTranslate + 0.04f;
	}

	yRotate = yRotate + 0.05f;
	if (yRotate >= 360.0f)
	{
		yRotate = 0.0f;
	}

	if (gfAlpha <= 1.0f)
	{
		gfAlpha = gfAlpha + 0.005f;
	}
}

void uninitializeOpening(void)
{
	// delete texture
	KillFont();

	if (texture_logo)
	{
		glDeleteTextures(1, &texture_logo);
		texture_logo = 0;
	}

	if (texture_earth)
	{
		glDeleteTextures(1, &texture_earth);
		texture_earth = 0;
	}

	if (texture_earthCloud)
	{
		glDeleteTextures(1, &texture_earthCloud);
		texture_earthCloud = 0;
	}

	if (texture_space)
	{
		glDeleteTextures(1, &texture_space);
		texture_space = 0;
	}

	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}
}
