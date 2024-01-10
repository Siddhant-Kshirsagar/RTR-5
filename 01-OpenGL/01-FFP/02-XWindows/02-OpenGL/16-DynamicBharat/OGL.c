//standard header file
#include<stdio.h> // for printf()
#include<stdlib.h> // for exit()
#include<memory.h> // for memset()

#define _USE_MATH_DEFINES
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

FILE *gpFILE = NULL;

// OpenGL global variable
GLXContext glxContext = NULL;
XVisualInfo *visualInfo = NULL;

// OpenAL related variable declaration
ALCenum error; // for error checking 
ALuint audioBuffer,audioSource; // VRAM madhil address cha represent karnara interger variable store karnyasathi

float OrangeColor[] = { 255.0f / 255.0f,103.0f / 255.0f, 31.0f / 255.0f };
float WhiteColor[] = { 255.0f / 255.0f, 255.0f / 255.0f , 255.0f / 255.0f };
float GreenColor[] = { 4.0f / 255.0f , 106.0f / 255.0f, 56.0f / 255.0f };


float JettBlueColor[] = { 186.0f / 255.0f, 226.0f / 255.0f, 238.0f / 255.0f };

GLfloat gfBLeft = -10.0f;

GLfloat gfHLeft = -12.25f;
GLfloat gfHTop = 10.0f;

GLfloat gfA1Left = -10.5f;
GLfloat gfA1Bottom = -10.0f;

GLfloat gfRRight = 10.75f;
GLfloat gfRTop = 10.0f;

GLfloat gfA2Right = 12.5f;
GLfloat gfA2Bottom = -10.0f;

GLfloat gfTRight = 14.0f;

GLfloat gfJettOneRight = -8.0f;

GLfloat gfJettTwoRight = -9.0f;
GLfloat gfJettTwoTop = 5.0f;

GLfloat gfJettThreeRight = -9.0f;
GLfloat gfJettThreeBottom = -5.0f;

GLfloat changeFactor = 0.05f;
GLfloat changeFactorJett = 0.0035f;

GLfloat gfJettTwoAngle = -45.0f;
GLfloat gfJettThreeAngle = 45.0f;

GLfloat gfFlag = -4.1f;
GLfloat gfFlag_B = -4.1f;
GLfloat gfFlag_H = -2.85f;
GLfloat gfFlag_A1 = -0.98f;
GLfloat gfFlag_R = 0.55f;
GLfloat gfFlag_A2 = 2.05f;
GLfloat gfFlag_T = 3.85f;

GLfloat Alpha_B = 0.0f;
GLfloat Alpha_H = 0.0f;
GLfloat Alpha_A1 = 0.0f;
GLfloat Alpha_R = 0.0f;
GLfloat Alpha_A2 = 0.0f;
GLfloat Alpha_T = 0.0f;



GLfloat gfFire_xAxisIncrement = 0.3f;
GLfloat gfFire_xAxisDecrement = 0.6f;

float jettTwoAnimation = 240.0f; 
float jettThreeAnimation = 300.0f;

Bool IsArrived_B = False;
Bool IsArrived_H = False;
Bool IsArrived_A1 = False;
Bool IsArrived_A2 = False;
Bool IsArrived_R = False;
Bool IsArrived_T = False;


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
        fprintf(gpFILE,"initialize() failed\n");
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
	void draw_BHARATLetters(void);
	void draw_BHARATLetters1(void);
	//function declaration
	void draw_jett(void);
	void jettScene(void);

    // code
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-10.0f);

	draw_BHARATLetters1();
	draw_BHARATLetters();

	if (IsArrived_T)
	{
		jettScene();
	}

    glXSwapBuffers(display,window);
}

void jettScene(void)
{
	//function declaration
	void draw_jett(void);

	//code
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// for jett One 
	glPushMatrix();
	glTranslatef(gfJettOneRight, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(0.4f, 0.4f, 1.0f);
	draw_jett();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfJettTwoRight, gfJettTwoTop, 0.0f);
	glPushMatrix();
	glRotatef(gfJettTwoAngle, 0.0f, 0.0f, 1.0f);
	glPushMatrix();
	glScalef(0.35f, 0.35f, 1.0f);
	draw_jett();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfJettThreeRight, gfJettThreeBottom, 0.0f);
	glPushMatrix();
	glRotatef(gfJettThreeAngle, 0.0f, 0.0f, 1.0f);
	glPushMatrix();
	glScalef(0.35f, 0.35f, 1.0f);
	draw_jett();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
}

void draw_BHARATLetters1(void)
{
	//function declaration
	void draw_B1(void);
	void draw_H1(void);
	void draw_A11(void);
	void draw_R1(void);
	void draw_T1(void);



	glPushMatrix();
	glTranslatef(gfBLeft, 0.0f, 0.0f);
	draw_B1();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(gfHLeft, gfHTop, 0.0f);
	draw_H1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfA1Left, gfA1Bottom, 0.0f);
	draw_A11();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfRRight, gfRTop, 0.0f);
	draw_R1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfA2Right, gfA2Bottom, 0.0f);
	draw_A11();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfTRight, 0.0f, 0.0f);
	draw_T1();
	glPopMatrix();





}
void draw_B1(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 left | bar
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 2 left | bar
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.0f);
	glVertex3f(-0.1f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);


	// quad 3 right ) B upper curve
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.65f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.7f, 0.0f);
	glVertex3f(0.2f, 0.7f, 0.0f);

	// quad 4 mid part
	glVertex3f(0.65f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.7f, 0.0f);
	glVertex3f(0.65f, 0.5f, 0.0f);
	glVertex3f(0.9f, 0.5f, 0.0f);


	//  quad 5 right ) B upper curve bottom
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	// quad 6 mid part
	glVertex3f(0.9f, 0.5f, 0.0f);
	glVertex3f(0.65f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);



	// quad 7 right ) B lower curve bottom part
	glVertex3f(0.2f, -1.0f, 0.0f);
	glVertex3f(0.65f, -1.0f, 0.0f);
	glVertex3f(0.5f, -0.7f, 0.0f);
	glVertex3f(0.2f, -0.7f, 0.0f);

	// quad 8 mid part
	glVertex3f(0.65f, -1.0f, 0.0f);
	glVertex3f(0.5f, -0.7f, 0.0f);
	glVertex3f(0.65f, -0.5f, 0.0f);
	glVertex3f(0.9f, -0.5f, 0.0f);


	//  quad 9 right ) B lower curve  top part
	glVertex3f(0.5f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	// quad 10 mid part
	glVertex3f(0.9f, -0.5f, 0.0f);
	glVertex3f(0.65f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.1f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	glEnd();

}

void draw_H1(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 left side
	glVertex3f(-0.3f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.0f, 0.0f);

	// quad 2
	glVertex3f(-0.3f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.0f, 0.0f);
	glVertex3f(-0.6f, -1.0f, 0.0f);
	glVertex3f(-0.3f, -1.0f, 0.0f);

	// quad 3  right side
	glVertex3f(0.3f, 1.0f, 0.0f);
	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.0f);

	// quad 4
	glVertex3f(0.3f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.6f, -1.0f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);


	// quad 5  mid part - 
	glVertex3f(0.3f, 0.1f, 0.0f);
	glVertex3f(-0.3f, 0.1f, 0.0f);
	glVertex3f(-0.3f, -0.1f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.0f);

	glEnd();

}

void draw_A11(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 left side /
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);

	// quad 2 left side /
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.8f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);

	// quad 3 right side /
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 4 right side /
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);


	// quad 5 mid part -
	glVertex3f(0.18f, 0.1f, 0.0f);
	glVertex3f(-0.18f, 0.1f, 0.0f);
	glVertex3f(-0.23f, -0.1f, 0.0f);
	glVertex3f(0.23f, -0.1f, 0.0f);


	glEnd();
}

void draw_R1(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 left side | top part
	glVertex3f(0.1f, 0.7f, 0.0f);
	glVertex3f(-0.2f, 1.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);

	// quad 2 left side | bottom part
	glVertex3f(0.1f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -1.0f, 0.0f);
	glVertex3f(0.1f, -1.0f, 0.0f);

	// quad 3 right side ) upper curve top part
	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(-0.2f, 1.0f, 0.0f);
	glVertex3f(0.1f, 0.7f, 0.0f);
	glVertex3f(0.4f, 0.7f, 0.0f);

	// quad 4 right side ) upper curve mid part
	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(0.4f, 0.7f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.8f, 0.4f, 0.0f);

	// quad 5 right side ) upper curve bottom part
	glVertex3f(0.8f, 0.4f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.1f, 0.25f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);

	// quad 6 right side \ 
	glVertex3f(0.1f, 0.0f, 0.0f);
	glVertex3f(0.1f, -0.3f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);




	glEnd();
}

void draw_T1(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 Mid part | top part
	glVertex3f(0.15f, 0.7f, 0.0f);
	glVertex3f(-0.15f, 0.7f, 0.0f);
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);

	// quad 2 mid part | bottom part
	glVertex3f(0.15f, 0.0f, 0.0f);
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glVertex3f(-0.15f, -1.0f, 0.0f);
	glVertex3f(0.15f, -1.0f, 0.0f);

	// quad 1 Mid part | top part
	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 0.7f, 0.0f);
	glVertex3f(0.6f, 0.7f, 0.0f);

	glEnd();
}

void draw_jett(void)
{
	//function declaration
	void draw_JettFire(float,float,float);

	// code
	// drawing core structure of jett
	glBegin(GL_TRIANGLES);
	// triangle 1 > center core 1st part from right
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(1.7f, 0.0f, 0.0f);
	glVertex3f(1.28f, 0.12f, 0.0f);
	glVertex3f(1.28f, -0.12f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	// quad 1 > center core 2nd part from right
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);

	glVertex3f(1.28f, 0.12f, 0.0f);
	glVertex3f(0.9f, 0.2f, 0.0f);
	glVertex3f(0.9f, -0.2f, 0.0f);
	glVertex3f(1.28f, -0.12f, 0.0f);

	// quad 2 > center core 3rd part from right
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(0.9f, 0.2f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(0.3f, 0.5f, 0.0f);
	glVertex3f(0.3f, -0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(0.9f, -0.2f, 0.0f);


	// quad 3 > center core 4th part from right
	
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(0.3f, 0.5f, 0.0f);
	glVertex3f(-1.5f, 0.5f, 0.0f);
	glVertex3f(-1.5f, -0.5f, 0.0f);
	glVertex3f(0.3f, -0.5f, 0.0f);



	// quad 4 > top side 1st wing
	
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(0.3f, 0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(-0.3f, 1.9f, 0.0f);
	glVertex3f(-0.9f, 1.9f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-0.7f, 0.5f, 0.0f);

	// quad 5 > top side 2nd wing
	
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-1.0f, 0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(-1.7f, 1.3f, 0.0f);
	glVertex3f(-2.1f, 1.4f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-1.7f, 0.5f, 0.0f);

	// quad 6 > top side nozzle
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-1.5f, 0.49f, 0.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(-1.65f, 0.4f, 0.0f);
	glVertex3f(-1.65f, 0.05f, 0.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-1.5f, 0.0f, 0.0f);

	// quad 7 > bottom side 1st wing


	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(0.3f, -0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(-0.3f, -1.9f, 0.0f);
	glVertex3f(-0.9f, -1.9f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-0.7f, -0.5f, 0.0f);

	// quad 8 > bottom side 2nd wing
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-1.0f, -0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(-1.7f, -1.3f, 0.0f);
	glVertex3f(-2.1f, -1.4f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-1.7f, -0.5f, 0.0f);

	// quad 9 > bottom side nozzle
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-1.5f, 0.0f, 0.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(-1.65f, -0.05f, 0.0f);
	glVertex3f(-1.65f, -0.4f, 0.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-1.5f,-0.49f, 0.0f);

	glEnd();

	// for extra detailing

	glBegin(GL_TRIANGLES);

	// triangle 1 > cockpit from right
	glColor3f(0.3f, 0.3f,0.3f);
	glVertex3f(1.5f, 0.0f, 0.0f);
	glColor3f(0.25f, 0.25f, 0.25f);
	glVertex3f(1.28f, 0.1f, 0.0f);
	glVertex3f(1.28f, -0.1f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);

	// quad 1 > cockpit 2nd part from right
	glColor3f(0.25f, 0.25f, 0.25f);

	glVertex3f(1.28f, 0.1f, 0.0f);
	glVertex3f(0.9f, 0.18f, 0.0f);
	glVertex3f(0.9f, -0.18f, 0.0f);
	glVertex3f(1.28f, -0.1f, 0.0f);

	// quad 2 > cockpit 3rd part from right

	glVertex3f(0.9f, 0.18f, 0.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.5f, 0.15f, 0.0f);
	glVertex3f(0.5f, -0.15f, 0.0f);
	glColor3f(0.25f, 0.25f, 0.25f);
	glVertex3f(0.9f, -0.18f, 0.0f);


	// quad 1 > for center spine design
	glColor3f(JettBlueColor[0] - 0.35f, JettBlueColor[1] - 0.35f, JettBlueColor[2] - 0.35f);
	glVertex3f(0.5f, 0.15f, 0.0f);
	glColor3f(JettBlueColor[0]-0.15f, JettBlueColor[1] - 0.15f, JettBlueColor[2] - 0.15f);
	glVertex3f(-1.5f, 0.01f, 0.0f);
	glVertex3f(-1.5f, -0.01f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.25f, JettBlueColor[1] - 0.25f, JettBlueColor[2] - 0.25f);
	glVertex3f(0.5f, -0.15f, 0.0f);


	// quad 1 > top side  wing missile 1st

	glColor3f(0.3f,0.3f,0.3f);
	glVertex3f(0.4f, 0.9f, 0.0f);
	glVertex3f(0.129f, 0.9f, 0.0f);
	glVertex3f(0.17f, 0.8f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.0f);

	// quad 2 > top side  wing missile 1st
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.4f, 0.9f, 0.0f);
	glVertex3f(0.5f, 0.86f, 0.0f);
	glVertex3f(0.5f, 0.84f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.0f);


	// quad 1 > top side wing missile 2nd 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.3f, 1.1f, 0.0f);
	glVertex3f(0.04f, 1.1f, 0.0f);
	glVertex3f(0.08f, 1.0f, 0.0f);
	glVertex3f(0.3f, 1.0f, 0.0f);
	// quad 2 > top side wing missile 2nd 

	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.3f, 1.1f, 0.0f);
	glVertex3f(0.4f, 1.06f, 0.0f);
	glVertex3f(0.4f, 1.04f, 0.0f);
	glVertex3f(0.3f, 1.0f, 0.0f);

	// quad 1 > top side wing missile 3rd 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.2f, 1.3f, 0.0f);
	glVertex3f(-0.05f, 1.3f, 0.0f);
	glVertex3f(-0.01f, 1.2f, 0.0f);
	glVertex3f(0.2f, 1.2f, 0.0f);
	// quad 2 > top side wing missile 3rd 
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.2f, 1.3f, 0.0f);
	glVertex3f(0.3f, 1.26f, 0.0f);
	glVertex3f(0.3f, 1.24f, 0.0f);
	glVertex3f(0.2f, 1.2f, 0.0f);

	// quad 1 > bottom side  wing missile 1st

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.4f, -0.9f, 0.0f);
	glVertex3f(0.129f, -0.9f, 0.0f);
	glVertex3f(0.17f, -0.8f, 0.0f);
	glVertex3f(0.4f, -0.8f, 0.0f);

	// quad 2 > bottom side  wing missile 1st
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.4f, -0.9f, 0.0f);
	glVertex3f(0.5f, -0.86f, 0.0f);
	glVertex3f(0.5f, -0.84f, 0.0f);
	glVertex3f(0.4f, -0.8f, 0.0f);


	// quad 1 > bottom side wing missile 2nd 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.3f, -1.1f, 0.0f);
	glVertex3f(0.04f, -1.1f, 0.0f);
	glVertex3f(0.08f, -1.0f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);
	// quad 2 > bottom side wing missile 2nd 

	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.3f, -1.1f, 0.0f);
	glVertex3f(0.4f, -1.06f, 0.0f);
	glVertex3f(0.4f, -1.04f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);

	// quad 1 > bottom side wing missile 3rd 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.2f, -1.3f, 0.0f);
	glVertex3f(-0.05f, -1.3f, 0.0f);
	glVertex3f(-0.01f, -1.2f, 0.0f);
	glVertex3f(0.2f, -1.2f, 0.0f);

	// quad 2 > bottom side wing missile 3rd 
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.2f, -1.3f, 0.0f);
	glVertex3f(0.3f, -1.26f, 0.0f);
	glVertex3f(0.3f, -1.24f, 0.0f);
	glVertex3f(0.2f, -1.2f, 0.0f);

	glEnd();

	draw_JettFire(-1.65, 0.4f, 0.0f);
	draw_JettFire(-1.65, -0.05f, 0.0f);

}

void draw_JettFire(float xAxis, float yAxis, float zAxis)
{
	int i = 0;
	float y = 0.0f;

	glBegin(GL_LINES);
	
	for (y = yAxis, i = 0; y >= yAxis - 0.34f; y = y - 0.02f, i++)
	{
		
		glColor3f(0.0f, 174.0f / 255.0f, 1.0f);
		glVertex3f(xAxis, y, zAxis);

		if (i < 8) // manually counted value for keep y axis minimum or maximum
		{
			if (i % 2 == 0)
			{
				glColor3f(1.0f, 97.0f / 255.0f, 0.0f);
				glVertex3f(xAxis - gfFire_xAxisIncrement, y-0.05f, zAxis);
			}
			else
			{
				glColor3f(1.0f, 97.0f / 255.0f, 0.0f);
				glVertex3f(xAxis - gfFire_xAxisDecrement, y - 0.05f, zAxis);
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				glColor3f(1.0f, 97.0f / 255.0f, 0.0f);
				glVertex3f(xAxis - gfFire_xAxisIncrement, y + 0.05f, zAxis);
			}
			else
			{
				glColor3f(1.0f, 97.0f / 255.0f, 0.0f);
				glVertex3f(xAxis - gfFire_xAxisDecrement, y + 0.05f , zAxis);
			}
		}
		
	
	}
	glEnd();
}


void draw_BHARATLetters(void)
{
	//function declaration
	void draw_B(void);
	void draw_H(void);
	void draw_A1(void);
	void draw_R(void);
	void draw_A2(void);
	void draw_T(void);


	
	glPushMatrix();
	glTranslatef(gfBLeft, 0.0f, 0.0f);
	draw_B();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(gfHLeft, gfHTop, 0.0f);
	draw_H();		
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfA1Left, gfA1Bottom, 0.0f);
	draw_A1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfRRight, gfRTop, 0.0f);
	draw_R();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfA2Right, gfA2Bottom, 0.0f);
	draw_A2();
	glPopMatrix();
				
	glPushMatrix();
	glTranslatef(gfTRight, 0.0f, 0.0f);
	draw_T();
	glPopMatrix();


	


}
void draw_B(void)
{
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_B);
	}
	// quad 1 left | bar
	
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(-0.1f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 2 left | bar
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(-0.1f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);


	// quad 3 right ) B upper curve
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.65f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 148.0f / 255.0f, 97.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, 0.7f, 0.0f);
	glVertex3f(0.2f, 0.7f, 0.0f);

	// quad 4 mid part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 148.0f / 255.0f, 97.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, 0.7f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 181.0f / 255.0f, 146.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, 0.5f, 0.0f);
	glVertex3f(0.9f, 0.5f, 0.0f);


	//  quad 5 right ) B upper curve bottom
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 233.0f / 255.0f, 222.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	// quad 6 mid part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 181.0f / 255.0f, 146.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.9f, 0.5f, 0.0f);
	glVertex3f(0.65f, 0.5f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 233.0f / 255.0f, 222.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, 0.1f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, 0.0f, 0.0f);



	// quad 7 right ) B lower curve bottom part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.2f, -1.0f, 0.0f);
	glVertex3f(0.65f, -1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(79.0f / 255.0f, 150.0f / 255.0f, 115.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, -0.7f, 0.0f);
	glVertex3f(0.2f, -0.7f, 0.0f);

	// quad 8 mid part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, -1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(79.0f / 255.0f, 150.0 / 255.0f, 115.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, -0.7f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(125.0f / 255.0f, 177.0f / 255.0f, 152.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, -0.5f, 0.0f);
	glVertex3f(0.9f, -0.5f, 0.0f);


	//  quad 9 right ) B lower curve  top part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(217.0f / 255.0f, 232.0f / 255.0f, 225.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	// quad 10 mid part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(125.0f / 255.0f, 177.0f / 255.0f, 152.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.9f, -0.5f, 0.0f);
	glVertex3f(0.65f, -0.5f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(217.0f / 255.0f, 232.0f / 255.0f, 225.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, -0.1f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, 0.0f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);
}

void draw_H(void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	// quad 1 left side
	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(-0.3f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(-0.6f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.0f, 0.0f);

	// quad 2
	glVertex3f(-0.3f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(-0.6f, -1.0f, 0.0f);
	glVertex3f(-0.3f, -1.0f, 0.0f);

	// quad 3  right side
	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(0.3f, 1.0f, 0.0f);
	glVertex3f(0.6f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.0f);

	// quad 4
	glVertex3f(0.3f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(0.6f, -1.0f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);


	// quad 5  mid part - 

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 241.0f / 255.0f, 234.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(0.3f, 0.1f, 0.0f);
	glVertex3f(-0.3f, 0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(228.0f / 255.0f, 229.0f / 255.0f, 234.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(-0.3f, -0.1f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);

}

void draw_A1(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	// quad 1 left side /
	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_A1);
	}

	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A1);
	}

	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);

	// quad 2 left side /
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_A1);
	}

	glVertex3f(-0.8f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);

	// quad 3 right side /
	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_A1);
	}

	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A1);
	}

	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 4 right side /
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_A1);
	}

	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);


	// quad 5 mid part -
	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A1);
	}

	glVertex3f(0.18f, 0.1f, 0.0f);
	glVertex3f(-0.18f, 0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A1);
	}

	glVertex3f(-0.23f, -0.1f, 0.0f);
	glVertex3f(0.23f, -0.1f, 0.0f);


	glEnd();
	glDisable(GL_BLEND);
}

void draw_A2(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	// quad 1 left side /
	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2],Alpha_A2);
	}
	
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2],Alpha_A2);
	}

	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);

	// quad 2 left side /
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2],Alpha_A2);
	}
	
	glVertex3f(-0.8f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);

	// quad 3 right side /
	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2],Alpha_A2);
	}
	
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2],Alpha_A2);
	}
	
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 4 right side /
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_A2);
	}
	
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);


	// quad 5 mid part -
	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A2);
	}
	
	glVertex3f(0.18f, 0.1f, 0.0f);
	glVertex3f(-0.18f, 0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2],Alpha_A2);
	}
	
	glVertex3f(-0.23f, -0.1f, 0.0f);
	glVertex3f(0.23f, -0.1f, 0.0f);


	glEnd();
	glDisable(GL_BLEND);

}


void draw_R(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	// quad 1 left side | top part
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_R);
	}

	glVertex3f(0.1f, 0.7f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_R);
	}

	glVertex3f(-0.2f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_R);
	}

	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);

	// quad 2 left side | bottom part
	glVertex3f(0.1f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_R);
	}

	glVertex3f(-0.2f, -1.0f, 0.0f);
	glVertex3f(0.1f, -1.0f, 0.0f);

	// quad 3 right side ) upper curve top part
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_R);
	}

	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(-0.2f, 1.0f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_R);
	}

	glVertex3f(0.1f, 0.7f, 0.0f);
	glVertex3f(0.4f, 0.7f, 0.0f);

	// quad 4 right side ) upper curve mid part
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_R);
	}

	glVertex3f(0.5f, 1.0f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_R);
	}

	glVertex3f(0.4f, 0.7f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 154.0f / 255.0f, 106.0f / 255.0f, Alpha_R);
	}
	
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.8f, 0.4f, 0.0f);

	// quad 5 right side ) upper curve bottom part
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 154.0f / 255.0f, 106.0f / 255.0f, Alpha_R);
	}
	
	glVertex3f(0.8f, 0.4f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 208.0f / 255.0f, 186.0f / 255.0f, Alpha_R);
	}
	
	glVertex3f(0.1f, 0.25f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_R);
	}
	
	glVertex3f(0.1f, 0.0f, 0.0f);

	// quad 6 right side \ 
	// if((!IsArrived_T) || gfFlag_R <= 0.65f)
    // {
	// 	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	// }
	// else
	// {
	// 	glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_R);
	// }

    if((!IsArrived_T) || gfFlag_R <= 0.65f)
    {
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_R);
	}
	
	glVertex3f(0.1f, 0.0f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(179.0f / 255.0f, 210.0f / 255.0f, 195.0f / 255.0f, Alpha_R);
	}
	
	glVertex3f(0.1f, -0.3f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_R);
	}
	
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);
}

void draw_T(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	// quad 1 Mid part | top part

	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_T);
	}
	
	glVertex3f(0.15f, 0.7f, 0.0f);

	glVertex3f(-0.15f, 0.7f, 0.0f);

	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_T);
	}
	
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);

	// quad 2 mid part | bottom part
	glVertex3f(0.15f, 0.0f, 0.0f);
	glVertex3f(-0.15f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_T);
	}
	
	glVertex3f(-0.15f, -1.0f, 0.0f);
	glVertex3f(0.15f, -1.0f, 0.0f);

	// quad 1 Mid part | top part
	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_T);
	}
	
	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);

	if((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_T);
	}
	
	glVertex3f(-0.6f, 0.7f, 0.0f);
	glVertex3f(0.6f, 0.7f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);
}

void update(void)
{
    // code
   //code
	// for fire animation
	gfFire_xAxisIncrement = gfFire_xAxisIncrement + 0.02f;
	if (gfFire_xAxisIncrement >= 0.6f)
	{
		gfFire_xAxisIncrement = 0.3f;
	}

	gfFire_xAxisDecrement = gfFire_xAxisDecrement - 0.02f;
	if (gfFire_xAxisDecrement <= 0.3f)
	{
		gfFire_xAxisDecrement = 0.6f;
	}

	// for letter animation
	if (IsArrived_B == False)
	{
		if (gfBLeft <= -4.0f)
		{
			gfBLeft = gfBLeft + changeFactor;
		}
		else
		{
			IsArrived_B = True;
		}

	}
	

	if (IsArrived_B == True)
	{
		if (gfHLeft <= -2.25f)
		{
			gfHLeft = gfHLeft + changeFactor;
		}
		if (gfHTop >= 0.0f)
		{
			gfHTop = gfHTop - changeFactor;
		}
		else
		{
			IsArrived_H = True;
		}
	}

	if (IsArrived_H == True)
	{
		if (gfA1Left <= -0.5f)
		{
			gfA1Left = gfA1Left + changeFactor;
		}
		if (gfA1Bottom <= 0.0f)
		{
			gfA1Bottom = gfA1Bottom + changeFactor;
		}
		else
		{
			IsArrived_A1 = True;
		}
	}

	if (IsArrived_A1 == True)
	{
		if (gfRRight >= 0.75f)
		{
			gfRRight = gfRRight - changeFactor;
		}
		if (gfRTop >= 0.0f)
		{
			gfRTop = gfRTop - changeFactor;
		}
		else
		{
			IsArrived_R = True;
		}
	}

	if (IsArrived_R == True)
	{
		if (gfA2Right >= 2.5f)
		{
			gfA2Right = gfA2Right - changeFactor;
		}
		if (gfA2Bottom <= 0.0f)
		{
			gfA2Bottom = gfA2Bottom + changeFactor;
		}
		else
		{
			IsArrived_A2 = True;
		}
	}

	if (IsArrived_A2 == True)
	{
		if (gfTRight >= 4.0f)
		{
			gfTRight = gfTRight - changeFactor;
		}
		else
		{
			IsArrived_T = True;
		}
	}

	// for jett animation
	if (IsArrived_T)
	{
		if (gfJettOneRight <= 10.0f)
		{
			gfJettOneRight = gfJettOneRight + changeFactorJett;
		}

		if (gfJettOneRight >= -4.1f && gfJettOneRight <= 4.16f)
		{
			gfFlag = gfJettOneRight; // for flag
		}

		if (gfJettOneRight >= -4.1f && gfJettOneRight <= -3.32f)
		{
			gfFlag_B = gfJettOneRight;
		}

		if (gfFlag_B >= -4.0f)
		{
			if (Alpha_B <= 1.0f)
			{
				Alpha_B = Alpha_B + 0.001f;
			}
			
		}

		if (gfJettOneRight >= -2.85f && gfJettOneRight <= -1.659f)
		{
			gfFlag_H = gfJettOneRight;
		}

		if (gfFlag_H >= -2.75f)
		{
			if (Alpha_H <= 1.0f)
			{
				Alpha_H = Alpha_H + 0.001f;
			}
			
		}

		if (gfJettOneRight >= -1.0f && gfJettOneRight <= 0.0f)
		{
			gfFlag_A1 = gfJettOneRight;
		}

		if (gfFlag_A1 >= -0.88f)
		{
			if (Alpha_A1 <= 1.0f)
			{
				Alpha_A1 = Alpha_A1 + 0.001f;
			}
				
		}

		if (gfJettOneRight >= 0.55f && gfJettOneRight <= 0.88f)
		{
			gfFlag_R = gfJettOneRight;
		}

		if (gfFlag_R >= 0.65f)
		{
			if (Alpha_R <= 1.0f)
			{
				Alpha_R = Alpha_R + 0.001f;
			}
				
		}

		if (gfJettOneRight >= 2.05f && gfJettOneRight <= 2.98f)
		{
			gfFlag_A2 = gfJettOneRight;
		}

		if (gfFlag_A2 >= 2.15f)
		{
			if (Alpha_A2 <= 1.0f)
			{
				Alpha_A2 = Alpha_A2 + 0.001f;
			}
				
		}

		if (gfJettOneRight >= 3.85f && gfJettOneRight <= 4.165f)
		{
			gfFlag_T = gfJettOneRight;
		}

		if (gfFlag_T >= 3.95f)
		{
			if (Alpha_T <= 1.0f)
			{
				Alpha_T = Alpha_T + 0.001f;
			}
				
		}

		// for jett two animation
		if (gfJettTwoRight <= 8.0f)
		{
			gfJettTwoRight = gfJettTwoRight + changeFactorJett;
		}
		
		if (gfJettTwoRight >= -9.0f && gfJettTwoRight <= -4.0f)
		{
			if (gfJettTwoTop >= 1.5f)
			{
				gfJettTwoTop = gfJettTwoTop - changeFactorJett;
			}
			if (gfJettTwoAngle <= 0.0f)
			{
				gfJettTwoAngle = gfJettTwoAngle + 0.0555f;
			}
		}
		else if (gfJettTwoRight >= 4.5f && gfJettTwoRight <= 8.0f)
		{
			if (gfJettTwoTop <= 8.0f)
			{
				gfJettTwoTop = gfJettTwoTop + changeFactorJett;
			}
			if (gfJettTwoAngle >= -45.0f)
			{
				gfJettTwoAngle = gfJettTwoAngle + 0.0555f;
			}
		}

		if (gfJettThreeRight <= 8.0f)
		{
			gfJettThreeRight = gfJettThreeRight + changeFactorJett;
		}

		if (gfJettThreeRight >= -9.0f && gfJettThreeRight <= -4.5f)
		{
			if (gfJettThreeBottom <= -1.5f)
			{
				gfJettThreeBottom = gfJettThreeBottom + changeFactorJett;
			}
			if (gfJettThreeAngle >= 0.0f)
			{
				gfJettThreeAngle = gfJettThreeAngle - 0.0555f;
			}
		}
		else if (gfJettThreeRight >= 4.5f && gfJettThreeRight <= 8.0f)
		{
			if (gfJettThreeBottom >= -8.0f)
			{
				gfJettThreeBottom = gfJettThreeBottom - changeFactorJett;
			}
			if (gfJettThreeAngle >= -45.0f)
			{
				gfJettThreeAngle = gfJettThreeAngle - 0.0555f;
			}
		}
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
