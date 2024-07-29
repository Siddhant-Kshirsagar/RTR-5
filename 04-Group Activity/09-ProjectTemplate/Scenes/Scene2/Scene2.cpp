
#include"Scene2.h"

GLuint texture_Ram_05;
GLuint texture_Ram_06;

void initializeScene2(void)
{
	if (loadGLTexture(&texture_Ram_05, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_05)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed For texture_Ram_05\n");
	}

	if (loadGLTexture(&texture_Ram_06, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_06)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture()Failed For texture_Ram_06\n");
	}
}

void displayScene2(void)
{
	// code
	if (ELLAPSED_TIME > START_TIME_SCENE_THREE - 31.0f)
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_06);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_05);
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

void updateScene2(void)
{

}

void uninitializeScene2(void)
{
	if (texture_Ram_05)
	{
		glDeleteTextures(1, &texture_Ram_05);
		texture_Ram_05 = 0;
	}

	if (texture_Ram_06)
	{
		glDeleteTextures(1, &texture_Ram_06);
		texture_Ram_06 = 0;
	}
}

