
#include"Opening.h"

GLuint texture_Ram_01;
GLuint texture_Ram_02;

void initializeOpening(void)
{
	if (loadGLTexture(&texture_Ram_01, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_01)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed For texture_Ram_01\n");
	}

	if (loadGLTexture(&texture_Ram_02, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_02)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture()Failed For texture_Ram_02\n");
	}
}

void displayOpening(void)
{
	// code
	if(ELLAPSED_TIME > START_TIME_SCENE_ONE - 39.0f)
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_02);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_01);
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

void updateOpening(void)
{

}

void uninitializeOpening(void)
{
	if (texture_Ram_01)
	{
		glDeleteTextures(1, &texture_Ram_01);
		texture_Ram_01 = 0;
	}

	if (texture_Ram_02)
	{
		glDeleteTextures(1, &texture_Ram_02);
		texture_Ram_02 = 0;
	}
}
