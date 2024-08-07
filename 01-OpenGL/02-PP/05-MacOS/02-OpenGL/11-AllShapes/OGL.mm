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
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
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
    [window setTitle: @"SGK: Bluescreen"];

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

	// for line
	GLuint vao_line;
	GLuint vbo_positionLine;

	// for triangle
	GLuint vao_triangle;
	GLuint vbo_positionTriangle;

	// for square
	GLuint vao_square;
	GLuint vbo_positionSquare;

	// for circle
	GLuint vao_circle;
	GLuint vbo_positionCircle;

	GLuint mvpMatrixUniform;
	GLuint colorUniform;
	// mat4 is datatype means 4 * 4 matrix (present in vmath.h)
	mat4 perspectiveProjectionMatrix;

	// code
	int iKeyPressed;
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
    [self resize:WIN_WIDTH : WIN_HEIGHT];

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
		"uniform mat4 uMVPMatrix;" \
		"in vec4 aPosition;" \
		"void main(void)" \
		"{" \
		"gl_Position= uMVPMatrix * aPosition;" \
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
	}

	// shader program
	// step 11 : create shader program
	shaderProgramObject = glCreateProgram();

	// step 12 : attach shader to this program
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// step 13 : bind attribute location with the shader program object
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_COLOR, "aColor");

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

	colorUniform = glGetUniformLocation(shaderProgramObject, "uColor");

	// step 16: declare position and color array 

	// position array inline initialization
	const GLfloat triangle_position[] =
	{
		0.0f,1.0f,0.0f, // glVertex3f() 1 st call for triangle 
		-1.0f,-1.0f,0.0f, // glVertex3f() 2nd call for triangle
		1.0f,-1.0f,0.0f // glVertex3f() 3rd  call for triangle
	};

	const GLfloat square_position[] =
	{
		1.0f,1.0f,0.0f, // glVertex3f() 1st call for square
		-1.0f,1.0f,0.0f,// glVertex3f() 2nd call for square
		-1.0f,-1.0f,0.0f,// glVertex3f() 3rd call for square
		1.0f,-1.0f,0.0f// glVertex3f() 4th call for square
	};

	float circle_position[1080];
	int i = 0;

	for (int iAngle = 0; iAngle < 360; iAngle = iAngle + 1)
	{
		float x = 0.0f + 1.0f * cos((iAngle * M_PI) / 180.0f);
		float y = 0.0f + 1.0f * sin((iAngle * M_PI) / 180.0f);
		float z = 0.0f;

		circle_position[i] = x;
		i++;
		circle_position[i] = y;
		i++;
		circle_position[i] = z;
		i++;
	}

	//// for line
	const float line_position[] = {
		// horizontal line
		-2.0f,0.0f,0.0f,
		2.0f,0.0f,0.0f,

		// vertical line
		0.0f,2.0f,0.0f,
		0.0f,-2.0f,0.0f
	};

	// for Triangle
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao_triangle);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao_triangle);

	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_positionTriangle);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_positionTriangle);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_position), triangle_position, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// for square
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao_square);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao_square);

	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_positionSquare);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_positionSquare);

	glBufferData(GL_ARRAY_BUFFER, sizeof(square_position), square_position, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// for circle
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao_circle);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao_circle);

	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_positionCircle);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_positionCircle);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 1080, circle_position, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// for line
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao_line);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao_line);

	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_positionLine);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_positionLine);

	glBufferData(GL_ARRAY_BUFFER, sizeof(line_position), line_position, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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


	if (iKeyPressed == 1)
	{
		[self drawLine];
		[self drawTriangle];
	}
	else if (iKeyPressed == 2)
	{
		[self drawLine];
		[self drawSquare];
	}
	else if (iKeyPressed == 3)
	{
		[self drawLine];
		[self drawCircle];
	}
	else if (iKeyPressed == 4)
	{
		[self drawLine];
	}

	glUseProgram(0);
}

-(void)drawLine
{
// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Triangle
	// Transformation
	// variable for transformation
	mat4 modelViewMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();
	// variable for lineCount
	int lineCount = 0;

	// positive x axis (Vertical line)
	for (float xAxis = 0.05f; xAxis <= 2.0f; xAxis = xAxis + 0.05f)
	{
		lineCount = lineCount + 1;
		if (lineCount % 5 == 0)
		{
			glLineWidth(2.0f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(xAxis, 0.0f, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 0.0f, 1.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 2, 2);

		// unbind vao 
		glBindVertexArray(0);
	}
	lineCount = 0;

	// negative x axis (Vertical line)
	for (float xAxis = -0.05f; xAxis >= -2.0f; xAxis = xAxis - 0.05f)
	{
		lineCount = lineCount + 1;
		if (lineCount % 5 == 0)
		{
			glLineWidth(2.0f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(xAxis, 0.0f, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 0.0f, 1.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 2, 2);

		// unbind vao 
		glBindVertexArray(0);
	}
	lineCount = 0;

	// positive y axis (Horizontal line)
	for (float yAxis = 0.05f; yAxis <= 2.0f; yAxis = yAxis + 0.05f)
	{
		lineCount = lineCount + 1;
		if (lineCount % 5 == 0)
		{
			glLineWidth(2.0f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, yAxis, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 0.0f, 1.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 0, 2);

		// unbind vao 
		glBindVertexArray(0);
	}
	lineCount = 0;

	// positive y axis (Horizontal line)
	for (float yAxis = -0.05f; yAxis >= -2.0f; yAxis = yAxis - 0.05f)
	{
		lineCount = lineCount + 1;
		if (lineCount % 5 == 0)
		{
			glLineWidth(2.0f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, yAxis, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 0.0f, 1.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 0, 2);

		// unbind vao 
		glBindVertexArray(0);
	}
	lineCount = 0;

	// center horizontal line
	{
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 1.0f, 0.0f, 0.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 0, 2);

		// unbind vao 
		glBindVertexArray(0);
	}

	// center vertical line
	{
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 1.0f, 0.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 2, 2);

		// unbind vao 
		glBindVertexArray(0);
	}

	glUseProgram(0);
}

-(void)drawCircle
{
	// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Triangle
	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -4.9f);

	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao_circle);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_LINE_LOOP, 0, 360);

	// unbind vao 
	glBindVertexArray(0);

	glUseProgram(0);
}

-(void)drawSquare
{
	//code

// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Triangle
	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -4.9f);

	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao_square);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	// unbind vao 
	glBindVertexArray(0);

	glUseProgram(0);
}

-(void)drawTriangle
{
	// code
	
	// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Triangle
	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -4.9f);

	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao_triangle);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_LINE_LOOP, 0, 3);

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

				pShader = 0;
			}
		}


		glUseProgram(0);

		glDeleteProgram(shaderProgramObject);

		shaderProgramObject = 0;
	}

	// line 

	// delete vbo for position
	if (vbo_positionLine)
	{
		glDeleteBuffers(1, &vbo_positionLine);
		vbo_positionLine = 0;
	}

	// delete vao 
	if (vao_line)
	{
		glDeleteVertexArrays(1, &vao_line);
		vao_line = 0;
	}

	// circle 

	// delete vbo for position
	if (vbo_positionCircle)
	{
		glDeleteBuffers(1, &vbo_positionCircle);
		vbo_positionCircle = 0;
	}

	// delete vao 
	if (vao_circle)
	{
		glDeleteVertexArrays(1, &vao_circle);
		vao_circle = 0;
	}

	// square 

	// delete vbo for position
	if (vbo_positionSquare)
	{
		glDeleteBuffers(1, &vbo_positionSquare);
		vbo_positionSquare = 0;
	}

	// delete vao 
	if (vao_square)
	{
		glDeleteVertexArrays(1, &vao_square);
		vao_square = 0;
	}



	// triangle 

	// delete vbo for position
	if (vbo_positionTriangle)
	{
		glDeleteBuffers(1, &vbo_positionTriangle);
		vbo_positionTriangle = 0;
	}

	// delete vao 
	if (vao_triangle)
	{
		glDeleteVertexArrays(1, &vao_triangle);
		vao_triangle = 0;
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

		case 'T':
		case 't':
			iKeyPressed = 1;
			break;

		case 'S':
		case 's':
			iKeyPressed = 2;
			break;

		case 'C':
		case 'c':
			iKeyPressed = 3;
			break;

		case 'G':
		case 'g':
			iKeyPressed = 4;
			break;

		default:
			iKeyPressed = 0;
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