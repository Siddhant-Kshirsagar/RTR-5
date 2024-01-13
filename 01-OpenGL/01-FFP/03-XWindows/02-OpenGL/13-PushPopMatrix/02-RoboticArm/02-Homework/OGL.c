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

int sholder = 0;
int elbow = 0;
int wrist = 0;

GLUquadric *quadric = NULL;

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
                    case 'S':
                        sholder = (sholder + 3) % 360;
                        break;
                    case 's':
                        sholder = (sholder - 3) % 360;
                        break;
                    case 'E':
                        elbow = (elbow + 3) % 360;
                        break;
                    case 'e':
                        elbow = (elbow - 3) % 360;
                        break;
                    case 'W':
                        wrist = (wrist + 3) % 360;
                        break;
                    case 'w':
                        wrist = (wrist - 3) % 360;
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

    // Initialize quadric
	quadric = gluNewQuadric();

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
    // code
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Do Initial translation for better visibility
	glTranslatef(0.0f,0.0f,-12.0f);

//Push Matrix
	glPushMatrix();
	{
		// do transformation for arm
		glRotatef((GLfloat)sholder, 0.0f, 0.0f, 1.0f); // we are rotating z axis because by default z Axis is behave like x Axis in glu Library
		glTranslatef(1.0f, 0.0f, 0.0f);

		glPushMatrix();
		{
			glScalef(2.0f, 0.5f, 1.0f);
			//draw arm
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.5f, 10, 10);
		}
		//pop the matrix where comeback where arm ended by translation not by scale
		glPopMatrix();

		// do transformation for forearm in MTRT 
		glTranslatef(1.0f, 0.0f, 0.0f);

		glRotatef((GLfloat)elbow, 0.0f, 0.0f, 1.0f);

		glTranslatef(1.0f, 0.0f, 0.0f);

		glPushMatrix();
		{
			glScalef(2.0f, 0.5f, 1.0f);
			//draw the fore arm
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.5f, 10, 10);
		}
		glPopMatrix();

		// draw palm of hand
		glTranslatef(1.0f, 0.0f, 0.0f);

		glRotatef((GLfloat)wrist, 0.0f, 0.0f, 1.0f);

		glTranslatef(0.4f, 0.0f, 0.0f);

		glPushMatrix();
		{
			glScalef(2.0f, 1.4f, 1.0f);
			//draw the hand
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.2f, 10, 10);
		}
		glPopMatrix();

		// draw thumb
		//glTranslatef(1.0f, 0.0f, 0.0f);

		glPushMatrix();
		{
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.45f, 0.0f, 0.0f);
			glScalef(0.8f, 0.4f, 1.0f);
			//draw the hand
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.2f, 10, 10);
		}
		glPopMatrix();

		// draw middle finger
		//glTranslatef(1.0f, 0.0f, 0.0f);

		//glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

		glPushMatrix();
		{
			glTranslatef(0.65f, 0.05f, 0.0f);

			glScalef(1.2f, 0.4f, 1.0f);
			//draw the hand
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.2f, 10, 10);
		}
		glPopMatrix();

		// draw index finger
		//glTranslatef(1.0f, 0.0f, 0.0f);

		glPushMatrix();
		{
			glRotatef(10.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.55f, 0.15f, 0.0f);

			glScalef(1.2f, 0.4f, 1.0f);
			//draw the hand
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.2f, 10, 10);
		}
		glPopMatrix();

		// draw ring finger
		//glTranslatef(1.0f, 0.0f, 0.0f);

		glPushMatrix();
		{
			glRotatef(-10.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.65f, -0.05f, 0.0f);

			glScalef(1.2f, 0.4f, 1.0f);
			//draw the hand
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.2f, 10, 10);
		}
		glPopMatrix();

		// draw pinky finger
		//glTranslatef(1.0f, 0.0f, 0.0f);

		glPushMatrix();
		{
			glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.55f, -0.15f, 0.0f);

			glScalef(1.0f, 0.3f, 1.0f);
			//draw the hand
			glColor3f(0.8f, 0.6f, 0.4f);
			gluSphere(quadric, 0.2f, 10, 10);
		}
		glPopMatrix();
		
	}
	glPopMatrix();


    glXSwapBuffers(display,window);
}

void update(void)
{
    // code
    
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

