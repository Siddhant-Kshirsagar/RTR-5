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

    GLuint mvpMatrixUniform;
    GLuint modelViewMatrixUniform;
    GLuint projectionMatrixUniform;
    GLuint ldUniform; // for light diffuse
    GLuint kdUniform; // for material diffuse
    GLuint lightPositionUniform;
    GLuint keyPressedUniform;;

    // mat4 is datatype means 4 * 4 matrix (present in vmath.h)
    mat4 perspectiveProjectionMatrix;

    bool bLightingEnable;
    bool bAnimationEnable;

    GLfloat lightDiffuse[4];
    GLfloat materialDiffuse[4];
    GLfloat lightPosition[4];
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
            "uniform mat4 uModelViewMatrix;" \
            "uniform mat4 uProjectionMatrix;" \
            "uniform vec3 uld;" \
            "uniform vec3 ukd;" \
            "uniform vec4 uLightPosition;" \
            "uniform highp int uKeyPressed;" \
            "out vec3 oDiffuseLight;" \
            "void main(void)" \
            "{" \
            "if(uKeyPressed == 1)" \
            "{" \
            "vec4 eyePosition = uModelViewMatrix * aPosition;" \
            "mat3 normalMatrix = mat3(transpose(inverse(uModelViewMatrix)));" \
            "vec3 n = normalize(normalMatrix * aNormal);" \
            "vec3 s = normalize(vec3(uLightPosition-eyePosition));" \
            "oDiffuseLight = uld * ukd * dot(s,n);" \
            "}"\
            "else" \
            "{" \
            "oDiffuseLight = vec3(0.0,0.0,0.0);" \
            "}" \
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
            "in vec3 oDiffuseLight;" \
            "uniform highp int uKeyPressed;" \
            "out vec4 FragColor;" \
            "void main(void)" \
            "{" \
            "if(uKeyPressed == 1)" \
            "{" \
            "FragColor = vec4(oDiffuseLight,1.0);" \
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
        modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelViewMatrix");
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
        ldUniform = glGetUniformLocation(shaderProgramObject, "uld");
        kdUniform = glGetUniformLocation(shaderProgramObject, "ukd");
        lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");
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

    lightDiffuse[0] = 1.0f;
    lightDiffuse[1] = 1.0f;
    lightDiffuse[2] = 1.0f;
    lightDiffuse[3] = 1.0f;

    materialDiffuse[0] = 0.5f;
    materialDiffuse[1] = 0.5f;
    materialDiffuse[2] = 0.5f;
    materialDiffuse[3] = 1.0f;

    lightPosition[0] = 0.0f;
    lightPosition[1] = 0.0f;
    lightPosition[2] = 2.0f;
    lightPosition[1] = 0.0f;
    
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

    // Sphere
    mat4 modelViewMatrix = mat4::identity();

    // Transformation
    mat4 translationMatrix = mat4::identity();
    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    modelViewMatrix = translationMatrix;

    // push above mvp(model view projection) into vertex shader's mvp uniform
    glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLightingEnable == TRUE)
    {
        glUniform1i(keyPressedUniform, 1);
        glUniform3fv(ldUniform, 1, lightDiffuse);
        glUniform3fv(kdUniform, 1, materialDiffuse);
        glUniform4fv(lightPositionUniform, 1, lightPosition);
    }
    else
    {
        glUniform1i(keyPressedUniform, 0);
    }

    // step 2 : bind with VAO(vertex array object)
    sphere.draw();

    glUseProgram(0);
}

-(void)update
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
