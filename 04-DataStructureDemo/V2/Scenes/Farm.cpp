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
#include"Farm.h"

#include"../External/stb_image.h"


GLUquadric *farmQuadric;

// for texture
GLuint texture_soil = 0;
GLuint texture_bazari = 0;
GLuint texture_plant = 0;
GLuint texture_whiteCloth = 0;
GLuint texture_dryGrass = 0;
GLuint texture_face = 0;
GLuint texture_wall = 0;
GLuint texture_door = 0;
GLuint texture_wall1 = 0;
GLuint texture_ceiling = 0;
GLuint texture_rockWall = 0;

struct MYCOLOR
{
    float fRed;
    float fGreen;
    float fBlue;
};

typedef struct MYCOLOR myColor;

extern FILE *gpFILE;

GLfloat randomAngle[3];


GLfloat gfRotation = 0.0f;
GLfloat gfTranslate_Y = 0.0f;
BOOL gbUpDownAnimation = FALSE;



int FarmInitialize(void)
{   
    //variable declaration
    BOOL bResult = FALSE;

    // initialize quadric
    farmQuadric = gluNewQuadric();

    // Texture initialization
    bResult = loadGLTexture(&texture_soil,MAKEINTRESOURCE(MY_SOIL_BITMAP));
    if(bResult == FALSE)
    {
        fprintf(gpFILE,"loadGLTexture() Failed \n");
        return(-1);
    }

    bResult = loadGLTexture(&texture_plant, MAKEINTRESOURCE(MY_PLANT_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-1);
    }

    bResult = loadGLTexture(&texture_bazari, MAKEINTRESOURCE(MY_BAZARI_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-1);
    }

    bResult = loadGLTexture(&texture_whiteCloth, MAKEINTRESOURCE(MY_WHITE_CLOTH_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-1);
    }

    bResult = loadGLTexture(&texture_dryGrass, MAKEINTRESOURCE(MY_DRY_GRASS_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-1);
    }

    bResult = loadGLTexture(&texture_face, MAKEINTRESOURCE(MY_FACE_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-1);
    }


    bResult = loadGLTexture(&texture_wall, MAKEINTRESOURCE(MY_WALL_TEXTURE_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-1);
    }

    bResult = loadGLTexture(&texture_door, MAKEINTRESOURCE(MY_DOOR_TEXTURE_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-2);
    }

    bResult = loadGLTexture(&texture_wall1, MAKEINTRESOURCE(MY_WALL1_TEXTURE_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-3);
    }

    bResult = loadGLTexture(&texture_ceiling, MAKEINTRESOURCE(MY_TOPCEILING_TEXTURE_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-4);
    }

    bResult = loadGLTexture(&texture_rockWall, MAKEINTRESOURCE(MY_ROCK_WALL_TEXTURE_BITMAP));
    if (bResult == FALSE)
    {
        fprintf(gpFILE, "loadGLTexture() Failed \n");
        return(-4);
    }

    return(0);
}


void FarmDisplay(void)
{
    // function declaration
    void drawFarm(void);
    void drawWell(void);
    void drawPanati(void);

    // code
    glPushMatrix();
    {
        
        drawFarm();
       

        glPushMatrix();
        {
            glTranslatef(320.0f, 0.0f, 20.0f);
            glScalef(2.5f, 2.0f, 2.5f);
            drawWell();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(50.0f, 0.0f, 150.0f);
            glScalef(5.0f, 2.0f, 5.0f);
            drawWell();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(280.0f, 0.0f, 160.0f);
            glScalef(3.5f, 2.0f, 3.5f);
            drawWell();
        }
        glPopMatrix();
       
        glTranslatef(200.0f, 0.0f, 30.0f);

        drawVillage();
    }
    glPopMatrix();
}

void FarmUpdate(void)
{

    gfRotation = gfRotation + 0.5f;
    if (gfRotation >= 360.0f)
    {
        gfRotation = 0.0f;
    }

    if (gbUpDownAnimation == FALSE)
    {
        
        if (gfTranslate_Y <= 0.1f)
        {
            gfTranslate_Y = gfTranslate_Y + 0.001f;
        }
        else
        {
            gbUpDownAnimation = TRUE;
        }
    }
    else
    {
        if (gfTranslate_Y >= -0.1f)
        {
            gfTranslate_Y = gfTranslate_Y - 0.001f;
        }
        else
        {
            gbUpDownAnimation = FALSE;
        }
    }


 
}


void drawVillage(void)
{
    void drawVillageHouse_1(void);
    void drawVillageHouse(void);

    glPushMatrix();
    {
        for (int x = 1; x <= 5; x++)
        {
            for (int z = 1; z <= 5; z++)
            {
                glPushMatrix();
                {
                    glTranslatef((GLfloat)(x * 40), 0.0f, (GLfloat)(z * 40));

                    if ((x % 2 == 0) ^ ( z %2 == 0))
                    {
                        glRotatef(-15.0f* (x * 10), 0.0f, 1.0f, 0.0f);
                    }
                    else
                    {
                        glRotatef(15.0f * (x * 10), 0.0f, 1.0f, 0.0f);
                    }

                    glScalef(0.5f, 0.5f, 0.5f);

                    if ((x % 2 == 0) ^ (z % 2 == 0))
                    { 
                        drawVillageHouse_1();
                    }
                    else
                    {                        
                        drawVillageHouse();
                    }
                }
                glPopMatrix();
            }
        }
    }
    glPopMatrix();

}

void drawVillageHouse_1(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f, -9.5f);
        glScalef(8.5f, 8.0f, 5.5f);

        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 80.0f / 255.0f; // r
        materialDiffuse[1] = 29.0f / 255.0f; // g
        materialDiffuse[2] = 8.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


        // shininess
        materialShininess = 0.25f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        glBindTexture(GL_TEXTURE_2D, texture_door);

        glBegin(GL_QUADS);

        //door
        //glColor3f(1.0f, 0.0f, 0.0f);
        glNormal3f(0.0f, 0.0f, 1.0f);


        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(0.25f, 0.8f, 1.025f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-0.25f, 0.8f, 1.025f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.25f, 0.0f, 1.025f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(0.25f, 0.0f, 1.025f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 255.0f / 255.0f; // r
        materialDiffuse[1] = 205.0f / 255.0f; // g
        materialDiffuse[2] = 156.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


        // shininess
        materialShininess = 0.25f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        glBindTexture(GL_TEXTURE_2D, texture_wall1);

        glBegin(GL_QUADS);

        //front face
        //glColor3f(1.0f, 0.0f, 0.0f);
        glNormal3f(0.0f, 0.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(3.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(3.0f, 3.0f);
        glVertex3f(-1.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 3.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);

        // right face
        //glColor3f(0.0f, 1.0f, 0.0f);
        glNormal3f(1.0f, 0.0f, 0.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, 1.5f, -1.0f);
        glTexCoord2f(3.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(3.0f, 3.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 3.0f);
        glVertex3f(1.0f, 0.0f, -1.0f);

        // back face
        //glColor3f(0.0f, 0.0f, 1.0f);
        glNormal3f(0.0f, 0.0f, -1.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.5f, -1.0f);
        glTexCoord2f(3.0f, 0.0f);
        glVertex3f(1.0f, 1.5f, -1.0f);
        glTexCoord2f(3.0f, 3.0f);
        glVertex3f(1.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 3.0f);
        glVertex3f(-1.0f, 0.0f, -1.0f);

        // left face
        //glColor3f(0.0f, 1.0f, 1.0f);
        glNormal3f(-1.0f, 0.0f, 0.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glTexCoord2f(3.0f, 0.0f);
        glVertex3f(-1.0f, 1.5f, -1.0f);
        glTexCoord2f(3.0f, 3.0f);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 3.0f);
        glVertex3f(-1.0f, 0.0f, 1.0f);



        // bottom face
        //glColor3f(1.0f, 1.0f, 0.0f);
        glNormal3f(0.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, -1.0f);
        glTexCoord2f(3.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glTexCoord2f(3.0f, 3.0f);
        glVertex3f(-1.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 3.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        // diffuse material
        materialDiffuse[0] = 80.0f / 255.0f; // r
        materialDiffuse[1] = 29.0f / 255.0f; // g
        materialDiffuse[2] = 8.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        glBindTexture(GL_TEXTURE_2D, texture_ceiling);
        glBegin(GL_QUADS);

        // top face (house top)
        //glColor3f(1.0f, 0.0f, 1.0f);
        glNormal3f(0.0f, 1.0f, 0.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, 1.55f, -1.2f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, 1.55f, -1.2f);
        glTexCoord2f(1.0f, 10.0f);
        glVertex3f(-1.0f, 0.95f, 1.2f);
        glTexCoord2f(0.0f, 10.0f);
        glVertex3f(1.0f, 0.95f, 1.2f);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

    }
    glPopMatrix();
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}

void drawVillageHouse(void)
{
    // 1st param :- x coordinate of right top
 // 2nd param :- y coordinate of right top
 // 3rd param :- z coordinate of right top
 // 4th param :- depth 
 // 5th param :- width of figure
 // 6th param :- height of figure
 // 7th param :- struct MYCOLOR (typedef myColor)
 // 8th param :- GLuint texture name
    void SGK_drawBox(float, float, float, float, float, float, myColor, GLuint);

    // variable declaration
    myColor brownColor = { 255.0f,255.0f,255.0f }; // value is given in 0.0f - 255.0f according to use in function of SGK_drawBox()

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, -9.5f);
            glScalef(8.5f, 8.0f, 5.5f);

            // ambient material
            materialAmbient[0] = 0.0f; // r
            materialAmbient[1] = 0.0f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 80.0f / 255.0f; // r
            materialDiffuse[1] = 29.0f / 255.0f; // g
            materialDiffuse[2] = 8.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


            // shininess
            materialShininess = 0.25f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glBindTexture(GL_TEXTURE_2D, texture_door);

            glBegin(GL_QUADS);

            //front face
            //glColor3f(1.0f, 0.0f, 0.0f);
            glNormal3f(0.0f, 0.0f, 1.0f);


            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.25f, 0.8f, 1.05f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-0.25f, 0.8f, 1.05f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-0.25f, 0.0f, 1.05f);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.25f, 0.0f, 1.05f);

            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);

            // ambient material
            materialAmbient[0] = 0.0f; // r
            materialAmbient[1] = 0.0f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 255.0f / 255.0f; // r
            materialDiffuse[1] = 255.0f / 255.0f; // g
            materialDiffuse[2] = 255.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


            // shininess
            materialShininess = 0.25f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glBindTexture(GL_TEXTURE_2D, texture_wall);

            glBegin(GL_QUADS);




            //front face
            //glColor3f(1.0f, 0.0f, 0.0f);
            glNormal3f(0.0f, 0.0f, 1.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-1.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, 1.0f);

            // right face
            //glColor3f(0.0f, 1.0f, 0.0f);
            glNormal3f(1.0f, 0.0f, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, -1.0f);

            // back face
            //glColor3f(0.0f, 0.0f, 1.0f);
            glNormal3f(0.0f, 0.0f, -1.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, -1.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, -1.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 0.0f, -1.0f);

            // left face
            //glColor3f(0.0f, 1.0f, 1.0f);
            glNormal3f(-1.0f, 0.0f, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-1.0f, 0.0f, -1.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 0.0f, 1.0f);

            // top face
            //glColor3f(1.0f, 0.0f, 1.0f);
            glNormal3f(0.0f, 1.0f, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);

            // bottom face
            //glColor3f(1.0f, 1.0f, 0.0f);
            glNormal3f(0.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, 0.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-1.0f, 0.0f, -1.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-1.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, 1.0f);

            glEnd();

            glBegin(GL_TRIANGLES);
            // left face
            //glColor3f(0.0f, 1.0f, 1.0f);
            glNormal3f(-1.0f, 0.0f, 0.0f);

            glTexCoord2f(0.5f, 1.0f);
            glVertex3f(-1.0f, 1.5f, 0.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);

            // right face
            //glColor3f(0.0f, 1.0f, 1.0f);
            glNormal3f(1.0f, 0.0f, 0.0f);

            glTexCoord2f(0.5f, 1.0f);
            glVertex3f(1.0f, 1.5f, 0.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);

            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glPopMatrix();

        // top of house front side
        glPushMatrix();
        {

            glTranslatef(0.0f, 2.29f, -8.0f);
            glScalef(8.5f, 8.0f, 4.5f);
            glRotatef(22.5f, 1.0f, 0.0f, 0.0f);

            // ambient material
            materialAmbient[0] = 0.05f; // r
            materialAmbient[1] = 0.05f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 239.0f / 255.0f; // r
            materialDiffuse[1] = 173.0f / 255.0f; // g
            materialDiffuse[2] = 58.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


            // shininess
            materialShininess = 0.45f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glBindTexture(GL_TEXTURE_2D, texture_dryGrass);
            glBegin(GL_QUADS);
            // top face
            glNormal3f(0.0f, 1.0f, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, -0.88f);
            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, -0.88f);
            glTexCoord2f(10.0f, 10.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glTexCoord2f(0.0f, 10.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);

            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);

        }
        glPopMatrix();

        // top of house back side
        glPushMatrix();
        {

            glTranslatef(0.0f, 2.29f, -11.0f);
            glScalef(8.5f, 8.0f, 4.5f);
            glRotatef(-22.25f, 1.0f, 0.0f, 0.0f);

            glBindTexture(GL_TEXTURE_2D, texture_dryGrass);
            glBegin(GL_QUADS);
            // top face
            //glColor3f(1.0f, 0.0f, 1.0f);
            glNormal3f(0.0f, 1.0f, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, -1.0f);
            glTexCoord2f(10.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);
            glTexCoord2f(10.0f, 10.0f);
            glVertex3f(-1.0f, 1.0f, 0.88f);
            glTexCoord2f(0.0f, 10.0f);
            glVertex3f(1.0f, 1.0f, 0.88f);

            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);

        }
        glPopMatrix();

        // top of house center part
        glPushMatrix();
        {

            glTranslatef(0.0f, 6.35f, -9.0f);
            glScalef(2.25f, 3.0f, 2.0f);
            SGK_drawBox(4.0f, 2.0f, 0.0f, 0.45f, 8.0f, 0.25f, brownColor, texture_dryGrass);

        }
        glPopMatrix();

        glPushMatrix();
        {
            // ambient material
            materialAmbient[0] = 0.05f; // r
            materialAmbient[1] = 0.05f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 196.0f / 255.0f; // r
            materialDiffuse[1] = 196.0f / 255.0f; // g
            materialDiffuse[2] = 196.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


            // shininess
            materialShininess = 0.45f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glScalef(4.0f, 2.0f, 4.0f);
            SGK_drawBox(4.0f, 2.0f, 1.0f, 0.25f, 3.0f, 2.0f, brownColor, texture_wall);
            SGK_drawBox(4.0f, 2.0f, 0.75f, 6.0f, 0.25f, 2.0f, brownColor, texture_wall);
            SGK_drawBox(4.0f, 2.0f, -5.0f, 0.25f, 8.0f, 2.0f, brownColor, texture_wall);
            SGK_drawBox(-4.0f, 2.0f, 0.75f, 6.0f, 0.25f, 2.0f, brownColor, texture_wall);
            SGK_drawBox(-1.25f, 2.0f, 1.0f, 0.25f, 3.0f, 2.0f, brownColor, texture_wall);
        }
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
}

void drawFarm(void)
{
    // function declaration
    void drawField(void);
    void drawCropsField(void);
    void drawCrowScarer(void);

    // variable declaration
    float x,z; // iterating variable 
    float radius, height;

    // code

    // toggle texture
    if(gbTexture == FALSE)
    {
        glDisable(GL_TEXTURE_2D);
        gluQuadricTexture(farmQuadric, GL_FALSE);
    }
    else
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        gluQuadricTexture(farmQuadric, GL_TRUE);
    }

    glPushMatrix();
    {

        for (int i = 0; i < 3; i++)
        {
            glPushMatrix();
            {
                glTranslatef(40.0f * (GLfloat)i, 0.0f, 0.0f);
                if (i < 2)
                {
                    if (selectedScene == 3 && scene3_One == TRUE)
                    {
                        drawCropsField();
                    }
                    else
                    {
                        drawField();
                    }                  
                }
                else
                {
                    drawField();
                }
                glTranslatef(0.0f, 0.0f, 40.0f);
                drawField();
                glTranslatef(0.0f, 0.0f, 40.0f);
                drawField();

            }
            glPopMatrix();
        }

        if (selectedScene == 3 && scene3_One)
        {
            glPushMatrix();
            {
                glTranslatef(20.0f, 0.0f, 20.0f);
                glScalef(0.65f, 0.65f, 0.65f);

                drawCrowScarer();
            }
            glPopMatrix();
        }
       /* glPushMatrix();
        {
                drawCropsField();
        }
        glPopMatrix();*/

        //glDisable(GL_LIGHTING);
        //glDisable(GL_LIGHT0);
    }
    glPopMatrix();

}

void drawWell(void)
{
    // varible declaration
    bool reset = true; // flag for display particle component

    glPushMatrix();
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, texture_rockWall);

        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 196.0f / 255.0f; // r
        materialDiffuse[1] = 196.0f / 255.0f; // g
        materialDiffuse[2] = 196.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);


        // shininess
        materialShininess = 0.25f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        glColor3f(1.0f, 1.0f, 1.0f);
        // side wall
        gluCylinder(farmQuadric, 2.0f, 2.0f, 1.0f, 25, 1);
        gluCylinder(farmQuadric, 1.8f, 1.8f, 1.0f, 25, 1);



        // top of side wall
        glPushMatrix();
        {
            glTranslatef(0.0f, 0.0f, 1.0f);
            gluDisk(farmQuadric, 1.8f, 2.0f, 25, 1);
        }
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glBindTexture(GL_TEXTURE_2D, 0);

        glPushMatrix();
        {
            // base of well
            //gluDisk(farmQuadric, 0.0f, 2.0f, 25, 1);

            glRotatef(-gfRotation, 0.0f, 0.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.0f, 0.5f, 0.5f, 0.5f);
            gluDisk(farmQuadric, 0.0f, 1.8f, 25, 1);

            glRotatef(gfRotation, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, 0.0f, 0.05f);
            gluDisk(farmQuadric, 0.0f, 1.8f, 25, 1);

            glRotatef(-gfRotation + 25.0f, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, 0.0f, 0.05f);
            gluDisk(farmQuadric, 0.0f, 1.8f, 25, 1);

            glRotatef((gfRotation + 25.0f), 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, 0.0f, 0.05f);
            gluDisk(farmQuadric, 0.0f, 1.8f, 25, 1);

            glRotatef(-(gfRotation + 125.0f), 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, 0.0f, 0.05f);
            gluDisk(farmQuadric, 0.0f, 1.8f, 25, 1);

            glColor4f(0.0f, 0.5f, 0.5f, 0.2f);
            glRotatef((gfRotation + 225.0f), 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, 0.0f, gfTranslate_Y);
            gluDisk(farmQuadric, 0.0f, 1.8f, 25, 1);

            glDisable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glPopMatrix();

    }
    glPopMatrix();

}


void drawCrowScarer(void)
{
    // function declaration
    void drawPot(void);

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

    glPushMatrix();
    {
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        // middle main stik
        glColor3f(37.0f / 255.0f, 20.0f / 255.0f, 10.0f / 255.0f);

        // brown color
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 37.0f / 255.0f; // r
        materialDiffuse[1] = 20.0f / 255.0f; // g
        materialDiffuse[2] = 10.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 0.078125f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);

        gluCylinder(farmQuadric, 0.2f, 0.2f, 12.0f, 15, 1);

        glBindTexture(GL_TEXTURE_2D, 0);

        // yello grass color
        glColor3f(239.0f / 255.0f, 173.0f / 255.0f, 58.0f / 255.0f);
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 239.0f / 255.0f; // r
        materialDiffuse[1] = 173.0f / 255.0f; // g
        materialDiffuse[2] = 58.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 0.078125f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
        // bottom part
        glTranslatef(0.0f, 0.0f, 4.0f);

        glBindTexture(GL_TEXTURE_2D, texture_dryGrass);

        gluCylinder(farmQuadric, 3.5f, 1.5f, 4.0f, 25, 1);

        glBindTexture(GL_TEXTURE_2D, 0);

        // white color
        glColor3f(1.0f, 1.0f, 1.0f);
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 255.0f / 255.0f; // r
        materialDiffuse[1] = 255.0f / 255.0f; // g
        materialDiffuse[2] = 255.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 0.078125f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        // middle part (trunk)
        glTranslatef(0.0f, 0.0f, 4.0f);

        glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);
        gluCylinder(farmQuadric, 1.5f, 1.5f, 4.0f, 25, 1);

        glTranslatef(0.0f, 0.0f, 4.0f);
        gluSphere(farmQuadric, 1.5f, 25, 25);

        glBindTexture(GL_TEXTURE_2D, 0);

        // right hand 
        glPushMatrix();
        {
            glTranslatef(1.25f, 0.0f, 0.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);
            gluCylinder(farmQuadric, 0.5f, 0.5f, 2.5f, 25, 1);
            glBindTexture(GL_TEXTURE_2D, 0);
            // yello grass color
            glColor3f(239.0f / 255.0f, 173.0f / 255.0f, 58.0f / 255.0f);
            // ambient material
            materialAmbient[0] = 0.05f; // r
            materialAmbient[1] = 0.05f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 239.0f / 255.0f; // r
            materialDiffuse[1] = 173.0f / 255.0f; // g
            materialDiffuse[2] = 58.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

            // shininess
            materialShininess = 0.078125f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glTranslatef(0.0f, 0.0f, 2.4f); // after above rotatef() x = y , y = z, z = x

            glBindTexture(GL_TEXTURE_2D, texture_dryGrass);
            gluCylinder(farmQuadric, 0.49f, 0.3f, 2.0f, 25, 1);

            glTranslatef(0.0f, 0.0f, 2.0f); // after above rotatef() x = y , y = z, z = x
            gluSphere(farmQuadric, 0.3f, 25, 25);
            glBindTexture(GL_TEXTURE_2D, 0);

        }
        glPopMatrix();

        // left hand 
        glPushMatrix();
        {
            // white color
       // ambient material
            materialAmbient[0] = 0.05f; // r
            materialAmbient[1] = 0.05f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 255.0f / 255.0f; // r
            materialDiffuse[1] = 255.0f / 255.0f; // g
            materialDiffuse[2] = 255.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

            // shininess
            materialShininess = 0.078125f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glColor3f(1.0f, 1.0f, 1.0f);
            glTranslatef(-1.25f, 0.0f, 0.0f);
            glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
            glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);

            gluCylinder(farmQuadric, 0.5f, 0.5f, 2.5f, 25, 1);

            glBindTexture(GL_TEXTURE_2D, 0);


            // yello grass color
        // ambient material
            materialAmbient[0] = 0.05f; // r
            materialAmbient[1] = 0.05f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 239.0f / 255.0f; // r
            materialDiffuse[1] = 173.0f / 255.0f; // g
            materialDiffuse[2] = 58.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

            // shininess
            materialShininess = 0.078125f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glColor3f(239.0f / 255.0f, 137.0f / 255.0f, 58.0f / 255.0f);

            glTranslatef(0.0f, 0.0f, 2.4f); // after above rotatef() x = y , y = z, z = x

            glBindTexture(GL_TEXTURE_2D, texture_dryGrass);

            gluCylinder(farmQuadric, 0.49f, 0.3f, 2.0f, 25, 1);

            glTranslatef(0.0f, 0.0f, 2.0f); // after above rotatef() x = y , y = z, z = x
            gluSphere(farmQuadric, 0.3f, 25, 25);

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, texture_dryGrass);
        gluCylinder(farmQuadric, 0.3f, 0.7f, 2.5f, 25, 1);

        glTranslatef(0.0f, 0.0f, 2.5f);
        gluSphere(farmQuadric, 0.7f, 25, 25);

        glBindTexture(GL_TEXTURE_2D, 0);

        glTranslatef(0.0f, 0.0f, 0.0f);
        drawPot();
    }
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT7);

}

void drawPot(void)
{
    glPushMatrix();
    {
        glColor3f(201.0f / 255.0f, 73.0f / 255.0f, 31.0f / 255.0f);
        // brown pot
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 201.0f / 255.0f; // r
        materialDiffuse[1] = 73.0f / 255.0f; // g
        materialDiffuse[2] = 31.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 0.078125f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        gluCylinder(farmQuadric, 1.0f, 0.5f, 0.5f, 25, 1);
        glTranslatef(0.0f, 0.0f, 1.75f);

        // brown pot
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 255.0f / 255.0f; // r
        materialDiffuse[1] = 255.0f / 255.0f; // g
        materialDiffuse[2] = 255.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 0.078125f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        glColor3f(1.0f, 1.0f, 1.0f);

        glBindTexture(GL_TEXTURE_2D, texture_face);

        glPushMatrix();
        {
            glRotatef(20.0f, 0.0f, 0.0f, 1.0);
            gluSphere(farmQuadric, 1.5f, 25, 25);
        }
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, 0);

    }
    glPopMatrix();
}


void drawCropsField(void)
{
    // function declaration
    void drawCrops(void);

    // variable declaration
    int i, j;
    int x = 20;
    int z = 20;

    float fAngle = 0.0f;

    // code

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    for (i = -(x - 2); i <= (x - 2); i = i + 4)
    {
        glPushMatrix();
        {
            // ambient material
            materialAmbient[0] = 0.05f; // r
            materialAmbient[1] = 0.05f; // g
            materialAmbient[2] = 0.0f; // b
            materialAmbient[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

            // diffuse material
            materialDiffuse[0] = 105.0f / 255.0f; // r
            materialDiffuse[1] = 55.0f / 255.0f; // g
            materialDiffuse[2] = 28.0f / 255.0f; // b
            materialDiffuse[3] = 1.0f; // a
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

            // shininess
            materialShininess = 0.078125f * 128;
            glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

            glBindTexture(GL_TEXTURE_2D, texture_soil);

            glTranslatef((GLfloat)i, 0.0f, -(GLfloat)z);

            glScalef(2.0f, 1.0f, 1.0f);
            gluSphere(farmQuadric, 0.68f, 5, 5);
            gluCylinder(farmQuadric, 0.68f, 0.68f, (GLfloat)(z * 2), 5, 1);

            glTranslatef(0.0f, 0.0f, (GLfloat)(z * 2));
            gluSphere(farmQuadric, 0.68f, 5, 5);

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glPopMatrix();


        fAngle = 0.0f;

        for (j = -(z - 2); j <= z; j = j + 4)
        {

            glPushMatrix();
            {
                glTranslatef(i, 0.2f, j);

                glRotatef(fAngle, 0.0f, 1.0f, 0.0f);

                drawCrops();

                if (fAngle <= 90.0f)
                {
                    fAngle = fAngle + 15.0f;
                }
            }
            glPopMatrix();
        }

    }

    // ambient material
    materialAmbient[0] = 0.05f; // r
    materialAmbient[1] = 0.05f; // g
    materialAmbient[2] = 0.0f; // b
    materialAmbient[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    // diffuse material
    materialDiffuse[0] = 105.0f / 255.0f; // r
    materialDiffuse[1] = 55.0f / 255.0f; // g
    materialDiffuse[2] = 28.0f / 255.0f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glBindTexture(GL_TEXTURE_2D, texture_soil);
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f((GLfloat)x, 0.0f, (GLfloat)-(z));

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f((GLfloat)-(x), 0.0f, (GLfloat)-(z));

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f((GLfloat)-(x), 0.0f, (GLfloat)z);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f((GLfloat)x, 0.0f, (GLfloat)z);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

}

void drawCrops(void)
{
    // code
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    // material for plant
    // ambient material
    materialAmbient[0] = 0.05f; // r
    materialAmbient[1] = 0.05f; // g
    materialAmbient[2] = 0.0f; // b
    materialAmbient[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

    // diffuse material
    materialDiffuse[0] = 40.0f / 255.0f; // r
    materialDiffuse[1] = 105.0f / 255.0f; // g
    materialDiffuse[2] = 28.0f / 255.0f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glPushMatrix();
    {
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // after rotate x = x, y = z, z = y
        glPushMatrix();
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            glBindTexture(GL_TEXTURE_2D, texture_plant);
            gluCylinder(farmQuadric, 0.2f, 0.1f, 7.0f, 7, 1);

            // leaves 

            glPushMatrix();
            {
                glTranslatef(0.0f, 0.0f, 1.0f);
                glPushMatrix();
                {
                    glRotatef(30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y
                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);

                }
                glPopMatrix();
                glPushMatrix();
                {
                    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y

                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);

                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);
                }
                glPopMatrix();

                glTranslatef(0.0f, 0.0f, 0.5f);
                glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
                glPushMatrix();
                {
                    glRotatef(30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y
                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);

                }
                glPopMatrix();
                glPushMatrix();
                {
                    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y

                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);

                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);
                }
                glPopMatrix();

                glTranslatef(0.0f, 0.0f, 1.0f);
                glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
                glPushMatrix();
                {
                    glRotatef(30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y
                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);

                }
                glPopMatrix();
                glPushMatrix();
                {
                    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y

                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);

                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);
                }
                glPopMatrix();

                glTranslatef(0.0f, 0.0f, 1.0f);
                glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
                glPushMatrix();
                {
                    glRotatef(30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y
                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);

                }
                glPopMatrix();
                glPushMatrix();
                {
                    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y

                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);

                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);
                }
                glPopMatrix();

                glTranslatef(0.0f, 0.0f, 0.8f);
                glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
                glPushMatrix();
                {
                    glRotatef(30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y
                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);

                }
                glPopMatrix();
                glPushMatrix();
                {
                    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y

                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);

                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);
                }
                glPopMatrix();

                glTranslatef(0.0f, 0.0f, 0.5f);
                glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
                glPushMatrix();
                {
                    glRotatef(30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y
                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);

                }
                glPopMatrix();
                glPushMatrix();
                {
                    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f); // after rotate x = x, y = z, z = y

                    glScalef(0.5f, 1.5f, 1.0f);
                    gluCylinder(farmQuadric, 0.1f, 0.08f, 1.5f, 7, 1);

                    glTranslatef(0.0f, 0.0f, 1.5f);
                    gluSphere(farmQuadric, 0.08f, 15, 15);

                    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
                    gluCylinder(farmQuadric, 0.08f, 0.05f, 1.0f, 7, 1);
                    glTranslatef(0.0f, 0.0f, 1.0f);
                    gluSphere(farmQuadric, 0.05f, 15, 15);
                }
                glPopMatrix();
            }
            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glPopMatrix();


        // material for bazari
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

        // diffuse material
        materialDiffuse[0] = 251.0f / 255.0f; // r
        materialDiffuse[1] = 177.0f / 255.0f; // g
        materialDiffuse[2] = 89.0f / 255.0f; // b
        materialDiffuse[3] = 1.0f; // a
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

        // shininess
        materialShininess = 0.078125f * 128;
        glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

        glTranslatef(0.0f, 0.0f, 7.0f);
        glScalef(0.5f, 0.5f, 2.0f);

        glBindTexture(GL_TEXTURE_2D, texture_bazari);

        glColor3f(251.0f / 255.0f, 177.0f / 255.0f, 89.0f / 255.0f);

        gluSphere(farmQuadric, 0.5f, 15, 15);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glPopMatrix();

    //glDisable(GL_CULL_FACE);
}

void drawField(void)
{
    // variable declaration
    int i;
    int x = 20;
    int z = 20;

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
    materialDiffuse[0] = 105.0f / 255.0f; // r
    materialDiffuse[1] = 55.0f / 255.0f; // g
    materialDiffuse[2] = 28.0f / 255.0f; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // shininess
    materialShininess = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glBindTexture(GL_TEXTURE_2D, texture_soil);
    for (i = -(x - 2); i <= (x - 2); i = i + 4)
    {
        glPushMatrix();
        {
            glTranslatef((GLfloat)i, 0.0f, -(GLfloat)z);

            glScalef(2.0f, 1.0f, 1.0f);
            gluSphere(farmQuadric, 0.68f, 5, 5);
            gluCylinder(farmQuadric, 0.68f, 0.68f, (GLfloat)(z * 2), 5, 1);

            glTranslatef(0.0f, 0.0f, (GLfloat)(z * 2));
            gluSphere(farmQuadric, 0.68f, 5, 5);
        }
        glPopMatrix();
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, texture_soil);
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f((GLfloat)x, 0.0f, (GLfloat)-(z));

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f((GLfloat)-(x), 0.0f, (GLfloat)-(z));

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f((GLfloat)-(x), 0.0f, (GLfloat)z);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f((GLfloat)x, 0.0f, (GLfloat)z);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

void FarmUninitialize(void)
{
    // code

    if (texture_ceiling)
    {
        glDeleteTextures(1, &texture_ceiling);
        texture_ceiling = 0;
    }

    if (texture_wall1)
    {
        glDeleteTextures(1, &texture_wall1);
        texture_wall1 = 0;
    }

    if (texture_door)
    {
        glDeleteTextures(1, &texture_door);
        texture_door = 0;
    }

    if (texture_wall)
    {
        glDeleteTextures(1, &texture_wall);
        texture_wall = 0;
    }


    if (texture_face)
    {
        glDeleteTextures(1, &texture_face);
        texture_face = 0;
    }

    if (texture_dryGrass)
    {
        glDeleteTextures(1, &texture_dryGrass);
        texture_dryGrass = 0;
    }

    if (texture_whiteCloth)
    {
        glDeleteTextures(1, &texture_whiteCloth);
        texture_whiteCloth = 0;
    }

    if (texture_bazari)
    {
        glDeleteTextures(1, &texture_bazari);
        texture_bazari = 0;
    }

    if (texture_plant)
    {
        glDeleteTextures(1, &texture_plant);
        texture_plant = 0;
    }

    if (texture_soil)
    {
        glDeleteTextures(1, &texture_soil);
        texture_soil = 0;
    }


    if(farmQuadric)
    {
        gluDeleteQuadric(farmQuadric);
        farmQuadric = NULL;
    }

    
}

