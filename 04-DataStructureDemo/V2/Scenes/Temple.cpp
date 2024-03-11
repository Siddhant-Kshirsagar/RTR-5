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

// User Header files
#include"Temple.h"

GLUquadric *templeQuadric;

//// for animation
GLfloat tAngle =0.0f; 

// for texture
GLuint texture_stone = 0;
GLuint texture_rope = 0;
GLuint texture_gold = 0;
GLuint texture_marble = 0;
GLuint texture_wall_1 = 0;
GLuint texture_wall_2 = 0;
GLuint texture_shreeGanesh = 0;
GLuint texture_invertedTree = 0;
GLuint texture_shreeVishnu = 0;
GLuint texture_tripund = 0;
GLuint texture_black = 0;
extern GLuint texture_whiteCloth;
GLuint texture_jyot = 0;
GLuint texture_bush = 0;
GLuint texture_belLeaves = 0;
extern GLuint texture_leaves;
static GLUquadric *quadric;


BOOL gbJyotAnimation = TRUE;
GLfloat gfJyotRotation = 0.0f;

BOOL bIsTrunk = TRUE;
extern GLuint texture_plant;
GLuint texture_wood = 0;
GLUquadric *commonQuadric;

float leafChangeFactor = 0.001f;
float rotationAngle = 0.0f;
int randomAxis = 0;
MY_POSITION myPosition[12];
BOOL gbFALLEnable = TRUE;
extern BOOL scene1_Two;
extern BOOL scene4_Three;

struct MYCOLOR
{
    float fRed;
    float fGreen;
    float fBlue;
};

typedef struct MYCOLOR myColor;

extern FILE *gpFILE;

extern BOOL scene4_Two;
extern BOOL scene4_One;


int TempleInitialize(void)
{   
    //variable declaration
    BOOL bResult = FALSE;
   

    // initialize quadric
    templeQuadric = gluNewQuadric();
    quadric = gluNewQuadric();

    // light related initialization
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glLightfv(GL_LIGHT5, GL_AMBIENT, lightAmbientOne);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, lightDiffuseOne);
    //glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecularOne);
    glLightfv(GL_LIGHT5, GL_POSITION, lightPositionOne);

    // Texture initialization
    bResult = loadGLTexture(&texture_stone,MAKEINTRESOURCE(MY_STONE_BITMAP));
    if(bResult == FALSE)
    {
        fprintf(gpFILE,"loadGLTexture() Failed \n");
        return(-1);
    }

    bResult = loadGLTexture(&texture_rope, MAKEINTRESOURCE(MY_ROPE_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-2);
    }

    bResult = loadGLTexture(&texture_gold, MAKEINTRESOURCE(MY_GOLD_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-3);
    }

    bResult = loadGLTexture(&texture_marble, MAKEINTRESOURCE(MY_MARBLE_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-3);
    }

    bResult = loadGLTexture(&texture_wall_1, MAKEINTRESOURCE(MY_WALL_1_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-4);
    }

    bResult = loadGLTexture(&texture_wall_2, MAKEINTRESOURCE(MY_WALL_2_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-5);
    }

    bResult = loadGLTexture(&texture_shreeGanesh, MAKEINTRESOURCE(MY_SHREEGANESH_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-6);
    }

    bResult = loadGLTexture(&texture_invertedTree, MAKEINTRESOURCE(MY_INVERTEDTREE));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-7);
    }

    bResult = loadGLTexture(&texture_shreeVishnu, MAKEINTRESOURCE(MY_SHREEVISHNU_1_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-8);
    }

    bResult = loadGLTexture(&texture_tripund, MAKEINTRESOURCE(MY_TRIPUNDBITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-8);
    }

    bResult = loadGLTexture(&texture_black, MAKEINTRESOURCE(MY_BLACKBITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-8);
    }

    bResult = loadGLPNGTexture(&texture_jyot, "./Assets/Texture/flame.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-1);
    }



    bResult = loadGLTexture(&texture_wood, MAKEINTRESOURCE(MY_BITMAP_TREE));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-1);
    }

    bResult = loadGLPNGTexture(&texture_bush, "./Assets/Texture/bush.png");
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLPNGTexture() failed\n");
        return(-5);
    }

    // to generate number of leaves at different position
    for (int i = 0; i < 12; i++)
    {
        float x = (float)((rand() % 4) - 3.0f);
        float z = (float)((rand() % 4) - 3.0f);

        myPosition[i].xAxis = x;
        myPosition[i].yAxis = 0.0f;
        myPosition[i].zAxis = z;
    }



    return(0);
}


void TempleDisplay(void)
{
    // function declaration
    void drawTemple(void);
    void drawPlant(void);
    void drawDhatura(void);
    void drawTrees(void);
    void drawCylinder(GLfloat base, GLfloat top, GLfloat height);
    void drawGrass(void);

    // code
    glPushMatrix();
    {
        if (selectedScene == 5)
        {
            animationStart = TRUE;
        }
        else
        {
            animationStart = FALSE;
        }
        glPushMatrix();
        {
            glEnable(GL_LIGHT5);
            drawTemple();
            glDisable(GL_LIGHT5);
        }
        glPopMatrix();

            glPushMatrix();
            {
                drawTrees();
            }
            glPopMatrix();

            glPushMatrix();
            {
                glPushMatrix();
                {
                    glTranslatef(-5.5f, 0.0f, 7.0f);
                    float rotate = 0.0f;
                    for (int i = 0; i < 12; i++)
                    {
                        glPushMatrix();
                        {
                            glTranslatef(myPosition[i].xAxis, myPosition[i].yAxis, myPosition[i].zAxis);
                            glRotatef(rotate, 0.0f, 1.0f, 0.0f);
                            glScalef(0.2f, 0.2f, 0.2f);
                            drawPlant();
                        }
                        glPopMatrix();

                        rotate = rotate + 30.0f;
                    }
                }
                glPopMatrix();

                glPushMatrix();
                {
                    GLfloat rotate = 0.0f;
                    glTranslatef(5.0f, 0.0f, 3.0f);
                    for (int i = 0; i < 12; i++)
                    {
                        glPushMatrix();
                        {
                            glTranslatef(5.0f , 0.0f, 3.0f * (i % 3));
                            glTranslatef(myPosition[i].xAxis, myPosition[i].yAxis, myPosition[i].zAxis);
                            glRotatef(rotate, 0.0f, 1.0f, 0.0f);
                            glScalef(0.2f, 0.2f, 0.2f);
                            drawPlant();
                        }
                        glPopMatrix();
                        rotate = rotate + 30.0f;
                    }
                }
                glPopMatrix();
            }
            glPopMatrix();

            if (selectedScene == 2)
            {
                glPushMatrix();
                {

                    glTranslatef(-1.0f, 0.601f, -2.5f);
                    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
                    glPushMatrix();
                    {
                        glScalef(0.1f, 0.1f, 0.1f);
                        drawPanati();
                    }
                    glPopMatrix();
                    glTranslatef(0.5f, 0.0f, 0.0f);
                    glPushMatrix();
                    {
                        glScalef(0.1f, 0.1f, 0.1f);
                        drawPanati();
                    }
                    glPopMatrix();

                    glTranslatef(0.5f, 0.0f, 0.0f);
                    glPushMatrix();
                    {
                        glScalef(0.1f, 0.1f, 0.1f);
                        drawPanati();
                    }
                    glPopMatrix();

                    glTranslatef(0.5f, 0.0f, 0.0f);
                    glPushMatrix();
                    {
                        glScalef(0.1f, 0.1f, 0.1f);
                        drawPanati();
                    }
                    glPopMatrix();
                    glTranslatef(0.5f, 0.0f, 0.0f);
                    glPushMatrix();
                    {
                        glScalef(0.1f, 0.1f, 0.1f);
                        drawPanati();
                    }
                    glPopMatrix();
                }
                glPopMatrix();
            }
    }
    glPopMatrix();
}

void TempleUpdate(void)
{
 
    // code
    if(animationStart)
    {
        tAngle = tAngle + 1.0f;
        if(tAngle >= 360.0f)
        {
            tAngle = 0.0f;
        }
    }

    if (gbFALLEnable)
    {
        rotationAngle = rotationAngle + 1.0f;
        if (rotationAngle >= 360.0f)
        {
            rotationAngle = 0.0f;
        }
    }


    if (gbJyotAnimation == FALSE)
    {
        if (gfJyotRotation <= 10.0f)
        {
            gfJyotRotation = gfJyotRotation + 0.5f;
        }
        else
        {
            gbJyotAnimation = TRUE;
        }

    }
    else
    {
        if (gfJyotRotation >= -10.0f)
        {
            gfJyotRotation = gfJyotRotation - 0.5f;
        }
        else
        {
            gbJyotAnimation = FALSE;
        }
    }
    
}

void drawPlant(void)
{
    // code
    GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
    GLfloat lightDiffuse[] = { 0.25f,0.25f,0.25f,1.0f };
    //GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
    GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

    // light related initialization
    glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDiffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPosition);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT3);
    glEnable(GL_TEXTURE_2D);

    GLfloat materialAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
    GLfloat materialDiffuse[] = { 81.0f / 255.0f,149.0f / 255.0f,0.0f / 255.0f,1.0f };
    GLfloat materialShininess = 128.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    gluQuadricTexture(quadric, GL_TRUE);

    glPushMatrix();
    {
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        glPushMatrix();
        {

            for (int i = 1; i <= 3; i++)
            {
                // ambient material
                materialAmbient[0] = 0.1f; // r
                materialAmbient[1] = 0.1f; // g
                materialAmbient[2] = 0.1f; // b
                materialAmbient[3] = 1.0f; // a
                glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                // diffuse material
                materialDiffuse[0] = 81.0f / 255.0f; // r
                materialDiffuse[1] = 149.0f / 255.0f; // g
                materialDiffuse[2] = 0.0f / 255.0f; // b
                materialDiffuse[3] = 1.0f; // a
                glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                // shininess
                materialShininess = 0.0025f * 128.0f;
                glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                glColor3f(81.0f / 255.0f, 149.0f / 255.0f, 0.0f);

                glBindTexture(GL_TEXTURE_2D, texture_plant);
                gluCylinder(quadric, 0.1f, 0.1f, 1.0f, 5, 1);
                
                glTranslatef(0.0f, 0.0f, 1.0f);
                gluSphere(quadric, 0.1f, 8, 8);

                glBindTexture(GL_TEXTURE_2D, 0);
                glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
                glPushMatrix();
                {
                    for (int i = 1; i <= 3; i++)
                    {

                        // diffuse material
                        materialDiffuse[0] = 81.0f / 255.0f; // r
                        materialDiffuse[1] = 149.0f / 255.0f; // g
                        materialDiffuse[2] = 0.0f / 255.0f; // b
                        materialDiffuse[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                        // shininess
                        materialShininess = 0.25f * 128;
                        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                        glColor3f(81.0f / 255.0f, 149.0f / 255.0f, 0.0f);

                        glRotatef(-5.0f, 0.0f, 1.0f, 0.0f);
                        glBindTexture(GL_TEXTURE_2D, texture_plant);
                        gluCylinder(quadric, 0.05f, 0.05f, 1.0f, 5, 1);
                        glTranslatef(0.0f, 0.0f, 1.0f);
                        gluSphere(quadric, 0.05f, 5, 5);
                       
                    }

                    glPushMatrix();
                    {
                        gluCylinder(quadric, 0.05f, 0.1f, 0.15f, 5, 1);
                        glBindTexture(GL_TEXTURE_2D, 0);

                    }
                    glPopMatrix();
                    glTranslatef(0.0f, 0.0f, 0.15f);

                    glPushMatrix();
                    {
                        // ambient material
                        materialAmbient[0] = 0.0f; // r
                        materialAmbient[1] = 0.0f; // g
                        materialAmbient[2] = 0.0f; // b
                        materialAmbient[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                        // diffuse material
                        materialDiffuse[0] = 80.0f / 255.0f; // r
                        materialDiffuse[1] = 49.0f / 255.0f; // g
                        materialDiffuse[2] = 6.0f / 255.0f; // b
                        materialDiffuse[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                        // shininess
                        materialShininess = 0.25f * 128;
                        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                        glColor3f(80.0f / 255.0f, 49.0f / 255.0f, 6.0f / 255.0f);

                        glScalef(1.f, 1.0f, 0.25f);
                        gluSphere(quadric, 0.125f, 5, 5);
                    }
                    glPopMatrix();

                    // ambient material
                    materialAmbient[0] = 0.0f; // r
                    materialAmbient[1] = 0.0f; // g
                    materialAmbient[2] = 0.0f; // b
                    materialAmbient[3] = 1.0f; // a
                    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                    // diffuse material
                    materialDiffuse[0] = 150.0f / 255.0f; // r
                    materialDiffuse[1] = 81.0f / 255.0f; // g
                    materialDiffuse[2] = 255.0f / 255.0f; // b
                    materialDiffuse[3] = 1.0f; // a
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                    // shininess
                    materialShininess = 0.25f * 128;
                    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                    glColor3f(150.0f / 255.0f, 81.0f / 255.0f, 255.0f / 255.0f);

                    for (float angle = 0.0f; angle <= 360.0f; angle = angle + 20.0f)
                    {
                        glPushMatrix();
                        {
                            glRotatef(angle, 0.0f, 0.0f, 1.0f);
                            glTranslatef(0.0f, 0.2f, 0.0f);
                            glScalef(0.45f, 1.0f, 1.0f);
                            gluDisk(quadric, 0.0f, 0.1f, 8, 1);
                        }
                        glPopMatrix();
                    }


                }
                glPopMatrix();

                glPushMatrix();
                {
                    for (int i = 1; i <= 3; i++)
                    {

                        // diffuse material
                        materialDiffuse[0] = 81.0f / 255.0f; // r
                        materialDiffuse[1] = 149.0f / 255.0f; // g
                        materialDiffuse[2] = 0.0f / 255.0f; // b
                        materialDiffuse[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                        // shininess
                        materialShininess = 0.25f * 128;
                        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                        glColor3f(81.0f / 255.0f, 149.0f / 255.0f, 0.0f);

                        glRotatef(-17.0f, 0.0f, 1.0f, 0.0f);
                        glBindTexture(GL_TEXTURE_2D, texture_plant);
                        gluCylinder(quadric, 0.05f, 0.05f, 1.0f, 5, 1);
                        glTranslatef(0.0f, 0.0f, 1.0f);
                        gluSphere(quadric, 0.05f, 5, 5);

                    }

                    glPushMatrix();
                    {
                        gluCylinder(quadric, 0.05f, 0.1f, 0.15f, 5, 1);
                        glBindTexture(GL_TEXTURE_2D, 0);

                    }
                    glPopMatrix();
                    glTranslatef(0.0f, 0.0f, 0.15f);

                    glPushMatrix();
                    {
                        // ambient material
                        materialAmbient[0] = 0.0f; // r
                        materialAmbient[1] = 0.0f; // g
                        materialAmbient[2] = 0.0f; // b
                        materialAmbient[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                        // diffuse material
                        materialDiffuse[0] = 80.0f / 255.0f; // r
                        materialDiffuse[1] = 49.0f / 255.0f; // g
                        materialDiffuse[2] = 6.0f / 255.0f; // b
                        materialDiffuse[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                        // shininess
                        materialShininess = 0.25f * 128;
                        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                        glColor3f(80.0f / 255.0f, 49.0f / 255.0f, 6.0f / 255.0f);

                        glScalef(1.f, 1.0f, 0.25f);
                        gluSphere(quadric, 0.125f, 8, 8);
                    }
                    glPopMatrix();

                    // ambient material
                    materialAmbient[0] = 0.0f; // r
                    materialAmbient[1] = 0.0f; // g
                    materialAmbient[2] = 0.0f; // b
                    materialAmbient[3] = 1.0f; // a
                    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                    // diffuse material
                    materialDiffuse[0] = 255.0f / 255.0f; // r
                    materialDiffuse[1] = 151.0f / 255.0f; // g
                    materialDiffuse[2] = 0.0f / 255.0f; // b
                    materialDiffuse[3] = 1.0f; // a
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                    // shininess
                    materialShininess = 0.25f * 128;
                    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                    glColor3f(150.0f / 255.0f, 81.0f / 255.0f, 255.0f / 255.0f);

                    for (float angle = 0.0f; angle <= 360.0f; angle = angle + 15.0f)
                    {
                        glPushMatrix();
                        {
                            glRotatef(angle, 0.0f, 0.0f, 1.0f);
                            glTranslatef(0.0f, 0.2f, 0.0f);
                            glScalef(0.45f, 1.0f, 1.0f);
                            gluDisk(quadric, 0.0f, 0.1f, 8, 1);
                        }
                        glPopMatrix();
                    }


                }
                glPopMatrix();

                glPushMatrix();
                {
                    for (int i = 1; i <= 3; i++)
                    {

                        // diffuse material
                        materialDiffuse[0] = 81.0f / 255.0f; // r
                        materialDiffuse[1] = 149.0f / 255.0f; // g
                        materialDiffuse[2] = 0.0f / 255.0f; // b
                        materialDiffuse[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                        // shininess
                        materialShininess = 0.25f * 128;
                        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                        glColor3f(81.0f / 255.0f, 149.0f / 255.0f, 0.0f);

                        glRotatef(-25.0f, 0.0f, 1.0f, 0.0f);
                        glBindTexture(GL_TEXTURE_2D, texture_plant);
                        gluCylinder(quadric, 0.05f, 0.05f, 1.0f, 5, 1);
                        glTranslatef(0.0f, 0.0f, 1.0f);
                        gluSphere(quadric, 0.05f, 5, 5);

                    }

                    glPushMatrix();
                    {
                        gluCylinder(quadric, 0.05f, 0.1f, 0.15f, 5, 1);
                        glBindTexture(GL_TEXTURE_2D, 0);

                    }
                    glPopMatrix();
                    glTranslatef(0.0f, 0.0f, 0.15f);

                    glPushMatrix();
                    {
                        // ambient material
                        materialAmbient[0] = 0.0f; // r
                        materialAmbient[1] = 0.0f; // g
                        materialAmbient[2] = 0.0f; // b
                        materialAmbient[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                        // diffuse material
                        materialDiffuse[0] = 80.0f / 255.0f; // r
                        materialDiffuse[1] = 49.0f / 255.0f; // g
                        materialDiffuse[2] = 6.0f / 255.0f; // b
                        materialDiffuse[3] = 1.0f; // a
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                        // shininess
                        materialShininess = 0.25f * 128;
                        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                        glColor3f(80.0f / 255.0f, 49.0f / 255.0f, 6.0f / 255.0f);

                        glScalef(1.f, 1.0f, 0.25f);
                        gluSphere(quadric, 0.125f, 8, 8);
                    }
                    glPopMatrix();

                    // ambient material
                    materialAmbient[0] = 0.1f; // r
                    materialAmbient[1] = 0.1f; // g
                    materialAmbient[2] = 0.1f; // b
                    materialAmbient[3] = 1.0f; // a
                    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

                    // diffuse material
                    materialDiffuse[0] = 229.0f / 255.0f; // r
                    materialDiffuse[1] = 13.0f / 255.0f; // g
                    materialDiffuse[2] = 13.0f / 255.0f; // b
                    materialDiffuse[3] = 1.0f; // a
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


                    // shininess
                    materialShininess = 0.25f * 128;
                    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

                    glColor3f(150.0f / 255.0f, 81.0f / 255.0f, 255.0f / 255.0f);

                    for (float angle = 0.0f; angle <= 360.0f; angle = angle + 15.0f)
                    {
                        glPushMatrix();
                        {
                            glRotatef(angle, 0.0f, 0.0f, 1.0f);
                            glTranslatef(0.0f, 0.2f, 0.0f);
                            glScalef(0.45f, 1.0f, 1.0f);
                            gluDisk(quadric, 0.0f, 0.1f, 8, 1);
                        }
                        glPopMatrix();
                    }


                }
                glPopMatrix();
            }

        }
        glPopMatrix();
    }
    glPopMatrix();

    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

}

void drawGrass(void)
{
    //variable declaration
    float modelview[16];
    int i, j;

    // code
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_bush);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    {        
        // get the current modelview matrix
        //glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

        // undo all rotations
        // beware all scaling is lost as well 
        //for (i = 0; i < 3; i += 2)
        //{
        //    for (j = 0; j < 3; j++)
        //    {
        //        if (i == j)
        //            modelview[i * 4 + j] = 1.0;
        //        else
        //            modelview[i * 4 + j] = 0.0;
        //    }
        //}

        // set the modelview with no rotations and scaling
        //glLoadMatrixf(modelview);

            glPushMatrix();
            {
                glTranslatef(0.0f, 1.0f, 0.0f);
                //glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                gluQuadricTexture(quadric, GL_TRUE);
                gluSphere(quadric, 1.0f, 7, 7);
                glTranslatef(0.0f, 0.0f, -0.05f);
                glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
                gluSphere(quadric, 1.0f, 7, 7);
                gluQuadricTexture(quadric, GL_FALSE);
            }
            glPopMatrix();

        glDisable(GL_BLEND);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
}

void drawTrees(void)
{
    //function declaration
    void drawTree(GLfloat base, GLfloat top, GLfloat height);

    //code
    if (selectedScene == 1 && scene1_Two == TRUE)
    {
    	gbFALLEnable = TRUE;
    }
    else
    {
    	gbFALLEnable = FALSE;
    }

    glPushMatrix();
    {
        glTranslatef(-20.0f, 0.0f, 0.0f);
        //glScalef(0.02f, 0.02f, 0.02f);
        drawTree(0.5f, 0.5f, 10.0f);
        bIsTrunk = TRUE;
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(10.0f, 0.0f, 40.0f);
        glRotatef(-60.0f, 0.0f, 1.0f, 0.0f);
        //glScalef(0.03f, 0.03f, 0.03f);
        drawTree(0.5f, 0.5f, 10.0f);
        bIsTrunk = TRUE;
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-198.0f, 0.0f, 15.0f);
        glRotatef(35.0f, 0.0f, 1.0f, 0.0f);
        //glScalef(0.03f, 0.03f, 0.03f);
        drawTree(0.5f, 0.5f, 10.0f);
        bIsTrunk = TRUE;
    }
    glPopMatrix();

}

void drawTree(GLfloat base, GLfloat top, GLfloat height)
{
    void drawCylinder(GLfloat, GLfloat, GLfloat);
    // GLfloat subHeight = height - (height / 3);
    static int i = 0;

    if (bIsTrunk == TRUE)
    {
        drawCylinder(base, top, height);

        bIsTrunk = FALSE;
    }
    else
    {
        if (scene1_Two)
        {
            if (height <= 1.0f)
            {
                // for leaf
                // glColor3f(0.0f,1.0f,0.0f);
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                gluQuadricTexture(quadric, GL_TRUE);

                glPushMatrix();
                {
                    glScalef(1.0f, 0.5f, 0.75f);
                    
                    //glColor3f(1.0f, 1.0f, 1.0f);
                    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);


                    glColor4f(100.0f / 255.0f, 100.0f / 255.0f, 0.0f / 255.0f, 0.75f);
                    glBindTexture(GL_TEXTURE_2D, texture_bush);
                    gluSphere(quadric, 0.5f, 4, 4);
                    glBindTexture(GL_TEXTURE_2D, 0);              
                }
                glPopMatrix();

                glDisable(GL_TEXTURE_2D);

                glDisable(GL_BLEND);

            }
            else
            {
                // glColor3f(150.0f / 255.0f, 75.0f / 255.0f, 0.0f / 255.0f);
                drawCylinder(base, top, height);
            }
        }
        else
        {
            if (height <= 2.0f)
            {
                // for leaf
                // glColor3f(0.0f,1.0f,0.0f);
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                gluQuadricTexture(quadric, GL_TRUE);


                glPushMatrix();
                {
                    glScalef(1.0f, 0.5f, 0.75f);

                        glBindTexture(GL_TEXTURE_2D, texture_plant);

                        glColor3f(1.0f, 1.0f, 1.0f);
                        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                        gluSphere(quadric, 0.5f, 4, 4);
                        glBindTexture(GL_TEXTURE_2D, 0);

                }
                glPopMatrix();

                glDisable(GL_TEXTURE_2D);

                glDisable(GL_BLEND);

            }
            else
            {
                drawCylinder(base, top, height);
            }
        }

    }

    if (height <= 1.0f)
    {

        return;
    }
    else
    {
        glPushMatrix();
        glTranslatef(0.0f, height * 0.8f, 0.0f);
        glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, height * 0.7f, 0.0f);
        glRotatef(45.0f, 0.0f, 0.0f, -1.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, height * 0.6f, 0.0f);
        glRotatef(40.0f, 1.0f, 0.0f, 0.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, height * 0.5f, 0.0f);
        glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, height * 0.9f, 0.0f);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        drawTree(top, base * 0.5f, height * 0.6f);
        glPopMatrix();
    }
}

void drawCylinder(GLfloat base, GLfloat top, GLfloat height)
{
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    
    glEnable(GL_TEXTURE_2D);

    glColor3f(40.0f / 255.0f, 20.0f / 255.0f, 10.0f / 255.0f);
    gluQuadricTexture(quadric, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_wood);
    gluCylinder(quadric, base, top, height, 10, 10);
    glBindTexture(GL_TEXTURE_2D, 0);
    gluQuadricTexture(quadric, GL_FALSE);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawPanati(void)
{
    // variable declaration
    GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
    GLfloat lightDiffuse[] = { 26.0f/255.0f,14.0f/255.0f,5.0f/255.0f,1.0f };
    GLfloat lightPosition[] = { 0.0f,50.0f,50.0f,1.0f };

    // code
    // light related initialization
    glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT4, GL_POSITION, lightPosition);



    glPushMatrix();
    {

        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

        GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
        GLfloat materialDiffuse[] = { 121.0f / 255.0f,43.0f / 255.0f,7.0f / 255.0f,1.0f };
        GLfloat materialShininess = 128.0f;

        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        glPushMatrix();
        {
            glEnable(GL_TEXTURE_2D);
            gluQuadricTexture(quadric, GL_TRUE);

            glEnable(GL_BLEND);
            
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindTexture(GL_TEXTURE_2D, texture_stone);
            glColor4f(0.1f, 0.1f, 0.1f, 0.8f);

            gluDisk(quadric, 1.0f, 2.0f, 20, 1);

            glBindTexture(GL_TEXTURE_2D, 0);

            glDisable(GL_BLEND);

            gluQuadricTexture(quadric, GL_FALSE);

            glDisable(GL_TEXTURE_2D);

            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT4);

            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

            gluDisk(quadric, 0.0f, 1.0f, 20, 1);
        }
        glPopMatrix();

        gluCylinder(quadric, 1.0f, 1.2f, 0.2f, 20, 1);

        glTranslatef(0.0f, 0.0f, 0.2f);
        gluCylinder(quadric, 1.2f, 1.4f, 0.2f, 20, 1);


        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHTING);

        glEnable(GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);
        glColor4f(200.0f/255.0f, 181.0f/255.0f, 137.0f/255.0f, 0.5f);

        gluDisk(quadric, 0.0f, 1.2f, 20, 1);

        glDisable(GL_BLEND);

        glDisable(GL_TEXTURE_2D);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT4);

        materialAmbient[0] = 0.0f;
        materialAmbient[1] = 0.0f;
        materialAmbient[2] = 0.0f;
        materialAmbient[3] = 1.0f;

        materialDiffuse[0] = 121.0f / 255.0f;
        materialDiffuse[1] = 43.0f / 255.0f;
        materialDiffuse[2] = 7.0f / 255.0f;
        materialDiffuse[3] = 1.0f;

        materialShininess = 128.0f;

        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        glTranslatef(0.0f, 0.0f, 0.2f);
        gluCylinder(quadric, 1.4f, 1.6f, 0.2f, 20, 1);

        glTranslatef(0.0f, 0.0f, 0.2f);
        glScalef(0.5f, 0.5f, 1.0f);

        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHTING);



        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, -1.9f, -0.7f);

        gluQuadricTexture(quadric, GL_TRUE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glBindTexture(GL_TEXTURE_2D, texture_jyot);
        glRotatef(gfJyotRotation, 0.0f, 1.0f, 0.0f);
        glColor3f(200.0f / 255.0f, 181.0f / 255.0f, 137.0f / 255.0f);
        gluSphere(quadric, 1.0f, 20, 20);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
        gluQuadricTexture(quadric, GL_FALSE);

        glDisable(GL_TEXTURE_2D);

    }
    glPopMatrix();
}

void drawTemple(void)
{

    // function declaration
    void drawKalas(void);
    void drawBell(void);
    void drawShivLing(void);

    // 1st param :- x coordinate of right top
    // 2nd param :- y coordinate of right top
    // 3rd param :- z coordinate of right top
    // 4th param :- depth 
    // 5th param :- width of figure
    // 6th param :- height of figure
    // 7th param :- struct MYCOLOR (typedef myColor)
    // 8th param :- GLuint texture name
    void SGK_drawBox(float, float, float, float, float, float, myColor,GLuint);

    // variable declaration
    float y,z; // iterating variable for draw stairs
    float radius, height;

    myColor whiteColor = { 1.0f,1.0f,1.0f };

    // code


    //material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);

    // toggle texture
    if(gbTexture == FALSE)
    {
        glDisable(GL_TEXTURE_2D);
        gluQuadricTexture(templeQuadric, GL_FALSE);
    }
    else
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        gluQuadricTexture(templeQuadric, GL_TRUE);
    }
	

    glPushMatrix();
    {
        // white
        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 0.55f; // r
        materialDiffuse[1] = 0.55f; // g
        materialDiffuse[2] = 0.55f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 0.25f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        // Top base part 
        {   
            glBindTexture(GL_TEXTURE_2D, texture_marble);
            glBegin(GL_QUADS);

            // bottom face
            glColor3f(0.5f, 0.5f, 0.5f);

            glNormal3f(0.0f,-1.0f,0.0f);
            
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(4.0f,4.0f,-8.0f); // top right 

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-4.0f,4.0f,-8.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-4.0f,4.0f,8.0f); // bottom left

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(4.0f,4.0f,8.0f); // bottom right

            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, texture_wall_1);
            glBegin(GL_QUADS);
            // right face
            glColor3f(1.0f,1.0f,1.0f);

            glNormal3f(1.0f,0.0f,0.0f);
            
            glTexCoord2f(10.0f, 3.0f);
            glVertex3f(4.0f,4.6f,-8.0f); // top right 

            glTexCoord2f(0.0f, 3.0f);
            glVertex3f(4.0f,4.6f,8.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(4.0f,4.0f,8.0f); // bottom left

            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(4.0f,4.0f,-8.0f); // bottom right

            // left face
            glColor3f(1.0f,1.0f,1.0f);

            glNormal3f(-1.0f,0.0f,0.0f);
            
            glTexCoord2f(10.0f, 3.0f);
            glVertex3f(-4.0f,4.6f,8.0f); // top right 

            glTexCoord2f(0.0f, 3.0f);
            glVertex3f(-4.0f,4.6f,-8.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-4.0f,4.0f,-8.0f); // bottom left

            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(-4.0f,4.0f,8.0f); // bottom right

            
            // back face
            glColor3f(1.0f,1.0f,1.0f);

            glNormal3f(0.0f,0.0f,-1.0f);

            glTexCoord2f(10.0f, 3.0f);
            glVertex3f(-4.0f,4.6f,-8.0f); // top right 

            glTexCoord2f(0.0f, 3.0f);
            glVertex3f(4.0f,4.6f,-8.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(4.0f,4.0f,-8.0f); // bottom left

            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(-4.0f,4.0f,-8.0f); // bottom right

            
            // front face
            glColor3f(1.0f,1.0f,1.0f);

            glNormal3f(0.0f,0.0f,1.0f);
            
            glTexCoord2f(10.0f, 3.0f);
            glVertex3f(4.0f,4.6f,8.0f); // top right 

            glTexCoord2f(0.0f, 3.0f);
            glVertex3f(-4.0f,4.6f,8.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-4.0f,4.0f,8.0f); // bottom left

            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(4.0f,4.0f,8.0f); // bottom right

            glEnd();

            glBindTexture(GL_TEXTURE_2D, texture_wall_1);
        }

        // bottom base part 
        {
            glBindTexture(GL_TEXTURE_2D, texture_stone);

            glBegin(GL_QUADS);

            glColor3f(1.0f,1.0f,1.0f);

            glNormal3f(0.0f,-1.0f,0.0f);
            
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(4.0f,0.0f,-8.0f); // top right 

            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-4.0f,0.0f,-8.0f); // top left

            glTexCoord2f(0.0f,0.0f);
            glVertex3f(-4.0f,0.0f,8.0f); // bottom left

            glTexCoord2f(1.0f,0.0f);
            glVertex3f(4.0f,0.0f,8.0f); // bottom right

            // bottom base Top part (back side)

            glNormal3f(0.0f,1.0f,0.0f);
            
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(4.0f,0.6f,-8.0f); // top right 

            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-4.0f,0.6f,-8.0f); // top left

            glTexCoord2f(0.0f,0.0f);
            glVertex3f(-4.0f,0.6f,6.8f); // bottom left

            glTexCoord2f(1.0f,0.0f);
            glVertex3f(4.0f,0.6f,6.8f); // bottom right

            // bottom base Top part (front side) left part

            glNormal3f(0.0f,1.0f,0.0f);
            
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(-1.0f,0.6f,6.8f); // top right 

            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-4.0f,0.6f,6.8f); // top left

            glTexCoord2f(0.0f,0.0f);
            glVertex3f(-4.0f,0.6f,8.0f); // bottom left

            glTexCoord2f(1.0f,0.0f);
            glVertex3f(-1.0f,0.6f,8.0f); // bottom right

            // bottom base front part -> left part

            glNormal3f(0.0f,0.0f,1.0f);
            
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(-1.0f,0.6f,8.0f); // top right 

            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-4.0f,0.6f,8.0f); // top left

            glTexCoord2f(0.0f,0.0f);
            glVertex3f(-4.0f,0.0f,8.0f); // bottom left

            glTexCoord2f(1.0f,0.0f);
            glVertex3f(-1.0f,0.0f,8.0f); // bottom right

             // bottom base Top part (front side) right part

            glNormal3f(0.0f,1.0f,0.0f);
            
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(1.0f,0.6f,6.8f); // top right 

            glTexCoord2f(1.0f,1.0f);
            glVertex3f(4.0f,0.6f,6.8f); // top left

            glTexCoord2f(0.0f,0.0f);
            glVertex3f(4.0f,0.6f,8.0f); // bottom left

            glTexCoord2f(1.0f,0.0f);
            glVertex3f(1.0f,0.6f,8.0f); // bottom right

            // bottom base front part -> right part

            glNormal3f(0.0f,0.0f,1.0f);
            
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(1.0f,0.6f,8.0f); // top right 

            glTexCoord2f(0.0f,1.0f);
            glVertex3f(4.0f,0.6f,8.0f); // top left

            glTexCoord2f(0.0f,0.0f);
            glVertex3f(4.0f,0.0f,8.0f); // bottom left

            glTexCoord2f(1.0f,0.0f);
            glVertex3f(1.0f,0.0f,8.0f); // bottom right

            // bottom base right part 

            glNormal3f(1.0f,0.0f,0.0f);
            
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(4.0f,0.6f,-8.0f); // top right 

            glTexCoord2f(0.0f,1.0f);
            glVertex3f(4.0f,0.6f,8.0f); // top left

            glTexCoord2f(0.0f,0.0f);
            glVertex3f(4.0f,0.0f,8.0f); // bottom left

            glTexCoord2f(1.0f,0.0f);
            glVertex3f(4.0f,0.0f,-8.0f); // bottom right

            // bottom base left part 

            glNormal3f(-1.0f,0.0f,0.0f);
            
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-4.0f,0.6f,8.0f); // top right 

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-4.0f,0.6f,-8.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-4.0f,0.0f,-8.0f); // bottom left

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-4.0f,0.0f,8.0f); // bottom right

            // bottom base back part 

            glNormal3f(0.0f,0.0f,-1.0f);
            
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-4.0f,0.6f,-8.0f); // top right 

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(4.0f,0.6f,-8.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(4.0f,0.0f,-8.0f); // bottom left

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-4.0f,0.0f,-8.0f); // bottom right

            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // stair part
        {
            glBindTexture(GL_TEXTURE_2D, texture_stone);

            glBegin(GL_QUADS);

            for (y = 0.2f, z = 8.0f; y <= 0.6f; y = y + 0.2f, z = z - 0.4f)
            {
                if (y >= 0.4f)
                {
                    glColor3f(1.0f, 1.0f, 1.0f);
                    // left side part of stairs
                    glNormal3f(1.0f, 0.0f, 0.0f);

                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(-1.0f, y, 8.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(-1.0f, y, z);

                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(-1.0f, y - 0.2f, z);

                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(-1.0f, y - 0.2f, 8.0f);

                    // right side part of stairs
                    glNormal3f(-1.0f, 0.0f, 0.0f);

                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(1.0f, y, 8.0f);

                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(1.0f, y, z);

                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(1.0f, y - 0.2f, z);

                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(1.0f, y - 0.2f, 8.0f);
                }

                // vertical part |
                glNormal3f(0.0f, 0.0f, 1.0f);

                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(1.0f, y, z);

                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-1.0f, y, z);

                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-1.0f, y - 0.2f, z);

                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(1.0f, y - 0.2f, z);

                // horrizontal part  _
                glNormal3f(0.0f, 1.0f, 0.0f);

                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(1.0f, y, z - 0.4f);

                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-1.0f, y, z - 0.4f);

                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-1.0f, y, z);

                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(1.0f, y, z);

            }

            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);

        }

        // pillar part
        {
            // chrome material
            // ambient material
            materialAmbient[0] = 0.25f; // r
            materialAmbient[1] = 0.25f; // g
            materialAmbient[2] = 0.25f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 0.4f; // r
            materialDiffuse[1] = 0.4f; // g
            materialDiffuse[2] = 0.4f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

            // shininess
            materialShininess = 0.6f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glBindTexture(GL_TEXTURE_2D, texture_marble);
            glColor3f(0.5f, 0.5f, 0.5f);
            // front rigth pillar
            glPushMatrix();
            {
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                glTranslatef(3.5f, 7.0f, -3.8f); // after rotatef()  x = x / y = -z / z = y
                glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
                gluCylinder(templeQuadric, 0.3f, 0.3f, 3.2f, 25, 1);

                glTranslatef(0.0f, 0.0f, -0.2f); // after rotatef()  x = x / y = -z / z = y
                gluCylinder(templeQuadric, 0.4f, 0.3f, 0.4f, 25, 1);
            }
            glPopMatrix();

            // front left pillar 
            glPushMatrix();
            {
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                glTranslatef(-3.5f, 7.0f, -3.8f); // after rotatef()  x = x / y = -z / z = y
                glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
                gluCylinder(templeQuadric, 0.3f, 0.3f, 3.2f, 25, 1);

                glTranslatef(0.0f, 0.0f, -0.2f); // after rotatef()  x = x / y = -z / z = y
                gluCylinder(templeQuadric, 0.4f, 0.3f, 0.4f, 25, 1);
            }
            glPopMatrix();

            // back left pillar 
            glPushMatrix();
            {
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                glTranslatef(-3.5f, -7.0f, -3.8f); // after rotatef()  x = x / y = -z / z = y
                glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
                gluCylinder(templeQuadric, 0.3f, 0.3f, 3.2f, 25, 1);

                glTranslatef(0.0f, 0.0f, -0.2f); // after rotatef()  x = x / y = -z / z = y
                gluCylinder(templeQuadric, 0.4f, 0.3f, 0.4f, 25, 1);
            }
            glPopMatrix();

            // back right pillar
            glPushMatrix();
            {
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                glTranslatef(3.5f, -7.0f, -3.8f); // after rotatef()  x = x / y = -z / z = y
                glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
                gluCylinder(templeQuadric, 0.3f, 0.3f, 3.2f, 25, 1);

                glTranslatef(0.0f, 0.0f, -0.2f); // after rotatef()  x = x / y = -z / z = y
                gluCylinder(templeQuadric, 0.4f, 0.3f, 0.4f, 25, 1);
            }
            glPopMatrix();

            // middle right pillar
            glPushMatrix();
            {
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                glTranslatef(3.5f, 0.0f, -3.8f); // after rotatef()  x = x / y = -z / z = y
                glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
                gluCylinder(templeQuadric, 0.3f, 0.3f, 3.2f, 25, 1);

                glTranslatef(0.0f, 0.0f, -0.2f); // after rotatef()  x = x / y = -z / z = y
                gluCylinder(templeQuadric, 0.4f, 0.3f, 0.4f, 25, 1);
            }
            glPopMatrix();

            // middle left pillar
            glPushMatrix();
            {
                glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                glTranslatef(-3.5f, 0.0f, -3.8f); // after rotatef()  x = x / y = -z / z = y
                glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
                gluCylinder(templeQuadric, 0.3f, 0.3f, 3.2f, 25, 1);

                glTranslatef(0.0f, 0.0f, -0.2f); // after rotatef()  x = x / y = -z / z = y
                gluCylinder(templeQuadric, 0.4f, 0.3f, 0.4f, 25, 1);
            }
            glPopMatrix();

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // ghumat part
        {
            // copper like
            // ambient material
            materialAmbient[0] = 0.2125f; // r
            materialAmbient[1] = 0.1275f; // g
            materialAmbient[2] = 0.054f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 0.714f; // r
            materialDiffuse[1] = 0.4284f; // g
            materialDiffuse[2] = 0.18144f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);

            glColor3f(1.0f, 0.5f, 0.0f);

            // shininess
            materialShininess = 0.2 * 128;
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);

            glBindTexture(GL_TEXTURE_2D, texture_wall_2);
            // middle layer of top
            glPushMatrix();
            {
                glTranslatef(0.0f, 4.6f, 0.0f);
                glScalef(0.59f, 1.0f, 1.18f);
                glRotatef(45.0f, 0.0f, 1.0f, 0.0f);

                for (radius = 9.6f, y = 0.2f; radius >= 8.0f; radius = radius - 0.4f, y = y + 0.2f)
                {
                    height = 0.2f;
                    glPushMatrix();
                    {
                        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                        glTranslatef(0.0f, 0.0f, -y); // after rotatef()  x = x / y = z / z = -y

                        gluCylinder(templeQuadric, radius - 0.2f, radius, height, 4, 1);

                        if (radius <= 8.1f)
                        {
                            gluCylinder(templeQuadric, 0.0f, radius - 0.2f, 0.0f, 4, 1);
                        }
                        else
                        {
                            glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
                            gluCylinder(templeQuadric, radius - 0.2f, radius - 0.4f, 0.0f, 4, 1);
                        }

                    }
                    glPopMatrix();
                }
            }
            glPopMatrix();

            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, texture_wall_1);

            // kalas base part
            glPushMatrix();
            {
                glTranslatef(0.0f, 5.6f, 0.4f);
                for (radius = 2.4f, y = 0.4f; radius >= 0.0f; radius = radius - 0.4f, y = y + 0.4f)
                {
                    height = 0.4f;
                    glPushMatrix();
                    {
                        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                        glTranslatef(0.0f, -4.0f, -y); // after rotatef()  x = x / y = z / z = -y
                        //glScalef(1.0f,1.0f,0.4f);
                        gluCylinder(templeQuadric, radius - 0.2f, radius, height, 9, 1);

                        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
                        gluCylinder(templeQuadric, radius - 0.2f, radius - 0.4f, 0.0f, 9, 1);
                    }
                    glPopMatrix();
                }
            }
            glPopMatrix();

            glBindTexture(GL_TEXTURE_2D, 0);
        }
            

        // gabhara part
        {

            glBindTexture(GL_TEXTURE_2D, texture_wall_1);

            glBegin(GL_QUADS);

            //  right face
            glColor3f(1.0f, 1.0f, 1.0f);

            glNormal3f(1.0f, 0.0f, 0.0f);

            glTexCoord2f(10.0f, 10.0f);
            glVertex3f(2.0f, 4.0f, -7.0f); // top right 

            glTexCoord2f(0.0f, 10.0f);
            glVertex3f(2.0f, 4.0f, 0.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(2.0f, 0.6f, 0.0f); // bottom left

            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(2.0f, 0.6f, -7.0f); // bottom right

            //  left face
            glColor3f(1.0f, 1.0f, 1.0f);

            glNormal3f(-1.0f, 0.0f, 0.0f);

            glTexCoord2f(10.0f, 10.0f);
            glVertex3f(-2.0f, 4.0f, 0.0f); // top right 

            glTexCoord2f(0.0f, 10.0f);
            glVertex3f(-2.0f, 4.0f, -7.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-2.0f, 0.6f, -7.0f); // bottom left

            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(-2.0f, 0.6f, 0.0f); // bottom right


            //  back face
            glColor3f(1.0f, 1.0f, 1.0f);

            glNormal3f(0.0f, 0.0f, -1.0f);

            glTexCoord2f(10.0f, 10.0f);
            glVertex3f(-2.0f, 4.0f, -7.0f); // top right 

            glTexCoord2f(0.0f, 10.0f);
            glVertex3f(2.0f, 4.0f, -7.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(2.0f, 0.6f, -7.0f); // bottom left

            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(-2.0f, 0.6f, -7.0f); // bottom right

            // front face left part bottom
            glColor3f(1.0f, 1.0f, 1.0f);

            glNormal3f(0.0f, 0.0f, 1.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-0.8f, 2.0f, 0.0f); // top right 

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-2.0f, 2.0f, 0.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-2.0f, 0.6f, 0.0f); // bottom left

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-0.8f, 0.6f, 0.0f); // bottom right

            //  front face right part bottom
            glColor3f(1.0f, 1.0f, 1.0f);

            glNormal3f(0.0f, 0.0f, 1.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(2.0f, 2.0f, 0.0f); // top right 

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0.8f, 2.0f, 0.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0.8f, 0.6f, 0.0f); // bottom left

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(2.0f, 0.6f, 0.0f); // bottom right

            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);

            // brass material
            // ambient material
            materialAmbient[0] = 0.329412f; // r
            materialAmbient[1] = 0.223529f; // g
            materialAmbient[2] = 0.027451f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            materialDiffuse[0] = 1.0f; // r
            materialDiffuse[1] = 1.0f; // g
            materialDiffuse[2] = 1.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

            // shininess
            materialShininess = 0.21794872f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glBindTexture(GL_TEXTURE_2D, texture_invertedTree);

            glBegin(GL_QUADS);

            // front face left part bottom
            glColor3f(1.0f, 1.0f, 1.0f);

            glNormal3f(0.0f, 0.0f, 1.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-0.8f, 4.0f, 0.0f); // top right 

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-2.0f, 4.0f, 0.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-2.0f, 2.0f, 0.0f); // bottom left

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-0.8f, 2.0f, 0.0f); // bottom right

            glEnd();

            glBindTexture(GL_TEXTURE_2D, texture_shreeVishnu);

            glBegin(GL_QUADS);

            //  front face right part bottom
            glColor3f(1.0f, 1.0f, 1.0f);

            glNormal3f(0.0f, 0.0f, 1.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(2.0f, 4.0f, 0.0f); // top right 

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0.8f, 4.0f, 0.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0.8f, 2.0f, 0.0f); // bottom left

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(2.0f, 2.0f, 0.0f); // bottom right

            glEnd();

            glBindTexture(GL_TEXTURE_2D, texture_shreeGanesh);

            glBegin(GL_QUADS);

            //  front face middle part
            glColor3f(1.0f, 1.0f, 1.0f);

            glNormal3f(0.0f, 0.0f, 1.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.8f, 4.0f, 0.0f); // top right 

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-0.8f, 4.0f, 0.0f); // top left

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-0.8f, 3.0f, 0.0f); // bottom left

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.8f, 3.0f, 0.0f); // bottom right

            glEnd();

            SGK_drawBox(0.8f, 0.8f, 0.0f, 0.2f, 1.6f, 0.2f, whiteColor, texture_gold);

            SGK_drawBox(0.8f, 3.0f, 0.0f, 0.2f, 1.6f, 0.2f, whiteColor, texture_gold);

            SGK_drawBox(0.8f, 2.8f, 0.0f, 0.2f, 0.2f, 2.0f, whiteColor, texture_gold);

            SGK_drawBox(-0.6f, 2.8f, 0.0f, 0.2f, 0.2f, 2.0f, whiteColor, texture_gold);

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // draw shivling
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.8f, -4.0f);
            glScalef(0.6f, 0.6f, 0.6f);
            drawShivLing();
        }
        glPopMatrix();

        // kalas
        glPushMatrix();
        {
            glTranslatef(0.0f, 8.0f, -3.6f);
            drawKalas();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.0f, 2.75f, 7.0f);
            glScalef(0.9f, 0.9f, 0.9f);
            drawBell();
        }
        glPopMatrix();
    }
    glPopMatrix();


    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

}

void drawShivLing(void)
{
    // code

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glPushMatrix();
    {

        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        //  gray material
       // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 1.0f; // r
        materialDiffuse[1] = 1.0f; // g
        materialDiffuse[2] = 1.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 128.0f;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);


        glBindTexture(GL_TEXTURE_2D, texture_black);
        // base part
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, -0.2f);
            gluCylinder(templeQuadric, 1.0f, 1.0f, 0.2f, 30, 1);

            glTranslatef(0.89f, 0.0f, 0.0f);
            glScalef(1.0f, 0.5f, 1.0f);
            gluCylinder(templeQuadric, 1.0f, 1.0f, 0.2f, 30, 1);
        }
        glPopMatrix();

        gluDisk(templeQuadric, 0.0f, 1.0f, 30, 30);


        glPushMatrix();
        {
            glTranslatef(0.89f, 0.0f, 0.0f);
            //glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
            glScalef(1.0f, 0.5f, 1.0f);
            gluDisk(templeQuadric, 0.0f, 1.0f, 30, 30);
        }
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);

        // middle part
        glBindTexture(GL_TEXTURE_2D, texture_tripund);
        glPushMatrix();
        {
            glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
            gluCylinder(templeQuadric, 0.5f, 0.5f, 0.8f, 30, 1);
        }
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, texture_black);
        // top sphere
        glTranslatef(0.0f, 0.0f, 0.8f);
        gluSphere(templeQuadric, 0.5f, 30, 30);
        glBindTexture(GL_TEXTURE_2D, 0);

    }
    glPopMatrix();


    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}

void drawKalas(void)
{

    //code

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    //  gold material
        // ambient material
    materialAmbient[0] = 0.24725f; // r
    materialAmbient[1] = 0.1995f; // g
    materialAmbient[2] = 0.0745f; // b
    materialAmbient[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    // diffuse material
    materialDiffuse[0] = 0.75164f; // r
    materialDiffuse[1] = 0.60648f; // g
    materialDiffuse[2] = 0.22648f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.4f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glBindTexture(GL_TEXTURE_2D, texture_gold);

    glColor3f(1.0f, 1.0f, 1.0f);

    // kalas bottom part
    glPushMatrix();
    {
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -0.2f); // after rotatef()  x = x / y = z / z = -y
        gluCylinder(templeQuadric, 0.1f, 0.2f, 0.2f, 25, 1);
    }
    glPopMatrix();

    // kalas middle part
    glPushMatrix();
    {
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -0.4f); // after rotatef()  x = x / y = z / z = -y
        gluCylinder(templeQuadric, 0.2f, 0.1f, 0.2f, 25, 1);
    }
    glPopMatrix();

    // kalas top part
    glPushMatrix();
    {
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -1.0f); // after rotatef()  x = x / y = z / z = -y
        gluCylinder(templeQuadric, 0.0f, 0.2f, 0.6f, 25, 1);

    }
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);


    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}

void drawBell(void)
{

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    //  gold material
    // ambient material
    materialAmbient[0] = 0.24725f; // r
    materialAmbient[1] = 0.1995f; // g
    materialAmbient[2] = 0.0745f; // b
    materialAmbient[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    // diffuse material
    materialDiffuse[0] = 0.75164f; // r
    materialDiffuse[1] = 0.60648f; // g
    materialDiffuse[2] = 0.22648f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.4f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture_gold);

    // bell inner top part
    glPushMatrix();
    {
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -0.7f); // after rotatef()  x = x / y = z / z = -y

        // bell inner part top sphere
        gluSphere(templeQuadric, 0.05f, 10, 10);

        glTranslatef(0.0f, 0.0f, 0.05f); // after rotatef()  x = x / y = z / z = -y

        // bell inner part middle cylinder
        gluCylinder(templeQuadric, 0.05f, 0.05f, 0.5f, 15, 1);

        // bell bottom part sphere
        glTranslatef(0.0f, 0.0f, 0.55f); // after rotatef()  x = x / y = z / z = -y
        glScalef(1.0f, 1.2f, 1.5f);
        gluSphere(templeQuadric, 0.08f, 15, 15);
    }
    glPopMatrix();

    // bell bottom part
    glPushMatrix();
    {
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -0.2f); // after rotatef()  x = x / y = z / z = -y
        gluCylinder(templeQuadric, 0.35f, 0.45f, 0.1f, 25, 1);
    }
    glPopMatrix();

    // bell middle part
    glPushMatrix();
    {
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -0.6f); // after rotatef()  x = x / y = z / z = -y
        gluCylinder(templeQuadric, 0.25f, 0.35f, 0.4f, 25, 1);
    }
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    // bell top part
    glPushMatrix();
    {
            glBindTexture(GL_TEXTURE_2D, texture_gold);

            // bell top
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, -0.8f); // after rotatef()  x = x / y = z / z = -y
            gluCylinder(templeQuadric, 0.05f, 0.25f, 0.2f, 25, 1);

            // bell top hook base
            glTranslatef(0.0f, 0.0f, -0.05f);// after rotatef()  x = x / y = z / z = -y
            gluCylinder(templeQuadric, 0.05f, 0.05f, 0.1f, 25, 1);

            // bell main hook
            glTranslatef(0.0f, -0.045f, -0.05f);// after rotatef()  x = x / y = z / z = y
            glPushMatrix();
            {
                glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
                gluCylinder(templeQuadric, 0.08f, 0.07f, 0.1f, 25, 1);
                gluCylinder(templeQuadric, 0.08f, 0.06f, 0.1f, 20, 1);
                gluCylinder(templeQuadric, 0.08f, 0.05f, 0.1f, 15, 1);
            }
            glPopMatrix();

            // bell bottom hook
            glPushMatrix();
            {
                glTranslatef(0.045f, 0.035f, -0.055f);// after rotatef()  x = x / y = z / z = -y
                glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
                gluCylinder(templeQuadric, 0.08f, 0.07f, 0.08f, 25, 1);
                gluCylinder(templeQuadric, 0.08f, 0.06f, 0.08f, 20, 1);
                gluCylinder(templeQuadric, 0.08f, 0.05f, 0.08f, 15, 1);
            }
            glPopMatrix();

            // bell top hook
            glPushMatrix();
            {

                glTranslatef(0.045f, 0.035f, -0.42f);// after rotatef()  x = x / y = z / z = -y
                glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
                gluCylinder(templeQuadric, 0.08f, 0.07f, 0.08f, 25, 1);
                gluCylinder(templeQuadric, 0.08f, 0.06f, 0.08f, 20, 1);
                gluCylinder(templeQuadric, 0.08f, 0.05f, 0.08f, 15, 1);
            }
            glPopMatrix();

            glBindTexture(GL_TEXTURE_2D, 0);


            // --------------- ROPE -------------------
            //  brown
            // ambient material
            materialAmbient[0] = 0.05f; // r
            materialAmbient[1] = 0.05f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 166.0f/255.0f; // r
            materialDiffuse[1] = 92.0f / 255.0f; // g
            materialDiffuse[2] = 30.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

            // shininess
            materialShininess = 0.078125f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

           

            glPushMatrix();
            {
                glBindTexture(GL_TEXTURE_2D, texture_rope);
                // top rope like part
                glTranslatef(0.005f, 0.035f, -0.42f);// after rotatef()  x = x / y = z / z = -y
                gluCylinder(templeQuadric, 0.04f, 0.04f, 0.3f, 15, 1);

                glBindTexture(GL_TEXTURE_2D, 0);
            }
            glPopMatrix();
    }
    glPopMatrix();

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}

void SGK_drawBox(float xAxis, float yAxis, float zAxis, float depth, float width, float height, myColor color,GLuint textureName)
{
    // code

    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // ambient material
    materialAmbient[0] = 0.05f; // r
    materialAmbient[1] = 0.05f; // g
    materialAmbient[2] = 0.0f; // b
    materialAmbient[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    // diffuse material
    materialDiffuse[0] = color.fRed / 255.0f; // r
    materialDiffuse[1] = color.fGreen / 255.0f; // g
    materialDiffuse[2] = color.fBlue / 255.0f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
            // ambient material
    materialAmbient[0] = 0.05f; // r
    materialAmbient[1] = 0.05f; // g
    materialAmbient[2] = 0.0f; // b
    materialAmbient[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    // diffuse material
    materialDiffuse[0] = 166.0f / 255.0f; // r
    materialDiffuse[1] = 92.0f / 255.0f; // g
    materialDiffuse[2] = 30.0f / 255.0f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
            // ambient material
    materialAmbient[0] = 0.05f; // r
    materialAmbient[1] = 0.05f; // g
    materialAmbient[2] = 0.0f; // b
    materialAmbient[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    // diffuse material
    materialDiffuse[0] = 166.0f / 255.0f; // r
    materialDiffuse[1] = 92.0f / 255.0f; // g
    materialDiffuse[2] = 30.0f / 255.0f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glBindTexture(GL_TEXTURE_2D, textureName);

    glBegin(GL_QUADS);
    //front
   
    glColor3f(color.fRed, color.fGreen, color.fBlue);
    glNormal3f(0.0f, 0.0f, 1.0f);;;;

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xAxis, yAxis, zAxis);//right top

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xAxis - width, yAxis, zAxis);//left top

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xAxis - width, yAxis - height, zAxis);//left bottom

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xAxis, yAxis - height, zAxis);//right bottom

    //right
    glNormal3f(1.0f, 0.0f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xAxis, yAxis, zAxis - depth);//right top

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xAxis, yAxis, zAxis);//left top

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xAxis, yAxis - height, zAxis);//left bottom

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xAxis, yAxis - height, zAxis - depth);//right bottom

    //back
    glNormal3f(0.0f, 0.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xAxis - width, yAxis, zAxis - depth);//right top

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xAxis, yAxis, zAxis - depth);//left top

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xAxis, yAxis - height, zAxis - depth);//left bottom

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xAxis - width, yAxis - height, zAxis - depth);//right bottom

    //left
    glNormal3f(-1.0f, 0.0f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xAxis - width, yAxis, zAxis);//right top

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xAxis - width, yAxis, zAxis - depth);//left top

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xAxis - width, yAxis - height, zAxis - depth);//left bottom

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xAxis - width, yAxis - height, zAxis);//right bottom

    //top
    glNormal3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xAxis, yAxis, zAxis - depth);//right top

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xAxis - width, yAxis, zAxis - depth);//left top

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xAxis - width, yAxis, zAxis);//left bottom

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xAxis, yAxis, zAxis);//right bottom

    //bottom
    glNormal3f(0.0f, -1.0f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(xAxis, yAxis - height, zAxis - depth);//right top

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(xAxis - width, yAxis - height, zAxis - depth);//left top

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(xAxis - width, yAxis - height, zAxis);//left bottom

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(xAxis, yAxis - height, zAxis);//right bottom

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}


void TempleUninitialize(void)
{
    // code

    if (quadric)
    {
        gluDeleteQuadric(quadric);
        quadric = NULL;
    }

    if (templeQuadric)
    {
        gluDeleteQuadric(templeQuadric);
        templeQuadric = NULL;
    }

    if (texture_belLeaves)
    {
        glDeleteTextures(1, &texture_belLeaves);
        texture_belLeaves = 0;
    }

    if (texture_bush)
    {
        glDeleteTextures(1, &texture_bush);
        texture_bush = 0;
    }

    if (texture_jyot)
    {
        glDeleteTextures(1, &texture_jyot);
        texture_jyot = 0;
    }

    if (texture_shreeVishnu)
    {
        glDeleteTextures(1, &texture_shreeVishnu);
        texture_shreeVishnu = 0;
    }

    if (texture_invertedTree)
    {
        glDeleteTextures(1, &texture_invertedTree);
        texture_invertedTree = 0;
    }

    if (texture_shreeGanesh)
    {
        glDeleteTextures(1, &texture_shreeGanesh);
        texture_shreeGanesh = 0;
    }

    if (texture_wall_2)
    {
        glDeleteTextures(1, &texture_wall_2);
        texture_wall_2 = 0;
    }

    if (texture_wall_1)
    {
        glDeleteTextures(1, &texture_wall_1);
        texture_wall_1 = 0;
    }

    if (texture_marble)
    {
        glDeleteTextures(1, &texture_marble);
        texture_marble = 0;
    }

    if (texture_gold)
    {
        glDeleteTextures(1, &texture_gold);
        texture_gold = 0;
    }

    if (texture_rope)
    {
        glDeleteTextures(1, &texture_rope);
        texture_rope = 0;
    }

    if (texture_stone)
    {
        glDeleteTextures(1, &texture_stone);
        texture_stone = 0;
    }



    
}

