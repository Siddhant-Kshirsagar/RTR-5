#import<Foundation/Foundation.h>
#import<Cocoa/Cocoa.h>
#import<QuartzCore/CVDisplayLink.h> // for thread 
#import<OpenGL/gl3.h>
#import<OpenGL/gl3ext.h>

#include"vmath.h"
using namespace vmath;

// Global function declaration
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef,const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags,CVOptionFlags*,void *);

//Macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global variable declaration
FILE *gpFILE = NULL;

enum
{
	AMC_ATTRIBUTE_POSITION = 0
};

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


    // Step 1: Declare rectangle for frame/border of window
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
    [window setTitle: @"SGK: Perspective White Triangle"];
    // // Give title to the window
    // [window setTitle: @"Tesselation Shader: numberOfLineSegment = %d",uiNumberOfLineSegment];

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
    GLuint vao;
    GLuint vbo_position;

    GLuint mvpMatrixUniform;

    GLuint numberOfSegmentUniform;
    GLuint numberOfStripUniform;
    GLuint lineColorUniform;

    GLuint uiNumberOfLineSegment;
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
    int height = rect.size.height;

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
		"in vec2 aPosition;" \
		"void main(void)" \
		"{" \
		"gl_Position= vec4(aPosition,0.0,1.0);" \
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
	}

    // Tesselation control shader
	// step 6 : write the Tesselation control shader code
	const GLchar *tesselationControlShaderCode =
		"#version 410 core" \
		"\n" \
		"layout(vertices=4) out;" \
		"uniform int uNumberOfSegment;" \
		"uniform int uNumberOfStrip;" \
		"void main(void)" \
		"{" \
		"gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;" \
		"gl_TessLevelOuter[0] = float(uNumberOfStrip);" \
		"gl_TessLevelOuter[1] = float(uNumberOfSegment);" \
		"}";

	// step 7 : create Tesselation control shader object
	GLuint tesselationControlShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);

	// step 8 : give Tesselation control shader source code to OpenGL
	glShaderSource(tesselationControlShaderObject, 1, (const GLchar **)&tesselationControlShaderCode, NULL);

	// step 9 : let OpenGL compile Tesselation control shader
	glCompileShader(tesselationControlShaderObject);

	// step 10 : 
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	// step 10 a:
	glGetShaderiv(tesselationControlShaderObject, GL_COMPILE_STATUS, &status);

	// step 10 b:
	if (status == GL_FALSE)
	{
		glGetShaderiv(tesselationControlShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 10 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));
			if (szInfoLog != NULL)
			{
				// step 10 d:
				glGetShaderInfoLog(tesselationControlShaderObject, infoLogLength + 1, NULL, szInfoLog);

				// step 10 e:
				fprintf(gpFILE, "tesselation control shader compilation log : %s\n", szInfoLog);

				// step 10 f:
				free(szInfoLog);

				szInfoLog = NULL;

			}

		}

		// step 10 g:
		[self uninitialize];
	}

	// Tesselation evaluation shader
	// step 6 : write the Tesselation evaluation shader code
	const GLchar *tesselationEvaluationShaderCode =
		"#version 410 core" \
		"\n" \
		"layout(isolines) in;" \
		"uniform mat4 uMVPMatrix;" \
		"void main(void)" \
		"{" \
		"vec3 p0 = gl_in[0].gl_Position.xyz;" \
		"vec3 p1 = gl_in[1].gl_Position.xyz;" \
		"vec3 p2 = gl_in[2].gl_Position.xyz;" \
		"vec3 p3 = gl_in[3].gl_Position.xyz;" \
		"vec3 p = p0 * (1 - gl_TessCoord.x) * (1 - gl_TessCoord.x) * (1 - gl_TessCoord.x) + p1 * 3.0 * gl_TessCoord.x * (1 - gl_TessCoord.x) * (1 - gl_TessCoord.x) + p2 * 3.0 * gl_TessCoord.x * gl_TessCoord.x * (1 - gl_TessCoord.x) + p3 * gl_TessCoord.x * gl_TessCoord.x * gl_TessCoord.x;" \
		"gl_Position = uMVPMatrix * vec4(p,1.0);" \
		"}";

	

	// step 7 : create Tesselation evaluation shader object
	GLuint tesselationEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);

	// step 8 : give Tesselation evaluation shader source code to OpenGL
	glShaderSource(tesselationEvaluationShaderObject, 1, (const GLchar **)&tesselationEvaluationShaderCode, NULL);

	// step 9 : let OpenGL compile Tesselation evaluation shader
	glCompileShader(tesselationEvaluationShaderObject);

	// step 10 : 
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	// step 10 a:
	glGetShaderiv(tesselationEvaluationShaderObject, GL_COMPILE_STATUS, &status);

	// step 10 b:
	if (status == GL_FALSE)
	{
		glGetShaderiv(tesselationEvaluationShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 10 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));
			if (szInfoLog != NULL)
			{
				// step 10 d:
				glGetShaderInfoLog(tesselationEvaluationShaderObject, infoLogLength + 1, NULL, szInfoLog);

				// step 10 e:
				fprintf(gpFILE, "tesselation evaluation shader compilation log : %s\n", szInfoLog);

				// step 10 f:
				free(szInfoLog);

				szInfoLog = NULL;

			}

		}

		// step 10 g:
		[self uninitialize];
	}

	// Fragment shader
	// step 6 : write the fragement shader code
	const GLchar *fragmentShaderCode =
		"#version 410 core" \
		"\n" \
        "uniform vec4 uLineColor;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = uLineColor;" \
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
	}

	// shader program
	// step 11 : create shader program
	shaderProgramObject = glCreateProgram();

	// step 12 : attach shader to this program
	glAttachShader(shaderProgramObject, vertexShaderObject);
    glAttachShader(shaderProgramObject, tesselationControlShaderObject);
	glAttachShader(shaderProgramObject, tesselationEvaluationShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// step 13 : bind attribute location with the shader program object
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

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
        
	}

	// get shader uniform location
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
    numberOfSegmentUniform = glGetUniformLocation(shaderProgramObject, "uNumberOfSegment");
	numberOfStripUniform = glGetUniformLocation(shaderProgramObject, "uNumberOfStrip");
	lineColorUniform = glGetUniformLocation(shaderProgramObject, "uLineColor");

	// step 16: declare position

	// position array inline initialization
	const GLfloat pointPosition[] =
	{
		-1.0f,-1.0f, // 3rd quadrant
		-0.5f, 1.0f, // 2nd quadrant
		0.5f,-1.0f, // 4th quadrant
		1.0f,1.0f // 1st quadrant
	};
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao);

	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_position);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(pointPosition), pointPosition, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

    // set depth
    //for enable depth
	glClearDepth(1.0f); // compulsory
	glEnable(GL_DEPTH_TEST);// compulsory
	glDepthFunc(GL_LEQUAL);// compulsory

    // step 7 : - set clear color of window to blue (here OpenGL Start)
	glClearColor(0.0f, 0.0f,  0.0f, 1.0f);

    // initialize orthographic projection matrix 
	perspectiveProjectionMatrix = vmath::mat4::identity();

    uiNumberOfLineSegment = 1; // for 1 line

	[self resize : WIN_WIDTH : WIN_HEIGHT];

    return(0);
}

-(void)printGLInfo
{
    // code
	fprintf(gpFILE, "OpenGL Vender :	%s\n", glGetString(GL_VENDOR));
	fprintf(gpFILE, "OpenGL Renderer (driver version) :	%s\n", glGetString(GL_RENDERER));
	fprintf(gpFILE, "OpenGL Version :	%s\n", glGetString(GL_VERSION));
	fprintf(gpFILE, "OpenGL GLSL(Graphic Library Shading Language) Version :	%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
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

	// Transformation
	mat4 modelViewMatrix = vmath::translate(0.0f,0.0f,-3.0f);

	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    // push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glUniform1i(numberOfSegmentUniform, uiNumberOfLineSegment);
	glUniform1i(numberOfStripUniform, 1);
    GLfloat lineColor[]= { 1.0f,0.0f,0.0f,0.0f};
	glUniform4fv(lineColorUniform, 1,lineColor);

    // // Give title to the window
    // char str[255];
    // sprintf(str,"SGK : numberOfLineSegment = %d",uiNumberOfLineSegment);
    // [window setTitle: str];

    // step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao);

	// tell opengl by how many vertices your 1 patch is created
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_PATCHES, 0, 4);

	// unbind vao 
	glBindVertexArray(0);

	glUseProgram(0);
}

-(void)myupdate
{
    //code
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

	// delete vbo for position
	if (vbo_position)
	{
		glDeleteBuffers(1, &vbo_position);
		vbo_position = 0;
	}

	// delete vao 
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
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
        [self release];
        [NSApp terminate:self];

        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self];
            break;
        
        case 'U':
        case 'u':
            uiNumberOfLineSegment++;
			if (uiNumberOfLineSegment >= 30)
			{
				uiNumberOfLineSegment = 30;
			}
			break;
        
        case 'D':
        case 'd':
			uiNumberOfLineSegment--;
			if (uiNumberOfLineSegment <= 1)
			{
				uiNumberOfLineSegment = 1;
			}
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
