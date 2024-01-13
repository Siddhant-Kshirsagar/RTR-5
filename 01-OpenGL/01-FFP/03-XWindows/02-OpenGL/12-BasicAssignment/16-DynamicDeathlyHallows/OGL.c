//standard header file
#include<stdio.h> // for printf()
#include<stdlib.h> // for exit()
#include<memory.h> // for memset()

#define _USE_MATH_DEFINES 1
#include<math.h>

// x11 header file
#include<X11/Xlib.h> // for all XWindow API (in window Windows.h similarly int XWindow X11/Xlib.h)
#include<X11/Xutil.h> // for XVisualInfo and related API
#include<X11/XKBlib.h>

// OpenGL header file
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

// OpenAL related header file (for audio)
#include<AL/alut.h>

// Macros
#define WINWIDTH 800
#define WINHEIGHT 600

// global variable declaration
Display *display = NULL; // Display is interface between XServer and Client
Colormap colormap;
Window window;

Bool bFullScreen = False;

Bool bActiveWindow = False;

// OpenGL global variable
GLXContext glxContext = NULL;
XVisualInfo *visualInfo = NULL;

// OpenAL related variable declaration
ALCenum error; // for error checking 
ALuint audioBuffer,audioSource; // VRAM madhil address cha represent karnara interger variable store karnyasathi

FILE *gpFILE = NULL;

float length = 1.0f; // lenght of side of equilateral triangle 
float semiPerimeter = 0.0f;
float radius = 0.0f;
float aSide, bSide, cSide;
float xCenter, yCenter;
float height = 0.0f;
float xTop, yTop;
float fAngle = 0.0f;
float gfTriangle_X = -3.0f, gfTriangle_Y = -3.0f;
float gfCircle_X = 3.0f, gfCircle_Y = -3.0f;
float gfLine_Y = 3.0f;
float gfMovement = 0.007f;
float xAxisNegative ;
float yAxisNegative ;
float xAxisPositive ;
float yAxisPositive ;

Bool gbCenter = False;

// entry point function
int main(void)
{
    // local function declaration
    void uninitialize(void);
    int initialize(void);
    void resize(int,int);
    void draw(void);
    void update(void);
    void ToggleFullScreen(void);

    // local variable declaration
    int defaultScreen;
    // int defaultDepth;

    XSetWindowAttributes windowAttribute;
    Colormap colormap;
    int styleMask;
    Atom windowManagerDelete;
    XEvent event;
    KeySym keySym;

    int screenWidth;
    int screenHeight;

    char keys[26];

    int frameBufferAttribute[] = 
    { 
        GLX_DOUBLEBUFFER, True,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE,24,
        None
    };

    Bool bDone = False;
    int iResult = 0;

    // code

    gpFILE = fopen("Log.txt","w");
    if(gpFILE == NULL)
    {
        printf("Log file cannot be open\n");
        exit(0);
    }
    fprintf(gpFILE,"Program started successfully\n");

    // step 1: open connection with xserver and get display interface
    display = XOpenDisplay(NULL);

    if(display == NULL)
    {
        printf("\n\tXOpenDisplay() failed.\n");
        uninitialize();
        exit(1); // erroneous or abortive exit
    }

    // step 2: get default screen from above display
    defaultScreen = XDefaultScreen(display);

    visualInfo = glXChooseVisual(display,defaultScreen,frameBufferAttribute);
    if(visualInfo == NULL)
    {
        printf("\n\tglXChooseVisual() failed.\n");
        uninitialize();
        exit(1);
    }

    // step 5: set window attribute
    memset((void*)&windowAttribute,0,sizeof(XWindowAttributes));
    windowAttribute.border_pixel = 0; // window border color default use karnya sathi 0 dila
    windowAttribute.background_pixel = XBlackPixel(display,visualInfo->screen); // background la black color denyasathi
    windowAttribute.background_pixmap = 0; // background la brush/image deto tevha hi value dyaychi atta black color detoy mhanun ha 0 dila
    windowAttribute.colormap = XCreateColormap(display,
    XRootWindow(display,visualInfo->screen),
    visualInfo->visual,
    AllocNone);

    // step 6: assign this Colormap to global Colormap
    colormap = windowAttribute.colormap;

    // step 7:set the style of window
    styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;

    // step 8 : create window
    window = XCreateWindow(display,
    XRootWindow(display,visualInfo->screen),
    0,
    0,
    WINWIDTH,
    WINHEIGHT,
    0,
    visualInfo->depth,
    InputOutput,
    visualInfo->visual,
    styleMask,
    &windowAttribute
    );

    if(!window)
    {
        printf("\n\tXCreateWindow() failed.\n");
        uninitialize();
        exit(1);
    }

    // step 9: specify to which event this window should response
    XSelectInput(display,window,
    ExposureMask | VisibilityChangeMask | StructureNotifyMask | 
    KeyPressMask | ButtonPressMask | PointerMotionMask | FocusChangeMask);

    // step 10: specify window manager Atom
    windowManagerDelete = XInternAtom(display,"WM_DELETE_WINDOW",True);

    // step 11 : add above Atom as protocol for window manager
    XSetWMProtocols(display,window,&windowManagerDelete,1);

    // step 12 : give caption to the window
    XStoreName(display,window,"Siddhant Ganesh Kshirsagar : XWindow");

    // step 13 : show / map the window
    XMapWindow(display,window);

    // center the window
    screenWidth = XWidthOfScreen(XScreenOfDisplay(display,visualInfo->screen));
    screenHeight = XHeightOfScreen(XScreenOfDisplay(display,visualInfo->screen));
    XMoveWindow(display,window,(screenWidth - WINWIDTH)/2,(screenHeight-WINHEIGHT)/2);

    // OpenGL initialization
    iResult = initialize();
    if(iResult != 0)
    {
        printf("initialize () failed.\n");
        uninitialize();
        exit(1);
    }

    // step 14 : game loop
    memset((void*)&event,0,sizeof(XEvent));
    while(bDone == False)
    {
        while(XPending(display))
        {
            XNextEvent(display,&event);
            switch (event.type)
            {
            case KeyPress:
            {
                // smilar to WM_KEYDOWN in windows O.S
                keySym = XkbKeycodeToKeysym(display,
                event.xkey.keycode,0,0);
                switch (keySym)
                {
                case XK_Escape:
                    bDone = True;
                    break;
                
                default:
                    break;
                }
                // similar to WM_CHAR in windows O.S
                XLookupString(&event.xkey,
                keys,
                sizeof(keys),
                NULL,
                NULL);
                switch(keys[0])
                {
                    case 'F':
                    case 'f':
                    if(bFullScreen == False)
                    {
                        ToggleFullScreen();
                        bFullScreen = True;
                    }
                    else
                    {
                        ToggleFullScreen();
                        bFullScreen = False;
                    }
                    break;

                    default:
                    break;
                }
            }
                break;

            case MapNotify:
                
                break;

            case FocusIn: // similar to WM_SETFOCUS in windows
                bActiveWindow = True;
                break;

            case FocusOut: // similar to WM_KILLFOCUS in windows
                bActiveWindow = False;
                break;

            case ConfigureNotify: // similar to WM_SIZE in windows
                resize(event.xconfigure.width,event.xconfigure.height);
                break;

            case ButtonPress:
                switch (event.xbutton.button)
                {
                case 1:
                    
                    break;
                case 2:
                    
                    break;
                case 3:
                    
                    break;
                default:
                    break;
                }
                break;

            case 33:
                bDone = True;
                break;
            
            default:
                break;
            }
        }
        // rendering 
        if(bActiveWindow == True)
        {
            // display
            draw();

            // update
            update();
        }
    }
    
    uninitialize();
    return(0);
}

void ToggleFullScreen(void)
{
    // local variable declaration
    Atom windowManagerStateNormal;
    Atom windowManagerStateFullScreen;
    XEvent event;

    //code
    windowManagerStateNormal = XInternAtom(display,"_NET_WM_STATE",False);

    windowManagerStateFullScreen = XInternAtom(display,"_NET_WM_STATE_FULLSCREEN",False);

    // use memset to initialize all members of XEvent to 0 and give above values of Atom
    memset((void*)&event,0,sizeof(XEvent));
    event.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.message_type = windowManagerStateNormal;
    event.xclient.format = 32;
    event.xclient.data.l[0] = bFullScreen ? 0 : 1;
    event.xclient.data.l[1] = windowManagerStateFullScreen;

    // send event
    XSendEvent(display,
    XRootWindow(display,visualInfo->screen),
    False,
    SubstructureNotifyMask,
    &event); 
}

int initialize(void)
{
    // local function declaration
    void resize(int,int);
    void initializeAudio(void);
    
    // code
    // create OpenGL context
    glxContext = glXCreateContext(display,visualInfo,NULL,True);
    if(glxContext == NULL)
    {
        printf("glXCreateContext() failed()\n");
        return(1);
    }

    // make this OpenGL context as current context
    if(glXMakeCurrent(display,window,glxContext) == False)
    {
        printf("glXMakeCurrent() failed\n");
        return(2);
    }

    //for enable depth
	glShadeModel(GL_SMOOTH); // optional (beautyfication color,light,texture shade)
	glClearDepth(1.0f); // compulsory
	glEnable(GL_DEPTH_TEST);// compulsory
	glDepthFunc(GL_LEQUAL);// compulsory
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // optional (beautyfication for artifact e.g if we draw circle or sphere we see ellipse curve but we don't need that so we disable that)


    // clear color
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    
	initializeAudio();

    // warm up Resize call
    resize(WINWIDTH,WINHEIGHT);

    return(0);
}

void resize(int width,int height)
{
    // code
    if(height == 0)
    {
        height = 1;
    }
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}

void draw(void) // similar to display() in windows
{
    //function declaration

    // 1st param radius of circle
	// 2nd param x coordinate of center of circle
	// 3rd param y coordinate of center of cirlce
	void drawCircle(float,float,float);
	// param length of side triangle 
	void drawTriangle(float);

    //variable declaration
	 xAxisNegative = -(length * 0.5f);
	 yAxisNegative = -(length * 0.5f);
	 xAxisPositive = (length * 0.5f);
	 yAxisPositive = (length * 0.5f);
    
    // code
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    bSide = sqrt((pow((xAxisNegative - xAxisPositive), 2)) + (pow((yAxisNegative - yAxisNegative), 2))); // _ side
	//for calculating apex of triangle 
	xTop = xAxisNegative + (bSide * cos((60.0f * M_PI) / 180.0f));
	yTop = yAxisNegative + (bSide * sin((60.0f * M_PI) / 180.0f));

	aSide = sqrt((pow((xAxisNegative - xTop), 2)) + (pow((yAxisNegative - yTop), 2)));// / side
	cSide = sqrt((pow((xTop - xAxisPositive), 2)) + (pow((yTop - yAxisNegative), 2)));  // \ side

	fprintf(gpFILE, "xAxis = %f \t Distance base = %f \t Distance a-b / = %f \t Distance a-c \\ = %f \t height = %f \n", xAxisNegative, bSide, aSide, cSide, height);

	// for line

	if (gbCenter)
	{
		glTranslatef(0.0f, 0.0f, -5.0f);
		glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	}
	else
	{

		glTranslatef(0.0f, gfLine_Y, -5.0f);
		//glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
	}

	//for straight line
	glBegin(GL_LINES);
	glVertex3f(xTop, yTop, 0.0f);

	glVertex3f(xTop, yAxisNegative, 0.0f);
	glEnd();

	glLoadIdentity();
	// for triangle
	if (gbCenter)
	{
		glTranslatef(0.0f, 0.0f, -5.0f);
		glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	}
	else
	{
		glTranslatef(gfTriangle_X, gfTriangle_Y, -5.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
	}

	drawTriangle(length);

	// semi perimeter
	semiPerimeter = ((aSide + bSide + cSide) * 1.0f) / 2.0f;

	//formula for get radius of circle
	radius = sqrt(((semiPerimeter - aSide) * (semiPerimeter - bSide) * (semiPerimeter - cSide) / semiPerimeter));

	//formula 1 for calculating center of triangle 
	xCenter = (xTop + xAxisNegative + xAxisPositive) / 3.0f;
	yCenter = (yTop + yAxisNegative + yAxisNegative) / 3.0f;

	//formula 2 for calculating center of triangle 
	/*xCenter = ((aSide * xTop) + (bSide * xAxisNegative) + (cSide * xAxisPositive)) / (aSide + bSide + cSide);
	yCenter = ((aSide * yTop) + (bSide * yAxisNegative) + (cSide * yAxisNegative)) / (aSide + bSide + cSide);*/

	fprintf(gpFILE, "xCenter = %f \t yCenter = %f \t xTop = %f \n ", xCenter, yCenter,xTop);

	//for circle
	glLoadIdentity();

	if (gbCenter)
	{
		glTranslatef(0.0f, 0.0f, -5.0f);
		glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	}
	else
	{
		glTranslatef(gfCircle_X, gfCircle_Y, -5.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
	}
	drawCircle(radius, xCenter, yCenter);

	


    glXSwapBuffers(display,window);
}

void drawTriangle(float length)
{

	//for draw triangle 

	glBegin(GL_LINE_LOOP);

	glVertex3f(xTop, yTop, 0.0f);
	glVertex3f(xAxisNegative, yAxisNegative, 0.0f);
	glVertex3f(xAxisPositive, yAxisNegative, 0.0f);

	glEnd();



}
void drawCircle(float radius, float xAxis,float yAxis)
{
	//variable declaration
	float x, y;
	glBegin(GL_LINE_STRIP);
	for (float fAngle = 0.0f; fAngle <= 360.0f; fAngle = fAngle + 0.1f)
	{
		x = xAxis + radius * cos((fAngle * M_PI) / 180.0f);
		y = yAxis + radius * sin((fAngle * M_PI) / 180.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}

void update(void)
{
    // code
    
    fAngle = fAngle + 10.0f;
	if (fAngle >= 360.0f)
	{
		fAngle = 0.0f;
	}

    if (gbCenter == False)
	{
		

		if (gfTriangle_X >= 0.0f)
		{
			gbCenter = True;
	
		}
		else
		{

			gfTriangle_X = gfTriangle_X + gfMovement;
			gfTriangle_Y = gfTriangle_Y + gfMovement;

			gfCircle_X = gfCircle_X - gfMovement;
			gfCircle_Y = gfCircle_Y + gfMovement;

			gfLine_Y = gfLine_Y - gfMovement;
		}

	}
	else
	{
		gbCenter = True;
		gfTriangle_X = 0.0f;
		gfTriangle_Y = 0.0f;

		gfCircle_X = 0.0f;
		gfCircle_Y = 0.0f;
		gfLine_Y = 0.0f;
	}

}

void uninitialize(void)
{
    // function declaration
    void uninitializeAudio(void);

    // local variable declaration
    GLXContext currentGLXContext = NULL;

    //code

    uninitializeAudio();

    // free visualInfo
    if(visualInfo)
    {
        free(visualInfo); // malloc() karun memory dili mhanun free() call kela
        visualInfo = NULL;
    }

    // uncurrent glxContext
    currentGLXContext = glXGetCurrentContext();
    if(currentGLXContext != NULL && currentGLXContext == glxContext)
    {
        glXMakeCurrent(display,None,NULL);
    }

    // destroy OpenGL context
    if(glxContext)
    {
        glXDestroyContext(display,glxContext);
        glxContext = NULL;
    }
    if(window)
    {
        XDestroyWindow(display,window);
    }

    if(colormap)
    {
        XFreeColormap(display,colormap);
    }

    if(display)
    {
        XCloseDisplay(display);
        display = NULL;
    }

    if(gpFILE)
    {
        fprintf(gpFILE,"Program Ended\n");
        fclose(gpFILE);
        gpFILE = NULL;
    }
}

void initializeAudio(void)
{
	// function declaration
	void uninitializeAudio(void);
	
	// code
	
	// 5 step to play audio
	
	// 1st step initilize ALUT
	alutInit(NULL,NULL);	
	
	error = alutGetError();
	if(error!=ALUT_ERROR_NO_ERROR)
	{
		printf("alutInit() failed\n");
		uninitializeAudio();
		return;
	}
	
	// 2nd step :- create audio buffer from audio file
	audioBuffer = alutCreateBufferFromFile("MyMusic.wav");
	
	error = alutGetError();
	if(error!=ALUT_ERROR_NO_ERROR)
	{
		printf("alutCreateBufferFromFile() failed\n");
		uninitializeAudio();
		return;
	}	
	
	// 3rd step :- create audio source
	alGenSources(1,&audioSource);
	
	error = alGetError();
	if(error!=AL_NO_ERROR)
	{
		printf("alGenSources() failed\n");
		uninitializeAudio();
		return;
	}
	
	// 4th step :- Attach above created audio buffer to above audio source
	alSourcei(audioSource,AL_BUFFER,audioBuffer);
	
	error = alGetError();
	if(error!=AL_NO_ERROR)
	{
		printf("alSourcei() failed\n");
		uninitializeAudio();
		return;
	}
	
	// 5th step :- play audio from the source
	alSourcePlay(audioSource);
	
	error = alGetError();
	if(error!=AL_NO_ERROR)
	{
		printf("alSourcePlay() failed\n");
		uninitializeAudio();
		return;
	}
	
	return;	
}

void uninitializeAudio(void)
{
	// local variable declaration
	ALint state;
	
	// code
	
	// 1st step :- stop playing audio from source
	alGetSourcei(audioSource,AL_SOURCE_STATE,&state);
	
	if(state == AL_PLAYING) // check if audio is playing or not
	{
		alSourceStop(audioSource);
		
		error = alGetError();
		if(error!=AL_NO_ERROR)
		{
			printf("alSourceStop() failed\n");
		}
	}
	
	// 2nd step :- detach audio buffer from the audio source
	alSourcei(audioSource,AL_BUFFER,0);
	
	error = alGetError();
	if(error!=AL_NO_ERROR)
	{
		printf("alSourcei() failed\n");
	}
	
	// 3 step :- Delete audio source
	alDeleteSources(1,&audioSource);
	
	error = alGetError();
	if(error!=AL_NO_ERROR)
	{
		printf("alDeleteSources() failed\n");
	}
	else
	{
		audioSource = 0;
	}
		
	// 4th step :- Delete audio buffer
	alDeleteBuffers(1,&audioBuffer);
		
	error = alGetError();
	if(error!=AL_NO_ERROR)
	{
		printf("alDeleteBuffer() failed\n");
	}
	else
	{
		audioBuffer = 0;
	}
	
	// 5th step :- uninitialize ALUT
	alutExit();	
}
