#pragma once

#include<stdio.h>
#include<stdlib.h>

#include<Windows.h>

#include<gl/GL.h>
#include<gl/GLU.h>

#include"Common.h"
#include"../OGLMain.h"

enum SCENE
{
	SCENE_OPENING = 0,
	SCENE_ONE,
	SCENE_TWO,
	SCENE_THREE,
	SCENE_ENDCREDITS,
};

#define START_TIME_OPENING 0.0f				// 0 to 10 sec opening scene
#define START_TIME_SCENE_ONE 78.0f			// 10 to 20 sec scene 1
#define START_TIME_SCENE_TWO 141.0f			// 20 to 30 sec scene 2 
#define START_TIME_SCENE_THREE 204.0f		// 30 to 40 sec scene 3
#define START_TIME_ENDCREDITS 268.0f			// 40 to 50 sec end credits


// global variables
extern FILE *gpFILE;			// for use in others file 
extern int selectedScene; // selected scene counter (jo scene havay tya scene pasun start honar)
extern float ELLAPSED_TIME; // global time for scene

// global function declaration
BOOL loadGLTexture(GLuint *texture, TCHAR imageResourceID[]);
BOOL initializeAudio(void);
BOOL uninitalizeAudio(void);
