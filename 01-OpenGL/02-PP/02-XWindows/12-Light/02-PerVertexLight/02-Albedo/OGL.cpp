//standard header file
#include<stdio.h> // for printf()
#include<stdlib.h> // for exit()
#include<memory.h> // for memset()

// x11 header file
#include<X11/Xlib.h> // for all XWindow API (in window Windows.h similarly int XWindow X11/Xlib.h)
#include<X11/Xutil.h> // for XVisualInfo and related API
#include<X11/XKBlib.h>

// OpenGL header file
#include<GL/glew.h> // this header file must be inluded before gl.h
#include<GL/gl.h>
#include<GL/glx.h>

#include"vmath.h"
using namespace vmath;

#include"Sphere.h"

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
typedef GLXContext (*glXCreateContextAttribsARBProc) (Display *,GLXFBConfig,GLXContext,Bool,const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

GLXFBConfig glxFBConfig;
GLXContext glxContext = NULL;
XVisualInfo *visualInfo = NULL;

// OpenGL related variable for shaders and pp related
GLuint shaderProgramObject = 0;

// for sphere
// for sphere
GLuint vao_sphere = 0;
GLuint vbo_positionSphere = 0;
GLuint vbo_normalSphere = 0;
GLuint vbo_elementSphere = 0;
GLuint vbo_texureSphere = 0;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

unsigned int gNumVertices;
unsigned int gNumElements;

GLuint modelMatrixUniform = 0;
GLuint viewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;

GLuint lightAmbientUniform = 0;
GLuint lightDiffuseUniform = 0;
GLuint lightSpecularUniform = 0;
GLuint lightPositionUniform = 0;

GLuint materialDiffuseUniform = 0;  
GLuint materialAmbientUniform = 0;
GLuint materialSpecularUniform = 0;
GLuint materialShininessUniform = 0;


GLuint keyPressedUniform = 0;

Bool bLightingEnable = False;

GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 0.5f,0.2f,0.7f,1.0f };
GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f,1.0f };
GLfloat materialShininess = 128.0f;

// mat4 is datatype means 4 * 4 matrix (present in vmath.h)
mat4 perspectiveProjectionMatrix;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_TEXCOORD,
	AMC_ATTRIBUTE_NORMAL
};

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
        GLX_X_RENDERABLE, True, // PP Related
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, // PP Related
        GLX_RENDER_TYPE, GLX_RGBA_BIT, // PP Related
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR, // PP Related
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE,24,
        GLX_STENCIL_SIZE, 8,
        None
    };

    Bool bDone = False;

    int iResult = 0;

    // PP related variable 
    GLXFBConfig *glxFBConfigs = NULL;
    GLXFBConfig bestGLXFBConfig;
    XVisualInfo *tempXVisualInfo = NULL;

    int numFBConfig;
    int bestFrameBufferConfig = -1;
    int bestNumberOfSample = -1;

    int worstFrameBufferConfig = -1;
    int worstNumberOfSample = 9999;

    int sampleBuffers,samples;
    int i;

    // code

    // log file code
    gpFILE = fopen("Log.txt","w");
    if(gpFILE == NULL)
    {
        printf("Log file cannot be open\n");
        exit(0);
    }
    else
    {
        fprintf(gpFILE, "Program Started Successfully\n");
	    fprintf(gpFILE, "\n==============================================================================\n");
    }
    
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

    // step PP 1 : get avalable GLXFBConfig from xWindows
    glxFBConfigs = glXChooseFBConfig(display,XDefaultScreen(display),frameBufferAttribute,&numFBConfig);
    if(glxFBConfigs == NULL)
    {
        printf("glXChooseFBConfig() failed. matching glxFBConfings not found\n");
        uninitialize();
        exit(-1);
    }
    fprintf(gpFILE,"\t%d matching glxFBConfings found.\n",numFBConfig);

    // step PP 2 : find best matching fb config above array
    for(i = 0; i<numFBConfig; i++)
    {
        tempXVisualInfo = glXGetVisualFromFBConfig(display, glxFBConfigs[i]);
        if(tempXVisualInfo != NULL)
        {
            // get sample buffers
            glXGetFBConfigAttrib(display, glxFBConfigs[i],GLX_SAMPLE_BUFFERS,&sampleBuffers);

            // get samples
            glXGetFBConfigAttrib(display, glxFBConfigs[i], GLX_SAMPLES, &samples);
        

            if(bestFrameBufferConfig < 0 || sampleBuffers && samples > bestNumberOfSample)
            {
                bestFrameBufferConfig = i;
                bestNumberOfSample = samples;
            }

            if(worstFrameBufferConfig < 0 || !sampleBuffers || samples < worstNumberOfSample)
            {
                worstFrameBufferConfig = i;
                worstNumberOfSample = samples;
            }

            XFree(tempXVisualInfo);
            tempXVisualInfo = NULL;
        }
    }

    // Accordingly get best glxFBConfig
    bestGLXFBConfig = glxFBConfigs[bestFrameBufferConfig];

    // Assign this found bestGLXFBConfig to global glxFBConfig
    glxFBConfig = bestGLXFBConfig;

    // free the memory given to the array
    XFree(glxFBConfigs);
    glxFBConfigs = NULL;

    visualInfo = glXGetVisualFromFBConfig(display,bestGLXFBConfig);
    if(visualInfo == NULL)
    {
        printf("\t glXGetVisualFromFBConfig() Failed. The choosen visual ID 0x%lu\n",visualInfo->visualid);
        uninitialize();
        exit(-1);
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
                    case 'L':
                    case 'l':
                        if (bLightingEnable == False)
                        {
                            bLightingEnable = True;
                        }
                        else
                        {
                            bLightingEnable = False;
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

            //Update
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

    // function declaration
    void printGLInfo(void);
    void resize(int,int);
    void uninitialize(void);

    //variable declaration
    int attribs_new[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 6,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    int attribs_old[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        None
    };
    
    // code
    // get the address of function in function pointer
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((GLubyte*)"glXCreateContextAttribsARB");
    if(glXCreateContextAttribsARB == NULL)
    {
        fprintf(gpFILE,"glXGetProcAddress failed. Cannot get required function address \n");
        uninitialize();
        exit(0);
    }

    // create PP compatible glxContext
    glxContext = glXCreateContextAttribsARB(display, glxFBConfig, 0, True, attribs_new);
    if(!glxContext)
    {
        fprintf(gpFILE,"Core profile based glxContext cannot be obtained.\n falling back to context\n");

        // getting old context
        glxContext = glXCreateContextAttribsARB(display, glxFBConfig, 0, True, attribs_old);
        if(!glxContext)
        {
            fprintf(gpFILE,"old glxContext cannot be found.\n");
            uninitialize();
            exit(0);
        }
        else
        {
            fprintf(gpFILE,"old glxContext found.\n");
        }
    }
    else
    {
        fprintf(gpFILE,"core profile glxContext obtained successfully.\n");
    }

    // check glxContext support direct / hardware rendering 
    if(!glXIsDirect(display,glxContext))
    {
        fprintf(gpFILE,"Not supported direct / hardware rendering.\n");
    }
    else
    {
        fprintf(gpFILE,"supported direct / hardware rendering.\n");
    }
 
    // make this OpenGL context as current context
    if(glXMakeCurrent(display,window,glxContext) == False)
    {
        fprintf(gpFILE,"glXMakeCurrent() failed\n");
        return(2);
    }

    	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFILE, "glewInit() Failed to initialize GLEW\n");
		return(-6);
	}
	
	// print GL Info 
	printGLInfo();

    // Vertex Shader
	// step 1 : vertex shader code
	const GLchar *vertexShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec4 aPosition;" \
		"in vec3 aNormal;" \
		"uniform mat4 uModelMatrix;" \
		"uniform mat4 uViewMatrix;" \
		"uniform mat4 uProjectionMatrix;" \
		"uniform vec3 uLightAmbient;" \
		"uniform vec3 uLightDiffuse;" \
		"uniform vec3 uLightSpecular;" \
		"uniform vec4 uLightPosition;" \
		"uniform vec3 uMaterialAmbient;" \
		"uniform vec3 uMaterialDiffuse;" \
		"uniform vec3 uMaterialSpecular;" \
		"uniform float uMaterialShineness;" \
		"uniform int uKeyPressed;" \
		"out vec3 oPhongADSLight;" \
		"void main(void)" \
		"{" \
		"if(uKeyPressed == 1)" \
		"{" \
		"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" \
		"vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);" \
		"vec3 lightDirection = normalize(vec3(uLightPosition-eyeCoordinates));" \
		"vec3 reflectionVector = reflect(-lightDirection,transformedNormals);" \
		"vec3 viewerVector = normalize(-eyeCoordinates.xyz);" \
		"vec3 lightAmbient = uLightAmbient * uMaterialAmbient;" \
		"vec3 lightDiffuse = uLightDiffuse * uMaterialDiffuse * max(dot(lightDirection,transformedNormals),0.0);" \
		"vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector,viewerVector),0.0),uMaterialShineness);" \
		"oPhongADSLight = lightAmbient + lightDiffuse + lightSpecular;" \
		"}"\
		"else" \
		"{" \
		"oPhongADSLight = vec3(0.0,0.0,0.0);" \
		"}" \
		"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;" \
		"}";

	// step 2 : create vertex shader object
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// step 3 : give vertex source code to openGL
	glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	// step 4 : let OpenGL compile vertex shader
	glCompileShader(vertexShaderObject);

	// step 5 :
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// step 5 a:
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

	// step 5 b:
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 5 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

			if (szInfoLog != NULL)
			{

				// step 5 d:
				glGetShaderInfoLog(vertexShaderObject, infoLogLength + 1, NULL, szInfoLog);

				// step 5 e:
				fprintf(gpFILE, "Vertex shader compilation error log : %s\n", szInfoLog);

				// step 5 f:
				free(szInfoLog);

				szInfoLog = NULL;

			}
		}


		// step 5 g:
		uninitialize();
	}

	// Fragment shader
	// step 6 : write the fragement shader code
	const GLchar *fragmentShaderCode =
		"#version 460 core" \
		"\n" \
		"in vec3 oPhongADSLight;" \
		"uniform int uKeyPressed;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"if(uKeyPressed == 1)" \
		"{" \
		"FragColor = vec4(oPhongADSLight,1.0);" \
		"}" \
		"else" \
		"{" \
		"FragColor = vec4(1.0,1.0,1.0,1.0);" \
		"}" \
		"}";
	
	// step 7 : create fragment shader object
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// step 8 : give fragment shader source code to OpenGL
	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderCode, NULL);

	// step 9 : let OpenGL compile fragment shader
	glCompileShader(fragmentShaderObject);

	// step 10 : 
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	// step 10 a:
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

	// step 10 b:
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 10 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));
			if (szInfoLog != NULL)
			{
				// step 10 d:
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength + 1, NULL, szInfoLog);

				// step 10 e:
				fprintf(gpFILE, "fragmenet shader compilation log : %s\n", szInfoLog);

				// step 10 f:
				free(szInfoLog);

				szInfoLog = NULL;

			}

		}

		// step 10 g:
		uninitialize();
	}

	// shader program
	// step 11 : create shader program
	shaderProgramObject = glCreateProgram();

	// step 12 : attach shader to this program
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// step 13 : bind attribute location with the shader program object
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");

	// step 14 : link the shader program
	glLinkProgram(shaderProgramObject);

	// step 15 : linking error check
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;
		
	// step 15 a : get linking status 
	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		// step 15 b :
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 15 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

			if (szInfoLog != NULL)
			{
				// step 15 d:
				glGetProgramInfoLog(shaderProgramObject, infoLogLength + 1, NULL, szInfoLog);

				// step 15 e:
				fprintf(gpFILE, "Shader program linking error log : %s \n", szInfoLog);

				// step 15 f:
				free(szInfoLog);

				szInfoLog = NULL;


			}
		}

		// step 15 e:
		uninitialize();
	}

	// get shader uniform location

	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
	lightAmbientUniform = glGetUniformLocation(shaderProgramObject, "uLightAmbient");
	lightDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uLightDiffuse");
	lightSpecularUniform = glGetUniformLocation(shaderProgramObject, "uLightSpecular");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");
	materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");
	materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");
	materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");
	materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShineness");
	keyPressedUniform = glGetUniformLocation(shaderProgramObject, "uKeyPressed");


	// step 16: declare position and color array 

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// for sphere
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao_sphere);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao_sphere);

	// position vbo
	glGenBuffers(1, &vbo_positionSphere);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_positionSphere);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &vbo_normalSphere);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalSphere);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// texture vbo
	glGenBuffers(1, &vbo_texureSphere);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texureSphere);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_textures), sphere_textures, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &vbo_elementSphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);

	glBindVertexArray(0);

	//for enable depth
	glClearDepth(1.0f); // compulsory
	glEnable(GL_DEPTH_TEST);// compulsory
	glDepthFunc(GL_LEQUAL);// compulsory

	// step 7 : - set clear color of window to blue (here OpenGL Start)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialize orthographic projection matrix 
	perspectiveProjectionMatrix = vmath::mat4::identity();

	resize(WINWIDTH, WINHEIGHT);

    return(0);
}

void printGLInfo(void)
{
	//variable declaration
	GLint numExtension;
	GLint i; // for loop counter

	// code
	fprintf(gpFILE, "OpenGL Vender :	%s\n", glGetString(GL_VENDOR));
	fprintf(gpFILE, "OpenGL Renderer :	%s\n", glGetString(GL_RENDERER));
	fprintf(gpFILE, "OpenGL Version :	%s\n", glGetString(GL_VERSION));
	fprintf(gpFILE, "OpenGL GLSL(Graphic Library Shading Language) Version :	%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	fprintf(gpFILE, "\n==============================================================================\n");

	// Listing of Supported Extension
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtension);

	for (i = 0; i < numExtension; i++)
	{
		fprintf(gpFILE, "\t%s\n", glGetStringi(GL_EXTENSIONS, i));
	}

	fprintf(gpFILE, "\n==============================================================================\n");
}


void resize(int width,int height)
{
    // code
    if(height == 0)
    {
        height = 1;
    }
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// set perspective projection matrix
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);


}

void draw(void) // similar to display() in windows
{
//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// step 1 : use shader program
	glUseProgram(shaderProgramObject);

   	// Transformation
	mat4 translationMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

	mat4 modelMatrix = mat4::identity();
	modelMatrix = translationMatrix;

	mat4 viewMatrix = mat4::identity();

	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	// *** bind vao ***
	glBindVertexArray(vao_sphere);

	if (bLightingEnable == True)
	{
		glUniform1i(keyPressedUniform, 1);
		glUniform3fv(lightAmbientUniform, 1, lightAmbient);
		glUniform3fv(lightDiffuseUniform, 1, lightDiffuse);
		glUniform3fv(lightSpecularUniform, 1, lightSpecular);
		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);
		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
		glUniform3fv(materialSpecularUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShininess);
		
	}
	else
	{
		glUniform1i(keyPressedUniform, 0);
	}

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	glUseProgram(0);

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
    // for shader program object 
	if (shaderProgramObject)
	{
		// step 1 : use shader Program object
		glUseProgram(shaderProgramObject);

		// step 2 : get number of attached shader
		GLint numShader = 0;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShader);
			
		if (numShader > 0)
		{
			GLuint *pShader = (GLuint *)malloc(numShader * sizeof(GLuint));
			if (pShader != NULL)
			{
				glGetAttachedShaders(shaderProgramObject, numShader, NULL, pShader);

				for (GLint i = 0; i < numShader; i++)
				{
					glDetachShader(shaderProgramObject, pShader[i]);

					glDeleteShader(pShader[i]);

					pShader[i] = 0;
				}

				free(pShader);

				pShader = NULL;
			}
		}


		glUseProgram(0);

		glDeleteProgram(shaderProgramObject);

		shaderProgramObject = 0;
	}

	// sphere 
    // delete vbo for texture
	if (vbo_texureSphere)
	{
		glDeleteBuffers(1, &vbo_texureSphere);
		vbo_texureSphere = 0;
	}
	// delete vbo for element
	if (vbo_elementSphere)
	{
		glDeleteBuffers(1, &vbo_elementSphere);
		vbo_elementSphere = 0;
	}

	// delete vbo for normal
	if (vbo_normalSphere)
	{
		glDeleteBuffers(1, &vbo_normalSphere);
		vbo_normalSphere = 0;
	}
	// delete vbo for position
	if (vbo_positionSphere)
	{
		glDeleteBuffers(1, &vbo_positionSphere);
		vbo_positionSphere = 0;
	}

	// delete vao 
	if (vao_sphere)
	{
		glDeleteVertexArrays(1, &vao_sphere);
		vao_sphere = 0;
	}

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

