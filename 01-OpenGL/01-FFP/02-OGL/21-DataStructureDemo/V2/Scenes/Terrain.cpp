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

#include "Terrain.h"


//#define STB_IMAGE_IMPLEMENTATION
#include"../External/stb_image.h"

// Below is the code related to Project

#pragma region TEXTURE

GLuint texture_mountain = 0;
GLuint texture_clouds = 0;
GLuint texture_landscape = 0;
GLuint texture_tree = 0;
GLuint texture_tree_1 = 0;
GLuint texture_tree_2 = 0;
GLuint texture_whiteCloudSky = 0;
GLuint texture_grass = 0;
extern GLuint texture_whiteCloth;

#pragma endregion 

#pragma region TERRAIN

#define MAP_SIZE 256              // Size Of Our .BMP Height Map

float terrainHeightMap[MAP_SIZE][MAP_SIZE][3]; // use for store terrain data

unsigned char *Data; // use to read height map data

void RenderHeightMap(void);


#pragma endregion


#pragma region TREE

void drawTree(void);
void drawTree_1(void);
void drawTree_2(void);

BOOL gbShowTree = FALSE;

#pragma endregion

extern BOOL scene4_Three;

GLUquadric *terrainQuadric = NULL;

GLfloat yRotation = 0.0f;




int initializeScene(void)
{


	// variable declaration
	BOOL bResult;
	int sizeOfArray;
	int iWidth, iHeight, iComponents;
	float x, z;


	// code

	// load texture
	bResult = loadGLTexture(&texture_mountain, MAKEINTRESOURCE(MY_BITMAP_MOUNTAIN));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of Mountain texture is Failed.\n");
		return(-1);
	}

	bResult = loadGLTexture(&texture_clouds, MAKEINTRESOURCE(MY_BITMAP_CLOUD));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of cloud texture is Failed.\n");
		return(-2);
	}

	bResult = loadGLTexture(&texture_landscape, MAKEINTRESOURCE(MY_BITMAP_LANDSCAPE));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of landscape texture is Failed.\n");
		return(-3);
	}

	bResult = loadGLPNGTexture(&texture_tree, "./Assets/Texture/tree.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLPNGTexture() failed\n");
		return(-4);
	}
	bResult = loadGLPNGTexture(&texture_tree_1, "./Assets/Texture/tree_1.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLPNGTexture() failed\n");
		return(-5);
	}

	bResult = loadGLPNGTexture(&texture_tree_2, "./Assets/Texture/tree_2.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLPNGTexture() failed\n");
		return(-6);
	}

	bResult = loadGLTexture(&texture_whiteCloudSky, MAKEINTRESOURCE(MY_BITMAP_WHITE_CLOUD_SKY));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of landscape texture is Failed.\n");
		return(-7);
	}


	bResult = loadGLPNGTexture(&texture_grass, "./Assets/Texture/grass.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLPNGTexture() failed\n");
		return(-8);
	}

	// read height map data
	Data = stbi_load("./Assets/Texture/GreatLakes.bmp", &iWidth, &iHeight, &iComponents, 0);

	// error check 
	if (Data == NULL)
	{
		fprintf(gpFILE, "Image loading failed\n");
		return(-9);
	}

	// generate height map
	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int y = 0; y < MAP_SIZE; y++)
		{

			terrainHeightMap[x][y][0] = (float)x * 30.0f;
			terrainHeightMap[x][y][1] = Data[(x + y * MAP_SIZE) * 3] * 12;
			terrainHeightMap[x][y][2] = -(float)y * 30.0f;

			/*fprintf(gpFILE, "Array Index = %d , Array Value = %d, Array Value After Multiplying = %d (x = %d , y = %d )\n ", (x + y * MAP_SIZE) * 3, Data[(x + y * MAP_SIZE) * 3], Data[(x + y * MAP_SIZE) * 3] * 8, x, y);*/
			//fprintf(gpFILE, "Array value = %f \n", terrainHeightMap[x][y][1]);
		}
	}

	/*fprintf(gpFILE, "Width = %d  Height = %d Size of Array = %d\n", iWidth, iHeight, sizeOfArray); */

	// free data
	if (Data)
	{
		stbi_image_free(Data);
		Data = NULL;
	}

	// enable texture
	glEnable(GL_TEXTURE_2D);

	// quadric implementation
	terrainQuadric = gluNewQuadric();

	return(0);
}




void displayScene(void)
{
	//function declaration
	void buildTerrain(void);
	void drawGrass_1(void);

	// code
	glEnable(GL_TEXTURE_2D);

	buildTerrain();

	{
		glPushMatrix();
		{
			glTranslatef(150.0f, -104.0f, 102.0f);

			glPushMatrix();
			{
				glTranslatef(0.0f, -1.0f, 0.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(3.5f, 5.5f, 2.5f);
				glColor3f(1.0f, 1.0f, 1.0f);
				drawTree();
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(6.0f, -3.25f, 0.0f);
				glRotatef(yRotation, 0.0f, 1.0f, 0.0f);
				glScalef(13.0f, 1.0f, 13.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glColor4f(47.0f / 255.0f, 201.0f / 255.0f, 219.0f / 255.0f, 0.5f);
				glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);
				glBegin(GL_QUADS);

				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(1.0f, 0.0f, -1.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-1.0f, 0.0f, -1.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-1.0f, 0.0f, 1.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(1.0f, 0.0f, 1.0f);

				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);

				glDisable(GL_BLEND);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.0f, -1.0f, 0.0f);
				glTranslatef(3.5f, 1.5f, 2.0f);
				glColor3f(1.0f, 1.0f, 1.0f);
				drawTree();
			}
			glPopMatrix();


		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(220.0f, -103.0f, 100.0f);

			int y = 0;
			for (int x = 0; x < 10; x = x + 5)
			{
				for (int z = 0; z < 20; z = z + 5)
				{
					glPushMatrix();
					{
						glTranslatef((GLfloat)x * (3+y), 0.0f, (GLfloat)z * 2);
						drawTree_1();
						y = y % 3 + 1;
					}
					glPopMatrix();
				}
			}

		}
		glPopMatrix();

	}

	// for grass type 1
	glPushMatrix();
	{
		glTranslatef(275.0f, -107.0f, 160.0f);
		for (int x = -4.0f; x <= 4.0f; x = x + 1)
		{
			for (int z = 0.0f; z <= 2.0f; z = z + 2)
			{
				glTranslatef((GLfloat)x, 0.0f, (GLfloat)z);
				glPushMatrix();
				{	
					//glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
					//glScalef(10.0f, 10.0f, 10.0f);
					glColor3f(1.0f, 1.0f, 1.0f);
					drawGrass_1();
				}
				glPopMatrix();
			}
		}

	}
	glPopMatrix();
}

void drawGrass_1(void)
{
	//variable declaration
	float modelview[16];
	int i, j;

	//code
	glBindTexture(GL_TEXTURE_2D, texture_grass);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	{
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

		glPushMatrix();
		{
			//glTranslatef((GLfloat)x, 0.0f, (GLfloat)z);

			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);


			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);


			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glEnd();

		}
		glPopMatrix();

		glDisable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
}


void buildTerrain(void)
{
	//code
	glPushMatrix();
	{
		glTranslatef(200.0f, 375.0f, 0.0f);
		glPushMatrix();
		{
			glTranslatef(-570.0f, -500.0f, 680.0f);
			glPushMatrix();
			{
				glScalef(0.15f, 0.15f, 0.175f);

				RenderHeightMap();
			}
			glPopMatrix();
		}
		glPopMatrix();

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
			materialDiffuse[0] = 1.0f; // r
			materialDiffuse[1] = 1.0f; // g
			materialDiffuse[2] = 1.0f; // b
			materialDiffuse[3] = 1.0f; // a
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

			// shininess
			materialShininess = 0.25f * 128;
			glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

			glTranslatef(0.0f, -400.0f, 0.0f);
			glScalef(580.0f, 100.0f, 660.0f);

			if (selectedScene == 4 && scene4_Three == TRUE)
			{
				glBindTexture(GL_TEXTURE_2D, texture_clouds);

				gluQuadricTexture(terrainQuadric, GL_TRUE);

				glPushMatrix();
				{
					glTranslatef(0.0f, -0.8f, 0.0f);
					glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
					glColor3f(1.0f, 1.0f, 1.0f);
					gluSphere(terrainQuadric, 1.0f, 100, 100);
					
				}
				glPopMatrix();

				gluQuadricTexture(terrainQuadric, GL_FALSE);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texture_whiteCloudSky);

				gluQuadricTexture(terrainQuadric, GL_TRUE);

				glPushMatrix();
				{
					glTranslatef(0.0f, -0.8f, 0.0f);
					glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
					glColor3f(1.0f, 1.0f, 1.0f);
					gluSphere(terrainQuadric, 1.0f, 100, 100);

				}
				glPopMatrix();

				gluQuadricTexture(terrainQuadric, GL_FALSE);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void updateScene(void)
{

	yRotation = yRotation + 1.0f;
	if (yRotation >= 360.0f)
	{
		yRotation = 0.0f;
	}

}

void RenderHeightMap(void)             // This Renders The Height Map As Quads
{


	// code
	int x = 0, y = 0;                        // Create Some Variables For iteration
	float float_x, float_y;					// use for texture coordinates
	float fColor;							// to give different color value according to height

	glPushMatrix();
	{	
		//for ground
		for (x = 0; x < MAP_SIZE - 1; x++)
		{
			glBindTexture(GL_TEXTURE_2D, texture_mountain);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_2D, GL_MODULATE);

			glBegin(GL_TRIANGLE_STRIP);          

			for (y = 0; y < MAP_SIZE - 1; y++)
			{
				float_x = (float)x / (float)(MAP_SIZE - 1);
				float_y = (float)y / (float)(MAP_SIZE - 1);

				glColor3f(1.0f, 1.0f, 1.0f);

				fColor = -0.15f + (terrainHeightMap[x][y][1] / 255.0f);
				fColor = (fColor > 1.0f ? 0.7f : fColor);
				if (fColor < 0.5f)
				{
					glColor3f(fColor, fColor - 0.2f, 0.0f);	
				}
				else
				{
					glColor3f(fColor - 0.9f, fColor - 0.4f, fColor - 0.9f);
				}

				glTexCoord2f(float_x, float_y);    // First Texture Coordinate (Bottom Left)

				glVertex3f(terrainHeightMap[x][y][0], terrainHeightMap[x][y][1], terrainHeightMap[x][y][2]);            // Send This Vertex To OpenGL To Be Rendered

				fColor = -0.15f + (terrainHeightMap[x + 1][y][1] / 255.0f);
				fColor = (fColor > 1.0f ? 0.7f : fColor);
				if (fColor < 0.5f)
				{
					glColor3f(fColor, fColor - 0.2f, 0.0f);
				}
				else
				{
					glColor3f(fColor - 0.9f, fColor - 0.4f, fColor - 0.9f);
				}


				float_x = (float)(x + 1) / (float)(MAP_SIZE - 1);
				float_y = (float)y / (float)(MAP_SIZE - 1);

				glTexCoord2f(float_x, float_y);  // Second Texture Coordinate (Top Left)

				glVertex3f(terrainHeightMap[x + 1][y][0], terrainHeightMap[x + 1][y][1], terrainHeightMap[x + 1][y][2]);             // Send This Vertex To OpenGL To Be Rendered


				fColor = -0.15f + (terrainHeightMap[x][y + 1][1] / 255.0f);
				fColor = (fColor > 1.0f ? 0.7f : fColor);
				if (fColor < 0.5f)
				{
					glColor3f(fColor, fColor - 0.2f, 0.0f);
				}
				else
				{
					glColor3f(fColor - 0.9f, fColor - 0.4f, fColor - 0.9f);
				}

				float_x = (float)(x) / (float)(MAP_SIZE - 1);
				float_y = (float)(y + 1) / (float)(MAP_SIZE - 1);

				glTexCoord2f(float_x, float_y);  // Fourth Texture Coordinate (Bottom Right)
				glVertex3f(terrainHeightMap[x][y + 1][0], terrainHeightMap[x][y + 1][1], terrainHeightMap[x][y + 1][2]);           // Send This Vertex To OpenGL To Be Rendered


				fColor = -0.15f + (terrainHeightMap[x + 1][y + 1][1] / 255.0f);
				fColor = (fColor > 1.0f ? 0.7f : fColor);
				if (fColor < 0.5f)
				{
					glColor3f(fColor, fColor - 0.2f, 0.0f);
				}
				else
				{
					glColor3f(fColor - 0.9f, fColor - 0.4f, fColor - 0.9f);
				}


				float_x = (float)(x + 1) / (float)(MAP_SIZE - 1);
				float_y = (float)(y + 1) / (float)(MAP_SIZE - 1);

				glTexCoord2f(float_x, float_y); // Third Texture Coordinate (Top Right)

				glVertex3f(terrainHeightMap[x + 1][y + 1][0], terrainHeightMap[x + 1][y + 1][1], terrainHeightMap[x + 1][y + 1][2]);             // Send This Vertex To OpenGL To Be Rendered

			}
			glEnd();

			glBindTexture(GL_TEXTURE_2D, 0);

		}
	}
	glPopMatrix();
}

void drawTree(void)
{
	//variable declaration
	float modelview[16];
	int i, j;

	// code
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture_tree);

	//glColor3f(0.5f, 0.5f, 0.5f);

	glPushMatrix();
	{
		// get the current modelview matrix
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

		//// undo all rotations
		//// beware all scaling is lost as well 
		//for (i = 0; i < 3; i++)
		//{
		//	for (j = 0; j < 3; j++) 
		//	{
		//		if (i == j)
		//			modelview[i * 4 + j] = 1.0;
		//		else
		//			modelview[i * 4 + j] = 0.0;
		//	}
		//}


		// The only difference now is that
		// the i variable will jump over the
		// up vector, 2nd column in OpenGL convention
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

		glScalef(4.0f, 4.0f, 4.0f);
		glPushMatrix();
		{
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);

			glEnd();

		}
		glPopMatrix();


		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
	}
	glPopMatrix();

}

void drawTree_1(void)
{
	//variable declaration
	float modelview[16];
	int i, j;

	// code
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture_tree_1);

	//glColor3f(0.5f, 0.5f, 0.5f);

	glPushMatrix();
	{
		// get the current modelview matrix
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

		//// undo all rotations
		//// beware all scaling is lost as well 
		//for (i = 0; i < 3; i++)
		//{
		//	for (j = 0; j < 3; j++) 
		//	{
		//		if (i == j)
		//			modelview[i * 4 + j] = 1.0;
		//		else
		//			modelview[i * 4 + j] = 0.0;
		//	}
		//}


		// The only difference now is that
		// the i variable will jump over the
		// up vector, 2nd column in OpenGL convention
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

		glScalef(4.0f, 4.0f, 4.0f);
		glPushMatrix();
		{
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);

			glEnd();

		}
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
	}
	glPopMatrix();
}

void uninitializeScene(void)
{
	// code
	if (texture_grass)
	{
		glDeleteTextures(1, &texture_grass);
		texture_grass = 0;
	}

	if (terrainQuadric)
	{
		gluDeleteQuadric(terrainQuadric);
		terrainQuadric = NULL;
	}

	if (texture_whiteCloudSky)
	{
		glDeleteTextures(1, &texture_whiteCloudSky);
		texture_whiteCloudSky = 0;
	}

	if (texture_tree_2)
	{
		glDeleteTextures(1, &texture_tree_2);
		texture_tree_2 = 0;
	}

	if (texture_tree_1)
	{
		glDeleteTextures(1, &texture_tree_1);
		texture_tree_1 = 0;
	}

	if (texture_tree)
	{
		glDeleteTextures(1, &texture_tree);
		texture_tree = 0;
	}

	if (texture_landscape)
	{
		glDeleteTextures(1, &texture_landscape);
		texture_landscape = 0;
	}

	if (texture_mountain)
	{
		glDeleteTextures(1, &texture_mountain);
		texture_mountain = 0;
	}

	if (texture_clouds)
	{
		glDeleteTextures(1, &texture_clouds);
		texture_clouds = 0;
	}



}
