
#include"Scene3.h"

GLuint texture_Ram_07;
GLuint texture_Ram_08;

void initializeScene3(void)
{
	if (loadGLTexture(&texture_Ram_07, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_07)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() Failed For texture_Ram_07\n");
	}

	if (loadGLTexture(&texture_Ram_08, MAKEINTRESOURCE(IDBITMAP_TEXTURE_RAM_08)) == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture()Failed For texture_Ram_08\n");
	}
}

void displayScene3(void)
{
	// code
	if (ELLAPSED_TIME > START_TIME_ENDCREDITS - 32.0f)
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_08);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture_Ram_07);
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

void updateScene3(void)
{

}

void uninitializeScene3(void)
{
	if (texture_Ram_07)
	{
		glDeleteTextures(1, &texture_Ram_07);
		texture_Ram_07 = 0;
	}

	if (texture_Ram_08)
	{
		glDeleteTextures(1, &texture_Ram_08);
		texture_Ram_08 = 0;
	}
}
