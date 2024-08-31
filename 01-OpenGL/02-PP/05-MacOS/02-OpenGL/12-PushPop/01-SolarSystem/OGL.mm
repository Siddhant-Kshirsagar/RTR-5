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

#define MODEL_VIEW_MATRIX_STACK 32

// Global variable declaration
FILE *gpFILE = NULL;

/*
enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
    AMC_ATTRIBUTE_TEXCOOORD,
    AMC_ATTRIBUTE_NORMAL
};
*/
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
    [window setTitle: @"SGK: White Cube"];

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
    GLuint shaderProgramObject;

    Mesh sphere;

    GLuint modelViewMatrixUniform;
    GLuint projectionMatrixUniform;
    GLuint colorUniform; 

    // mat4 is datatype means 4 * 4 matrix (present in vmath.h)
    mat4 perspectiveProjectionMatrix;

    mat4 matrixStack[MODEL_VIEW_MATRIX_STACK];
    int matrixStackTop;

    int year;
    int day;
    int mDay;
    int mYear;
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

    // Vertex Shader
    // step 1 : vertex shader code
    const GLchar *vertexShaderSourceCode =
        "#version 410 core" \
        "\n" \
        "in vec4 aPosition;" \
		"in vec3 aNormal;" \
		"uniform mat4 uModelViewMatrix;" \
		"uniform mat4 uProjectionMatrix;" \
		"void main(void)" \
		"{" \
		"gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;" \
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
            szInfoLog = (GLchar *)malloc(sizeof(GLchar)*(infoLogLength + 1));

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
        [self uninitialize];
        [self release];
        [NSApp terminate:self];
    }

    // Fragment shader
    // step 6 : write the fragement shader code
    const GLchar *fragmentShaderCode =
        "#version 410 core" \
        "\n" \
        "uniform vec4 uColor;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = uColor;" \
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
        [self uninitialize];
        [self release];
        [NSApp terminate:self];
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
        [self uninitialize];
        [self release];
        [NSApp terminate:self];
        
    }

    // get shader uniform location
	modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelViewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
	colorUniform = glGetUniformLocation(shaderProgramObject, "uColor");

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

    matrixStackTop = -1;
    [self initializeMatrixStack];

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

    // step 1 : use shader program
    glUseProgram(shaderProgramObject);

    // sphere
	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();

	//mat4 lookAt = mat4::identity();
	//lookAt = vmath::lookat(vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	translationMatrix = vmath::translate(0.0f, 0.0f, -25.0f);

	//modelMatrix = translationMatrix;

	[self pushMatrix:modelMatrix];
	{
		modelMatrix = modelMatrix * translationMatrix;

		[self pushMatrix:modelMatrix];
		{
			scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(1.5f, 1.5f, 1.5f);

			modelMatrix = modelMatrix * scaleMatrix;

			modelViewMatrix = viewMatrix * modelMatrix;

			// draw sun
			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

			glUniform4f(colorUniform, 1.0f, 0.5f, 0.0f, 1.0f);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
            sphere.draw();
		}
		modelMatrix = [self popMatrix];

		// do transformation for earth
		rotationMatrix = vmath::rotate((GLfloat)year, 0.0f, 1.0f, 0.0f);  // year rotation

		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(6.5f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * rotationMatrix * translationMatrix;

		rotationMatrix = mat4::identity();
		rotationMatrix = vmath::rotate((GLfloat)day, 0.0f, 1.0f, 0.0f);  // year rotation

		modelMatrix = modelMatrix * rotationMatrix;
		[self pushMatrix:modelMatrix];
		{
			scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);

			modelMatrix = modelMatrix * scaleMatrix;

			modelViewMatrix = viewMatrix * modelMatrix;

			// draw earth
			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

			glUniform4f(colorUniform, 0.4f, 0.9f, 1.0f, 1.0f);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
            sphere.draw();
		}
		modelMatrix = [self popMatrix];

		// transformation of moon

		rotationMatrix = mat4::identity();
		rotationMatrix = vmath::rotate((GLfloat)mYear, 0.0f, 1.0f, 0.0f);

		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(2.5f, 0.0f, 0.0f);

		modelMatrix = modelMatrix * rotationMatrix * translationMatrix;

		rotationMatrix = mat4::identity();
		rotationMatrix = vmath::rotate((GLfloat)mDay, 0.0f, 1.0f, 0.0f);

		modelMatrix = modelMatrix * rotationMatrix;
		
		[self pushMatrix:modelMatrix];
		{
			scaleMatrix = mat4::identity();
			scaleMatrix = vmath::scale(0.25f,0.25f,0.25f);

			modelMatrix = modelMatrix * scaleMatrix;

			modelViewMatrix = viewMatrix * modelMatrix;

			// draw moon
			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

			glUniform4f(colorUniform, 1.0f, 1.0f, 1.0f, 1.0f);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
            sphere.draw();
		}
		modelMatrix = [self popMatrix];
	}
	modelMatrix = [self popMatrix];


    glUseProgram(0);
}

-(void)myupdate
{
    //code

}

-(void) initializeMatrixStack
{
	//code
	matrixStackTop = 0;
	for (int i = 0; i < MODEL_VIEW_MATRIX_STACK; i++)
	{
		matrixStack[i] = mat4::identity(); // initialize all members of array to identity matrix
	}
}

-(void) pushMatrix:(mat4) matrix
{

	// code
	if (matrixStackTop >= MODEL_VIEW_MATRIX_STACK)
	{
		fprintf(gpFILE, "Error - Execeeded matrix stack limit\n");
		[self uninitialize];
        [self release];
        [NSApp terminate:self];
	}

	matrixStack[matrixStackTop] = matrix; // save array in stack 
	matrixStackTop++; // increment stack pointer
}

-(mat4) popMatrix
{
	// code
	
	if (matrixStackTop < 0)
	{
		fprintf(gpFILE, "Error - Matrix stack is empty\n");
		[self uninitialize];
        [self release];
        [NSApp terminate:self];
	}

	matrixStack[matrixStackTop] = mat4::identity();
	matrixStackTop--;

	mat4 matrix = matrixStack[matrixStackTop];

	return(matrix);
}


-(void)uninitialize
{
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
    [self myupdate];

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
        case 27:
        [self uninitialize];
        [self release];
        [NSApp terminate:self];

        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self];
            break;
        case 'Y':
			year = (year + 3) % 360;
			break;
		case 'y':
			year = (year - 3) % 360;
			break;
		case 'D':
			day = (day + 3) % 360;
			break;
		case 'd':
			day = (day - 3) % 360;
			break;
		case 'M':
			mDay = (mDay + 3) % 360;
			break;
		case 'm':
			mDay = (mDay - 3) % 360;
			break;
		case 'N':
			mYear = (mYear + 3) % 360;
			break;
		case 'n':
			mYear = (mYear - 3) % 360;
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
