#import<Foundation/Foundation.h>
#import<Cocoa/Cocoa.h>
#import<QuartzCore/CVDisplayLink.h> // for thread
#import<OpenGL/gl3.h>
#import<OpenGL/gl3ext.h>

#include"Sphere.h"
#include"vmath.h"
using namespace vmath;

// Global function declaration
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef,const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags,CVOptionFlags*,void *);

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global variable declaration
FILE *gpFILE = NULL;

@interface AppDelegate:NSObject <NSApplicationDelegate,NSWindowDelegate>
@end

@interface GLView:NSOpenGLView
@end

// main function
int main(int argc, char* argv[])
{
    //code

    // Create auto release pool for memory management
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

    // Create global shared NSApplication object
    NSApp = [NSApplication sharedApplication];

    // Set it's delegate to our own custom app delegate
    [NSApp setDelegate: [[AppDelegate alloc]init]];

    // Start NSApp run loop
    [NSApp run];

    // Let auto release pool release all pending object in our application
    [pool release];

    return(0);
}

// Implementation of our custom AppDelegate interface
@implementation AppDelegate
{
    NSWindow *window;
    GLView *glView;
}

-(void)applicationDidFinishLaunching:(NSNotification *)notification
{
    // code
    
    // log file opening code
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath =[appDirPath stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/Log.txt",parentDirPath];
    const char *pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];

    gpFILE = fopen(pszLogFileNameWithPath,"w");
    if(!gpFILE)
    {
        NSLog(@"Log File Cannot be created");
        [self release];
        [NSApp terminate:self];
    }
    fprintf(gpFILE,"Program started successfully \n");
    fprintf(gpFILE,"\n---------------------------\n");


    // Step 1: Declare reccAngle for frame/border of window
    NSRect win_rect = NSMakeRect(0.0,0.0,800.0,600.0);

    // Step 2: Create window
    window = [[NSWindow alloc]initWithContentRect: win_rect
                                        styleMask: NSWindowStyleMaskTitled|
                                                   NSWindowStyleMaskClosable|
                                                   NSWindowStyleMaskMiniaturizable|
                                                   NSWindowStyleMaskResizable
                                         backing : NSBackingStoreBuffered
                                           defer : NO];

    // Give title to the window
    [window setTitle: @"SGK: Per Vertex Per Fragment Toggling"];

    // center the window
    [window center];

    // painting window background black
    [window setBackgroundColor:[NSColor blackColor]];

    // create the custom view
    glView = [[GLView alloc]initWithFrame: win_rect];

    // set this newly created custom view as view of our newly created custom windows
    [window setContentView: glView];

    //
    [window setDelegate:self];

    // Actually show the focus, give it keyboard focus and make it top on Z-order
    [window makeKeyAndOrderFront:self];
}
    // Destroy application
    -(void)applicationWillTerminate:(NSNotification *)notification
    {
        // log file closing code
        if(gpFILE)
        {
            fprintf(gpFILE,"Program finished successfully\n");
            fclose(gpFILE);
            gpFILE = NULL;
        }
    }

    -(void)windowWillClose:(NSNotification *)notification
    {
        // code
        [NSApp terminate:self];
    }

    -(void)dealloc
    {
        [super dealloc];
        [glView release];
        [window release];
    }

@end

// Implementation of custom view interface
@implementation GLView
{
    CVDisplayLinkRef displayLink;
    // OpenGL related variable
   GLuint shaderProgramObject_PV;
    GLuint shaderProgramObject_PF;

    Mesh sphere;

    GLuint modelMatrixUniform_PV;
    GLuint viewMatrixUniform_PV;
    GLuint projectionMatrixUniform_PV;

    GLuint lightAmbientUniform_PV;
    GLuint lightDiffuseUniform_PV;
    GLuint lightSpecularUniform_PV;
    GLuint lightPositionUniform_PV;

    GLuint materialDiffuseUniform_PV;
    GLuint materialAmbientUniform_PV;
    GLuint materialSpecularUniform_PV;
    GLuint materialShininessUniform_PV;

    GLuint keyPressedUniform_PV;

    GLuint modelMatrixUniform_PF;
    GLuint viewMatrixUniform_PF;
    GLuint projectionMatrixUniform_PF;

    GLuint lightAmbientUniform_PF;
    GLuint lightDiffuseUniform_PF;
    GLuint lightSpecularUniform_PF;
    GLuint lightPositionUniform_PF;

    GLuint materialDiffuseUniform_PF;
    GLuint materialAmbientUniform_PF;
    GLuint materialSpecularUniform_PF;
    GLuint materialShininessUniform_PF;

    GLuint keyPressedUniform_PF;

    bool bLightingEnable;
    bool bAnimationEnable;
    GLchar chooseShader ; // char 'v' for per vertex light / char 'f' per fragment light

    struct LIGHT
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec4 position;
    };

    struct LIGHT light;

    GLfloat materialAmbient[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;


    // mat4 is datatype means 4 * 4 matrix (present in vmath.h)
    mat4 perspectiveProjectionMatrix;
}

-(id)initWithFrame:(NSRect)frame
{
    // code
    self = [super initWithFrame:frame];

    if(self)
    {
        // declare OpenGL pixel format attribute
        NSOpenGLPixelFormatAttribute attribute[] =
        {
            NSOpenGLPFAOpenGLProfile,NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFAColorSize,24,
            NSOpenGLPFADepthSize,32,
            NSOpenGLPFAAlphaSize,8,
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFADoubleBuffer,
            0
        };

        NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc]initWithAttributes:attribute]autorelease];

        if(pixelFormat == nil)
        {
            fprintf(gpFILE,"NSOpenGLPixelFormat failed.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }

        // create openGL context using above pixel format
        NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil]autorelease];
       
        if(glContext == nil)
        {
            fprintf(gpFILE,"NSOpenGLContext failed.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }

        // set pixel format
        [self setPixelFormat:pixelFormat];

        // set OpenGL context
        [self setOpenGLContext : glContext];


    }

    return(self);
}

-(void)prepareOpenGL
{
    [super prepareOpenGL];
    
    // make current context
    [[self openGLContext]makeCurrentContext];

    // matching monitor retracing with the double buffer swapping
    GLint swapInterval = 1;
    [[self openGLContext]setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];

    int result = [self initialize];

    // create and start displayLink
    
    // 1:create display link according to the current attribute
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    // 2:set callback for openGL thread
    CVDisplayLinkSetOutputCallback(displayLink,&MyDisplayLinkCallback,self);
    // 3: convert NSOpenGL PixelFormat into CGLPixelFormat
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];

    // 4: convert NSContext into CGLContext
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];

    // 5: set above two for displaylink
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink,cglContext,cglPixelFormat);

    //
    CVDisplayLinkStart(displayLink);


}

-(void)reshape
{
    [super reshape];

    // make current context
    [[self openGLContext]makeCurrentContext];

    // lock context
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

    NSRect rect = [self bounds];

    int width = rect.size.width;
   int  height = rect.size.height;

    // call user defined function resize
    [self resize:width : height];

    // unlock context
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

    // call rendering function here to avoid flickring
    [self drawView];

}

-(int)initialize
{
    //code
    [self printGLInfo];

    // Per Vertex 
	{
		// step 1 : vertex shader code
		const GLchar *vertexShaderSourceCode_PV =
			"#version 410 core" \
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
		GLuint vertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

		// step 3 : give vertex source code to openGL
		glShaderSource(vertexShaderObject_PV, 1, (const GLchar **)&vertexShaderSourceCode_PV, NULL);

		// step 4 : let OpenGL compile vertex shader
		glCompileShader(vertexShaderObject_PV);

		// step 5 :
		GLint status = 0;
		GLint infoLogLength = 0;
		GLchar *szInfoLog = NULL;

		// step 5 a:
		glGetShaderiv(vertexShaderObject_PV, GL_COMPILE_STATUS, &status);

		// step 5 b:
		if (status == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 5 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

				if (szInfoLog != NULL)
				{

					// step 5 d:
					glGetShaderInfoLog(vertexShaderObject_PV, infoLogLength + 1, NULL, szInfoLog);

					// step 5 e:
					fprintf(gpFILE, "Vertex shader compilation error log : %s\n", szInfoLog);

					// step 5 f:
					free(szInfoLog);

					szInfoLog = NULL;

				}
			}


			// step 5 g:
			[self uninitialize];
		}

		// Fragment shader
		// step 6 : write the fragement shader code
		const GLchar *fragmentShaderCode_PV =
			"#version 410 core" \
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
		GLuint fragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);

		// step 8 : give fragment shader source code to OpenGL
		glShaderSource(fragmentShaderObject_PV, 1, (const GLchar **)&fragmentShaderCode_PV, NULL);

		// step 9 : let OpenGL compile fragment shader
		glCompileShader(fragmentShaderObject_PV);

		// step 10 : 
		status = 0;
		infoLogLength = 0;
		szInfoLog = NULL;

		// step 10 a:
		glGetShaderiv(fragmentShaderObject_PV, GL_COMPILE_STATUS, &status);

		// step 10 b:
		if (status == GL_FALSE)
		{
		glGetShaderiv(fragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 10 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));
			if (szInfoLog != NULL)
			{
				// step 10 d:
				glGetShaderInfoLog(fragmentShaderObject_PV, infoLogLength + 1, NULL, szInfoLog);

				// step 10 e:
				fprintf(gpFILE, "fragmenet shader compilation log : %s\n", szInfoLog);

				// step 10 f:
				free(szInfoLog);

				szInfoLog = NULL;

			}

		}

		// step 10 g:
		[self uninitialize];
		}

		// shader program
		// step 11 : create shader program
		shaderProgramObject_PV = glCreateProgram();

		// step 12 : attach shader to this program
		glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);
		glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

		// step 13 : bind attribute location with the shader program object
		glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_POSITION, "aPosition");

		glBindAttribLocation(shaderProgramObject_PV, AMC_ATTRIBUTE_NORMAL, "aNormal");

		// step 14 : link the shader program
		glLinkProgram(shaderProgramObject_PV);

		// step 15 : linking error check
		status = 0;
		infoLogLength = 0;
		szInfoLog = NULL;

		// step 15 a : get linking status 
		glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
		// step 15 b :
		glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 15 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

			if (szInfoLog != NULL)
			{
				// step 15 d:
				glGetProgramInfoLog(shaderProgramObject_PV, infoLogLength + 1, NULL, szInfoLog);

				// step 15 e:
				fprintf(gpFILE, "Shader program linking error log : %s \n", szInfoLog);

				// step 15 f:
				free(szInfoLog);

				szInfoLog = NULL;


			}
		}

		// step 15 e:
		[self uninitialize];
		}

		// get shader uniform location

		modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uModelMatrix");
		viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uViewMatrix");
		projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uProjectionMatrix");
		lightAmbientUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uLightAmbient");
		lightDiffuseUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse");
		lightSpecularUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular");
		lightPositionUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uLightPosition");
		materialAmbientUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialAmbient");
		materialDiffuseUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialDiffuse");
		materialSpecularUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialSpecular");
		materialShininessUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uMaterialShineness");
		keyPressedUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uKeyPressed");
	}


    // Per Fragment 
    {
        // step 1 : vertex shader code
		const GLchar *vertexShaderSourceCode_PF =
			"#version 410 core" \
			"\n" \
			"in vec4 aPosition;" \
			"in vec3 aNormal;" \
			"uniform mat4 uModelMatrix;" \
			"uniform mat4 uViewMatrix;" \
			"uniform mat4 uProjectionMatrix;" \
			"uniform vec4 uLightPosition;" \
			"uniform int uKeyPressed;" \
			"out vec3 oTransformedNormals;" \
			"out vec3 oLightDirection;" \
			"out vec3 oViewerVector;" \
			"void main(void)" \
			"{" \
			"if(uKeyPressed == 1)" \
			"{" \
			"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" \
			"oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;" \
			"oLightDirection = vec3(uLightPosition-eyeCoordinates);" \
			"oViewerVector = -eyeCoordinates.xyz;" \
			"}" \
			"else" \
			"{" \
			"oTransformedNormals = vec3(0.0,0.0,0.0);" \
			"oLightDirection = vec3(0.0,0.0,0.0);" \
			"oViewerVector = vec3(0.0,0.0,0.0);" \
			"}" \
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;" \
			"}";

		// step 2 : create vertex shader object
		GLuint vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

		// step 3 : give vertex source code to openGL
		glShaderSource(vertexShaderObject_PF, 1, (const GLchar **)&vertexShaderSourceCode_PF, NULL);

		// step 4 : let OpenGL compile vertex shader
		glCompileShader(vertexShaderObject_PF);

		// step 5 :
		GLint status = 0;
		GLint infoLogLength = 0;
		GLchar *szInfoLog = NULL;

		// step 5 a:
		glGetShaderiv(vertexShaderObject_PF, GL_COMPILE_STATUS, &status);

		// step 5 b:
		if (status == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 5 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

				if (szInfoLog != NULL)
				{

					// step 5 d:
					glGetShaderInfoLog(vertexShaderObject_PF, infoLogLength + 1, NULL, szInfoLog);

					// step 5 e:
					fprintf(gpFILE, "Vertex shader compilation error log : %s\n", szInfoLog);

					// step 5 f:
					free(szInfoLog);

					szInfoLog = NULL;

				}
			}


			// step 5 g:
			[self uninitialize];
		}

		// Fragment shader 
		// step 6 : write the fragement shader code
		const GLchar *fragmentShaderCode_PF =
			"#version 410 core" \
			"\n" \
			"in vec3 oTransformedNormals;" \
			"in vec3 oLightDirection;" \
			"in vec3 oViewerVector;" \
			"uniform vec3 uLightAmbient;" \
			"uniform vec3 uLightDiffuse;" \
			"uniform vec3 uLightSpecular;" \
			"uniform vec3 uMaterialAmbient;" \
			"uniform vec3 uMaterialDiffuse;" \
			"uniform vec3 uMaterialSpecular;" \
			"uniform float uMaterialShineness;" \
			"uniform int uKeyPressed;" \
			"out vec4 FragColor;" \
			"void main(void)" \
			"{" \
			"vec3 phongADSLight;" \
			"if(uKeyPressed == 1)" \
			"{" \
			"vec3 normalizeTransformedNormals = normalize(oTransformedNormals);" \
			"vec3 normalizeLightDirection = normalize(oLightDirection);" \
			"vec3 normalizeViewerVector = normalize(oViewerVector);" \
			"vec3 lightAmbient = uLightAmbient * uMaterialAmbient;" \
			"vec3 lightDiffuse = uLightDiffuse * uMaterialDiffuse * max(dot(normalizeLightDirection,normalizeTransformedNormals),0.0);" \
			"vec3 reflectionVector = reflect(-normalizeLightDirection,normalizeTransformedNormals);" \
			"vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector,normalizeViewerVector),0.0),uMaterialShineness);" \
			"phongADSLight = lightAmbient + lightDiffuse + lightSpecular;" \
			"}" \
			"else" \
			"{" \
			"phongADSLight = vec3(1.0,1.0,1.0);" \
			"}" \
			"FragColor = vec4(phongADSLight,1.0);" \
			"}";

		// step 7 : create fragment shader object
		GLuint fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

		// step 8 : give fragment shader source code to OpenGL
		glShaderSource(fragmentShaderObject_PF, 1, (const GLchar **)&fragmentShaderCode_PF, NULL);

		// step 9 : let OpenGL compile fragment shader
		glCompileShader(fragmentShaderObject_PF);

		// step 10 : 
		status = 0;
		infoLogLength = 0;
		szInfoLog = NULL;

		// step 10 a:
		glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &status);

		// step 10 b:
		if (status == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 10 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));
				if (szInfoLog != NULL)
				{
					// step 10 d:
					glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLength + 1, NULL, szInfoLog);

					// step 10 e:
					fprintf(gpFILE, "fragmenet shader compilation log : %s\n", szInfoLog);

					// step 10 f:
					free(szInfoLog);

					szInfoLog = NULL;

				}

			}

			// step 10 g:
			[self uninitialize];
		}

		// shader program
		// step 11 : create shader program
		shaderProgramObject_PF = glCreateProgram();

		// step 12 : attach shader to this program
		glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);
		glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

		// step 13 : bind attribute location with the shader program object
		glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_POSITION, "aPosition");

		glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_NORMAL, "aNormal");

		// step 14 : link the shader program
		glLinkProgram(shaderProgramObject_PF);

		// step 15 : linking error check
		status = 0;
		infoLogLength = 0;
		szInfoLog = NULL;

		// step 15 a : get linking status 
		glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			// step 15 b :
			glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 15 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

				if (szInfoLog != NULL)
				{
					// step 15 d:
					glGetProgramInfoLog(shaderProgramObject_PF, infoLogLength + 1, NULL, szInfoLog);

					// step 15 e:
					fprintf(gpFILE, "Shader program linking error log : %s \n", szInfoLog);

					// step 15 f:
					free(szInfoLog);

					szInfoLog = NULL;


				}
			}

			// step 15 e:
			[self uninitialize];
		}

		// get shader uniform location

		modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uModelMatrix");
		viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uViewMatrix");
		projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");
		lightAmbientUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uLightAmbient");
		lightDiffuseUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse");
		lightSpecularUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular");
		lightPositionUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uLightPosition");
		materialAmbientUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialAmbient");
		materialDiffuseUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialDiffuse");
		materialSpecularUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialSpecular");
		materialShininessUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uMaterialShineness");
		keyPressedUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uKeyPressed");
    }

    // step 16: declare position
    makeSphere(sphere, 2.0, 30, 30);

    // set depth
    //for enable depth
    glClearDepth(1.0f); // compulsory
    glEnable(GL_DEPTH_TEST);// compulsory
    glDepthFunc(GL_LEQUAL);// compulsory

    // step 7 : - set clear color of window to blue (here OpenGL Start)
    glClearColor(0.0f, 0.0f,  0.0f, 1.0f);

    // initialize orthographic projection matrix
    perspectiveProjectionMatrix = vmath::mat4::identity();

    light.ambient = vec3(0.1f,0.1f,0.1f);
    light.diffuse = vec3(1.0f,1.0f,1.0f);
    light.specular = vec3(1.0f,1.0f,1.0f);
    light.position = vec4(100.0f,100.0f,100.0f,1.0f);

    materialAmbient[0] = 0.0f;
    materialAmbient[1] = 0.0f;
    materialAmbient[2] = 0.0f;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5f;
    materialDiffuse[1] = 0.2f;
    materialDiffuse[2] = 0.7f;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.7f;
    materialSpecular[1] = 0.7f;
    materialSpecular[2] = 0.7f;
    materialSpecular[3] = 1.0f;
    
    materialShininess = 128.0f;

    chooseShader = 'v';

    [self resize : WIN_WIDTH : WIN_HEIGHT];

    return(0);
}

-(void)printGLInfo
{
    // code
    fprintf(gpFILE, "OpenGL Vender :    %s\n", glGetString(GL_VENDOR));
    fprintf(gpFILE, "OpenGL Renderer (driver version) :    %s\n", glGetString(GL_RENDERER));
    fprintf(gpFILE, "OpenGL Version :    %s\n", glGetString(GL_VERSION));
    fprintf(gpFILE, "OpenGL GLSL(Graphic Library Shading Language) Version :    %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    fprintf(gpFILE, "\n ------------------------------------------\n");
}

-(void)resize :(int)width :(int)height
{
    // code
    //code
    if (height <= 0)
    {
        height = 1;
    }

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // set perspective projection matrix
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

-(void)display
{
    //code
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if (chooseShader == 'v')
	{
        // step 1 : use shader program
        glUseProgram(shaderProgramObject_PV);

        // cube
        // Transformation
        mat4 modelMatrix = mat4::identity();

        mat4 translationMatrix = mat4::identity();
        translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

        modelMatrix = translationMatrix;

        mat4 viewMatrix = mat4::identity();

        // push above mvp(model view projection) into vertex shader's mvp uniform
        glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);

        glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);

        glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, perspectiveProjectionMatrix);

        // step 2 : bind with VAO(vertex array object)
        if (bLightingEnable == TRUE)
        {
            glUniform1i(keyPressedUniform_PV, 1);

            glUniform3fv(lightAmbientUniform_PV, 1, light.ambient);
            glUniform3fv(lightDiffuseUniform_PV, 1, light.diffuse);
            glUniform3fv(lightSpecularUniform_PV, 1, light.specular);
            glUniform4fv(lightPositionUniform_PV, 1, light.position);

            glUniform3fv(materialAmbientUniform_PV, 1, materialAmbient);
            glUniform3fv(materialDiffuseUniform_PV, 1, materialDiffuse);
            glUniform3fv(materialSpecularUniform_PV, 1, materialSpecular);
            glUniform1f(materialShininessUniform_PV, materialShininess);
        }
        else
        {
            glUniform1i(keyPressedUniform_PV, 0);
        }

        sphere.draw();

        glUseProgram(0);
    }
    if (chooseShader == 'f')
	{
		// step 1 : use shader program
		glUseProgram(shaderProgramObject_PF);

		// sphere
		// Transformation
		mat4 translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

		mat4 modelMatrix = mat4::identity();
		modelMatrix = translationMatrix;

		mat4 viewMatrix = mat4::identity();

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);

		glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);

		glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, perspectiveProjectionMatrix);

		if (bLightingEnable == TRUE)
		{
			glUniform1i(keyPressedUniform_PF, 1);
			glUniform3fv(lightAmbientUniform_PF, 1, light.ambient);
			glUniform3fv(lightDiffuseUniform_PF, 1, light.diffuse);
			glUniform3fv(lightSpecularUniform_PF, 1, light.specular);
			glUniform4fv(lightPositionUniform_PF, 1, light.position);

			glUniform3fv(materialAmbientUniform_PF, 1, materialAmbient);
			glUniform3fv(materialDiffuseUniform_PF, 1, materialDiffuse);
			glUniform3fv(materialSpecularUniform_PF, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PF, materialShininess);

		}
		else
		{
			glUniform1i(keyPressedUniform_PF, 0);
		}

        sphere.draw();

		glUseProgram(0);
	}

}

-(void)myupdate
{
    //code

}

-(void)uninitialize
{
    //code
    // for shader program object
    if (shaderProgramObject_PV)
    {
        // step 1 : use shader Program object
        glUseProgram(shaderProgramObject_PV);

        // step 2 : get number of attached shader
        GLint numShader = 0;

        glGetProgramiv(shaderProgramObject_PV, GL_ATTACHED_SHADERS, &numShader);
            
        if (numShader > 0)
        {
            GLuint *pShader = (GLuint *)malloc(numShader * sizeof(GLuint));
            if (pShader != NULL)
            {
                glGetAttachedShaders(shaderProgramObject_PV, numShader, NULL, pShader);

                for (GLint i = 0; i < numShader; i++)
                {
                    glDetachShader(shaderProgramObject_PV, pShader[i]);

                    glDeleteShader(pShader[i]);

                    pShader[i] = 0;
                }

                free(pShader);

                pShader = NULL;
            }
        }


        glUseProgram(0);

        glDeleteProgram(shaderProgramObject_PV);

        shaderProgramObject_PV = 0;
    }

     if (shaderProgramObject_PF)
    {
        // step 1 : use shader Program object
        glUseProgram(shaderProgramObject_PF);

        // step 2 : get number of attached shader
        GLint numShader = 0;

        glGetProgramiv(shaderProgramObject_PF, GL_ATTACHED_SHADERS, &numShader);
            
        if (numShader > 0)
        {
            GLuint *pShader = (GLuint *)malloc(numShader * sizeof(GLuint));
            if (pShader != NULL)
            {
                glGetAttachedShaders(shaderProgramObject_PF, numShader, NULL, pShader);

                for (GLint i = 0; i < numShader; i++)
                {
                    glDetachShader(shaderProgramObject_PF, pShader[i]);

                    glDeleteShader(pShader[i]);

                    pShader[i] = 0;
                }

                free(pShader);

                pShader = NULL;
            }
        }


        glUseProgram(0);

        glDeleteProgram(shaderProgramObject_PF);

        shaderProgramObject_PF = 0;
    }
}

-(void)drawRect:(NSRect)dirtyRect
{
    
}

// our rendering function
-(void)drawView
{
    // make current context
    [[self openGLContext]makeCurrentContext];

    // lock context
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

    [self display];
    if(bAnimationEnable == true)
    {
        [self myupdate];
    }

    CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);

    // unlock context
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

}

-(CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    // code
    // Create auto release pool for memory management
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

    // call rendering function here too
    [self drawView];

    // Let auto release pool release all pending object in our application
    [pool release];

    return(kCVReturnSuccess);
}

-(BOOL) acceptsFirstResponder
{
    // code
    [[self window]makeFirstResponder:self];
    return(YES);
}

-(void)keyDown:(NSEvent *)event
{
    // code
    int key = (int)[[event characters] characterAtIndex: 0];

    switch(key)
    {
        case 27: // ESC for fullscreen
        [[self window]toggleFullScreen:self];
            break;

        case 'Q': // application will terminate 
		case 'q':
            [self release];
            [NSApp terminate:self];

		case 'L':
		case 'l':
			if (bLightingEnable == FALSE)
			{
				bLightingEnable = TRUE;
			}
			else
			{
				chooseShader = 'v';
				bLightingEnable = FALSE;
			}
			break;

		case 'F':
		case 'f':
			chooseShader = 'f';
			break;
		case 'V':
		case 'v':
			chooseShader = 'v';
			break;
		default:
			break;
    }
}

-(void)mouseDown:(NSEvent *)event
{
    // code
}

-(void)rightMouseDown:(NSEvent *)event
{
    // code
}

-(void)dealloc
{
    [super dealloc];
    if(displayLink)
    {
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
        displayLink = nil;
    }

}

@end

// defination of global callback function
CVReturn MyDisplayLinkCallback(
    CVDisplayLinkRef displayLink,
    const CVTimeStamp *now,
    const CVTimeStamp *outputTime,
    CVOptionFlags flagsIn,
    CVOptionFlags *flagsOut,
    void *displayLinkContext)
{
    // code
    CVReturn result = [(GLView*)displayLinkContext getFrameForTime:outputTime];
    return(result);
}
/*
clang++ -Wno-deprecated-declarations -c -o OGL.o OGL.mm

mkdir -p OGL.app/Contents/MacOS

clang++ -o OGL.app/Contents/MacOS/OGL OGL.o -framework Cocoa -framework QuartzCore -framework OpenGL

*/
