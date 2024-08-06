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
    AMC_ATTRIBUTE_TEXCOORD,
    AMC_ATTRIBUTE_NORMAL
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


    // Step 1: Declare recpAngle for frame/border of window
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
    [window setTitle: @"SGK: White Pyramid"];

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

    // for cube
    GLuint vao_pyramid;
    GLuint vbo_positionPyramid;
    GLuint vbo_normal;

    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint lightAmbientUniform[2];
    GLuint lightDiffuseUniform[2];
    GLuint lightSpecularUniform[2];
    GLuint lightPositionUniform[2];

    GLuint materialDiffuseUniform;
    GLuint materialAmbientUniform;
    GLuint materialSpecularUniform;
    GLuint materialShininessUniform;

    GLuint keyPressedUniform;

    bool bLightingEnable;
    bool bAnimationEnable;

    struct LIGHT
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec4 position;
    };

    struct LIGHT light[2];

    GLfloat materialAmbient[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;

    // mat4 is datatype means 4 * 4 matrix (present in vmath.h)
    mat4 perspectiveProjectionMatrix;

    GLfloat pAngle;

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
        "in vec4 aPosition;" \
        "in vec3 aNormal;" \
        "uniform mat4 uModelMatrix;" \
        "uniform mat4 uViewMatrix;" \
        "uniform mat4 uProjectionMatrix;" \
        "uniform vec3 uLightAmbient[2];" \
        "uniform vec3 uLightDiffuse[2];" \
        "uniform vec3 uLightSpecular[2];" \
        "uniform vec4 uLightPosition[2];" \
        "uniform vec3 uMaterialAmbient;" \
        "uniform vec3 uMaterialDiffuse;" \
        "uniform vec3 uMaterialSpecular;" \
        "uniform float uMaterialShineness;" \
        "uniform int uKeyPressed;" \
        "out vec3 oPhongADSLight;" \
        "void main(void)" \
        "{" \
        "oPhongADSLight = vec3(0.0,0.0,0.0);" \
        "if(uKeyPressed == 1)" \
        "{" \
        "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" \
        "vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);" \
        "vec3 lightAmbient[2];" \
        "vec3 lightDirection[2];" \
        "vec3 lightDiffuse[2];" \
        "vec3 reflectionVector[2];" \
        "vec3 lightSpecular[2];" \
        "for(int i = 0; i<2; i++)" \
        "{" \
        "lightAmbient[i] = uLightAmbient[i] * uMaterialAmbient;" \
        "lightDirection[i] = normalize(vec3(uLightPosition[i]-eyeCoordinates));"
        "lightDiffuse[i] = uLightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i],transformedNormals),0.0);" \
        "reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);" \
        "vec3 viewerVector = normalize(-eyeCoordinates.xyz);" \
        "lightSpecular[i] = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],viewerVector),0.0),uMaterialShineness);" \
        "oPhongADSLight =  oPhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];" \
        "}"\
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
        
    }

    // get shader uniform location
    modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");

    viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");

    projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");

    lightAmbientUniform[0] = glGetUniformLocation(shaderProgramObject, "uLightAmbient[0]");
    lightDiffuseUniform[0] = glGetUniformLocation(shaderProgramObject, "uLightDiffuse[0]");
    lightSpecularUniform[0] = glGetUniformLocation(shaderProgramObject, "uLightSpecular[0]");
    lightPositionUniform[0] = glGetUniformLocation(shaderProgramObject, "uLightPosition[0]");


    lightAmbientUniform[1] = glGetUniformLocation(shaderProgramObject, "uLightAmbient[1]");
    lightDiffuseUniform[1] = glGetUniformLocation(shaderProgramObject, "uLightDiffuse[1]");
    lightSpecularUniform[1] = glGetUniformLocation(shaderProgramObject, "uLightSpecular[1]");
    lightPositionUniform[1] = glGetUniformLocation(shaderProgramObject, "uLightPosition[1]");

    materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");
    materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");
    materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");
    materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShineness");
    keyPressedUniform = glGetUniformLocation(shaderProgramObject, "uKeyPressed");

    // step 16: declare position

    // position array inline initialization
    const GLfloat pyramid_position[] =
    {
        // front
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        // right
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,

        // back
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // left
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f
    };

    GLfloat pyramid_Normals[] =
    {
        // front
        0.000000f, 0.447214f,  0.894427f, // front-top
        0.000000f, 0.447214f,  0.894427f, // front-left
        0.000000f, 0.447214f,  0.894427f, // front-right

        // right
        0.894427f, 0.447214f,  0.000000f, // right-top
        0.894427f, 0.447214f,  0.000000f, // right-left
        0.894427f, 0.447214f,  0.000000f, // right-right

        // back
        0.000000f, 0.447214f, -0.894427f, // back-top
        0.000000f, 0.447214f, -0.894427f, // back-left
        0.000000f, 0.447214f, -0.894427f, // back-right

        // left
       -0.894427f, 0.447214f,  0.000000f, // left-top
       -0.894427f, 0.447214f,  0.000000f, // left-left
       -0.894427f, 0.447214f,  0.000000f, // left-right
    };



    // for pyramid
    // step 17 : create VAO (vertex array object)
    glGenVertexArrays(1, &vao_pyramid);

    // step 18 : bind with VAO (vertex array object)
    glBindVertexArray(vao_pyramid);

    // step 19 : VBO(Vertex Buffer Object) for position
    glGenBuffers(1, &vbo_positionPyramid);

    // step 20 : bind with VBO( Vertex Buffer Object) for position
    glBindBuffer(GL_ARRAY_BUFFER, vbo_positionPyramid);

    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_position), pyramid_position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for normal
    glGenBuffers(1, &vbo_normal);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);

    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_Normals), pyramid_Normals, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

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

    light[0].ambient = vec3(0.0f, 0.0f, 0.0f);
    light[1].ambient = vec3(0.0f, 0.0f, 0.0f);
    light[0].diffuse = vec3(1.0, 0.0f, 0.0f);
    light[1].diffuse = vec3(0.0, 0.0f, 1.0f);
    light[0].specular = vec3(1.0f, 0.0f, 0.0f);
    light[1].specular = vec3(0.0f, 0.0f, 1.0f);
    light[0].position = vec4(-2.0f, 0.0f, 0.0f, 1.0f);
    light[1].position = vec4(2.0f, 0.0f, 0.0f, 1.0f);

    materialAmbient[0] = 0.0f;
    materialAmbient[1] = 0.0f;
    materialAmbient[2] = 0.0f;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 1.0f;
    materialDiffuse[1] = 1.0f;
    materialDiffuse[2] = 1.0f;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 1.0f;
    materialSpecular[1] = 1.0f;
    materialSpecular[2] = 1.0f;
    materialSpecular[3] = 1.0f;
    
    materialShininess = 128.0f;

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

    // pyramid

    // Transformation
    mat4 translationMatrix = mat4::identity();
    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    // rotation matrix
    mat4 rotationMatrix = mat4::identity();
    rotationMatrix = vmath::rotate(pAngle, 0.0f, 1.0f, 0.0f);

    mat4 modelMatrix = mat4::identity();
    modelMatrix = translationMatrix * rotationMatrix;

    mat4 viewMatrix = mat4::identity();

    // push above mvp(model view projection) into vertex shader's mvp uniform
    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    // step 2 : bind with VAO(vertex array object)
    glBindVertexArray(vao_pyramid);


    if (bLightingEnable == TRUE)
    {
        glUniform1i(keyPressedUniform, 1);

        glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
        glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
        glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
        glUniform4fv(lightPositionUniform[0], 1, light[0].position);

        glUniform3fv(lightAmbientUniform[1], 1, light[1].ambient);
        glUniform3fv(lightDiffuseUniform[1], 1, light[1].diffuse);
        glUniform3fv(lightSpecularUniform[1], 1, light[1].specular);
        glUniform4fv(lightPositionUniform[1], 1, light[1].position);

        glUniform3fv(materialAmbientUniform, 1, materialAmbient);
        glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
        glUniform3fv(materialSpecularUniform, 1, materialSpecular);
        glUniform1f(materialShininessUniform, materialShininess);
    }
    else
    {
        glUniform1i(keyPressedUniform, 0);
    }

    // step 3 : draw geometry / shape / model /scene
    glDrawArrays(GL_TRIANGLES, 0, 12);

    // unbind vao
    glBindVertexArray(0);

    glUseProgram(0);
}

-(void)myupdate
{
    //code
    //triangle rotate
    pAngle = pAngle + 1.0f;
    if (pAngle >= 360.0f)
    {
        pAngle = pAngle - 360.0f;
    }

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

    // pyramid
    // delete vbo for normal
    if (vbo_normal)
    {
        glDeleteBuffers(1, &vbo_normal);
        vbo_normal = 0;
    }

    // delete vbo for position
    if (vbo_positionPyramid)
    {
        glDeleteBuffers(1, &vbo_positionPyramid);
        vbo_positionPyramid = 0;
    }

    // delete vao
    if (vao_pyramid)
    {
        glDeleteVertexArrays(1, &vao_pyramid);
        vao_pyramid = 0;
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
    
    if(bAnimationEnable == true)
    {
        [self myupdate];
    }
    
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
            
        case 'L':
        case 'l':
            if(bLightingEnable == false)
            {
                bLightingEnable = true;
            }
            else
            {
                bLightingEnable = false;
            }
            break;
            
        case 'A':
        case 'a':
            if(bAnimationEnable == false)
            {
                bAnimationEnable = true;
            }
            else
            {
                bAnimationEnable = false;
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
