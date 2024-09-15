#import<OpenGLES/ES3/gl.h>
#import<OpenGLES/ES3/glext.h>
#import"GLESView.h"

#include"vmath.h"
using namespace vmath;

GLint shaderProgramObject =0;

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
};

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
            "precision highp float;"
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
    glClearDepthf(1.0f); // compulsory
    glEnable(GL_DEPTH_TEST);// compulsory
    glDepthFunc(GL_LEQUAL);// compulsory

    // step 7 : - set clear color of window to blue (here OpenGL Start)
    glClearColor(0.0f, 0.0f,  0.0f, 1.0f);
    
    // initialize perspective projection matrix
    perspectiveProjectionMatrix = vmath::mat4::identity();
    
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
   iKeyPressed = iKeyPressed + 1;
   if(iKeyPressed>4)
   {
    iKeyPressed = 0;
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
