//standard header file
#include<stdio.h> // for printf()
#include<stdlib.h> // for exit()
#include<memory.h> // for memset()

// x11 header file
#include<X11/Xlib.h> // for all XWindow API (in window Windows.h similarly int XWindow X11/Xlib.h)
#include<X11/Xutil.h> // for XVisualInfo and related API
#include<X11/XKBlib.h>

// Macros
#define WINWIDTH 800
#define WINHEIGHT 600

// global variable declaration
Display *display = NULL; // Display is interface between XServer and Client
Colormap colormap;
Window window;

// entry point function
int main(void)
{
    // local function declaration
    void uninitialize(void);

    // local variable declaration
    int defaultScreen;
    int defaultDepth;
    XVisualInfo visualInfo;
    Status status;
    XSetWindowAttributes windowAttribute;
    Colormap colormap;
    int styleMask;
    Atom windowManagerDelete;
    XEvent event;
    KeySym keySym;

    int screenWidth;
    int screenHeight;

    // code

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

    // step 3: get default depth from above two step
    defaultDepth = XDefaultDepth(display,defaultScreen);

    // step 4: get visual info from above three step
    memset((void*)&visualInfo,0,sizeof(XVisualInfo));
    status = XMatchVisualInfo(display,defaultScreen,defaultDepth,TrueColor,&visualInfo);
    if(status == 0)
    {
        printf("\n\tXMatchVisualInfo() failed.\n");
        uninitialize();
        exit(1);
    }

    // step 5: set window attribute
    memset((void*)&windowAttribute,0,sizeof(XWindowAttributes));
    windowAttribute.border_pixel = 0; // window border color default use karnya sathi 0 dila
    windowAttribute.background_pixel = XWhitePixel(display,visualInfo.screen); // background la black color denyasathi
    windowAttribute.background_pixmap = 0; // background la brush/image deto tevha hi value dyaychi atta black color detoy mhanun ha 0 dila
    windowAttribute.colormap = XCreateColormap(display,
    XRootWindow(display,visualInfo.screen),
    visualInfo.visual,
    AllocNone);

    // step 6: assign this Colormap to global Colormap
    colormap = windowAttribute.colormap;

    // step 7:set the style of window
    styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;

    // step 8 : create window
    window = XCreateWindow(display,
    XRootWindow(display,visualInfo.screen),
    0,
    0,
    WINWIDTH,
    WINHEIGHT,
    0,
    visualInfo.depth,
    InputOutput,
    visualInfo.visual,
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
    KeyPressMask | ButtonPressMask | PointerMotionMask);

    // step 10: specify window manager Atom
    windowManagerDelete = XInternAtom(display,"WM_DELETE_WINDOW",True);

    // step 11 : add above Atom as protocol for window manager
    XSetWMProtocols(display,window,&windowManagerDelete,1);

    // step 12 : give caption to the window
    XStoreName(display,window,"Siddhant Ganesh Kshirsagar : XWindow");

    // step 13 : show / map the window
    XMapWindow(display,window);

    // center the window
    screenWidth = XWidthOfScreen(XScreenOfDisplay(display,visualInfo.screen));
    screenHeight = XHeightOfScreen(XScreenOfDisplay(display,visualInfo.screen));
    XMoveWindow(display,window,(screenWidth - WINWIDTH)/2,(screenHeight-WINHEIGHT)/2);

    // step 14 : event loop
    while(1)
    {
        XNextEvent(display,&event);
        switch (event.type)
        {
        case KeyPress:
        {
            keySym = XkbKeycodeToKeysym(display,
            event.xkey.keycode,0,0);
            switch (keySym)
            {
            case XK_Escape:
                uninitialize();
                exit(0);
                break;
            
            default:
                break;
            }
        }
            break;
            case 33:
            uninitialize();
            exit(0);
            break;
        
        default:
            break;
        }
    }
    uninitialize();
    return(0);
}

void uninitialize(void)
{
    //code
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
}

