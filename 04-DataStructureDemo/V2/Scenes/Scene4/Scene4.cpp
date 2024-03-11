#include"Scene4.h"

#pragma region CAMERA

static int bazierPointsIndex = 0; // use as iterative variable for accessing BazierPoints elements
static int bazierPointsIndex_1 = 0; // use as iterative variable for accessing BazierPoints elements
static int bazierPointsIndex_2 = 0; // use as iterative variable for accessing BazierPoints elements

static std::vector<float> BazierPoints;
static std::vector<float> BazierPoints_1;
static std::vector<float> BazierPoints_2;

static BOOL bVector = FALSE;

BOOL scene4_One = TRUE;
BOOL scene4_Two = FALSE;
BOOL scene4_Three = FALSE;

static float upVector_X = 0.0f;
static float upVector_Y = 1.0f;
static float upVector_Z = 0.0f;

static float controlPoints[4][3] = {
	{-8.5f,-6.5f,165.0f},
	{-8.0f,-5.0f,156.0f},
	{-7.0f,-3.0f,155.0f},
	{5.0f,0.0f,172.0f},
};

static float controlPoints_1[4][3] = {
	{-7.0f,-5.0f,160.0f},
	{-9.0f,-4.0f,170.0f},
	{-10.0f,-3.0f,160.0f},
	{-12.0f,-2.0f,160.0f},
};

static float controlPoints_2[4][3] = {
	{-270.0f,-5.0f,160.0f},
	{-190.0f,10.0f,370.0f},
	{-50.0f,30.0f,260.0f},
	{70.0f,50.0f,-100.0f},
};


#pragma endregion

GLuint texture_waterDrop = 0;
GLfloat terrainAngle = 0.0f;

GLuint texture_thunder = 0;
BOOL gbThunderEnable = FALSE;
GLfloat thunder_alpha = 1.0f;

extern GLUquadric *quadric;

#pragma region Butterfly and catepillar

GLuint texture_skin = 0; // for catepillar
GLuint texture_wing = 0;

BOOL gbWalkAnimation_catepillar = FALSE;
static GLfloat gfRotation = 0.0f;
GLfloat gfWalk_movement = 0.0f;
GLfloat gfTranslatefCatepillar = 1.0f;

BOOL gbButterfly_wingAnimtaion = FALSE;
GLfloat gfRotation_wings = 0.0f;
GLfloat gfRotation_antena = 0.0f;
GLfloat gfRotation_leg_1bent = 230.0f;
GLfloat gfRotation_leg_2bent = 0.0f;
BOOL gbButterfly_legAnimation = FALSE;


BOOL gbRotateButterfly = FALSE;
float gfRotateButterflyAngle = 0.0f;
float gfRotate_Y = 0.0f; // for butterfly body rotation


static float controlPoints_butterfly[4][3] = {
    {-8.5f,-5.5f,156.0f},
    {-12.0f,-5.0f,165.5f},
    {-15.0f,-5.25f,154.0f},
    {-11.5f,-5.0f,155.0f},
};

static std::vector<float> BazierPoints_butterfly;
static int bazierPointsIndex_butterfly = 0; // use as iterative variable for accessing BazierPoints elements

#pragma endregion

int initializeScene4(void)
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

	bResult = loadGLPNGTexture(&texture_waterDrop, "./Assets/Texture/WaterDrop.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of water texture is Failed.\n");
		return(-5);
	}

    bResult = loadGLTexture(&texture_skin, MAKEINTRESOURCE(MY_SKIN_TEXTURE));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "load of loadGLTexture is Failed.\n");
        return(-6);
    }

    bResult = loadGLPNGTexture(&texture_wing, "./Assets/Texture/wings.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "load of loadGLPNGTexture is Failed.\n");
        return(-6);
    }

    bResult = loadGLPNGTexture(&texture_thunder, "./Assets/Texture/thunder.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "load of loadGLPNGTexture is Failed.\n");
        return(-6);
    }


    // calculate bazier curve coordinates (for butterfly animation)
    for (float t = 0.0f; t <= 1.0f; t = t + 0.00245f)
    {
        float x = pow((1 - t), 3) * controlPoints_butterfly[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_butterfly[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_butterfly[2][0] + pow(t, 3) * controlPoints_butterfly[3][0];
        float y = pow((1 - t), 3) * controlPoints_butterfly[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_butterfly[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_butterfly[2][1] + pow(t, 3) * controlPoints_butterfly[3][1];
        float z = pow((1 - t), 3) * controlPoints_butterfly[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_butterfly[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_butterfly[2][2] + pow(t, 3) * controlPoints_butterfly[3][2];

        BazierPoints_butterfly.push_back(x);
        BazierPoints_butterfly.push_back(y);
        BazierPoints_butterfly.push_back(z);
    }

	// calculate bazier curve coordinates
	for (float t = 0.0f; t <= 1.0f; t = t + 0.00355f)
	{
		float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
		float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
		float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

		BazierPoints.push_back(x);
		BazierPoints.push_back(y);
		BazierPoints.push_back(z);
	}

	// calculate bazier curve coordinates
	for (float t = 0.0f; t <= 1.0f; t = t + 0.0035f)
	{
		float x = pow((1 - t), 3) * controlPoints_1[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][0] + pow(t, 3) * controlPoints_1[3][0];
		float y = pow((1 - t), 3) * controlPoints_1[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][1] + pow(t, 3) * controlPoints_1[3][1];
		float z = pow((1 - t), 3) * controlPoints_1[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_1[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_1[2][2] + pow(t, 3) * controlPoints_1[3][2];

		BazierPoints_1.push_back(x);
		BazierPoints_1.push_back(y);
		BazierPoints_1.push_back(z);
	}

	// calculate bazier curve coordinates
	for (float t = 0.0f; t <= 1.0f; t = t + 0.001f)
	{
		float x = pow((1 - t), 3) * controlPoints_2[0][0] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][0] + pow(t, 3) * controlPoints_2[3][0];
		float y = pow((1 - t), 3) * controlPoints_1[0][1] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][1] + pow(t, 3) * controlPoints_2[3][1];
		float z = pow((1 - t), 3) * controlPoints_2[0][2] + 3 * pow((1 - t), 2) * t * controlPoints_2[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints_2[2][2] + pow(t, 3) * controlPoints_2[3][2];

		BazierPoints_2.push_back(x);
		BazierPoints_2.push_back(y);
		BazierPoints_2.push_back(z);
	}

	return(0);
}

void displayScene4(void)
{
	// function declaration
	void rainTypeFour(void);
    void drawButterfly(void);
	// code
	//glLoadIdentity();

	glPushMatrix();
	{
		if (scene4_One)
		{

			if ((bazierPointsIndex + 3) + 4 < BazierPoints.size() && ELLAPSED_TIME < 170.0f)
			{
				bazierPointsIndex = bazierPointsIndex + 3;
			}
			else
			{
				bazierPointsIndex = 0;

				scene4_One = FALSE;
				scene4_Two = TRUE;

				resetFadingFactors();
				sceneCompletedTime = ELLAPSED_TIME;

			}
		}
		else if (scene4_Two)
		{
			if ((bazierPointsIndex_1 + 3) + 4 < BazierPoints_1.size() && ELLAPSED_TIME < 180.0f)
			{
				bazierPointsIndex_1 = bazierPointsIndex_1 + 3;
			}
			else
			{
				bazierPointsIndex_1 = 0;

				scene4_Two = FALSE;
				scene4_Three = TRUE;

				resetFadingFactors();
				sceneCompletedTime = ELLAPSED_TIME;

			}
		}
		else if (scene4_Three)
		{
			if ((bazierPointsIndex_2 + 3) + 4 < BazierPoints_2.size())
			{
				bazierPointsIndex_2 = bazierPointsIndex_2 + 3;
			}

		}

        if (scene4_Three)
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
			glFogf(GL_FOG_END, 1000.0f); // fog end depth

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

				if (scene4_One)
				{

					gluLookAt(BazierPoints[bazierPointsIndex], BazierPoints[bazierPointsIndex + 1], BazierPoints[bazierPointsIndex + 2], -5.0f, -5.0f, 150.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (scene4_Two)
				{
					gluLookAt(BazierPoints_1[bazierPointsIndex_1], BazierPoints_1[bazierPointsIndex_1 + 1], BazierPoints_1[bazierPointsIndex_1 + 2], -12.0f, -6.0f, 156.0f, 0.0f, 1.0f, 0.0f);

				}
				else if (scene4_Three)
				{
					gluLookAt(BazierPoints_2[bazierPointsIndex_2], BazierPoints_2[bazierPointsIndex_2 + 1], BazierPoints_2[bazierPointsIndex_2 + 2], -100.0f, 30.0f, 156.0f, 0.0f, 1.0f, 0.0f);
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

            if (selectedScene == 4)
            {
                if (scene4_Two)
                {
                    if ((bazierPointsIndex_butterfly + 3) + 4 < BazierPoints_butterfly.size())
                    {
                        bazierPointsIndex_butterfly = bazierPointsIndex_butterfly + 3;
                    }
                    else
                    {
                        bazierPointsIndex_butterfly = 0;
                    }

                    glPushMatrix();
                    {
                        glTranslatef(BazierPoints_butterfly[bazierPointsIndex_butterfly], BazierPoints_butterfly[bazierPointsIndex_butterfly + 1], BazierPoints_butterfly[bazierPointsIndex_butterfly + 2]);
                        //glTranslatef(0.0f, 2.0f, 2.0f);
                        glRotatef(gfRotateButterflyAngle, 0.0f, 0.0f, 1.0f);
                        glRotatef(gfRotate_Y, 0.0f, 1.0f, 0.0f);
                        glScalef(0.1f, 0.1f, 0.1f);
                        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
                        drawButterfly();
                    }
                    glPopMatrix();
                }

            }

            if (selectedScene == 4 && scene4_Three == TRUE)
            {

                if (bazierPointsIndex_2 > BazierPoints_2.size() / 3)
                {
                    gbThunderEnable = TRUE;
                }
                if (gbThunderEnable && thunder_alpha > 0.0f)
                {
                    glPushMatrix();
                    {

                        glTranslatef(-100.0f, 100.0f, 0.0f);

                        //variable declaration
                        float modelview[16];
                        int i, j;

                        // get the current modelview matrix
                        glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

                        // undo all rotations
                        // beware all scaling is lost as well 
                        for (i = 0; i < 3; i += 2)
                        {
                            for (j = 0; j < 3; j++)
                            {
                                if (i == j)
                                    modelview[i * 4 + j] = 1.0;
                                else
                                    modelview[i * 4 + j] = 0.0;
                            }
                        }

                        // set the modelview with no rotations and scaling
                        glLoadMatrixf(modelview);

                        glScalef(20.0f, 125.0f, 1.0f);

                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        glBindTexture(GL_TEXTURE_2D, texture_thunder);
                     
                        glBegin(GL_QUADS);

                        glColor4f(175.0f, 225.0f/255.0f, 236.0f/255.0f, thunder_alpha);

                        glTexCoord2f(1.0f, 1.0f);
                        glVertex3f(1.0f, 1.0f, 0.0f);

                        glTexCoord2f(0.0f, 1.0f);
                        glVertex3f(-1.0f, 1.0f, 0.0f);

                        glTexCoord2f(0.0f, 0.0f);
                        glVertex3f(-1.0f, -1.0f, 0.0f);

                        glTexCoord2f(1.0f, 0.0f);
                        glVertex3f(1.0f, -1.0f, 0.0f);
                        glEnd();
                        glBindTexture(GL_TEXTURE_2D, 0);
                        glDisable(GL_BLEND);
                    }
                    glPopMatrix();
                }

                glPushMatrix();
                {
                    glTranslatef(-50.0f, 0.0f, 150.0f);
                    rainTypeFour();
                }
                glPopMatrix();

            }

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

void drawButterfly(void)
{
    // code
    GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
    GLfloat lightDiffuse[] = { 0.5f,0.5f,0.5f,1.0f };
    //GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
    GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

    // light related initialization
    glLightfv(GL_LIGHT7, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT7, GL_DIFFUSE, lightDiffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT7, GL_POSITION, lightPosition);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT7);

    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quadric, GL_TRUE);

    // ambient material
    materialAmbient[0] = 0.0f; // r
    materialAmbient[1] = 0.0f; // g
    materialAmbient[2] = 0.0f; // b
    materialAmbient[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    // diffuse material
    materialDiffuse[0] = 17.0f / 255.0f; // r
    materialDiffuse[1] = 14.0f / 255.0f; // g
    materialDiffuse[2] = 6.0f / 255.0f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.5f * 128.0f;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glPushMatrix();
    {
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);



        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 31.0f / 255.0f; // r
        materialDiffuse[1] = 17.0f / 255.0f; // g
        materialDiffuse[2] = 3.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 0.5f * 128.0f;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);



        glPushMatrix();
        {
            glBindTexture(GL_TEXTURE_2D, texture_skin);
            glScalef(0.8f, 0.8f, 0.8f);
            glPushMatrix(); // left antena  
            {
                glTranslatef(-0.03f, 0.0f, 0.35f);
                glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);

                gluSphere(quadric, 0.01f, 10, 10);
                gluCylinder(quadric, 0.01f, 0.01f, 0.2f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.2f);
                gluSphere(quadric, 0.01f, 10, 10);
                glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
                glRotatef(-gfRotation_antena, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.01f, 0.01f, 0.15f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.15f);
                gluSphere(quadric, 0.015f, 15, 15);
            }
            glPopMatrix();

            glPushMatrix(); // right antena  
            {
                glTranslatef(0.03f, 0.0f, 0.35f);
                glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
                glRotatef(gfRotation_antena, 1.0f, 0.0f, 0.0f);
                gluSphere(quadric, 0.01f, 10, 10);
                gluCylinder(quadric, 0.01f, 0.01f, 0.2f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.2f);
                gluSphere(quadric, 0.01f, 10, 10);
                glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
                glRotatef(-gfRotation_antena, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.01f, 0.01f, 0.15f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.15f);
                gluSphere(quadric, 0.015f, 15, 15);
            }
            glPopMatrix();

            glPushMatrix(); // head 
            {
                glTranslatef(0.0f, 0.0f, 0.3f);
                glScalef(0.5f, 0.5f, 0.65f);
                gluSphere(quadric, 0.2f, 15, 15);
            }
            glPopMatrix();

            glPushMatrix(); // mid body
            {
                glScalef(0.5f, 0.4f, 0.75f);
                gluSphere(quadric, 0.375f, 15, 15);
            }
            glPopMatrix();

            glPushMatrix(); // left side fore leg
            {
                glTranslatef(-0.05f, 0.125f, 0.15f);
                glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfRotation_leg_1bent, 1.0f, 0.0f, 0.0f);
                gluSphere(quadric, 0.015f, 10, 10);
                gluCylinder(quadric, 0.015f, 0.015f, 0.2f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.2f);
                gluSphere(quadric, 0.015f, 10, 10);
                glRotatef(gfRotation_leg_2bent, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.015f, 0.012f, 0.15f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.15f);
                gluSphere(quadric, 0.012f, 10, 10);
                glRotatef(-40.0f, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.012f, 0.005f, 0.05f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.05f);
                gluSphere(quadric, 0.005f, 10, 10);

            }
            glPopMatrix();

            glPushMatrix(); // right side fore leg
            {
                glTranslatef(0.05f, 0.125f, 0.15f);
                glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfRotation_leg_1bent, 1.0f, 0.0f, 0.0f);
                gluSphere(quadric, 0.015f, 10, 10);
                gluCylinder(quadric, 0.015f, 0.015f, 0.2f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.2f);
                gluSphere(quadric, 0.015f, 10, 10);
                glRotatef(gfRotation_leg_2bent, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.015f, 0.012f, 0.15f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.15f);
                gluSphere(quadric, 0.012f, 10, 10);
                glRotatef(-40.0f, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.012f, 0.005f, 0.05f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.05f);
                gluSphere(quadric, 0.005f, 10, 10);

            }
            glPopMatrix();

            glPushMatrix(); // left side mid leg
            {
                glTranslatef(-0.05f, 0.125f, 0.05f);
                glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfRotation_leg_1bent, 1.0f, 0.0f, 0.0f);
                gluSphere(quadric, 0.015f, 10, 10);
                gluCylinder(quadric, 0.015f, 0.015f, 0.2f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.2f);
                gluSphere(quadric, 0.015f, 10, 10);
                glRotatef(gfRotation_leg_2bent, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.015f, 0.012f, 0.15f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.15f);
                gluSphere(quadric, 0.012f, 10, 10);
                glRotatef(-40.0f, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.012f, 0.005f, 0.05f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.05f);
                gluSphere(quadric, 0.005f, 10, 10);

            }
            glPopMatrix();

            glPushMatrix(); // right side mid leg
            {
                glTranslatef(0.05f, 0.125f, 0.05f);
                glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfRotation_leg_1bent, 1.0f, 0.0f, 0.0f);
                gluSphere(quadric, 0.015f, 10, 10);
                gluCylinder(quadric, 0.015f, 0.015f, 0.2f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.2f);
                gluSphere(quadric, 0.015f, 10, 10);
                glRotatef(gfRotation_leg_2bent, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.015f, 0.012f, 0.15f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.15f);
                gluSphere(quadric, 0.012f, 10, 10);
                glRotatef(-40.0f, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.012f, 0.005f, 0.05f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.05f);
                gluSphere(quadric, 0.005f, 10, 10);

            }
            glPopMatrix();

            glPushMatrix(); // left side back leg
            {
                glTranslatef(-0.05f, 0.125f, -0.125f);
                glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfRotation_leg_1bent, 1.0f, 0.0f, 0.0f);
                gluSphere(quadric, 0.015f, 10, 10);
                gluCylinder(quadric, 0.015f, 0.015f, 0.2f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.2f);
                gluSphere(quadric, 0.015f, 10, 10);
                glRotatef(gfRotation_leg_2bent, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.015f, 0.012f, 0.15f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.15f);
                gluSphere(quadric, 0.012f, 10, 10);
                glRotatef(-40.0f, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.012f, 0.005f, 0.05f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.05f);
                gluSphere(quadric, 0.005f, 10, 10);

            }
            glPopMatrix();

            glPushMatrix(); // right side back leg
            {
                glTranslatef(0.05f, 0.125f, -0.125f);
                glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfRotation_leg_1bent, 1.0f, 0.0f, 0.0f);
                gluSphere(quadric, 0.015f, 10, 10);
                gluCylinder(quadric, 0.015f, 0.015f, 0.2f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.2f);
                gluSphere(quadric, 0.015f, 10, 10);
                glRotatef(gfRotation_leg_2bent, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.015f, 0.012f, 0.15f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.15f);
                gluSphere(quadric, 0.012f, 10, 10);
                glRotatef(-40.0f, 1.0f, 0.0f, 0.0f);
                gluCylinder(quadric, 0.012f, 0.005f, 0.05f, 10, 1);

                glTranslatef(0.0f, 0.0f, 0.05f);
                gluSphere(quadric, 0.005f, 10, 10);

            }
            glPopMatrix();


            glPushMatrix(); // lower body(abdoman)
            {
                glTranslatef(0.0f, 0.0f, -0.7f);
                glScalef(0.5f, 0.5f, 1.5f);
                gluSphere(quadric, 0.3f, 20, 20);
            }
            glPopMatrix();

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT7);



        glBindTexture(GL_TEXTURE_2D, texture_wing);

        glColor3f(1.0f, 1.0f, 1.0f);

        glPushMatrix();
        {
            glRotatef(gfRotation_wings, 0.0f, 0.0f, 1.0f);
            glPushMatrix();
            {
                glTranslatef(-0.05f, 0.0f, 0.0f);
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                glScalef(1.5f, 1.5f, 1.0f);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glBegin(GL_QUADS);


                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(0.0f, 1.0f, 0.0f);

                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-1.0f, 1.0f, 0.0f);

                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-1.0f, -1.0f, 0.0f);


                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(0.0f, -1.0f, 0.0f);

                glEnd();

                glDisable(GL_BLEND);

            }
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glRotatef(-gfRotation_wings, 0.0f, 0.0f, 1.0f);
            glPushMatrix();
            {
                glTranslatef(0.05f, 0.0f, 0.0f);
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                glScalef(1.5f, 1.5f, 1.0f);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


                glBegin(GL_QUADS);


                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(1.0f, 1.0f, 0.0f);

                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(0.0f, 1.0f, 0.0f);

                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(0.0f, -1.0f, 0.0f);

                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(1.0f, -1.0f, 0.0f);

                glEnd();

                glDisable(GL_BLEND);
            }
            glPopMatrix();
        }
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, 0);


    }
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}

void drawCatepillar(void)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();
    {
        GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
        GLfloat lightDiffuse[] = { 0.5f,0.5f,0.5f,1.0f };
        //GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
        GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

        // light related initialization
        glLightfv(GL_LIGHT7, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT7, GL_DIFFUSE, lightDiffuse);
        //glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT7, GL_POSITION, lightPosition);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT7);

        glEnable(GL_TEXTURE_2D);

        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 92.0f / 255.0f; // r
        materialDiffuse[1] = 178.0f / 255.0f; // g
        materialDiffuse[2] = 15.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 128.0f;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        gluQuadricTexture(quadric, GL_TRUE);

        glBindTexture(GL_TEXTURE_2D, texture_skin);
        glColor3f(0.0f, 0.5f, 0.5f);
        glPushMatrix();
        {

            glPushMatrix();
            {
                //glTranslatef(0.0f, 0.0f, 0.25f);
                gluSphere(quadric, 0.5f, 20, 20);
            }
            glPopMatrix();

            for (int i = 0; i < 10; i++)
            {
                // ambient material
                materialAmbient[0] = 0.0f; // r
                materialAmbient[1] = 0.0f; // g
                materialAmbient[2] = 0.0f; // b
                materialAmbient[3] = 1.0f; // a
                glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                // diffuse material
                materialDiffuse[0] = 92.0f / 255.0f; // r
                materialDiffuse[1] = 178.0f / 255.0f; // g
                materialDiffuse[2] = 15.0f / 255.0f; // b
                materialDiffuse[3] = 1.0f; // a
                glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

                // shininess
                materialShininess = 128.0f;
                glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                if (i > 3 && i < 7)
                {
                    glTranslatef(0.0f, gfWalk_movement, -(0.3f + gfWalk_movement));
                }
                else
                {
                    glTranslatef(0.0f, -gfWalk_movement, -(0.3f + gfWalk_movement));
                }



                gluCylinder(quadric, 0.45f, 0.475f, 0.15f, 20, 1);
                // right side leg
                glPushMatrix();
                {
                    glRotatef(-50.0f, 0.0f, 0.0f, 1.0f);
                    glPushMatrix();
                    {
                        // ambient material
                        materialAmbient[0] = 0.0f; // r
                        materialAmbient[1] = 0.0f; // g
                        materialAmbient[2] = 0.0f; // b
                        materialAmbient[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                        // diffuse material
                        materialDiffuse[0] = 167.0f / 255.0f; // r
                        materialDiffuse[1] = 198.0f / 255.0f; // g
                        materialDiffuse[2] = 142.0f / 255.0f; // b
                        materialDiffuse[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

                        //// specular material
                        //materialSpecular[0] = 0.7f; // r
                        //materialSpecular[1] = 0.04f; // g
                        //materialSpecular[2] = 0.04f; // b
                        //materialSpecular[3] = 1.0f; // a
                        //glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

                        // shininess
                        materialShininess = 128.0f;
                        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                        glTranslatef(0.5f, 0.0f, 0.15f);
                        glPushMatrix();
                        {
                            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
                            glRotatef(gfRotation, 0.0f, 1.0f, 0.0f);
                            gluSphere(quadric, 0.1f, 10, 10);
                            gluCylinder(quadric, 0.1f, 0.06f, 0.15f, 10, 1);

                            glTranslatef(0.0f, 0.0f, 0.15f);
                            gluSphere(quadric, 0.06f, 10, 10);

                            glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
                            gluCylinder(quadric, 0.06f, 0.02f, 0.15f, 5, 1);
                            glTranslatef(0.0f, 0.0f, 0.15f);
                            gluSphere(quadric, 0.02f, 5, 5);
                        }
                        glPopMatrix();
                    }
                    glPopMatrix();
                }
                glPopMatrix();
                // left side leg
                glPushMatrix();
                {
                    glRotatef(50.0f, 0.0f, 0.0f, 1.0f);
                    glPushMatrix();
                    {
                        // ambient material
                        materialAmbient[0] = 0.0f; // r
                        materialAmbient[1] = 0.0f; // g
                        materialAmbient[2] = 0.0f; // b
                        materialAmbient[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                        // diffuse material
                        materialDiffuse[0] = 167.0f / 255.0f; // r
                        materialDiffuse[1] = 198.0f / 255.0f; // g
                        materialDiffuse[2] = 142.0f / 255.0f; // b
                        materialDiffuse[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

                        // shininess
                        materialShininess = 128.0f;
                        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                        glTranslatef(-0.5f, 0.0f, 0.15f);
                        glPushMatrix();
                        {
                            glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
                            glRotatef(gfRotation, 0.0f, 1.0f, 0.0f);
                            gluSphere(quadric, 0.1f, 10, 10);
                            gluCylinder(quadric, 0.1f, 0.06f, 0.15f, 10, 1);

                            glTranslatef(0.0f, 0.0f, 0.15f);
                            gluSphere(quadric, 0.06f, 10, 10);

                            glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
                            gluCylinder(quadric, 0.06f, 0.02f, 0.15f, 5, 1);
                            glTranslatef(0.0f, 0.0f, 0.15f);
                            gluSphere(quadric, 0.02f, 5, 5);
                        }
                        glPopMatrix();
                    }
                    glPopMatrix();
                }
                glPopMatrix();

                glTranslatef(0.0f, 0.0f, -(0.1f + gfWalk_movement));
                gluSphere(quadric, 0.5f, 20, 20);
            }
            glPopMatrix();
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHT7);
        glDisable(GL_LIGHTING);
    }
    glPopMatrix();

}

void rainTypeFour(void)
{
	for (GLint i = 0; i < 10000; i++)
	{
		GLfloat x = (GLfloat)(rand() % 300) - 150.0f;
		GLfloat y = (GLfloat)(rand() % 100) - 10.0f;
		GLfloat z = (GLfloat)(rand() % 300) - 150.0f;

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, texture_waterDrop);
		glColor3f(222.0f / 255.0f, 244.0f / 255.0f, 1.0f);
		
		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y, z);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x - 0.5f, y, z);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x - 0.5f, y - 0.5f, z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x, y - 0.5f, z);

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}

void updateScene4(void)
{
	// for terrain
	updateScene();

	// for temple
	TempleUpdate();

    if (selectedScene == 4)
    {
        if (gfTranslatefCatepillar < 1.4f)
        {
            gfTranslatefCatepillar = gfTranslatefCatepillar + 0.0007f;
        }

        if (gbWalkAnimation_catepillar == FALSE)
        {
            if (gfWalk_movement <= 0.05f)
            {
                gfWalk_movement = gfWalk_movement + 0.0005f;
            }
            if (gfRotation <= 10.0f)
            {
                gfRotation = gfRotation + 0.8f;
            }
            else
            {
                gbWalkAnimation_catepillar = TRUE;
            }
        }
        else
        {
            if (gfWalk_movement >= 0.0f)
            {
                gfWalk_movement = gfWalk_movement - 0.0005f;
            }
            if (gfRotation >= -10.0f)
            {
                gfRotation = gfRotation - 0.8f;
            }
            else
            {
                gbWalkAnimation_catepillar = FALSE;
            }
        }

        if (gbButterfly_wingAnimtaion == FALSE)
        {
            if (gfRotation_antena <= 20.0f)
            {
                gfRotation_antena = gfRotation_antena + 0.5f;
            }
            if (gfRotation_wings <= 80.0f)
            {
                gfRotation_wings = gfRotation_wings + 8.0f;
            }
            else
            {
                gbButterfly_wingAnimtaion = TRUE;
            }
        }
        else
        {
            if (gfRotation_antena >= 0.0f)
            {
                gfRotation_antena = gfRotation_antena - 0.5f;
            }

            if (gfRotation_wings >= 0.0f)
            {
                gfRotation_wings = gfRotation_wings - 8.0f;
            }
            else
            {
                gbButterfly_wingAnimtaion = FALSE;
            }
        }

        if (gbButterfly_legAnimation == FALSE)
        {


            if (gfRotation_leg_2bent >= 30.0f)
            {
                gfRotation_leg_2bent = gfRotation_leg_2bent - 2.0f;
            }

            if (gfRotation_leg_1bent <= 260.0f)
            {
                gfRotation_leg_1bent = gfRotation_leg_1bent + 0.5f;
            }
            else
            {
                gbButterfly_legAnimation = TRUE;
            }
        }
        else
        {
            if (gfRotation_leg_2bent <= 100.0f)
            {
                gfRotation_leg_2bent = gfRotation_leg_2bent + 2.0f;
            }


            if (gfRotation_leg_1bent >= 230.0f)
            {
                gfRotation_leg_1bent = gfRotation_leg_1bent - 0.5f;
            }
            else
            {
                gbButterfly_legAnimation = FALSE;
            }
        }



        if (gbRotateButterfly == FALSE)
        {

            if (gfRotateButterflyAngle <= 20.0f)
            {
                gfRotateButterflyAngle = gfRotateButterflyAngle + 0.5f;
            }
            if (gfRotate_Y <= 50.0f)
            {
                gfRotate_Y = gfRotate_Y + 1.0f;
            }
            else
            {
                gbRotateButterfly = TRUE;
            }
        }
        else
        {

            if (gfRotateButterflyAngle >= -20.0f)
            {
                gfRotateButterflyAngle = gfRotateButterflyAngle - 0.5f;
            }

            if (gfRotate_Y >= -20.0f)
            {
                gfRotate_Y = gfRotate_Y - 1.0f;
            }
            else
            {
                gbRotateButterfly = FALSE;
            }
        }
    }

    if (gbThunderEnable)
    {
        static int i = 0;
        if (i < 2)
        {
            if (thunder_alpha > 0.0f)
            {
                thunder_alpha = thunder_alpha - 0.05f;
            }
            else
            {
               /* gbThunderEnable = FALSE;*/
                i = i + 1;
                if (i < 2)
                {
                    thunder_alpha = 1.0f;
                }
                
            }
        }
        else
        {
            gbThunderEnable = FALSE;
        }
    }
}



void uninitializeScene4(void)
{

	// for farm
	FarmUninitialize();

	// for temple
	TempleUninitialize();

	// for terrain
	uninitializeScene();

    if (texture_thunder)
    {
        glDeleteTextures(1, &texture_thunder);
        texture_thunder = 0;
    }

    if (texture_wing)
    {
        glDeleteTextures(1, &texture_wing);
        texture_wing = 0;
    }

    if (texture_skin)
    {
        glDeleteTextures(1, &texture_skin);
        texture_skin = 0;
    }

	if (texture_waterDrop)
	{
		glDeleteTextures(1,&texture_waterDrop);
		texture_waterDrop = 0;
	}

}