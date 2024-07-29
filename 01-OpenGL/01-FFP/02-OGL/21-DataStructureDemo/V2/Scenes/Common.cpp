
#include"./Common.h"
//#include"./Temple.h"
//#include"./Terrain.h"
//#include"./Farm.h"

#define STB_IMAGE_IMPLEMENTATION
#include"../External/stb_image.h"

// variable declaration 
int selectedScene = 0; // selected scene counter (jo scene havay tya scene pasun start honar)
float ELLAPSED_TIME; // global time for scene
FILE *gpFILE = NULL; // for log file
float sceneCompletedTime = 0.0f;

// for camera position
GLfloat gfHorizontal = -5.0f;
GLfloat gfVertical = -4.5f;
GLfloat gfDepth = 155.0f;

// for camera center
GLfloat gfCameraHorizontal = -5.0f;
GLfloat gfCameraVertical = -4.5f;
GLfloat gfCameraDepth = 150.0f;

GLfloat changeFactor = 1.0f;

BOOL bEnableBazierCamera = FALSE;
BOOL switchCameraToMovement = FALSE;

// for light toggle
BOOL gbLight = FALSE; // 'L' / 'l' key

// for texture toggle
BOOL gbTexture = TRUE; // 'Y' / 'y' key

BOOL animationStart = FALSE; // 'T' / 't' key

	// variable declaration
GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat lightAmbientOne[] = { 0.1f,0.1f,0.1f,1.0f };
GLfloat lightDiffuseOne[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightSpecularOne[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightPositionOne[] = { 100.0f,-100.0f,100.0f,1.0f };


GLfloat materialAmbient[4];
GLfloat materialDiffuse[4];
//GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f,1.0f };
GLfloat materialShininess;


int iScreenWidth;
int iScreenHeight;

#pragma region FOG_EFFECT

BOOL gbFogEnable = FALSE;

GLuint filter; // which filter to use
GLuint fogMode[] = { GL_EXP,GL_EXP2,GL_LINEAR }; // Storage for three types of fog
GLuint fogFilter = 2; //which fog to use
GLfloat fogColor[4] = { 0.9f,0.9f,0.9f,1.0f }; // Fog color

#pragma endregion


GLuint base; // base will hold the number of first display list we created

GLYPHMETRICSFLOAT gmf[256]; // will hold the storage and information orientation and placement for 256 outline font


// for color struct
struct MYCOLOR
{
    float fRed;
    float fGreen;
    float fBlue;
};

typedef struct MYCOLOR myColor;

#pragma region Data Structure Linked List



struct Node *create_List(MY_POSITION);
struct Node *insert_node_AtLastPosition(struct Node *first, MY_POSITION position);
struct Node *destroy_list(struct Node *first);

struct Node *create_List(MY_POSITION position)
{
	struct Node *head = NULL;

	head = (struct Node *)malloc(sizeof(struct Node));

	head->myPosition.xAxis = position.xAxis;
	head->myPosition.yAxis = position.yAxis;
	head->myPosition.zAxis = position.zAxis;
	head->next = NULL;

	struct Node *temp = head;

	return(head);
}

struct Node *insert_node_AtLastPosition(struct Node *first, MY_POSITION position)
{
	struct Node *Last = (struct Node *)malloc(sizeof(struct Node));
	struct Node *temp = first;

	if (first == NULL)
	{
		Last->myPosition.xAxis = position.xAxis;
		Last->myPosition.yAxis = position.yAxis;
		Last->myPosition.zAxis = position.zAxis;
		Last->next = NULL;
		first = Last;
	}
	else
	{
		while (temp->next != NULL)
		{
			temp = temp->next;
		}

		temp->next = Last;
		Last->myPosition.xAxis = position.xAxis;
		Last->myPosition.yAxis = position.yAxis;
		Last->myPosition.zAxis = position.zAxis;
		Last->next = NULL;
	}

	return(first);
}

struct Node *destroy_list(struct Node *first)
{
	struct Node *temp = first;
	while (temp != NULL)
	{
		temp = first->next;
		free(first);
		first = temp;
	}
	return(temp);
}


#pragma endregion



BOOL fadeInEnabled = FALSE;
GLfloat fadeInAlpha = 1.0f;
BOOL fadeOutEnabled = FALSE;
GLfloat fadeOutAlpha = 0.0f;
float fadeInAlphaFactor = 0.015f;
float fadeOutAlphaFactor = 0.015f;

extern BOOL scene1_Three;


void fadeIn(void)
{
	if (fadeInAlpha >= 0.0f)
	{
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -0.5f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);
		glColor4f(0.0f, 0.0f, 0.0f, fadeInAlpha);

		glVertex3f(10.0f, 10.0f, 0.0f);
		glVertex3f(-10.0f, 10.0f, 0.0f);
		glVertex3f(-10.0f, -10.0f, 0.0f);
		glVertex3f(10.0f, -10.0f, 0.0f);

		glEnd();

		glDisable(GL_BLEND);
	}
}

void fadeOut(void)
{
	if (fadeOutAlpha > 0.0f)
	{
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -0.1f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);
		glColor4f(0.0f, 0.0f, 0.0f, fadeOutAlpha);

		glVertex3f(10.0f, 10.0f, 0.0f);
		glVertex3f(-10.0f, 10.0f, 0.0f);
		glVertex3f(-10.0f, -10.0f, 0.0f);
		glVertex3f(10.0f, -10.0f, 0.0f);

		glEnd();

		glDisable(GL_BLEND);
	}
}

void resetFadingFactors()
{
	fadeInEnabled = FALSE;
	fadeOutEnabled = FALSE;
	fadeInAlpha = 1.0f;
	fadeOutAlpha = 0.0f;
}

// for texture
// for .bmp
BOOL loadGLTexture(GLuint *texture, TCHAR imageResourceID[])
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

// for .png
BOOL loadGLPNGTexture(GLuint *texture, char *image)
{
	//local variable declaration
	int iWidth, iHeight, iComponents;

	unsigned char *Data = stbi_load(image, &iWidth, &iHeight, &iComponents, 0);

	if (!Data)
	{
		fprintf(gpFILE, "LoadPNGTexture Failed For %s : \n", image);
		stbi_image_free(Data);
	}
	else
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, texture);

		glBindTexture(GL_TEXTURE_2D, *texture);

		// set texture parameter
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

		glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

		//Delete Object
		stbi_image_free(Data);

		return(TRUE);
	}

	return(FALSE);
}





GLvoid BuildFont(GLvoid)
{
	HFONT font; // will hold the window font id
	base = glGenLists(256); // storage for 256

	font = CreateFont(
		-12,	// Height of the font in pixel (negative value means the font is based on the height and width of the device)
		0,		// width of the font (0 means the font width is the same as the height)
		0,		// angle of escapement(rotation) of the font
		0,		// orientation angle
		FW_BOLD,		//Font weight (how thick the characters appear)
		FALSE,			// Italic attribute
		FALSE,			// underline attribute a line throught a character
		FALSE,			// character set (ANSI_CHARSET is a character set that include most characters used in Western languages)
		ANSI_CHARSET,
		OUT_TT_PRECIS,		//Output precision (use TrueType font)
		CLIP_DEFAULT_PRECIS,		// Clipping percision
		ANTIALIASED_QUALITY,		// Output quality (use antialiasing to improve font smoothness)
		DEFAULT_PITCH,		//Font pitch and family (how characters are spaced)-> vertical spacing bet line and text and family is group of font similar to design character set
		"Algerian"
	);

	SelectObject(ghdc, font);

	wglUseFontOutlines(ghdc, 0, 255, base, 10.0f, 0.2f, WGL_FONT_POLYGONS, gmf);
}

GLvoid KillFont(GLvoid)
{
	glDeleteLists(base, 256); // deleting the list
}

GLvoid glPrint(const char *fmt, ...)
{
	float length = 0;
	char text[256];
	va_list ap; // for handling the variable arguments

	if (fmt == NULL)
	{
		return;
	}

	va_start(ap, fmt); // initialize the va_list to start the variable argument processing 
	vsprintf(text, fmt, ap); // formating the variable argument according to the fmt
	va_end(ap);

	for (unsigned int loop = 0; loop < (strlen(text)); loop++)
	{
		length = length + gmf[text[loop]].gmfCellIncX;
	}

	glTranslatef(-length / 2.0f, 0.0f, 0.0f);

	glPushAttrib(GL_LIST_BIT); // push that flag which hold the state of font in the attribute stack

	glListBase(base); // starting of display list

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // call the list (render the list)

	glPopAttrib(); // state restore before calling push attrib

}





// for audio initialize
BOOL initializeAudio(void)
{

	return(TRUE);
}

// for audio uninitialize
BOOL uninitalizeAudio(void)
{
	// code
	return(TRUE);
}


int initializeCommonScene(void)
{
    // variable declaration
    BOOL bResult = FALSE;

    // code


	return(0);
}



void displayCommonScene(void)
{



}

void updateCommonScene(void)
{
	// code

	if (fadeInEnabled)
	{
		if (fadeInAlpha >= 0.0f)
		{
			fadeInAlpha -= fadeInAlphaFactor;
		}
	}

	if (fadeOutEnabled)
	{
		if (fadeOutAlpha <= 1.0f)
		{
			fadeOutAlpha += fadeOutAlphaFactor;
		}

	}

}



void uninitializeCommonScene(void)
{
	// code

	
}

