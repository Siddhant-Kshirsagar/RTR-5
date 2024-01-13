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
XVisualInfo visualInfo;

Bool bFullScreen = False;

// entry point function
int main(void)
{
    // local function declaration
    void uninitialize(void);
    void ToggleFullScreen(void);

    // local variable declaration
    int defaultScreen;
    int defaultDepth;

    Status status;
    XSetWindowAttributes windowAttribute;
    Colormap colormap;
    int styleMask;
    Atom windowManagerDelete;
    XEvent event;
    KeySym keySym;

    int screenWidth;
    int screenHeight;

    char keys[26];

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
            // smilar to WM_KEYDOWN in windows O.S
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
            printf("MapNotfiy event is received\n");
            break;

        case FocusIn: // similar to WM_SETFOCUS in windows
            printf("FocusIn message is received\n");
            break;

        case FocusOut: // similar to WM_KILLFOCUS in windows
            printf("FoucsOut message is received\n");
            break;

        case ConfigureNotify: // similar to WM_SIZE in windows
            printf("ConfigureNotify event is received\n");
            break;

        case Expose: // similar to WM_PAINT in windows
            break;

        case ButtonPress:
            switch (event.xbutton.button)
            {
            case 1:
                printf("Left mouse button is clicked\n");
                break;
            case 2:
                printf("Middle mouse button is clicked\n");
                break;
            case 3:
                printf("Right mouse button is clicked\n");
                break;
            default:
                break;
            }
            break;
        case DestroyNotify:
            printf("DestroyNotify message is received\n");
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
    XRootWindow(display,visualInfo.screen),
    False,
    SubstructureNotifyMask,
    &event); 
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

