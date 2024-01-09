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

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

// Below is the code related to Project

int timer = 0; // use as iterative variable for accessing BazierPoints elements
bool bEnableBazierCamera = true; // to control camera movement on Key = 'V'/'v'

std::vector<float> BazierPoints;

float controlPoints[4][3] = {
    {0.0f,15.0f,-15.0f},
    {0.0f,10.0f,-5.0f},
    {0.0f,5.0f,15.0f},
    {0.0f,0.0f,10.0f},
    
};

// for camera
GLfloat gfHorizontal = 0.0f;
GLfloat gfVertical = 0.0f;
GLfloat gfDepth = 10.0f;

// for camera
GLfloat gfCameraHorizontal = 0.0f;
GLfloat gfCameraVertical = 0.0f;
GLfloat gfCameraDepth = 0.0f;


#pragma region TEXTURE

BOOL loadGLTexture_BMP(GLuint *, TCHAR[]);

BOOL loadPNGTexture(GLuint *texture, const char *image);

GLuint texture_marathiLetters = 0;
GLuint texture_kites = 0;
GLuint texture_corn = 0;
GLuint texture_motherAndBabyPlaying = 0;
GLuint texture_sandClock = 0;
GLuint texture_baby = 0;
GLuint texture_moonInNight = 0;
GLuint texture_birdFlockWithBackground = 0;

#pragma endregion

#pragma region CORN SCENE

 GLfloat gfcornScene = 0.0f;

void cornScene(void);

#pragma endregion

#pragma region KITE SCENE

 GLfloat gfKiteScene = 0.0f;

void kiteScene(void);

#pragma endregion

#pragma region MOTHER AND SON SCENE 

 GLfloat gfmotherAndBabyScene = 0.0f;

void motherAndSonScene(void);

#pragma endregion

#pragma region CLOCK SCENE

 GLfloat gfsandClockScene = 0.0f;

void clockScene(void);

#pragma endregion

#pragma region MOON SCENE

 GLfloat gfmoonScene = 0.0f;

void moonScene(void);

#pragma endregion

#pragma region BIRD SCENE

 GLfloat gfbirdFlockWithBackgroundScene = 0.0f;

void birdScene(void);

#pragma endregion 

#pragma region marathi Letter

 GLfloat gfAngle = 0.0f; // angle for rotation 0.0f to 360.0f

 GLfloat gfTranslate = 2.0f; // for translate 

void marathiLetterScene(void);

#pragma endregion



//scenes
#define SCENE_TIME1 12		// scene duration in SECONDS
#define SCENE_TIME2 44
#define SCENE_TIME3 50
#define SCENE_TIME4 54
#define SCENE_TIME5 61
#define SCENE_TIME6 80

extern float elapseTime;

BOOL bScene1_Done = FALSE;

#pragma region FADE OUT SCENE

float fadeOut_scene_alpha = 0.0f;
float fadeIn_scene_alpha = 1.0f;

void resetFadeValue(void);

#pragma endregion


extern FILE *gpFILE;

// variable declaration

// for gluSphere
GLUquadric *quadric;

#pragma region LIGHT
GLfloat lightAmbient[] = { 0.5f,0.5f,0.5f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,10.0f,25.0f,1.0f };

BOOL gbLight = FALSE;

#pragma endregion

BOOL switchCameraToMovement = FALSE;


int initializeScene(void)
{
    BOOL bResult;

    bResult = loadPNGTexture(&texture_marathiLetters, "marathiMulakshare.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadPNGTexture() for texture marathi letters failed...!\n");
        return(-7); 
    }

    bResult = loadPNGTexture(&texture_kites, "kites.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadPNGTexture() for texture kites failed...!\n");
        return(-8); 
    }

    bResult = loadPNGTexture(&texture_corn, "corn.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadPNGTexture() for texture corn failed...!\n");
        return(-9);  
    }

    bResult = loadPNGTexture(&texture_baby, "baby.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadPNGTexture() for texture baby failed...!\n");
        return(-10);  
    }

    bResult = loadPNGTexture(&texture_birdFlockWithBackground, "birdFlockWithBackground.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadPNGTexture() for texture birdFlockWithBackground failed...!\n");
        return(-11);
    }

    bResult = loadPNGTexture(&texture_moonInNight, "moon.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadPNGTexture() for texture moon failed...!\n");
        return(-13);
    }

    bResult = loadPNGTexture(&texture_motherAndBabyPlaying, "motherAndBabyPlaying.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadPNGTexture() for texture motherAndBabyPlaying failed...!\n");
        return(-15);
    }

    bResult = loadPNGTexture(&texture_sandClock, "sandClock.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadPNGTexture() for texture sandClock failed...!\n");
        return(-17);
    }

    // enable texture
    glEnable(GL_TEXTURE_2D);

    quadric = gluNewQuadric();

    for (float t = 0.0f; t <= 1.0f; t = t + 0.002f)
    {
        float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
        float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
        float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

        BazierPoints.push_back(x);
        BazierPoints.push_back(y);
        BazierPoints.push_back(z);
    }

    // for light 
    // light related initialization
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

    glEnable(GL_LIGHT1);

    return(0);
}

void displayScene(void)
{
    //function declaration
    void globalCube(void);

     

    if ((timer + 3) + 4 < BazierPoints.size())
    {
        timer = timer + 3;
    }
    else
    {
        //timer = 0;
        
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

    glColor3f(1.0f, 1.0f, 1.0f);

    if (elapseTime <= SCENE_TIME6)
    {
        if (elapseTime <= SCENE_TIME1)
        {
            if (gbLight == FALSE)
            {
                glEnable(GL_LIGHTING);
                gbLight = TRUE;
            }
        }
        globalCube();
    }
    

    if (bScene1_Done == FALSE)
    {
        if (elapseTime < SCENE_TIME1)
        {
            marathiLetterScene();
        }
        else if (elapseTime <= SCENE_TIME2 && elapseTime >= SCENE_TIME1)
        {

            glPushMatrix();
            {
                glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
                glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
               
                glPushMatrix();
                {
                    glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
                    glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
                    glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
                    glTranslatef(-2.0f, 0.0f, 0.0f);
                    motherAndSonScene();
                }
                glPopMatrix();


                glPushMatrix();
                {
                    glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
                    glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
                    glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
                    glTranslatef(2.0f, 0.0f, 0.0f);
                    cornScene();
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        else if (elapseTime <= SCENE_TIME3 && elapseTime >= SCENE_TIME2)
        {

            glPushMatrix();
            {
                glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
                glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
                glPushMatrix();
                {
                    glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
                    glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
                    glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
                    glTranslatef(-2.0f, 0.0f, 0.0f);
                    kiteScene();
                }
                glPopMatrix();


                glPushMatrix();
                {
                    glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
                    glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
                    glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
                    glTranslatef(2.0f, 0.0f, 0.0f);
                    moonScene(); 
                }
                glPopMatrix();
            }
            glPopMatrix();

        }
        else if (elapseTime <= SCENE_TIME4 && elapseTime >= SCENE_TIME3)
        {

            glPushMatrix();
            {
                glRotatef(gfAngle - 0.03f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfAngle - 0.01f, 0.0f, 1.0f, 0.0f);
                glRotatef(gfAngle - 0.05f, 1.0f, 0.0f, 0.0f);
                glPushMatrix();
                {
                    glRotatef(gfAngle - 0.03f, 0.0f, 0.0f, 1.0f);
                    glRotatef(gfAngle - 0.01f, 0.0f, 1.0f, 0.0f);
                    glRotatef(gfAngle - 0.05f, 1.0f, 0.0f, 0.0f);
                    glTranslatef(2.0f, 0.0f, 0.0f);
                    clockScene();
                }
                glPopMatrix();


                glPushMatrix();
                {
                    glRotatef(gfAngle - 0.03f, 0.0f, 0.0f, 1.0f);
                    glRotatef(gfAngle - 0.01f, 0.0f, 1.0f, 0.0f);
                    glRotatef(gfAngle - 0.05f, 1.0f, 0.0f, 0.0f);
                    glTranslatef(-2.0f, 0.0f, 0.0f);
                    birdScene();
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        else if (elapseTime <= SCENE_TIME5 && elapseTime >= SCENE_TIME4)
        {
            // show Cube only 

            /*glPushMatrix();
            {
                glRotatef(gfAngle, 0.0f, 0.0f, 1.0f);
                glRotatef(gfAngle, 0.0f, 1.0f, 0.0f);
                glRotatef(gfAngle, 1.0f, 0.0f, 0.0f);
                glPushMatrix();
                {
                    glRotatef(gfAngle, 0.0f, 0.0f, 1.0f);
                    glRotatef(gfAngle, 0.0f, 1.0f, 0.0f);
                    glRotatef(gfAngle, 1.0f, 0.0f, 0.0f);
                    glTranslatef(2.0f, 0.0f, 0.0f);
                    motherAndSonScene();
                }
                glPopMatrix();


                glPushMatrix();
                {
                    glRotatef(gfAngle, 0.0f, 0.0f, 1.0f);
                    glRotatef(gfAngle, 0.0f, 1.0f, 0.0f);
                    glRotatef(gfAngle, 1.0f, 0.0f, 0.0f);
                    glTranslatef(-2.0f, 0.0f, 0.0f);
                    cornScene();
                }
                glPopMatrix();
            }
            glPopMatrix();*/

        }
        else if (elapseTime <= SCENE_TIME6 && elapseTime >= SCENE_TIME5)
        {

        glPushMatrix();
        {
            glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
            glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
            glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
            glPushMatrix();
            {
                glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
                glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
                glTranslatef(-2.0f, 0.0f, 0.0f);
                motherAndSonScene();
            }
            glPopMatrix();


            glPushMatrix();
            {
                glRotatef(gfAngle - 0.15f, 0.0f, 0.0f, 1.0f);
                glRotatef(gfAngle - 0.12f, 0.0f, 1.0f, 0.0f);
                glRotatef(gfAngle - 0.13f, 1.0f, 0.0f, 0.0f);
                glTranslatef(2.0f, 0.0f, 0.0f);
                cornScene();
            }
            glPopMatrix();

           
        }
        glPopMatrix();

        glPushMatrix();
        {
            if (elapseTime >= SCENE_TIME6 - 2.0f)
            {
                if (gbLight == TRUE)
                {
                    glDisable(GL_LIGHTING);
                    gbLight = FALSE;
                }
                

                if (fadeOut_scene_alpha <= 1.0f)
                {
                    fadeOut_scene_alpha = fadeOut_scene_alpha + 0.008f;
                }

                /*      glLoadIdentity();*/
                glTranslatef(0.0f, 0.0f, 4.0f);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glBegin(GL_QUADS);
                glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_alpha);

                glVertex3f(10.0f, 10.0f, 0.0f);
                glVertex3f(-10.0f, 10.0f, 0.0f);
                glVertex3f(-10.0f, -10.0f, 0.0f);
                glVertex3f(10.0f, -10.0f, 0.0f);

                glEnd();

                glDisable(GL_BLEND);

            }
        }
        glPopMatrix();

        }

    }

}


void updateScene(void)
{
    gfAngle = gfAngle + 0.3f;
    if (gfAngle >= 360.0f)
    {
        gfAngle = 0.0f;
    }
    
}

void uninitializeScene(void)
{
    if (quadric)
    {
        gluDeleteQuadric(quadric);
        quadric = NULL;
    }

    if (texture_marathiLetters)
    {
        glDeleteTextures(1, &texture_marathiLetters);
        texture_marathiLetters = 0;
    }

    if (texture_baby)
    {
        glDeleteTextures(1, &texture_baby);
        texture_baby = 0;
    }

    if (texture_birdFlockWithBackground)
    {
        glDeleteTextures(1, &texture_birdFlockWithBackground);
        texture_birdFlockWithBackground = 0;
    }

    if (texture_corn)
    {
        glDeleteTextures(1, &texture_corn);
        texture_corn = 0;
    }

    if (texture_kites)
    {
        glDeleteTextures(1, &texture_kites);
        texture_kites = 0;
    }

    if (texture_moonInNight)
    {
        glDeleteTextures(1, &texture_moonInNight);
        texture_moonInNight = 0;
    }

    if (texture_sandClock)
    {
        glDeleteTextures(1, &texture_sandClock);
        texture_sandClock = 0;
    }

    if (texture_motherAndBabyPlaying)
    {
        glDeleteTextures(1, &texture_motherAndBabyPlaying);
        texture_motherAndBabyPlaying = 0;
    }

}

// ********* TEXTURE RELATED FUNCTION ************

BOOL loadGLTexture_BMP(GLuint *texture, TCHAR imageResourceID[])
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

BOOL loadPNGTexture(GLuint *texture, const char *image)
{
    // variable declarations
    int iWidth, iHeight, iComponents;

    // stbi_set_flip_vertically_on_load(1);
    unsigned char *Data = stbi_load(image, &iWidth, &iHeight, &iComponents, 0);

    if (!Data)
    {
        //fprintf(gpFILE, "loadPNGTexture() failed for %s: ", image);
        stbi_image_free(Data);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glGenTextures(1, texture);

        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (iComponents == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
        }
        else if (iComponents == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
        }

        glBindTexture(GL_TEXTURE_2D, 0); 	// unbind texture

        // delete objext 
        stbi_image_free(Data);
        return TRUE;
    }
    return FALSE;
}


// ************ Scene related function ***********

void globalCube(void)
{

    glPushMatrix();
    {
        glRotatef(gfAngle, 1.0f, 0.0f, 0.0f);
        glRotatef(gfAngle, 0.0f, 1.0f, 0.0f);
        glRotatef(gfAngle, 0.0f, 0.0f, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture(GL_TEXTURE_2D, texture_baby);

        glBegin(GL_QUADS);

        //front face
        //glColor3f(1.0f, 0.0f, 0.0f);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(20.0f, 20.0f, 20.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-20.0f, 20.0f, 20.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-20.0f, -20.0f, 20.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(20.0f, -20.0f, 20.0f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, texture_baby);

        glBegin(GL_QUADS);

        // right face
        //glColor3f(0.0f, 1.0f, 0.0f);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(20.0f, 20.0f, -20.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(20.0f, 20.0f, 20.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(20.0f, -20.0f, 20.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(20.0f, -20.0f, -20.0f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, texture_baby);

        glBegin(GL_QUADS);

        // back face
        //glColor3f(0.0f, 0.0f, 1.0f);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-20.0f, 20.0f, -20.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(20.0f, 20.0f, -20.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(20.0f, -20.0f, -20.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-20.0f, -20.0f, -20.0f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, texture_baby);

        glBegin(GL_QUADS);

        // left face
        //glColor3f(0.0f, 1.0f, 1.0f);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-20.0f, 20.0f, 20.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-20.0f, 20.0f, -20.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-20.0f, -20.0f, -20.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-20.0f, -20.0f, 20.0f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, texture_baby);

        glBegin(GL_QUADS);

        // top face
        //glColor3f(1.0f, 0.0f, 1.0f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(20.0f, 20.0f, -20.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-20.0f, 20.0f, -20.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-20.0f, 20.0f, 20.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(20.0f, 20.0f, 20.0f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, texture_baby);

        glBegin(GL_QUADS);

        // bottom face
        //glColor3f(1.0f, 1.0f, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(20.0f, -20.0f, -20.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-20.0f, -20.0f, -20.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-20.0f, -20.0f, 20.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(20.0f, -20.0f, 20.0f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_BLEND);
    }
    glPopMatrix();
}


void marathiLetterScene(void)
{
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 1.0f);

        glRotatef(gfAngle, 1.0f, 0.0f, 0.0f);
        glRotatef(gfAngle, 0.0f, 1.0f, 0.0f);
        glRotatef(gfAngle, 0.0f, 0.0f, 1.0f);

        glPushMatrix();
        {
            for (int i = 1; i <= 10; i++)
            {
                glRotatef(gfAngle, 1.0f, 0.0f, 0.0f);
                glRotatef(gfAngle, 0.0f, 1.0f, 0.0f);
                glRotatef(gfAngle, 0.0f, 0.0f, 1.0f);

                 glEnable(GL_BLEND);
                 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glPushMatrix();
                {
                    glTranslatef(gfTranslate * ((GLfloat)i), 0.0f, 0.0f);

                    glBindTexture(GL_TEXTURE_2D, texture_marathiLetters);

                    glBegin(GL_QUADS);

                    glNormal3f(0.0f, 0.0f, 1.0f);
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
                }
                glPopMatrix();

                glPushMatrix();
                {
                    glTranslatef(-gfTranslate * ((GLfloat)i), 0.0f, 0.0f);
                    glBindTexture(GL_TEXTURE_2D, texture_marathiLetters);

                    glBegin(GL_QUADS);

                    glNormal3f(0.0f, 0.0f, 1.0f);
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
                }
                glPopMatrix();

                glPushMatrix();
                {
                    glTranslatef(0.0f, gfTranslate * ((GLfloat)i), 0.0f);
                    glBindTexture(GL_TEXTURE_2D, texture_marathiLetters);

                    glBegin(GL_QUADS);

                    glNormal3f(0.0f, 0.0f, 1.0f);
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
                }
                glPopMatrix();

                glPushMatrix();
                {
                    glTranslatef(0.0f, -gfTranslate * ((GLfloat)i), 0.0f);

                    glBindTexture(GL_TEXTURE_2D, texture_marathiLetters);

                    glBegin(GL_QUADS);

                    glNormal3f(0.0f, 0.0f, 1.0f);
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
                }
                glPopMatrix();

                glPushMatrix();
                {
                    glTranslatef(0.0f, 0.0f, gfTranslate * ((GLfloat)i));
                    glBindTexture(GL_TEXTURE_2D, texture_marathiLetters);

                    glBegin(GL_QUADS);

                    glNormal3f(0.0f, 0.0f, 1.0f);
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
                }
                glPopMatrix();

                glPushMatrix();
                {
                    glTranslatef(0.0f, 0.0f, -gfTranslate * ((GLfloat)i));

                    glBindTexture(GL_TEXTURE_2D, texture_marathiLetters);

                    glBegin(GL_QUADS);

                    glNormal3f(0.0f, 0.0f, 1.0f);
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
                }
                glPopMatrix();

                glDisable(GL_BLEND);
            }

        }
        glPopMatrix();
    }
    glPopMatrix();

}

void cornScene(void)
{
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f, gfcornScene);

        glBindTexture(GL_TEXTURE_2D, texture_corn);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
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
    }
    glPopMatrix();
}

void kiteScene(void)
{
    glPushMatrix();
    {
        glTranslatef(0.0f, gfKiteScene, 0.0f);

        glBindTexture(GL_TEXTURE_2D, texture_kites);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
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
    }
    glPopMatrix();
}

void motherAndSonScene(void)
{
    glPushMatrix();
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTranslatef(gfmotherAndBabyScene, 0.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, texture_motherAndBabyPlaying);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
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

    }
    glPopMatrix();

    glDisable(GL_BLEND);
}

void clockScene(void)
{
    
    glPushMatrix();
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTranslatef(0.0f, 0.0f, gfsandClockScene);

        glBindTexture(GL_TEXTURE_2D, texture_sandClock);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
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

    }
    glPopMatrix();

    
    glDisable(GL_BLEND);
}

void birdScene(void)
{
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f, gfbirdFlockWithBackgroundScene);

        glBindTexture(GL_TEXTURE_2D, texture_birdFlockWithBackground);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
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

    }
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_BLEND);
}

void moonScene(void)
{
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f, gfmoonScene);

        glBindTexture(GL_TEXTURE_2D, texture_moonInNight);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
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
    }
    glPopMatrix();
}

void resetFadeValue(void)
{
    fadeOut_scene_alpha = 0.0f;
}