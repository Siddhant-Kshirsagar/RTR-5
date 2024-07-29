
#include"Scene1.h"

GLuint texture_Ram_03;
GLuint texture_Ram_04;

void initializeScene1(void)
{
	if (loadGLTexture(&texture_Ram_03, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_03)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed For texture_Ram_03\n");
	}

	if (loadGLTexture(&texture_Ram_04, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_04)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture()Failed For texture_Ram_04\n");
	}
}

void displayScene1(void)
{
	// code
	if (ELLAPSED_TIME > START_TIME_SCENE_TWO - 31.0f)
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_04);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_03);
	}

	glBegin(GL_QUADS);

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

void updateScene1(void)
{

}

void uninitializeScene1(void)
{
	if (texture_Ram_03)
	{
		glDeleteTextures(1, &texture_Ram_03);
		texture_Ram_03 = 0;
	}

	if (texture_Ram_04)
	{
		glDeleteTextures(1, &texture_Ram_04);
		texture_Ram_04 = 0;
	}
}

