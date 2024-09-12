#import<OpenGLES/ES3/gl.h>
#import<OpenGLES/ES3/glext.h>
#import"GLESView.h"

#include"Sphere.h"
#include"vmath.h"
using namespace vmath;

GLint shaderProgramObject =0;

// enum
// {
//     AMC_ATTRIBUTE_POSITION = 0
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
    Mesh sphere;

    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint lightAmbientUniform;
    GLuint lightDiffuseUniform;
    GLuint lightSpecularUniform;
    GLuint lightPositionUniform;

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

    struct LIGHT light;

    GLfloat materialAmbient[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;

    GLfloat lightAngleZero;
    GLfloat lightAngleOne;
    GLfloat lightAngleTwo;

    GLuint currentWinWidth;
    GLuint currentWinHeight;

    GLchar rotationAxis;

    int iRotation;

    // mat4 is datatype means 4 * 4 matrix (present in vmath.h)
    mat4 perspectiveProjectionMatrix;

       
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
            
            [self uninitialize];
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
            [self uninitialize];
            [self release];
            exit(0);
        }
        
        // initialize framePerSeconds
        framePerSeconds = 60; // value 60 is recommended from ios 8.2
        
        // initialize isDisplayLink existance variable
        isDisplayLink = NO;
        
        // call our initialize method
        int result = [self initialize];
        if(result != 0)
        {
            printf("initialize() failed.\n");
            
            [self uninitialize];
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
    
    // call our display function here
    [self display];
    
    [self update];
    
    // bind with colorRenderBuffer again
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    // present colorRenderBuffer with will internally do double buffering similar to swapbuffers() in windows
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
    
}

// override layout Subviews method
-(void)layoutSubviews
{
    // code
    
    // bind with colorRenderBuffer again
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    
    // create colorRenderBuffer storage  again
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)[self layer]];
    
   // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
    
    // find the width of the color buffer
    GLint width;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    
    // find the height
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
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
        
        [self uninitialize];
        [self release];
        exit(0);
    }
    
    // call our resize here
    [self resize:width :height];
    
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


-(int)initialize
{
    //code
    [self printGLInfo];

    // Vertex Shader
        // step 1 : vertex shader code
        const GLchar *vertexShaderSourceCode =
            "#version 300 core" \
            "\n" \
           "in vec4 aPosition;" \
        "in vec3 aNormal;" \
        "uniform mat4 uModelMatrix;" \
        "uniform mat4 uViewMatrix;" \
        "uniform mat4 uProjectionMatrix;" \
        "uniform vec4 uLightPosition;" \
        "uniform highp int uKeyPressed;" \
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
            [self uninitialize];
            [self release];
            exit(0);
        }

        // Fragment shader
        // step 6 : write the fragement shader code
        const GLchar *fragmentShaderCode =
            "#version 300 core" \
            "\n" \
            "precision highp float;" \
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
        "uniform highp int uKeyPressed;" \
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
            [self uninitialize];
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
                    printf("Shader program linking error log : %s \n", szInfoLog);

                    // step 15 f:
                    free(szInfoLog);

                    szInfoLog = NULL;


                }
            }

            // step 15 e:
            [self uninitialize];
            [self release];
            exit(0);
            
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

    // step 16: declare position
    makeSphere(sphere, 2.0, 30, 30);

    
    // set depth
    //for enable depth
    glClearDepthf(1.0f); // compulsory
    glEnable(GL_DEPTH_TEST);// compulsory
    glDepthFunc(GL_LEQUAL);// compulsory

    // step 7 : - set clear color of window to blue (here OpenGL Start)
    glClearColor(0.0f, 0.0f,  0.0f, 1.0f);
    
    // initialize perspective projection matrix
    perspectiveProjectionMatrix = vmath::mat4::identity();

    light.ambient = vec3(0.1f,0.1f,0.1f);
    light.diffuse = vec3(1.0f,1.0f,1.0f);
    light.specular = vec3(1.0f,1.0f,1.0f);
    light.position = vec4(0.0f,0.0f,5.0f,1.0f);

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
    
    return(0);
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

-(void)resize :(int)width :(int)height
{
    //code
    if (height <= 0)
    {
        height = 1;
    }

    currentWinWidth = width;
    currentWinHeight = height;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // set perspective projection matrix
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

-(void)display
{
    // variable declaration

    GLfloat row1_Y, row2_Y, row3_Y, row4_Y, row5_Y, row6_Y;
    GLfloat col1_X, col2_X, col3_X, col4_X;

    GLfloat difference_X = currentWinWidth / 6; // for column
    GLfloat difference_Y = currentWinHeight / 6; // for rows

    col1_X = 0.0f;
    col2_X = col1_X + difference_X;
    col3_X = col2_X + difference_X;
    col4_X = col3_X + difference_X;

    row1_Y = currentWinHeight - difference_Y;
    row2_Y = row1_Y - difference_Y;
    row3_Y = row2_Y - difference_Y;
    row4_Y = row3_Y - difference_Y;
    row5_Y = row4_Y - difference_Y;
    row6_Y = row5_Y - difference_Y;

    //code
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // step 1 : use shader program
    glUseProgram(shaderProgramObject);

    // first column of Precious Stone
    // emerad
    // ambient material
    materialAmbient[0] = 0.0215f; // r
    materialAmbient[1] = 0.1745f; // g
    materialAmbient[2] = 0.0215f; // b
    materialAmbient[3] = 1.0f; // a

    // diffuse material
    materialDiffuse[0] = 0.07568f; // r
    materialDiffuse[1] = 0.61424f; // g
    materialDiffuse[2] = 0.07568f; // b
    materialDiffuse[3] = 1.0f; // a

    // specular material
    materialSpecular[0] = 0.0633f; // r
    materialSpecular[1] = 0.727811f; // g
    materialSpecular[2] = 0.633f; // b
    materialSpecular[3] = 1.0f; // a

    // shininess
    materialShininess = 0.6f * 128;

    glViewport(col1_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

    // 2nd sphere on 1st column, jade
    // ambient material
    materialAmbient[0] = 0.135f; // r
    materialAmbient[1] = 0.2225f; // g
    materialAmbient[2] = 0.1575f; // b
    materialAmbient[3] = 1.0f; // a

    // diffuse material
    materialDiffuse[0] = 0.54f; // r
    materialDiffuse[1] = 0.89f; // g
    materialDiffuse[2] = 0.63f; // b
    materialDiffuse[3] = 1.0f; // a

    // specular material
    materialSpecular[0] = 0.316228f; // r
    materialSpecular[1] = 0.316228f; // g
    materialSpecular[2] = 0.316228f; // b
    materialSpecular[3] = 1.0f; // a

    // shininess
    materialShininess = 0.1f * 128;

    glViewport(col1_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

    // 3rd sphere on 1st column, obsidian
    // ambient material
    materialAmbient[0] = 0.05375f; // r
    materialAmbient[1] = 0.05f; // g
    materialAmbient[2] = 0.06625f; // b
    materialAmbient[3] = 1.0f; // a

    // diffuse material
    materialDiffuse[0] = 0.18275f; // r
    materialDiffuse[1] = 0.17f; // g
    materialDiffuse[2] = 0.22525f; // b
    materialDiffuse[3] = 1.0f; // a

    // specular material
    materialSpecular[0] = 0.332741f; // r
    materialSpecular[1] = 0.328634f; // g
    materialSpecular[2] = 0.346435f; // b
    materialSpecular[3] = 1.0f; // a

    // shininess
    materialShininess = 0.3f * 128;


    glViewport(col1_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

    {
        // 4th sphere on 1st column, pearl
        // ambient material
        materialAmbient[0] = 0.25f; // r
        materialAmbient[1] = 0.20725f; // g
        materialAmbient[2] = 0.20725f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 1.0f; // r
        materialDiffuse[1] = 0.829f; // g
        materialDiffuse[2] = 0.829f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.296648f; // r
        materialSpecular[1] = 0.296648f; // g
        materialSpecular[2] = 0.296648f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.088f * 128;

        glViewport(col1_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 5th sphere on 1st column, ruby
        // ambient material
        materialAmbient[0] = 0.1745f; // r
        materialAmbient[1] = 0.01175f; // g
        materialAmbient[2] = 0.01175f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.61424f; // r
        materialDiffuse[1] = 0.04136f; // g
        materialDiffuse[2] = 0.04136f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.727811f; // r
        materialSpecular[1] = 0.626959f; // g
        materialSpecular[2] = 0.626959f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.6f * 128;

        glViewport(col1_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 6th shpere on 1st column, turquoise
        // ambient material
        materialAmbient[0] = 0.1f; // r
        materialAmbient[1] = 0.18725f; // g
        materialAmbient[2] = 0.1745f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.396f; // r
        materialDiffuse[1] = 0.74151f; // g
        materialDiffuse[2] = 0.69102f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.297254f; // r
        materialSpecular[1] = 0.30829f; // g
        materialSpecular[2] = 0.306678f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.1f * 128;

        glViewport(col1_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 2nd column metal
        // 1st sphere on 2nd column, brass
        // ambient material
        materialAmbient[0] = 0.329412f; // r
        materialAmbient[1] = 0.223529f; // g
        materialAmbient[2] = 0.027451f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.780392f; // r
        materialDiffuse[1] = 0.568627f; // g
        materialDiffuse[2] = 0.113725f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.992157f; // r
        materialSpecular[1] = 0.941176f; // g
        materialSpecular[2] = 0.807843f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.21794872f * 128;

        glViewport(col2_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 2nd sphere
        // ambient material
        materialAmbient[0] = 0.2125f; // r
        materialAmbient[1] = 0.1275f; // g
        materialAmbient[2] = 0.054f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.714f; // r
        materialDiffuse[1] = 0.4284f; // g
        materialDiffuse[2] = 0.18144f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.393548f; // r
        materialSpecular[1] = 0.271906f; // g
        materialSpecular[2] = 0.166721f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.2 * 128;

        glViewport(col2_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 3rd sphere on 2nd column, chrome
        // ambient material
        materialAmbient[0] = 0.25f; // r
        materialAmbient[1] = 0.25f; // g
        materialAmbient[2] = 0.25f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.4f; // r
        materialDiffuse[1] = 0.4f; // g
        materialDiffuse[2] = 0.4f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.774597f; // r
        materialSpecular[1] = 0.774597f; // g
        materialSpecular[2] = 0.774597f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.6f * 128;

        glViewport(col2_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 4th sphere on 2nd column, copper
        // ambient material
        materialAmbient[0] = 0.19125f; // r
        materialAmbient[1] = 0.0735f; // g
        materialAmbient[2] = 0.0225f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.7038f; // r
        materialDiffuse[1] = 0.27048f; // g
        materialDiffuse[2] = 0.0828f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.256777f; // r
        materialSpecular[1] = 0.137622f; // g
        materialSpecular[2] = 0.086014f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.1f * 128;

        glViewport(col2_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 5th sphere on 2nd column, gold
        // ambient material
        materialAmbient[0] = 0.24725f; // r
        materialAmbient[1] = 0.1995f; // g
        materialAmbient[2] = 0.0745f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.75164f; // r
        materialDiffuse[1] = 0.60648f; // g
        materialDiffuse[2] = 0.22648f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.628281f; // r
        materialSpecular[1] = 0.555802f; // g
        materialSpecular[2] = 0.366065f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.4f * 128;

        glViewport(col2_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 6th sphere on 2nd column, silver
            // ambient material
        materialAmbient[0] = 0.19225f; // r
        materialAmbient[1] = 0.19225f; // g
        materialAmbient[2] = 0.19225f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.50754f; // r
        materialDiffuse[1] = 0.50754f; // g
        materialDiffuse[2] = 0.50754f; // b
        materialDiffuse[3] = 1.0f; // a


        // specular material
        materialSpecular[0] = 0.508273f; // r
        materialSpecular[1] = 0.508273f; // g
        materialSpecular[2] = 0.508273f; // b
        materialSpecular[3] = 1.0f; // a


        // shininess
        materialShininess = 0.4f * 128;

        glViewport(col2_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 1st sphere on 3rd column, black
            // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.01f; // r
        materialDiffuse[1] = 0.01f; // g
        materialDiffuse[2] = 0.01f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.50f; // r
        materialSpecular[1] = 0.50f; // g
        materialSpecular[2] = 0.50f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.25f * 128;

        glViewport(col3_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 2nd sphere on 3rd column, cyan
            // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.1f; // g
        materialAmbient[2] = 0.06f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.0f; // r
        materialDiffuse[1] = 0.50980392f; // g
        materialDiffuse[2] = 0.50980392f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.50196078f; // r
        materialSpecular[1] = 0.50196078f; // g
        materialSpecular[2] = 0.50196078f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.25f * 128;

        glViewport(col3_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 3rd sphere on 2nd column, green
        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.1f; // r
        materialDiffuse[1] = 0.35f; // g
        materialDiffuse[2] = 0.1f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.45f; // r
        materialSpecular[1] = 0.55f; // g
        materialSpecular[2] = 0.45f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.25f * 128;

        glViewport(col3_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 4th sphere on 3rd column, red
        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.0f; // g
        materialDiffuse[2] = 0.0f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.7f; // r
        materialSpecular[1] = 0.6f; // g
        materialSpecular[2] = 0.6f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.25f * 128;

        glViewport(col3_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }
        // 5th sphere on 3rd column, white
        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.55f; // r
        materialDiffuse[1] = 0.55f; // g
        materialDiffuse[2] = 0.55f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.70f; // r
        materialSpecular[1] = 0.70f; // g
        materialSpecular[2] = 0.70f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.25f * 128;

        glViewport(col3_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 6th sphere on 3rd column, yellow
        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.0f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.60f; // r
        materialSpecular[1] = 0.60f; // g
        materialSpecular[2] = 0.50f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.25f * 128;

        glViewport(col3_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 1st sphere on 4th column, black
        // ambient material
        materialAmbient[0] = 0.02f; // r
        materialAmbient[1] = 0.02f; // g
        materialAmbient[2] = 0.02f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.1f; // r
        materialDiffuse[1] = 0.1f; // g
        materialDiffuse[2] = 0.1f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.4f; // r
        materialSpecular[1] = 0.4f; // g
        materialSpecular[2] = 0.4f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.078125f * 128;

        glViewport(col4_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }
        // 2nd sphere on 4th column, cyan
        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.05f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.4f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.5f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.04f; // r
        materialSpecular[1] = 0.7f; // g
        materialSpecular[2] = 0.7f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.078125f * 128;

        glViewport(col4_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }
        // 3rd sphere on 4th column, green
        // ambient material
        materialAmbient[0] = 0.0f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.4f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.4f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.04f; // r
        materialSpecular[1] = 0.7f; // g
        materialSpecular[2] = 0.04f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.078125f * 128;

        glViewport(col4_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }
        // 4th sphere on 4th column, red
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.0f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.4f; // g
        materialDiffuse[2] = 0.4f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.7f; // r
        materialSpecular[1] = 0.04f; // g
        materialSpecular[2] = 0.04f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.078125f * 128;

        glViewport(col4_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 5th sphere on 4th column, white
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.05f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.5f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.7f; // r
        materialSpecular[1] = 0.7f; // g
        materialSpecular[2] = 0.7f; // b
        materialSpecular[3] = 1.0f; // a


        // shininess
        materialShininess = 0.078125f * 128;


        glViewport(col4_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);


        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }

        // 6th sphere on 4th column, yellow
        // ambient material
        materialAmbient[0] = 0.05f; // r
        materialAmbient[1] = 0.05f; // g
        materialAmbient[2] = 0.0f; // b
        materialAmbient[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.4f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.7f; // r
        materialSpecular[1] = 0.7f; // g
        materialSpecular[2] = 0.04f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShininess = 0.078125f * 128;

        glViewport(col4_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

        {
            // Transformation
            mat4 translationMatrix = mat4::identity();
            translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

            mat4 modelMatrix = mat4::identity();
            modelMatrix = translationMatrix;

            mat4 viewMatrix = mat4::identity();

            // push above mvp(model view projection) into vertex shader's mvp uniform
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            if (bLightingEnable == TRUE)
            {
                glUniform1i(keyPressedUniform, 1);

                glUniform3fv(lightAmbientUniform, 1, light.ambient);
                glUniform3fv(lightDiffuseUniform, 1, light.diffuse);
                glUniform3fv(lightSpecularUniform, 1, light.specular);
                glUniform4fv(lightPositionUniform, 1, light.position);

                glUniform3fv(materialAmbientUniform, 1, materialAmbient);
                glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
                glUniform3fv(materialSpecularUniform, 1, materialSpecular);
                glUniform1f(materialShininessUniform, materialShininess);

            }
            else
            {
                glUniform1i(keyPressedUniform, 0);
            }

            sphere.draw();
        }
    }

    glUseProgram(0);
}

-(void)update
{
    //code

    if (bLightingEnable)
    {
        if (rotationAxis == 'x')
        {
            // x Axis
            light.position[0] = 0.0f;
            light.position[1] = (float)5.0f * sin(lightAngleZero);
            light.position[2] = (float)5.0f * cos(lightAngleZero);
            light.position[3] = 1.0f;


            lightAngleZero = lightAngleZero + 0.05f;
            if (lightAngleZero >= 360.0f)
            {
                lightAngleZero = lightAngleZero - 360.0f;
            }
        }
        

        if (rotationAxis == 'y')
        {
            light.position[0] = (float)5.0f * sin(lightAngleOne);
            light.position[1] = 0.0f;
            light.position[2] = (float)5.0f * cos(lightAngleOne);
            light.position[3] = 1.0f;


            lightAngleOne = lightAngleOne + 0.05f;
            if (lightAngleOne >= 360.0f)
            {
                lightAngleOne = lightAngleOne - 360.0f;
            }
        }

        if (rotationAxis == 'z')
        {
            light.position[0] = (float)5.0f * sin(lightAngleTwo);
            light.position[1] = (float)5.0f * cos(lightAngleTwo);
            light.position[2] = 0.0f;
            light.position[3] = 1.0f;

            lightAngleTwo = lightAngleTwo + 0.05f;
            if (lightAngleTwo >= 360.0f)
            {
                lightAngleTwo = lightAngleTwo - 360.0f;
            }
        }
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
    if(bLightingEnable == false)
    {
        bLightingEnable = true;
    }
    else
    {
        bLightingEnable = false;
    }
}

-(void)onDoubleTap:(UITapGestureRecognizer*) gestureRecognizer
{
    // code
 
}

-(void)onSwipe:(UISwipeGestureRecognizer*) gestureRecognizer
{
    // code
    [self uninitialize];
    [self release];
    exit(0);
}

-(void)onLongPress:(UILongPressGestureRecognizer*) gestureRecognizer
{
    // code
   if(iRotation == 0)
   {
    rotationAxis = 'x';
    iRotation = 1;
   }
   else if(iRotation == 1)
   {
    rotationAxis = 'y';
    iRotation = 2;
   }
   else if(iRotation == 2)
   {
    rotationAxis = 'z';
    iRotation = 0;
   }
    
}

-(void)dealloc
{
    // code
    [super dealloc];
    
    //[self uninitialize];
    
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
