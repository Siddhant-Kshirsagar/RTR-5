//standard header file
#include<stdio.h> // for printf()
#include<stdlib.h> // for exit()
#include<memory.h> // for memset()

// x11 header file
#include<X11/Xlib.h> // for all XWindow API (in window Windows.h similarly int XWindow X11/Xlib.h)
#include<X11/Xutil.h> // for XVisualInfo and related API
#include<X11/XKBlib.h>

// OpenGL header file
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

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

// for light
Bool gbLight = False;

GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,0.0f,1.0f };

GLUquadric *quadric = NULL;

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;

GLuint keyPressed = 0;

GLuint currentWinWidth = 0;
GLuint currentWinHeight = 0;


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
        exit(0);
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
                	case 'L':
                    case 'l':
                    {
                        if (gbLight == False)
                        {
                            glEnable(GL_LIGHTING);
                            gbLight = True;
                        }
                        else
                        {
                            glDisable(GL_LIGHTING);
                            gbLight = False;
                        }
                    }
                    break;

                    case 'X':
                    case 'x':
                        keyPressed = 1;
                        angleForXRotation = 0.0f; // reset
                        break;

                    case 'Y':
                    case 'y':
                        keyPressed = 2;
                        angleForYRotation = 0.0f; // reset
                        break;

                    case 'Z':
                    case 'z':
                        keyPressed = 3;
                        angleForZRotation = 0.0f; // reset
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
				currentWinWidth = event.xconfigure.width;
				currentWinHeight = event.xconfigure.height;
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
   glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

   	// light related initialization
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

	glEnable(GL_LIGHT0);

	// initialize quadric
	quadric = gluNewQuadric();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // warm up Resize call
    resize(WINWIDTH,WINHEIGHT);

    return(0);
}

void resize(int width,int height)
{
    // code
    if (height <= 0)
	{
		height = 1;
	}

	// glViewport(0, 0,(GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void draw(void) // similar to display() in windows
{
	// variable declaration
	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];
	GLfloat materialShininess;

	GLfloat row1_Y, row2_Y, row3_Y, row4_Y, row5_Y, row6_Y;
	GLfloat col1_X, col2_X, col3_X, col4_X;

	GLfloat difference_X = currentWinWidth/6; // for column
	GLfloat difference_Y = currentWinHeight/6; // for rows

	col1_X = 0.0f;
	col2_X = col1_X + difference_X;
	col3_X = col2_X + difference_X;
	col4_X = col3_X + difference_X;

	row1_Y = currentWinHeight - difference_Y;
	row2_Y = row1_Y - difference_Y;
	row3_Y = row2_Y - difference_Y;
	row4_Y = row3_Y - difference_Y;
	row5_Y = row4_Y - difference_Y;
	row6_Y = row5_Y - difference_Y;





	//code
	  // code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// animation
	if (keyPressed == 1)
	{
		glRotatef(angleForXRotation, 1.0f, 0.0f, 0.0f);
		lightPosition[2] = angleForXRotation;
	}
	else if (keyPressed == 2)
	{
		glRotatef(angleForYRotation, 0.0f, 1.0f, 0.0f);
		lightPosition[0] = angleForYRotation;
	}
	else if (keyPressed == 3)
	{
		glRotatef(angleForZRotation, 0.0f, 0.0f, 1.0f);
		lightPosition[1] = angleForZRotation;
	}
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// first column of Precious Stone
	// emerad
	// ambient material
	materialAmbient[0] = 0.0215f; // r
	materialAmbient[1] = 0.1745f; // g
	materialAmbient[2] = 0.0215f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.07568f; // r
	materialDiffuse[1] = 0.61424f; // g
	materialDiffuse[2] = 0.07568f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.0633f; // r
	materialSpecular[1] = 0.727811f; // g
	materialSpecular[2] = 0.633f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col1_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd sphere on 1st column, jade
	// ambient material
	materialAmbient[0] = 0.135f; // r
	materialAmbient[1] = 0.2225f; // g
	materialAmbient[2] = 0.1575f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.54f; // r
	materialDiffuse[1] = 0.89f; // g
	materialDiffuse[2] = 0.63f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.316228f; // r
	materialSpecular[1] = 0.316228f; // g
	materialSpecular[2] = 0.316228f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col1_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 3rd sphere on 1st column, obsidian
	// ambient material
	materialAmbient[0] = 0.05375f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.06625f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.18275f; // r
	materialDiffuse[1] = 0.17f; // g
	materialDiffuse[2] = 0.22525f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.332741f; // r
	materialSpecular[1] = 0.328634f; // g
	materialSpecular[2] = 0.346435f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.3f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col1_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 4th sphere on 1st column, pearl
	// ambient material
	materialAmbient[0] = 0.25f; // r
	materialAmbient[1] = 0.20725f; // g
	materialAmbient[2] = 0.20725f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 1.0f; // r
	materialDiffuse[1] = 0.829f; // g
	materialDiffuse[2] = 0.829f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.296648f; // r
	materialSpecular[1] = 0.296648f; // g
	materialSpecular[2] = 0.296648f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.088f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col1_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 5th sphere on 1st column, ruby
	// ambient material
	materialAmbient[0] = 0.1745f; // r
	materialAmbient[1] = 0.01175f; // g
	materialAmbient[2] = 0.01175f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.61424f; // r
	materialDiffuse[1] = 0.04136f; // g
	materialDiffuse[2] = 0.04136f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.727811f; // r
	materialSpecular[1] = 0.626959f; // g
	materialSpecular[2] = 0.626959f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col1_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 6th shpere on 1st column, turquoise
	// ambient material
	materialAmbient[0] = 0.1f; // r
	materialAmbient[1] = 0.18725f; // g
	materialAmbient[2] = 0.1745f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.396f; // r
	materialDiffuse[1] = 0.74151f; // g
	materialDiffuse[2] = 0.69102f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.297254f; // r
	materialSpecular[1] = 0.30829f; // g
	materialSpecular[2] = 0.306678f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col1_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd column metal
	// 1st sphere on 2nd column, brass
	// ambient material
	materialAmbient[0] = 0.329412f; // r
	materialAmbient[1] = 0.223529f; // g
	materialAmbient[2] = 0.027451f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.780392f; // r
	materialDiffuse[1] = 0.568627f; // g
	materialDiffuse[2] = 0.113725f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.992157f; // r
	materialSpecular[1] = 0.941176f; // g
	materialSpecular[2] = 0.807843f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.21794872f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col2_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd sphere
	// ambient material
	materialAmbient[0] = 0.2125f; // r
	materialAmbient[1] = 0.1275f; // g
	materialAmbient[2] = 0.054f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.714f; // r
	materialDiffuse[1] = 0.4284f; // g
	materialDiffuse[2] = 0.18144f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.393548f; // r
	materialSpecular[1] = 0.271906f; // g
	materialSpecular[2] = 0.166721f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.2 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col2_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 3rd sphere on 2nd column, chrome
	// ambient material
	materialAmbient[0] = 0.25f; // r
	materialAmbient[1] = 0.25f; // g
	materialAmbient[2] = 0.25f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.4f; // r
	materialDiffuse[1] = 0.4f; // g
	materialDiffuse[2] = 0.4f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.774597f; // r
	materialSpecular[1] = 0.774597f; // g
	materialSpecular[2] = 0.774597f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col2_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 4th sphere on 2nd column, copper
	// ambient material
	materialAmbient[0] = 0.19125f; // r
	materialAmbient[1] = 0.0735f; // g
	materialAmbient[2] = 0.0225f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.7038f; // r
	materialDiffuse[1] = 0.27048f; // g
	materialDiffuse[2] = 0.0828f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.256777f; // r
	materialSpecular[1] = 0.137622f; // g
	materialSpecular[2] = 0.086014f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col2_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 5th sphere on 2nd column, gold
	// ambient material
	materialAmbient[0] = 0.24725f; // r
	materialAmbient[1] = 0.1995f; // g
	materialAmbient[2] = 0.0745f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.75164f; // r
	materialDiffuse[1] = 0.60648f; // g
	materialDiffuse[2] = 0.22648f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.628281f; // r
	materialSpecular[1] = 0.555802f; // g
	materialSpecular[2] = 0.366065f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.4f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col2_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 6th sphere on 2nd column, silver
		// ambient material
	materialAmbient[0] = 0.19225f; // r
	materialAmbient[1] = 0.19225f; // g
	materialAmbient[2] = 0.19225f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.50754f; // r
	materialDiffuse[1] = 0.50754f; // g
	materialDiffuse[2] = 0.50754f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.508273f; // r
	materialSpecular[1] = 0.508273f; // g
	materialSpecular[2] = 0.508273f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.4f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col2_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 1st sphere on 3rd column, black
		// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.01f; // r
	materialDiffuse[1] = 0.01f; // g
	materialDiffuse[2] = 0.01f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.50f; // r
	materialSpecular[1] = 0.50f; // g
	materialSpecular[2] = 0.50f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col3_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd sphere on 3rd column, cyan 
		// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.1f; // g
	materialAmbient[2] = 0.06f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.0f; // r
	materialDiffuse[1] = 0.50980392f; // g
	materialDiffuse[2] = 0.50980392f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.50196078f; // r
	materialSpecular[1] = 0.50196078f; // g
	materialSpecular[2] = 0.50196078f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col3_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 3rd sphere on 2nd column, green
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.1f; // r
	materialDiffuse[1] = 0.35f; // g
	materialDiffuse[2] = 0.1f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.45f; // r
	materialSpecular[1] = 0.55f; // g
	materialSpecular[2] = 0.45f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col3_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 4th sphere on 3rd column, red
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.0f; // g
	materialDiffuse[2] = 0.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7f; // r
	materialSpecular[1] = 0.6f; // g
	materialSpecular[2] = 0.6f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col3_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 5th sphere on 3rd column, white
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.55f; // r
	materialDiffuse[1] = 0.55f; // g
	materialDiffuse[2] = 0.55f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.70f; // r
	materialSpecular[1] = 0.70f; // g
	materialSpecular[2] = 0.70f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col3_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 6th sphere on 3rd column, yellow
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.60f; // r
	materialSpecular[1] = 0.60f; // g
	materialSpecular[2] = 0.50f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col3_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 1st sphere on 4th column, black
	// ambient material
	materialAmbient[0] = 0.02f; // r
	materialAmbient[1] = 0.02f; // g
	materialAmbient[2] = 0.02f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.1f; // r
	materialDiffuse[1] = 0.1f; // g
	materialDiffuse[2] = 0.1f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.4f; // r
	materialSpecular[1] = 0.4f; // g
	materialSpecular[2] = 0.4f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col4_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd sphere on 4th column, cyan
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.05f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.4f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.5f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.04f; // r
	materialSpecular[1] = 0.7f; // g
	materialSpecular[2] = 0.7f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col4_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 3rd sphere on 4th column, green
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.4f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.4f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.04f; // r
	materialSpecular[1] = 0.7f; // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col4_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 4th sphere on 4th column, red
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.4f; // g
	materialDiffuse[2] = 0.4f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7f; // r
	materialSpecular[1] = 0.04f; // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col4_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 5th sphere on 4th column, white
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.05f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.5f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7f; // r
	materialSpecular[1] = 0.7f; // g
	materialSpecular[2] = 0.7f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col4_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 6th sphere on 4th column, yellow
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.4f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7f; // r
	materialSpecular[1] = 0.7f; // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glViewport(col4_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	glXSwapBuffers(display,window);
}

void update(void)
{
    // code
   if (keyPressed == 1)
	{
		angleForXRotation = angleForXRotation + 1.0f;
		if (angleForXRotation >= 360.0f)
		{
			angleForXRotation = angleForXRotation - 360.0f;

		}
	}

	if (keyPressed == 2)
	{
		angleForYRotation = angleForYRotation + 1.0f;
		if (angleForYRotation >= 360.0f)
		{
			angleForYRotation = angleForYRotation - 360.0f;
		}
	}

	if (keyPressed == 3)
	{
		angleForZRotation = angleForZRotation + 1.0f;
		if (angleForZRotation >= 360.0f)
		{
			angleForZRotation = angleForZRotation - 360.0f;
		}
	}
}

void uninitialize(void)
{
    // local variable declaration
    GLXContext currentGLXContext = NULL;

    //code
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

    if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}

    if(gpFILE)
    {
        fprintf(gpFILE,"Program Ended\n");
        fclose(gpFILE);
        gpFILE = NULL;
    }
}

