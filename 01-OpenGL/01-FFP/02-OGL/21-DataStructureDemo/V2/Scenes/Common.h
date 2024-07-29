#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h> // for va_list functions

#include<Windows.h>

#include<gl/GL.h>
#include<gl/GLU.h>

#include"Common.h"
#include"../OGLMain.h"


//#define STB_IMAGE_IMPLEMENTATION
//#include"../External/stb_image.h"

#include<vector>

enum SCENE
{
	SCENE_OPENING = 0,
	SCENE_ONE,
	SCENE_TWO,
	SCENE_THREE,
	SCENE_FOUR,
	SCENE_ENDCREDITS,
};

enum title
{
	TITLE_ONE = 1,
	TITLE_TWO,
	TITLE_THREE,
	TITLE_FOUR,
	TITLE_FIVE,
	TITLE_SIX,
	TITLE_SEVEN,
	TITLE_EIGHT,
	TITLE_NINE,
	TITLE_TEN,
	TITLE_ELEVEN,
	TITLE_TWELVE,
	TITLE_THIRTEEN,
	TITLE_FOURTEEN,
	TITLE_FIFTEEN,
	TITLE_SIXTEEN,
	TITLE_SEVENTEEN,
	TITLE_EIGHTEEN,
	TITLE_NINETEEN,
	TITLE_TWENTY,
};

struct MYPOSITION
{
	float xAxis;
	float yAxis;
	float zAxis;
};

typedef struct MYPOSITION MY_POSITION;

struct Node
{
	MY_POSITION myPosition;
	struct Node *next;
};

struct Node *create_List(MY_POSITION);
struct Node *insert_node_AtLastPosition(struct Node *first, MY_POSITION position);
struct Node *destroy_list(struct Node *first);

// for scene time
#define START_TIME_OPENING 0.0f			
#define START_TIME_SCENE_ONE 21.0f	
#define START_TIME_SCENE_TWO 82.0f	
#define START_TIME_SCENE_TWO_PART_TWO (START_TIME_SCENE_TWO + 9.0f)
#define START_TIME_SCENE_TWO_PART_THREE (START_TIME_SCENE_TWO_PART_TWO + 9.0f)
#define START_TIME_SCENE_THREE 123.0f		
#define START_TIME_SCENE_FOUR 161.0f
#define START_TIME_ENDCREDITS 204.0f

#define SKIP_ELLAPSED_TIME START_TIME_SCENE_FOUR


#define FADE_OUT_TIME_DURATION 4.0f
#define FADE_IN_TIME_DURATION 4.0f

extern float sceneCompletedTime;

extern BOOL scene3_One;
extern BOOL scene3_Two;
extern BOOL scene3_Three;

extern GLfloat lightAmbient[];
extern GLfloat lightDiffuse[];
// GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
extern GLfloat lightPosition[];

extern GLfloat lightAmbientOne[];
extern GLfloat lightDiffuseOne[];
//GLfloat lightSpecularOne[] = { 0.0f,0.0f,1.0f,1.0f };
extern GLfloat lightPositionOne[];

extern GLfloat materialAmbient[];
extern GLfloat materialDiffuse[];
// GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f,1.0f };
extern GLfloat materialShininess;


// global variables
extern FILE *gpFILE;			// for use in others file 
extern int selectedScene; // selected scene counter (jo scene havay tya scene pasun start honar)
extern float ELLAPSED_TIME; // global time for scene

extern GLfloat gfHorizontal;
extern GLfloat gfVertical;
extern GLfloat gfDepth;

extern GLfloat gfCameraHorizontal;
extern GLfloat gfCameraVertical;
extern GLfloat gfCameraDepth;

extern GLfloat changeFactor;

extern BOOL switchCameraToMovement;
extern BOOL bEnableBazierCamera;

extern BOOL gbLight;
extern BOOL animationStart;
extern BOOL gbTexture;

extern int iScreenWidth;
extern int iScreenHeight;

void fadeIn(void);
void fadeOut(void);
void resetFadingFactors(void);

extern BOOL fadeInEnabled;
extern GLfloat fadeInAlpha;
extern BOOL fadeOutEnabled;
extern GLfloat fadeOutAlpha;
extern float fadeInAlphaFactor;
extern float fadeOutAlphaFactor;

extern BOOL scene2_ONE ;
extern BOOL scene2_TWO ;
extern BOOL scene2_THREE ;

#pragma region FOG_EFFECT

extern BOOL gbFogEnable;

extern GLuint filter; // which filter to use
extern GLuint fogMode[]; // Storage for three types of fog
extern GLuint fogFilter; //which fog to use
extern GLfloat fogColor[4]; // Fog color

#pragma endregion

//function declaration
// 1st param (float) center x Coordinate of body
// 2nd param (float) center y coordinate of body
// 3rd param (float) center z coordinate of body
void drawBody(float, float, float);

// 1st param (float) center x Coordinate of body
// 2nd param (float) center y coordinate of body
// 3rd param (float) center z coordinate of body
void drawBody_1(float, float, float);

void drawPanati(void);


// global function declaration
BOOL loadGLTexture(GLuint *texture, TCHAR imageResourceID[]);
BOOL loadGLPNGTexture(GLuint *texture, char *image);


GLvoid BuildFont(GLvoid);
GLvoid KillFont(GLvoid);
GLvoid glPrint(const char *fmt, ...);

BOOL initializeAudio(void);
BOOL uninitalizeAudio(void);


int initializeCommonScene(void);
void displayCommonScene(void);
void updateCommonScene(void);
void uninitializeCommonScene(void);
