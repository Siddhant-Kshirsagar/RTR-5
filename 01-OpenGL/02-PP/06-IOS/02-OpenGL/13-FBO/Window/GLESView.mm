#import<OpenGLES/ES3/gl.h>
#import<OpenGLES/ES3/glext.h>
#import"GLESView.h"

#include"Sphere.h"
#include"vmath.h"
using namespace vmath;

GLint shaderProgramObject =0;

#define FBO_WIDTH 512
#define FBO_HEIGHT 512

// enum
// {
//     AMC_ATTRIBUTE_POSITION = 0,
//     AMC_ATTRIBUTE_COLOR,
//     AMC_ATTRIBUTE_TEXCOORD,
// };

@implementation GLESView
{
    EAGLContext *eaglContext;
    
    GLuint customFrameBuffer;
    GLuint colorRenderBuffer;
    GLuint depthRenderBuffer;
    
    id displayLink;
    
    NSInteger framePerSeconds;
    
    BOOL isDisplayLink;
    
    // OpenGL related variable
    GLuint shaderProgramObject;
     GLuint vao_cube;
    GLuint vbo_positionCube;
    GLuint vbo_texCoordCube;
    
    GLuint mvpMatrixUniform_Cube;
    GLuint textureSampleUniform_Cube;

    GLuint textureSamplerUniform_Cube;

    GLfloat cAngle;

    GLuint shaderProgramObject_PV;
    GLuint shaderProgramObject_PF;

    Mesh sphere;

    GLuint modelMatrixUniform_PV;
    GLuint viewMatrixUniform_PV;
    GLuint projectionMatrixUniform_PV;

    GLuint lightAmbientUniform_PV[3];
    GLuint lightDiffuseUniform_PV[3];
    GLuint lightSpecularUniform_PV[3];
    GLuint lightPositionUniform_PV[3];

    GLuint materialDiffuseUniform_PV;
    GLuint materialAmbientUniform_PV;
    GLuint materialSpecularUniform_PV;
    GLuint materialShininessUniform_PV;

    GLuint keyPressedUniform_PV;

    GLuint modelMatrixUniform_PF;
    GLuint viewMatrixUniform_PF;
    GLuint projectionMatrixUniform_PF;

    GLuint lightAmbientUniform_PF[3];
    GLuint lightDiffuseUniform_PF[3];
    GLuint lightSpecularUniform_PF[3];
    GLuint lightPositionUniform_PF[3];

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

    struct LIGHT light[3];

    GLfloat materialAmbient[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;

    GLfloat lightAngleZero;
    GLfloat lightAngleOne;
    GLfloat lightAngleTwo;

    // mat4 is datatype means 4 * 4 matrix (present in vmath.h)
    mat4 perspectiveProjectionMatrix;

    // FBO(Frame Buffer Object) related variable
    GLint winWidth;
    GLint winHeight;

    GLuint FBO; // Frame Buffer Object
    GLuint RBO; // Render Buffer Object
    GLuint texture_FBO;
    BOOL bFBOResult;

    int iToggle;
}

-(id)initWithFrame:(CGRect)frame
{
    // code
    self = [super initWithFrame:frame];
    if(self)
    {
        // set the background to black
        [self setBackgroundColor:[UIColor blackColor]];
        
        // 1 : Create EAGL layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*)[super layer];
        
        // 2 : set properties of eaglLayer
        [eaglLayer setOpaque:YES];
        
        NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:
        [NSNumber numberWithBool:NO],kEAGLDrawablePropertyRetainedBacking,
        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,nil];
        
        [eaglLayer setDrawableProperties:dictionary];
        
        // 3 : create EAGLContext
        eaglContext = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES3];
        
        if(eaglContext == nil)
        {
            printf("OpenGLES context creation failed.\n");
            
            [self uninitialize_Cube];
            [self release];
            exit(0);
        }
        
        // 4 : set this context as current context
        [EAGLContext setCurrentContext:eaglContext];
        
        // create custom framebuffer
        glGenFramebuffers(1, &customFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, customFrameBuffer);
       
        // create color render buffer
        glGenRenderbuffers(1,&colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        // give storage to color render buffer by using ios method
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        
        // give above color buffer to custom framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
        // Depth Render buffer
        // find the width of the color buffer
        GLint width;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
        
        // find the height
        GLint height;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
        
//        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        
        // create and bind with depth render buffer
        glGenRenderbuffers(1, &depthRenderBuffer);
        
        // bind depth render buffer
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    
        // give storage to render buffer using opengl function
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
         
        // give this depth render buffer to custom frame buffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        
        // check status of the custom buffer
        GLenum frameBuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(frameBuffer_status != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("FrameBuffer creation failed.\n");
            printf("Status %u",frameBuffer_status);
            [self uninitialize_Cube];
            [self release];
            exit(0);
        }
        

//        glBindRenderbuffer(GL_RENDERBUFFER, 0);

//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // initialize_Cube framePerSeconds
        framePerSeconds = 60; // value 60 is recommended from ios 8.2
        
        // initialize_Cube isDisplayLink existance variable
        isDisplayLink = NO;
        
        // call our initialize_Cube method
        int result = [self initialize_Cube];
        if(result != 0)
        {
            printf("initialize_Cube() failed.\n");
            
            [self uninitialize_Cube];
            [self release];
            exit(0);
        }
        
      /*
        // create and bind with depth renderbuffer
        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, depthRenderBuffer);
        
        // give storage to depth render buffer
        */
        // Single Tap
        // 1 : create single tap gesture recognizer object
        UITapGestureRecognizer* singleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onSingleTap:)];
        
        // 2 : set number of tap
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        
        // 3 : set number of finger
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        
        // 4 : set the delegate as self
        [singleTapGestureRecognizer setDelegate:self];
        
        // 5 : add this gesture recognizer to the self
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        
        // Double Tap
        // same step as single tap
        // 1
        UITapGestureRecognizer* doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onDoubleTap:)];
        
        // 2
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        
        // 3
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        
        // 4
        [doubleTapGestureRecognizer setDelegate:self];
        
        // 5
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        // double should for single tap failed
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        // Swipe
        // 1 : create Object
        UISwipeGestureRecognizer* swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(onSwipe:)];
        
        // 2 : set delegate
        [swipeGestureRecognizer setDelegate:self];

        // 3 : add gesture recognizer
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        // Long press
        // 1 : create object
        UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(onLongPress:)];
        
        // 2 : set delegate
        [longPressGestureRecognizer setDelegate:self];

        // 3 : add gesture recognizer
        [self addGestureRecognizer:longPressGestureRecognizer];
        
        
    }
    return(self);
}

+(Class)layerClass
{
    // code
    return([CAEAGLLayer class]);
}

/*
-(void)drawRect:(CGRect)rect
{
    // code
    
}
 */

-(void)drawView:(id)displayLink

{
    // code
    // set current context again
    [EAGLContext setCurrentContext:eaglContext];
    
    // bind with custom framebuffer again
    glBindFramebuffer(GL_FRAMEBUFFER, customFrameBuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER_DEFAULT, FBO);

    
    // call our display function here
    [self display_Cube];
    
    [self update_Cube];
    
    // bind with colorRenderBuffer again
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    // present colorRenderBuffer with will internally do double buffering similar to swapbuffers() in windows
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
    
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
//
//    glBindFramebuffer(GL_FRAMEBUFFER_DEFAULT, 0);

//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

// override layout Subviews method
-(void)layoutSubviews
{
    // code

    glBindFramebuffer(GL_FRAMEBUFFER, customFrameBuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER_DEFAULT, FBO);


    // bind with colorRenderBuffer again
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    // create colorRenderBuffer storage  again
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)[self layer]];
    
    
    // find the width of the color buffer
    GLint width;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    
    // find the height
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    
    // create and bind with depth render buffer
    glGenRenderbuffers(1, &depthRenderBuffer);
    
    // bind depth render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    
    // give storage to depth render buffer by using opengl function
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
     
    // give this depth render buffer to custom render buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    
    // check status of the custom buffer
    GLenum frameBuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(frameBuffer_status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("FrameBuffer creation failed.\n");
        printf("framebuffer = %d",frameBuffer_status);
        
        [self uninitialize_Cube];
        [self release];
        exit(0);
    }
    
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
//
//    glBindFramebuffer(GL_FRAMEBUFFER_DEFAULT, 0);

//    glBindRenderbuffer(GL_FRAMEBUFFER, 0);

    
    // call our resize_Cube here
    [self resize_Cube:width :height];
    

    
    // it is recommended that call drawView
    [self drawView:displayLink];
    
}

// start display link custom method which will be call by AppDelegate
-(void)startDisplayLink
{
    // code
    if(isDisplayLink == NO) // is display link not present
    {
        // create display link
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        // set frame per seconds for this display link
        [displayLink setPreferredFramesPerSecond:framePerSeconds];
        // add this display to our run loop
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        // set isDisplayLink to YES
        isDisplayLink  = YES;
    }
}

// stop display link custom method which will be call by AppDelegate
-(void)stopDisplayLink
{
    // code
    if(isDisplayLink == YES)
    {
        // remove displayLink from run loop by invalidating it
        [displayLink invalidate];
        
        // set isDisplayLink to NO
        isDisplayLink = NO;
    }
}


-(int)initialize_Cube
{
    //code
    [self printGLInfo];

    // Vertex Shader
        // step 1 : vertex shader code
        const GLchar *vertexShaderSourceCode =
            "#version 300 core" \
            "\n" \
            "uniform mat4 uMVPMatrix;" \
            "in vec4 aPosition;" \
            "in vec2 aTexCoord;" \
            "out vec2 oTexCoord;" \
            "void main(void)" \
            "{" \
            "gl_Position= uMVPMatrix * aPosition;" \
            "oTexCoord = aTexCoord;" \
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
                    printf("Vertex shader compilation error log : %s\n", szInfoLog);

                    // step 5 f:
                    free(szInfoLog);

                    szInfoLog = NULL;

                }
            }


            // step 5 g:
            [self uninitialize_Cube];
            [self release];
            exit(0);
        }

        // Fragment shader
        // step 6 : write the fragement shader code
        const GLchar *fragmentShaderCode =
            "#version 300 core" \
            "\n" \
            "precision highp float;"
            "in vec2 oTexCoord;" \
            "uniform sampler2D uTextureSampler;" \
            "out vec4 FragColor;" \
            "void main(void)" \
            "{" \
            "FragColor = texture(uTextureSampler,oTexCoord);" \
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
                    printf("fragmenet shader compilation log : %s\n", szInfoLog);

                    // step 10 f:
                    free(szInfoLog);

                    szInfoLog = NULL;

                }

            }

            // step 10 g:
            [self uninitialize_Cube];
            [self release];
            exit(0);
        }

        // shader program
        // step 11 : create shader program
        shaderProgramObject = glCreateProgram();

        // step 12 : attach shader to this program
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // step 13 : bind attribute location with the shader program object
        glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

        glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

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
                    printf("Shader program linking error log : %s \n", szInfoLog);

                    // step 15 f:
                    free(szInfoLog);

                    szInfoLog = NULL;


                }
            }

            // step 15 e:
            [self uninitialize_Cube];
            [self release];
            exit(0);
            
        }

       // get shader uniform location
    mvpMatrixUniform_Cube = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");

    textureSamplerUniform_Cube = glGetUniformLocation(shaderProgramObject, "uTextureSampler");

    // step 16: declare position and color array

    const GLfloat cube_position[] =
    {
         // top
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        // bottom
        1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,

        // front
        1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        // back
        1.0f, 1.0f, -1.0f,
       -1.0f, 1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        // right
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,

        // left
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
    };

    const GLfloat cubeTexcoords[] =
    {
         // front
        1.0f, 1.0f, // top-right of front
        0.0f, 1.0f, // top-left of front
        0.0f, 0.0f, // bottom-left of front
        1.0f, 0.0f, // bottom-right of front

        // right
        1.0f, 1.0f, // top-right of right
        0.0f, 1.0f, // top-left of right
        0.0f, 0.0f, // bottom-left of right
        1.0f, 0.0f, // bottom-right of right

        // back
        1.0f, 1.0f, // top-right of back
        0.0f, 1.0f, // top-left of back
        0.0f, 0.0f, // bottom-left of back
        1.0f, 0.0f, // bottom-right of back

        // left
        1.0f, 1.0f, // top-right of left
        0.0f, 1.0f, // top-left of left
        0.0f, 0.0f, // bottom-left of left
        1.0f, 0.0f, // bottom-right of left

        // top
        1.0f, 1.0f, // top-right of top
        0.0f, 1.0f, // top-left of top
        0.0f, 0.0f, // bottom-left of top
        1.0f, 0.0f, // bottom-right of top

        // bottom
        1.0f, 1.0f, // top-right of bottom
        0.0f, 1.0f, // top-left of bottom
        0.0f, 0.0f, // bottom-left of bottom
        1.0f, 0.0f, // bottom-right of bottom
    };


    // for cube
    // step 17 : create VAO (vertex array object)
    glGenVertexArrays(1, &vao_cube);

    // step 18 : bind with VAO (vertex array object)
    glBindVertexArray(vao_cube);

    // step 19 : VBO(Vertex Buffer Object) for position
    glGenBuffers(1, &vbo_positionCube);

    // step 20 : bind with VBO( Vertex Buffer Object) for position
    glBindBuffer(GL_ARRAY_BUFFER, vbo_positionCube);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position), cube_position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VBO(Vertex Buffer Object) for texCoord

    glGenBuffers(1, &vbo_texCoordCube);

    // step 20 : bind with VBO( Vertex Buffer Object) for position
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoordCube);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoords), cubeTexcoords, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    
    // set depth
    //for enable depth
    glClearDepthf(1.0f); // compulsory
    glEnable(GL_DEPTH_TEST);// compulsory
    glDepthFunc(GL_LEQUAL);// compulsory

    // step 7 : - set clear color of window to blue (here OpenGL Start)
    glClearColor(0.0f, 0.0f,  0.0f, 1.0f);

    // initialize_Cube perspective projection matrix
    perspectiveProjectionMatrix = vmath::mat4::identity();

    if ([self createFBO:FBO_WIDTH :FBO_HEIGHT] == YES)
    {
        bFBOResult = [self initialize_Sphere];
    }
    
    return(0);
}

-(BOOL)initialize_Sphere
{
    //code
    // Per Vertex
    {
        // step 1 : vertex shader code
        const GLchar *vertexShaderSourceCode_PV =
            "#version 300 core" \
            "\n" \
            "in vec4 aPosition;" \
            "in vec3 aNormal;" \
            "uniform mat4 uModelMatrix;" \
            "uniform mat4 uViewMatrix;" \
            "uniform mat4 uProjectionMatrix;" \
            "uniform vec3 uLightAmbient[3];" \
            "uniform vec3 uLightDiffuse[3];" \
            "uniform vec3 uLightSpecular[3];" \
            "uniform vec4 uLightPosition[3];" \
            "uniform vec3 uMaterialAmbient;" \
            "uniform vec3 uMaterialDiffuse;" \
            "uniform vec3 uMaterialSpecular;" \
            "uniform float uMaterialShineness;" \
            "uniform highp int uKeyPressed;" \
            "out vec3 oPhongADSLight;" \
            "void main(void)" \
            "{" \
            "oPhongADSLight = vec3(0.0,0.0,0.0);" \
            "if(uKeyPressed == 1)" \
            "{" \
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" \
            "vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);" \
            "vec3 lightAmbient[3];" \
            "vec3 lightDirection[3];" \
            "vec3 lightDiffuse[3];" \
            "vec3 reflectionVector[3];" \
            "vec3 lightSpecular[3];" \
            "for(int i = 0; i<3; i++)" \
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
                    printf( "Vertex shader compilation error log : %s\n", szInfoLog);

                    // step 5 f:
                    free(szInfoLog);

                    szInfoLog = NULL;

                }
            }


            // step 5 g:
            [self uninitialize_Sphere];
        }

        // Fragment shader
        // step 6 : write the fragement shader code
        const GLchar *fragmentShaderCode_PV =
            "#version 300 core" \
            "\n" \
            "precision highp float;"
            "in vec3 oPhongADSLight;" \
            "uniform highp int uKeyPressed;" \
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
                printf( "fragmenet shader compilation log : %s\n", szInfoLog);

                // step 10 f:
                free(szInfoLog);

                szInfoLog = NULL;

            }

        }

        // step 10 g:
        [self uninitialize_Sphere];
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
                printf( "Shader program linking error log : %s \n", szInfoLog);

                // step 15 f:
                free(szInfoLog);

                szInfoLog = NULL;


            }
        }

        // step 15 e:
        [self uninitialize_Sphere];
        }

        // get shader uniform location

        modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uModelMatrix");
        viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uViewMatrix");
        projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "uProjectionMatrix");

        lightAmbientUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "uLightAmbient[0]");
        lightDiffuseUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[0]");
        lightSpecularUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[0]");
        lightPositionUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "uLightPosition[0]");


        lightAmbientUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "uLightAmbient[1]");
        lightDiffuseUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[1]");
        lightSpecularUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[1]");
        lightPositionUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "uLightPosition[1]");

        lightAmbientUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "uLightAmbient[2]");
        lightDiffuseUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[2]");
        lightSpecularUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[2]");
        lightPositionUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "uLightPosition[2]");

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
            "#version 300 core" \
            "\n" \
            "in vec4 aPosition;" \
            "in vec3 aNormal;" \
            "uniform mat4 uModelMatrix;" \
            "uniform mat4 uViewMatrix;" \
            "uniform mat4 uProjectionMatrix;" \
            "uniform vec4 uLightPosition[3];" \
            "uniform highp int uKeyPressed;" \
            "out vec3 oTransformedNormals;" \
            "out vec3 oLightDirection[3];" \
            "out vec3 oViewerVector;" \
            "void main(void)" \
            "{" \
            "if(uKeyPressed == 1)" \
            "{" \
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" \
            "oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;" \
            "oLightDirection[0] = vec3(uLightPosition[0]-eyeCoordinates);" \
            "oLightDirection[1] = vec3(uLightPosition[1]-eyeCoordinates);" \
            "oLightDirection[2] = vec3(uLightPosition[2]-eyeCoordinates);" \
            "oViewerVector = -eyeCoordinates.xyz;" \
            "}"\
            "else" \
            "{" \
            "oTransformedNormals = vec3(0.0,0.0,0.0);" \
            "oLightDirection[0] = vec3(0.0,0.0,0.0);" \
            "oLightDirection[1] = vec3(0.0,0.0,0.0);" \
            "oLightDirection[2] = vec3(0.0,0.0,0.0);" \
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
                    printf( "Vertex shader compilation error log : %s\n", szInfoLog);

                    // step 5 f:
                    free(szInfoLog);

                    szInfoLog = NULL;

                }
            }


            // step 5 g:
            [self uninitialize_Sphere];
        }

        // Fragment shader
        // step 6 : write the fragement shader code
        const GLchar *fragmentShaderCode_PF =
            "#version 300 core" \
            "\n" \
            "precision highp float;"
            "in vec3 oTransformedNormals;" \
            "in vec3 oLightDirection[3];" \
            "in vec3 oViewerVector;" \
            "uniform vec3 uLightAmbient[3];" \
            "uniform vec3 uLightDiffuse[3];" \
            "uniform vec3 uLightSpecular[3];" \
            "uniform vec3 uMaterialAmbient;" \
            "uniform vec3 uMaterialDiffuse;" \
            "uniform vec3 uMaterialSpecular;" \
            "uniform float uMaterialShineness;" \
            "uniform highp int uKeyPressed;" \
            "out vec4 FragColor;" \
            "void main(void)" \
            "{" \
            "vec3 phongADSLight = vec3(0.0,0.0,0.0);" \
            "vec3 normalizeLightDirection[3];" \
            "vec3 lightAmbient[3];" \
            "vec3 lightDirection[3];" \
            "vec3 lightDiffuse[3];" \
            "vec3 reflectionVector[3];" \
            "vec3 lightSpecular[3];" \
            "if(uKeyPressed == 1)" \
            "{" \
            "vec3 normalizeTransformedNormals = normalize(oTransformedNormals);" \
            "normalizeLightDirection[0] = normalize(oLightDirection[0]);" \
            "normalizeLightDirection[1] = normalize(oLightDirection[1]);" \
            "normalizeLightDirection[2] = normalize(oLightDirection[2]);" \
            "vec3 normalizeViewerVector = normalize(oViewerVector);" \
            "for(int i = 0; i<3; i++)" \
            "{" \
            "lightAmbient[i] = uLightAmbient[i] * uMaterialAmbient;" \
            "lightDiffuse[i] = uLightDiffuse[i] * uMaterialDiffuse * max(dot(normalizeLightDirection[i],normalizeTransformedNormals),0.0);" \
            "reflectionVector[i] = reflect(-normalizeLightDirection[i],normalizeTransformedNormals);" \
            "lightSpecular[i] = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],normalizeViewerVector),0.0),uMaterialShineness);" \
            "phongADSLight =  phongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];" \
            "}"\
            "FragColor = vec4(phongADSLight,1.0);" \
            "}" \
            "else" \
            "{" \
            "FragColor = vec4(1.0,1.0,1.0,1.0);" \
            "}" \
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
                    printf( "fragmenet shader compilation log : %s\n", szInfoLog);

                    // step 10 f:
                    free(szInfoLog);

                    szInfoLog = NULL;

                }

            }

            // step 10 g:
            [self uninitialize_Sphere];
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
                    printf( "Shader program linking error log : %s \n", szInfoLog);

                    // step 15 f:
                    free(szInfoLog);

                    szInfoLog = NULL;


                }
            }

            // step 15 e:
            [self uninitialize_Sphere];
        }

        // get shader uniform location

        modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uModelMatrix");
        viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uViewMatrix");
        projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");

        lightAmbientUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightAmbient[0]");
        lightDiffuseUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[0]");
        lightSpecularUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[0]");
        lightPositionUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightPosition[0]");

        lightAmbientUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "uLightAmbient[1]");
        lightDiffuseUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[1]");
        lightSpecularUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[1]");
        lightPositionUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "uLightPosition[1]");

        lightAmbientUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "uLightAmbient[2]");
        lightDiffuseUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[2]");
        lightSpecularUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[2]");
        lightPositionUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "uLightPosition[2]");

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
    glClearDepthf(1.0f); // compulsory
    glEnable(GL_DEPTH_TEST);// compulsory
    glDepthFunc(GL_LEQUAL);// compulsory

    // step 7 : - set clear color of window to blue (here OpenGL Start)
    glClearColor(0.0f, 0.0f,  0.0f, 1.0f);

    // initialize orthographic projection matrix
    perspectiveProjectionMatrix = vmath::mat4::identity();

    
    light[0].ambient = vec3(0.0f, 0.0f, 0.0f);
    light[1].ambient = vec3(0.0f, 0.0f, 0.0f);
    light[2].ambient = vec3(0.0f, 0.0f, 0.0f);

    light[0].diffuse = vec3(1.0, 0.0f, 0.0f);
    light[1].diffuse = vec3(0.0, 1.0f, 0.0f);
    light[2].diffuse = vec3(0.0, 0.0f, 1.0f);

    light[0].specular = vec3(1.0f, 0.0f, 0.0f);
    light[1].specular = vec3(0.0f, 1.0f, 0.0f);
    light[2].specular = vec3(0.0f, 0.0f, 1.0f);

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

    chooseShader = 'v';

    [self resize_Sphere : winWidth : winHeight];

    return(YES);
}

-(BOOL) createFBO:(GLint) textureWidth :(GLint) textureHeight
{
    //variable declaration
    GLint maxRenderBufferSize = 0;

    //code
    // step 1 : check capacity of render buffer
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);

    if (maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight)
    {
        printf( "Texture Size Overflow\n");
        return(NO);
    }

    // step 2 : create custom frame buffer
    glGenFramebuffers(1, &FBO);

    // bind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
    // step 3 : create the texture for FBO in which we are going to render sphere
    glGenTextures(1, &texture_FBO);

    glBindTexture(GL_TEXTURE_2D,texture_FBO);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

    // step 4 : attach above texture to frame buffer at default color attachment 0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_FBO, 0);

    // step 5 : now create render buffer to hold depth of custom FBO
    // generate render buffer
    glGenRenderbuffers(1, &RBO);

    // bind render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);

    // set storage of above render buffer of texture size for depth
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

    // step 6 : attach above depth related render buffer to FBO(Frame Buffer Object) as depth attachment
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // step 7 : check the frame buffer status whether successfull or not
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf( "Frame Buffer status is not complete\n");
        printf( "CreateFBO() error = %d\n",glCheckFramebufferStatus(GL_FRAMEBUFFER));

        return(NO);
    }
    
    // unbind render buffer
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    // step 8 : unbind frame buffer
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return(YES);
}

-(void)printGLInfo
{
    // code
    printf("OpenGL Vender :    %s\n", glGetString(GL_VENDOR));
    printf("OpenGL Renderer (driver version) :    %s\n", glGetString(GL_RENDERER));
    printf("OpenGL Version :    %s\n", glGetString(GL_VERSION));
    printf("OpenGL GLSL(Graphic Library Shading Language) Version :    %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("\n ------------------------------------------\n");
}


-(void)resize_Cube :(int)width :(int)height
{
    //code
    if (height <= 0)
    {
        height = 1;
    }

    winWidth = width;
    winHeight = height;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // set perspective projection matrix
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

-(void)resize_Sphere :(int)width :(int)height
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

-(void)display_Cube
{
   //code
   // render FBO(Frame Buffer Object) scene
    if (bFBOResult == YES)
    {
        [self display_Sphere:FBO_WIDTH : FBO_HEIGHT];

        [self myupdate_Sphere];
    }
    // call resize_Cube() again to compenset the change by display_Sphere
    [self resize_Cube:winWidth :winHeight];
    
    glBindFramebuffer(GL_FRAMEBUFFER, customFrameBuffer);


    // reset color to white to compenset the change by display_Sphere
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // step 1 : use shader program
    glUseProgram(shaderProgramObject);

    // cube
    // Transformation
    mat4 modelViewMatrix = mat4::identity();
    mat4 translationMatrix = mat4::identity();
    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    // scale matrix
    mat4 scaleMatrix = mat4::identity();
    scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

    mat4 rotationMatrix1 = mat4::identity();
    rotationMatrix1 = vmath::rotate(cAngle, 1.0f, 0.0f, 0.0f);

    mat4 rotationMatrix2 = mat4::identity();
    rotationMatrix2 = vmath::rotate(cAngle, 0.0f, 1.0f, 0.0f);

    mat4 rotationMatrix3 = mat4::identity();
    rotationMatrix3 = vmath::rotate(cAngle, 0.0f, 0.0f, 1.0f);

    mat4 rotationMatrix = mat4::identity();
    rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

    modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

    // order of multiplication is very important
    mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    // push above mvp(model view projection) into vertex shader's mvp uniform
    glUniformMatrix4fv(mvpMatrixUniform_Cube, 1, GL_FALSE, modelViewProjectionMatrix);

    //for texture
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, texture_FBO);

    glUniform1i(textureSamplerUniform_Cube, 0);

    // step 2 : bind with VAO(vertex array object)
    glBindVertexArray(vao_cube);

    // step 3 : draw geometry / shape / model /scene
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    // unbind vao
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}

-(void)display_Sphere:(GLint)textureWidth :(GLint)textureHeight
{
    //code
        // Bind with FBO
//    glBindFramebuffer(GL_FRAMEBUFFER, customFrameBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // call resize_Sphere() to change size of sphere side code
    [ self resize_Sphere:textureWidth: textureHeight];

    // set  glClearColor to black to compenset the change done by display Cube
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if (chooseShader == 'v')
    {
        // step 1 : use shader program
        glUseProgram(shaderProgramObject_PV);

        // sphere
        // Transformation
        mat4 translationMatrix = mat4::identity();
        translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

        mat4 modelMatrix = mat4::identity();
        modelMatrix = translationMatrix;

        mat4 viewMatrix = mat4::identity();

        // push above mvp(model view projection) into vertex shader's mvp uniform
        glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);

        glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);

        glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, perspectiveProjectionMatrix);

        if (bLightingEnable == TRUE)
        {
            glUniform1i(keyPressedUniform_PV, 1);

            glUniform3fv(lightAmbientUniform_PV[0], 1, light[0].ambient);
            glUniform3fv(lightDiffuseUniform_PV[0], 1, light[0].diffuse);
            glUniform3fv(lightSpecularUniform_PV[0], 1, light[0].specular);
            glUniform4fv(lightPositionUniform_PV[0], 1, light[0].position);

            glUniform3fv(lightAmbientUniform_PV[1], 1, light[1].ambient);
            glUniform3fv(lightDiffuseUniform_PV[1], 1, light[1].diffuse);
            glUniform3fv(lightSpecularUniform_PV[1], 1, light[1].specular);
            glUniform4fv(lightPositionUniform_PV[1], 1, light[1].position);

            glUniform3fv(lightAmbientUniform_PV[2], 1, light[2].ambient);
            glUniform3fv(lightDiffuseUniform_PV[2], 1, light[2].diffuse);
            glUniform3fv(lightSpecularUniform_PV[2], 1, light[2].specular);
            glUniform4fv(lightPositionUniform_PV[2], 1, light[2].position);

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

            glUniform3fv(lightAmbientUniform_PF[0], 1, light[0].ambient);
            glUniform3fv(lightDiffuseUniform_PF[0], 1, light[0].diffuse);
            glUniform3fv(lightSpecularUniform_PF[0], 1, light[0].specular);
            glUniform4fv(lightPositionUniform_PF[0], 1, light[0].position);

            glUniform3fv(lightAmbientUniform_PF[1], 1, light[1].ambient);
            glUniform3fv(lightDiffuseUniform_PF[1], 1, light[1].diffuse);
            glUniform3fv(lightSpecularUniform_PF[1], 1, light[1].specular);
            glUniform4fv(lightPositionUniform_PF[1], 1, light[1].position);

            glUniform3fv(lightAmbientUniform_PF[2], 1, light[2].ambient);
            glUniform3fv(lightDiffuseUniform_PF[2], 1, light[2].diffuse);
            glUniform3fv(lightSpecularUniform_PF[2], 1, light[2].specular);
            glUniform4fv(lightPositionUniform_PF[2], 1, light[2].position);

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
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

-(void)update_Cube
{
    //code
    //cube rotate
    cAngle = cAngle + 1.0f;
    if (cAngle >= 360.0f)
    {
        cAngle = cAngle - 360.0f;
    }

}

-(void)myupdate_Sphere
{
    //code

    if (bLightingEnable)
    {
        // animating light zero
        light[0].position[0] = 0.0f;
        light[0].position[1] = (GLfloat)5.0f * sin(lightAngleZero);
        light[0].position[2] = (GLfloat)5.0f * cos(lightAngleZero);
        light[0].position[3] = 1.0f;

    
        lightAngleZero = lightAngleZero + 0.05f;
        if (lightAngleZero >= 360.0f)
        {
            lightAngleZero = lightAngleZero - 360.0f;
        }

        // animating light one
        light[1].position[0] = (GLfloat)5.0f * sin(lightAngleOne);
        light[1].position[1] = 0.0f;
        light[1].position[2] = (GLfloat)5.0f * cos(lightAngleOne);
        light[1].position[3] = 1.0f;

    
        lightAngleOne = lightAngleOne + 0.05f;
        if (lightAngleOne >= 360.0f)
        {
            lightAngleOne = lightAngleOne - 360.0f;
        }

        // animating light two
        light[2].position[0] = (GLfloat)5.0f * sin(lightAngleTwo);
        light[2].position[1] = (GLfloat)5.0f * cos(lightAngleTwo);
        light[2].position[2] = 0.0f;
        light[2].position[3] = 1.0f;

        lightAngleTwo = lightAngleTwo + 0.05f;
        if (lightAngleTwo >= 360.0f)
        {
            lightAngleTwo = lightAngleTwo - 360.0f;
        }
    }


}

-(void)uninitialize_Cube
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

                        pShader[i] = NULL;
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
    if (vbo_texCoordCube)
    {
        glDeleteBuffers(1, &vbo_texCoordCube);
        vbo_texCoordCube = 0;
    }

    // delete vbo for position
    if (vbo_positionCube)
    {
        glDeleteBuffers(1, &vbo_positionCube);
        vbo_positionCube = 0;
    }

    // delete vao_cube
    if (vao_cube)
    {
        glDeleteVertexArrays(1, &vao_cube);
        vao_cube = 0;
    }

     // delete texture
    if (texture_FBO)
    {
        glDeleteTextures(1, &texture_FBO);
        texture_FBO = 0;
    }

    // delete depthRenderBuffer
    if(depthRenderBuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderBuffer);
        depthRenderBuffer = 0;
    }
    
    // delete colorRenderBuffer
    if(colorRenderBuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderBuffer);
        colorRenderBuffer  = 0;
    }
    
    // delete customFrameBuffer
    if(customFrameBuffer)
    {
        glDeleteFramebuffers(1, &customFrameBuffer);
        customFrameBuffer = 0;
    }
    
    // release EAGLContext
    if(eaglContext && [EAGLContext currentContext] == eaglContext)
    {
        [EAGLContext  setCurrentContext:nil];
        [eaglContext release];
        eaglContext = nil;
    }
    
}

-(void)uninitialize_Sphere
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


-(BOOL)becomeFirstResponder
{
    // code
    return(YES);
}

-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent *)event
{
    
}

-(void)onSingleTap:(UITapGestureRecognizer*) gestureRecognizer
{
    // code
   if (bLightingEnable == FALSE)
    {
        bLightingEnable = TRUE;
    }
    else
    {
        chooseShader = 'v';
        bLightingEnable = FALSE;
    }
    
    printf("Single Tap()\n");
}

-(void)onDoubleTap:(UITapGestureRecognizer*) gestureRecognizer
{
    // code
 
}

-(void)onSwipe:(UISwipeGestureRecognizer*) gestureRecognizer
{
    // code
    [self uninitialize_Cube];
    [self release];
    exit(0);
}

-(void)onLongPress:(UILongPressGestureRecognizer*) gestureRecognizer
{
    // code
   if(iToggle == 0)
   {
    chooseShader = 'v';
    iToggle = 1;
   }
   else
   {
    chooseShader = 'f';
    iToggle = 0;
   }
    
}

-(void)dealloc
{
    // code
    [super dealloc];
    
    //[self uninitialize_Cube];
    
    // release displayLink
    if(displayLink)
    {
        [displayLink invalidate];
        [displayLink stop];
        [displayLink release];
        displayLink = nil;
    }
}

@end
