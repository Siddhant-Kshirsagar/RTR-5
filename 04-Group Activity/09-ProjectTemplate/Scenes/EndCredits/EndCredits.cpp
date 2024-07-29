
#include"EndCredits.h"

GLuint texture_Ram_09;
GLuint texture_Ram_10;

void initializeEndCreadits(void)
{
	if (loadGLTexture(&texture_Ram_09, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_09)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed For texture_Ram_09\n");
	}

	if (loadGLTexture(&texture_Ram_10, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_10)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture()Failed For texture_Ram_10\n");
	}
}

void displayEndCreadits(void)
{
	// code
	if (ELLAPSED_TIME > (START_TIME_ENDCREDITS + 63.0f)-32.0f)
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_09);
	}
	else if (ELLAPSED_TIME < (START_TIME_ENDCREDITS + 63.0f))
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_10);
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

void updateEndCreadits(void)
{

}

void uninitializeEndCreadits(void)
{
	if (texture_Ram_09)
	{
		glDeleteTextures(1, &texture_Ram_09);
		texture_Ram_09 = 0;
	}

	if (texture_Ram_10)
	{
		glDeleteTextures(1, &texture_Ram_10);
		texture_Ram_10 = 0;
	}
}
