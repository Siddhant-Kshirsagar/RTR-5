package com.sgk.gles_fbo;

import android.content.Context;

// event related package
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;

// java io
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.Matrix;

// opengles related package
import android.opengl.GLSurfaceView;
import android.opengl.GLES32;
// import android.javax.microedition.khronos.GL10;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

// for texture
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

// for sphere
import java.nio.ShortBuffer;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,OnDoubleTapListener,OnGestureListener
{
    // global variable
    private GestureDetector gestureDetector;

    // OpenGL related variable for cube
    private int shaderProgramObject_Cube = 0;
    private int[] vao_cube = new int[1];
    private int[] vbo_positionCube = new int[1];
    private int[] vbo_texcoordCube = new int[1];

    private int textureSamplerUniform_Cube = 0;

    private int mvpMatrixUniform_Cube = 0;
    private float[] perspectiveProjectionMatrix_Cube = new float[16];

    private final Context context;

    float cAngle = 0.0f;

    // OpenGL related variable for sphere
    private int shaderProgramObject_PV = 0;
    private int shaderProgramObject_PF = 0;

    private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

    private int numVertices = 0;
    private int numElements = 0;

    private int modelMatrixUniform_PV = 0;
    private int viewMatrixUniform_PV = 0;
    private int projectionMatrixUniform_PV = 0;

    private int[] lightAmbientUniform_PV = new int[3];
    private int[] lightDiffuseUniform_PV = new int[3];
    private int[] lightSpecularUniform_PV = new int[3];
    private int[] lightPositionUniform_PV = new int[3];

   private int materialAmbientUniform_PV = 0;
    private int materialDiffuseUniform_PV = 0;
    private int materialSpecularUniform_PV = 0;
    private int materialShininessUniform_PV = 0;

    private int keyPressedUniform_PV = 0;

    private int modelMatrixUniform_PF = 0;
    private int viewMatrixUniform_PF = 0;
    private int projectionMatrixUniform_PF = 0;

    private int[] lightAmbientUniform_PF = new int[3];
    private int[] lightDiffuseUniform_PF = new int[3];
    private int[] lightSpecularUniform_PF = new int[3];
    private int[] lightPositionUniform_PF = new int[3];

   private int materialAmbientUniform_PF = 0;
    private int materialDiffuseUniform_PF = 0;
    private int materialSpecularUniform_PF = 0;
    private int materialShininessUniform_PF = 0;

    private int keyPressedUniform_PF = 0;

    private float[] materialAmbient_Sphere = {0.0f,0.0f,0.0f,1.0f};
    private float[] materialDiffuse_Sphere = {1.0f,1.0f,1.0f,1.0f};
    private float[] materialSpecular_Sphere = {1.0f,1.0f,1.0f,1.0f};
    private float materialShininess_Sphere = 50.0f;


    private float[] perspectiveProjectionMatrix_Sphere = new float
    [16];

    private boolean bLightingEnable = false;

    private class Light{
        float[] ambient = new float[3];
        float[] diffuse = new float[3];
        float[] specular = new float[3];
        float[] position = new float[4];
    }

    Light []light = new Light[3];

    float lightAngleZero_Sphere = 0.0f;
    float lightAngleOne_Sphere = 0.0f;
    float lightAngleTwo_Sphere = 0.0f;

    private int chooseShader = 0;

    private int winWidth = 0;
    private int winHeight = 0;

    private int[] FBO = new int[1]; // frame buffer object 
    private int[] RBO= new int[1]; // render buffer object 
    private int[] texture_FBO = new int[1];
    private Boolean bFBOResult = false;

    private final int FBO_WIDTH = 512;
    private final int FBO_HEIGHT = 512;


    public GLESView(Context _context)
    {
        super(_context);

        context = _context;

        // OpenGLES related
        setEGLContextClientVersion(3);
        setRenderer(this);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        // Event related
        // create and set GestureDectector object 
        gestureDetector = new GestureDetector(context,this,null,false);
        gestureDetector.setOnDoubleTapListener(this);

        
    }

    // implementation of 3 methods of GLSurfaceView interface
    @Override
    public void onSurfaceCreated(GL10 gl,EGLConfig config)
    {
        // code
        initialize_Cube(gl);
    }

    @Override
    public void onSurfaceChanged(GL10 gl,int width,int height)
    {
        // code
        resize_Cube(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        //code
        display_Cube();
    }


    // implementation of onTouchEvent of view class
    @Override
    public boolean onTouchEvent(MotionEvent e)
    {
        // code
        if(!gestureDetector.onTouchEvent(e))
        {
            super.onTouchEvent(e);
        }
        return (true);
    }


    // implementation of 3 methods of onDoubleTapListner interface

    @Override
    public boolean onDoubleTap(MotionEvent e)
    {
        // code
        if (bLightingEnable == false)
        {
            bLightingEnable = true;
        }
        else
        {
            bLightingEnable = false;
        }
        return(true);
    }

    @Override 
    public boolean onDoubleTapEvent(MotionEvent e)
    {
        // code
        return(true);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e)
    {
        //code
        return(true);
    }

    // implementation of 6 methods of onGestureListner interface

    @Override
    public boolean onDown(MotionEvent e)
    {
        //code
        return(true);
    }

    @Override
    public boolean onFling(MotionEvent e1,MotionEvent e2,float velocityX, float velocityY)
    {
        // code
        return(true);

    }

    @Override
    public void onLongPress(MotionEvent e)
    {
        // code
        chooseShader = chooseShader + 1;
        if(chooseShader >=2)
        {
            chooseShader = 0;
        }
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
    {
        // code
        uninitialize_Cube();

        System.exit(0);
        return(true);
    }

    @Override
    public void onShowPress(MotionEvent e)
    {
        // code
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e)
    {
        //code
        return(true);
    }

    // Implementation of private method
    private void initialize_Cube(GL10 gl)
    {
        // code 
        // print OpenGLES Info
        printGLInfo(gl);

        // Vertex Shader
        final String vertexShaderSourceCode = String.format
        (
            "#version 320 es"+
            "\n"+
            "uniform mat4 uMVPMatrix;"+
            "in vec4 aPosition;"+
            "in vec2 aTexCoord;"+
            "out vec2 oTexCoord;"+
            "void main(void)"+
            "{"+
            "gl_Position= uMVPMatrix * aPosition;"+
            "oTexCoord = aTexCoord;"+
            "}"
        );

        int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        GLES32.glShaderSource(vertexShaderObject,vertexShaderSourceCode);


        GLES32.glCompileShader(vertexShaderObject);

        int[] shaderCompileStatus = new int[1];
        int[] infoLogLength = new int[1];
        String infoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

        // Compile error check
        if(shaderCompileStatus[0] == 0)
        {
            GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
                if(infoLog!=null)
                {
                    System.out.println("SGK: Vertex shader compilation error log : "+infoLog);

                    uninitialize_Cube();
                    System.exit(0);
                }
            }
        }

        // Fragment shader
        final String fragmentShaderSourceCode = String.format(
            "#version 320 es"+
            "\n"+
            "precision highp float;"+
            "in vec2 oTexCoord;"+
            "uniform highp sampler2D uTextureSampler;"+
            "out vec4 FragColor;"+
            "void main(void)"+
            "{"+
            "FragColor = texture(uTextureSampler,oTexCoord);"+
            "}"
        );

        int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        GLES32.glShaderSource(fragmentShaderObject,fragmentShaderSourceCode);

        GLES32.glCompileShader(fragmentShaderObject);

        shaderCompileStatus[0] = 0;
        infoLogLength[0] = 0;
        infoLog = null;

        GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

        // compile error check
        if(shaderCompileStatus[0] == 0)
        {
            GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
                if(infoLog!=null)
                {
                    System.out.println("SGK: Fragment shader compilation error log : "+infoLog);

                    uninitialize_Cube();
                    System.exit(0);
                }
            }
        }

        // shader program
        shaderProgramObject_Cube = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject_Cube,vertexShaderObject);
        GLES32.glAttachShader(shaderProgramObject_Cube,fragmentShaderObject);

        GLES32.glBindAttribLocation(shaderProgramObject_Cube,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

        GLES32.glBindAttribLocation(shaderProgramObject_Cube,VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORD,"aTexCoord");

        GLES32.glLinkProgram(shaderProgramObject_Cube);

        int[] programLinkStatus = new int[1];
        infoLogLength[0] = 0;
        infoLog = null;

        GLES32.glGetProgramiv(shaderProgramObject_Cube,GLES32.GL_INFO_LOG_LENGTH,programLinkStatus,0);

        // linking error check
        if(programLinkStatus[0] == 0)
        {
            GLES32.glGetShaderiv(shaderProgramObject_Cube,GLES32.GL_LINK_STATUS,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(shaderProgramObject_Cube);
                if(infoLog!=null)
                {
                System.out.println("SGK: Shader program link error log : "+infoLog);

                uninitialize_Cube();
                System.exit(0);
                }
            }
        }

        // get shader uniform location
        mvpMatrixUniform_Cube = GLES32.glGetUniformLocation(shaderProgramObject_Cube,"uMVPMatrix");

        // geometry / shape
        final float[] cube_position = {
            // front
            1.0f,  1.0f,  1.0f, // top-right of front
            -1.0f,  1.0f,  1.0f, // top-left of front
            -1.0f, -1.0f,  1.0f, // bottom-left of front
            1.0f, -1.0f,  1.0f, // bottom-right of front
        
            // right
            1.0f,  1.0f, -1.0f, // top-right of right
            1.0f,  1.0f,  1.0f, // top-left of right
            1.0f, -1.0f,  1.0f, // bottom-left of right
            1.0f, -1.0f, -1.0f, // bottom-right of right
        
            // back
            1.0f,  1.0f, -1.0f, // top-right of back
            -1.0f,  1.0f, -1.0f, // top-left of back
            -1.0f, -1.0f, -1.0f, // bottom-left of back
            1.0f, -1.0f, -1.0f, // bottom-right of back
        
            // left
            -1.0f,  1.0f,  1.0f, // top-right of left
            -1.0f,  1.0f, -1.0f, // top-left of left
            -1.0f, -1.0f, -1.0f, // bottom-left of left
            -1.0f, -1.0f,  1.0f, // bottom-right of left
        
            // top
            1.0f,  1.0f, -1.0f, // top-right of top
            -1.0f,  1.0f, -1.0f, // top-left of top
            -1.0f,  1.0f,  1.0f, // bottom-left of top
            1.0f,  1.0f,  1.0f, // bottom-right of top
        
            // bottom
            1.0f, -1.0f,  1.0f, // top-right of bottom
            -1.0f, -1.0f,  1.0f, // top-left of bottom
            -1.0f, -1.0f, -1.0f, // bottom-left of bottom
            1.0f, -1.0f, -1.0f, // bottom-right of bottom
        };

        final float cube_texCoord[] =
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

        // vao_cube(vertex array object)
        GLES32.glGenVertexArrays(1,vao_cube,0);

        GLES32.glBindVertexArray(vao_cube[0]);

        GLES32.glGenBuffers(1,vbo_positionCube,0);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_positionCube[0]);

        // prepare vertices array for glBufferData
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cube_position.length * 4);

        byteBuffer.order(ByteOrder.nativeOrder());

        FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

        positionBuffer.put(cube_position);
        positionBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,cube_position.length * 4,positionBuffer,GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,3,GLES32.GL_FLOAT,false,0,0);

        GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

        // vbo for texcoord
        GLES32.glGenBuffers(1,vbo_texcoordCube,0);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_texcoordCube[0]);

        // prepare vertices array for glBufferData
        byteBuffer = ByteBuffer.allocateDirect(cube_texCoord.length * 4);

        byteBuffer.order(ByteOrder.nativeOrder());

        positionBuffer = byteBuffer.asFloatBuffer();

        positionBuffer.put(cube_texCoord);
        positionBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,cube_texCoord.length * 4,positionBuffer,GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORD,2,GLES32.GL_FLOAT,false,0,0);

        GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_TEXCOORD);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

        GLES32.glBindVertexArray(0);

        // for depth enable setting
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // clear color
        GLES32.glClearColor(1.0f,1.0f,1.0f,1.0f);

        if(createFBO(FBO_WIDTH, FBO_HEIGHT) == true)
        {
            bFBOResult = initialize_Sphere();
        }
       
        // enable texture
	    GLES32.glEnable(GLES32.GL_TEXTURE_2D);

        Matrix.setIdentityM(perspectiveProjectionMatrix_Cube,0);
    }

    
    // Implementation of private method
    private Boolean initialize_Sphere()
    {
        // per vertex light
        {
        // Vertex Shader
        final String vertexShaderSourceCode_PV = String.format
        (
            "#version 320 es"+
            "\n"+
            "in vec4 aPosition;"+
            "in vec3 aNormal;"+
            "uniform mat4 uModelMatrix;"+
            "uniform mat4 uViewMatrix;"+
            "uniform mat4 uProjectionMatrix;"+
            "uniform vec3 uLightAmbient[3];"+
            "uniform vec3 uLightDiffuse[3];"+
            "uniform vec3 uLightSpecular[3];"+
            "uniform vec4 uLightPosition[3];"+
            "uniform vec3 uMaterialAmbient;"+
            "uniform vec3 uMaterialDiffuse;"+
            "uniform vec3 uMaterialSpecular;"+
            "uniform float uMaterialShineness;"+
            "uniform int uKeyPressed;"+
            "out vec3 oPhongADSLight;"+
            "void main(void)"+
            "{"+
            "if(uKeyPressed == 1)"+
            "{"+
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
            "vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);"+
            "vec3 lightAmbient[3];"+
            "vec3 lightDirection[3];"+
            "vec3 lightDiffuse[3];"+
            "vec3 reflectionVector[3];"+
            "vec3 lightSpecular[3];"+
            "for(int i = 0; i<3; i++)"+
            "{"+
            "lightAmbient[i] = uLightAmbient[i] * uMaterialAmbient;"+
            "lightDirection[i] = normalize(vec3(uLightPosition[i]-eyeCoordinates));"+
            "lightDiffuse[i] = uLightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i],transformedNormals),0.0);"+
            "reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);"+
            "vec3 viewerVector = normalize(-eyeCoordinates.xyz);"+
            "lightSpecular[i] = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],viewerVector),0.0),uMaterialShineness);"+
            "oPhongADSLight =  oPhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];"+
            "}"+
            "}"+
            "else"+
            "{"+
            "oPhongADSLight = vec3(0.0,0.0,0.0);"+
            "}"+
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"+
            "}"
        );

        int vertexShaderObject_PV = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        GLES32.glShaderSource(vertexShaderObject_PV,vertexShaderSourceCode_PV);

        GLES32.glCompileShader(vertexShaderObject_PV);

        int[] shaderCompileStatus = new int[1];
        int[] infoLogLength = new int[1];
        String infoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject_PV,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

        // Compile error check
        if(shaderCompileStatus[0] == 0)
        {
            GLES32.glGetShaderiv(vertexShaderObject_PV,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(vertexShaderObject_PV);
                if(infoLog!=null)
                {
                    System.out.println("SGK: Vertex shader compilation error log : "+infoLog);

                    uninitialize_Sphere();
                    System.exit(0);
                }
            }
        }

        // Fragment shader
        final String fragmentShaderSourceCode_PV = String.format(
            "#version 320 es"+
            "\n"+
            "precision highp float;"+
            "in vec3 oPhongADSLight;"+
            "uniform highp int uKeyPressed;"+
            "out vec4 FragColor;"+
            "void main(void)"+
            "{"+
            "if(uKeyPressed == 1)"+
            "{"+
            "FragColor = vec4(oPhongADSLight,1.0);"+
            "}"+
            "else"+
            "{"+
            "FragColor = vec4(1.0,1.0,1.0,1.0);"+
            "}"+
            "}"
        );

        int fragmentShaderObject_PV = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        GLES32.glShaderSource(fragmentShaderObject_PV,fragmentShaderSourceCode_PV);

        GLES32.glCompileShader(fragmentShaderObject_PV);

        shaderCompileStatus[0] = 0;
        infoLogLength[0] = 0;
        infoLog = null;

        GLES32.glGetShaderiv(fragmentShaderObject_PV,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

        // compile error check
        if(shaderCompileStatus[0] == 0)
        {
            GLES32.glGetShaderiv(fragmentShaderObject_PV,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject_PV);
                if(infoLog!=null)
                {
                    System.out.println("SGK: Fragment shader compilation error log : "+infoLog);

                    uninitialize_Sphere();
                    System.exit(0);
                }
            }
        }

        // shader program
        shaderProgramObject_PV = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject_PV,vertexShaderObject_PV);
        GLES32.glAttachShader(shaderProgramObject_PV,fragmentShaderObject_PV);

        GLES32.glBindAttribLocation(shaderProgramObject_PV,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

        GLES32.glBindAttribLocation(shaderProgramObject_PV,VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL,"aNormal");

        GLES32.glLinkProgram(shaderProgramObject_PV);

        int[] programLinkStatus = new int[1];
        infoLogLength[0] = 0;
        infoLog = null;

        GLES32.glGetProgramiv(shaderProgramObject_PV,GLES32.GL_INFO_LOG_LENGTH,programLinkStatus,0);

        // linking error check
        if(programLinkStatus[0] == 0)
        {
            GLES32.glGetShaderiv(shaderProgramObject_PV,GLES32.GL_LINK_STATUS,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(shaderProgramObject_PV);
                if(infoLog!=null)
                {
                System.out.println("SGK: Shader program link error log : "+infoLog);

                uninitialize_Sphere();
                System.exit(0);
                }
            }
        }

        // get shader uniform location
        modelMatrixUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uModelMatrix");
        viewMatrixUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uViewMatrix");
        projectionMatrixUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uProjectionMatrix");

        lightAmbientUniform_PV[0] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightAmbient[0]");
        lightDiffuseUniform_PV[0] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[0]");
        lightSpecularUniform_PV[0] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[0]");
        lightPositionUniform_PV[0] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightPosition[0]");

        lightAmbientUniform_PV[1] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightAmbient[1]");
        lightDiffuseUniform_PV[1] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[1]");
        lightSpecularUniform_PV[1] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[1]");
        lightPositionUniform_PV[1] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightPosition[1]");
        
        lightAmbientUniform_PV[2] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightAmbient[2]");
        lightDiffuseUniform_PV[2] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightDiffuse[2]");
        lightSpecularUniform_PV[2] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightSpecular[2]");
        lightPositionUniform_PV[2] = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uLightPosition[2]");


        materialAmbientUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uMaterialAmbient");
        materialDiffuseUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uMaterialDiffuse");
        materialSpecularUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uMaterialSpecular");
        materialShininessUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uMaterialShineness");
        keyPressedUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "uKeyPressed");
    }
    
    // per fragment light
    {
        // Vertex Shader
        final String vertexShaderSourceCode_PF = String.format
        (
            "#version 320 es"+
            "\n"+
            "in vec4 aPosition;"+
            "in vec3 aNormal;"+
            "uniform mat4 uModelMatrix;"+
            "uniform mat4 uViewMatrix;"+
            "uniform mat4 uProjectionMatrix;"+
            "uniform vec4 uLightPosition[3];"+
            "uniform int uKeyPressed;"+
            "out vec3 oTransformedNormals;"+
            "out vec3 oLightDirection[3];"+
            "out vec3 oViewerVector;"+
            "void main(void)"+
            "{"+
            "if(uKeyPressed == 1)"+
            "{"+
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
            "oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;"+
            "oLightDirection[0] = vec3(uLightPosition[0]-eyeCoordinates);"+
            "oLightDirection[1] = vec3(uLightPosition[1]-eyeCoordinates);"+
            "oLightDirection[2] = vec3(uLightPosition[2]-eyeCoordinates);"+
            "oViewerVector = -eyeCoordinates.xyz;"+
            "}"+
            "else"+
            "{"+
            "oTransformedNormals = vec3(0.0,0.0,0.0);"+
            "oLightDirection[0] = vec3(0.0,0.0,0.0);"+
            "oLightDirection[1] = vec3(0.0,0.0,0.0);"+
            "oLightDirection[2] = vec3(0.0,0.0,0.0);"+
            "oViewerVector = vec3(0.0,0.0,0.0);"+
            "}"+
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"+
            "}"
        );

        int vertexShaderObject_PF = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        GLES32.glShaderSource(vertexShaderObject_PF,vertexShaderSourceCode_PF);

        GLES32.glCompileShader(vertexShaderObject_PF);

        int[] shaderCompileStatus = new int[1];
        int[] infoLogLength = new int[1];
        String infoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject_PF,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

        // Compile error check
        if(shaderCompileStatus[0] == 0)
        {
            GLES32.glGetShaderiv(vertexShaderObject_PF,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(vertexShaderObject_PF);
                if(infoLog!=null)
                {
                    System.out.println("SGK: Vertex shader compilation error log : "+infoLog);

                    uninitialize_Sphere();
                    System.exit(0);
                }
            }
        }

        // Fragment shader
        final String fragmentShaderSourceCode_PF = String.format(
            "#version 320 es"+
            "\n"+
            "precision highp float;"+
            "in vec3 oTransformedNormals;"+
            "in vec3 oLightDirection[3];"+
            "in vec3 oViewerVector;"+
            "uniform vec3 uLightAmbient[3];"+
            "uniform vec3 uLightDiffuse[3];"+
            "uniform vec3 uLightSpecular[3];"+
            "uniform vec3 uMaterialAmbient;"+
            "uniform vec3 uMaterialDiffuse;"+
            "uniform vec3 uMaterialSpecular;"+
            "uniform float uMaterialShineness;"+
            "uniform highp int uKeyPressed;"+
            "out vec4 FragColor;"+
            "void main(void)"+
            "{"+
            "vec3 phongADSLight;"+
            "vec3 normalizeLightDirection[3];"+
            "vec3 lightAmbient[3];"+
            "vec3 lightDirection[3];"+
            "vec3 lightDiffuse[3];"+
            "vec3 reflectionVector[3];"+
            "vec3 lightSpecular[3];"+
            "if(uKeyPressed == 1)"+
            "{"+
            "vec3 normalizeTransformedNormals = normalize(oTransformedNormals);"+
            "normalizeLightDirection[0] = normalize(oLightDirection[0]);"+
            "normalizeLightDirection[1] = normalize(oLightDirection[1]);"+
            "normalizeLightDirection[2] = normalize(oLightDirection[2]);"+
            "vec3 normalizeViewerVector = normalize(oViewerVector);"+
            "for(int i = 0; i<3; i++)"+
            "{"+
            "lightAmbient[i] = uLightAmbient[i] * uMaterialAmbient;"+
            "lightDiffuse[i] = uLightDiffuse[i] * uMaterialDiffuse * max(dot(normalizeLightDirection[i],normalizeTransformedNormals),0.0);"+
            "reflectionVector[i] = reflect(-normalizeLightDirection[i],normalizeTransformedNormals);"+
            "lightSpecular[i] = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],normalizeViewerVector),0.0),uMaterialShineness);"+
            "phongADSLight =  phongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];"+
            "}"+
            "FragColor = vec4(phongADSLight,1.0);"+
            "}"+
            "else"+
            "{"+
            "FragColor = vec4(1.0,1.0,1.0,1.0);"+
            "}"+
            "}"
        );

        int fragmentShaderObject_PF = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        GLES32.glShaderSource(fragmentShaderObject_PF,fragmentShaderSourceCode_PF);

        GLES32.glCompileShader(fragmentShaderObject_PF);

        shaderCompileStatus[0] = 0;
        infoLogLength[0] = 0;
        infoLog = null;

        GLES32.glGetShaderiv(fragmentShaderObject_PF,GLES32.GL_COMPILE_STATUS,shaderCompileStatus,0);

        // compile error check
        if(shaderCompileStatus[0] == 0)
        {
            GLES32.glGetShaderiv(fragmentShaderObject_PF,GLES32.GL_INFO_LOG_LENGTH,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject_PF);
                if(infoLog!=null)
                {
                    System.out.println("SGK: Fragment shader compilation error log : "+infoLog);

                    uninitialize_Sphere();
                    System.exit(0);
                }
            }
        }

        // shader program
        shaderProgramObject_PF = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject_PF,vertexShaderObject_PF);
        GLES32.glAttachShader(shaderProgramObject_PF,fragmentShaderObject_PF);

        GLES32.glBindAttribLocation(shaderProgramObject_PF,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

        GLES32.glBindAttribLocation(shaderProgramObject_PF,VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL,"aNormal");

        GLES32.glLinkProgram(shaderProgramObject_PF);

        int[] programLinkStatus = new int[1];
        infoLogLength[0] = 0;
        infoLog = null;

        GLES32.glGetProgramiv(shaderProgramObject_PF,GLES32.GL_INFO_LOG_LENGTH,programLinkStatus,0);

        // linking error check
        if(programLinkStatus[0] == 0)
        {
            GLES32.glGetShaderiv(shaderProgramObject_PF,GLES32.GL_LINK_STATUS,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(shaderProgramObject_PF);
                if(infoLog!=null)
                {
                System.out.println("SGK: Shader program link error log : "+infoLog);

                uninitialize_Sphere();
                System.exit(0);
                }
            }
        }

        // get shader uniform location
        modelMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uModelMatrix");
        viewMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uViewMatrix");
        projectionMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");

        lightAmbientUniform_PF[0] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightAmbient[0]");
        lightDiffuseUniform_PF[0] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[0]");
        lightSpecularUniform_PF[0] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[0]");
        lightPositionUniform_PF[0] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightPosition[0]");

        lightAmbientUniform_PF[1] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightAmbient[1]");
        lightDiffuseUniform_PF[1] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[1]");
        lightSpecularUniform_PF[1] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[1]");
        lightPositionUniform_PF[1] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightPosition[1]");
        
        lightAmbientUniform_PF[2] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightAmbient[2]");
        lightDiffuseUniform_PF[2] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[2]");
        lightSpecularUniform_PF[2] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[2]");
        lightPositionUniform_PF[2] = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uLightPosition[2]");


        materialAmbientUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uMaterialAmbient");
        materialDiffuseUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uMaterialDiffuse");
        materialSpecularUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uMaterialSpecular");
        materialShininessUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uMaterialShineness");
        keyPressedUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "uKeyPressed");
    }

        // geometry / shape
        Sphere sphere = new Sphere();
        float sphere_vertices[] = new float[1146];
        float sphere_normals[] = new float[1146];
        float sphere_textures[] = new float[764];
        short sphere_elements[] = new short[2280];

        sphere.getSphereVertexData(sphere_vertices,sphere_normals,sphere_textures,sphere_elements);

        numVertices = sphere.getNumberOfSphereVertices();
        numElements = sphere.getNumberOfSphereElements();

        // vao_sphere(vertex array object)
        GLES32.glGenVertexArrays(1,vao_sphere,0);

        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glGenBuffers(1,vbo_sphere_position,0);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_sphere_position[0]);

        // prepare vertices array for glBufferData
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sphere_vertices.length * 4);

        byteBuffer.order(ByteOrder.nativeOrder());

        FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();

        verticesBuffer.put(sphere_vertices);
        verticesBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,sphere_vertices.length * 4,verticesBuffer,GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,3,GLES32.GL_FLOAT,false,0,0);

        GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

       // normal vbo
       GLES32.glGenBuffers(1,vbo_sphere_normal,0);
       GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_sphere_normal[0]);
       
       byteBuffer=ByteBuffer.allocateDirect(sphere_normals.length * 4);
       byteBuffer.order(ByteOrder.nativeOrder());
       verticesBuffer = byteBuffer.asFloatBuffer();
       verticesBuffer.put(sphere_normals);
       verticesBuffer.position(0);
       
       GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
        sphere_normals.length * 4,
        verticesBuffer,
        GLES32.GL_STATIC_DRAW);
       
       GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL,
        3,
        GLES32.GL_FLOAT,
        false,0,0);
       
       GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL);
       
       GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
       
       // element vbo
       GLES32.glGenBuffers(1,vbo_sphere_element,0);
       GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER,vbo_sphere_element[0]);
       
       byteBuffer=ByteBuffer.allocateDirect(sphere_elements.length * 2);
       byteBuffer.order(ByteOrder.nativeOrder());
       ShortBuffer elementsBuffer=byteBuffer.asShortBuffer();
       elementsBuffer.put(sphere_elements);
       elementsBuffer.position(0);
       
       GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER,
        sphere_elements.length * 2,
        elementsBuffer,
        GLES32.GL_STATIC_DRAW);
       
       GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER,0);


        GLES32.glBindVertexArray(0);

        // for depth enable setting
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // Enable culling 
        GLES32.glEnable(GLES32.GL_CULL_FACE);

        // clear color
        GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

        Matrix.setIdentityM(perspectiveProjectionMatrix_Sphere,0);

        light[0] = new Light();
        light[1] = new Light();
        light[2] = new Light();

        light[0].ambient[0] = 0.0f;
        light[0].ambient[1] = 0.0f;
        light[0].ambient[2] = 0.0f;

        light[1].ambient[0] = 0.0f;
        light[1].ambient[1] = 0.0f;
        light[1].ambient[2] = 0.0f;

        light[2].ambient[0] = 0.0f;
        light[2].ambient[1] = 0.0f;
        light[2].ambient[2] = 0.0f;

        light[0].diffuse[0] = 1.0f;
        light[0].diffuse[1] = 0.0f;
        light[0].diffuse[2] = 0.0f;

        light[1].diffuse[0] = 0.0f;
        light[1].diffuse[1] = 1.0f;
        light[1].diffuse[2] = 0.0f;

        light[2].diffuse[0] = 0.0f;
        light[2].diffuse[1] = 0.0f;
        light[2].diffuse[2] = 1.0f;


        light[0].specular[0] = 1.0f;
        light[0].specular[1] = 0.0f;
        light[0].specular[2] = 0.0f;

        light[1].specular[0] = 0.0f;
        light[1].specular[1] = 1.0f;
        light[1].specular[2] = 0.0f;

        light[2].specular[0] = 0.0f;
        light[2].specular[1] = 0.0f;
        light[2].specular[2] = 1.0f;

        light[0].position[0] = -2.0f;
        light[0].position[1] = 0.0f;
        light[0].position[2] = 0.0f;
        light[0].position[3] = 1.0f;

        light[1].position[0] = 2.0f;
        light[1].position[1] = 0.0f;
        light[1].position[2] = 0.0f;
        light[1].position[3] = 1.0f;

        light[2].position[0] = 0.0f;
        light[2].position[1] = 2.0f;
        light[2].position[2] = 0.0f;
        light[2].position[3] = 1.0f;

        resize_Sphere(winWidth, winHeight);

        return(true);

    }

    Boolean createFBO(int textureWidth, int textureHeight)
    {
        // variable declaration
        int[] maxRenderBufferSize = new int[1];

        //code
        // step 1 : check capacity of render buffer
        GLES32.glGetIntegerv(GLES32.GL_MAX_RENDERBUFFER_SIZE, maxRenderBufferSize,0);

        if (maxRenderBufferSize[0] < textureWidth || maxRenderBufferSize[0] < textureHeight)
        {
            System.out.println("SGK: Texture Size Overflow\n");
            return(false);
        }

        // step 2 : create custom frame buffer
        GLES32.glGenFramebuffers(1, FBO,0);

        // bind frame buffer
        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, FBO[0]);
        
        // step 3 : create the texture for FBO in which we are going to render sphere
        GLES32.glGenTextures(1, texture_FBO,0);

        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D,texture_FBO[0]);

        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_S, GLES32.GL_CLAMP_TO_EDGE);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_T, GLES32.GL_CLAMP_TO_EDGE);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR);

        GLES32.glTexImage2D(GLES32.GL_TEXTURE_2D, 0, GLES32.GL_RGB, textureWidth, textureHeight, 0, GLES32.GL_RGB, GLES32.GL_UNSIGNED_SHORT_5_6_5, null);

        // step 4 : attach above texture to frame buffer at default color attachment 0
        GLES32.glFramebufferTexture2D(GLES32.GL_FRAMEBUFFER, GLES32.GL_COLOR_ATTACHMENT0, GLES32.GL_TEXTURE_2D, texture_FBO[0], 0);

        // step 5 : now create render buffer to hold depth of custom FBOGLES32.
        // generate render buffer
        GLES32.glGenRenderbuffers(1, RBO,0);

        // bind render buffer
        GLES32.glBindRenderbuffer(GLES32.GL_RENDERBUFFER, RBO[0]);

        // set storage of above render buffer of texture size for depth
        GLES32.glRenderbufferStorage(GLES32.GL_RENDERBUFFER, GLES32.GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

        // step 6 : attach above depth related render buffer to FBO(Frame Buffer Object) as depth attachment
        GLES32.glFramebufferRenderbuffer(GLES32.GL_FRAMEBUFFER, GLES32.GL_DEPTH_ATTACHMENT, GLES32.GL_RENDERBUFFER, RBO[0]);

        // step 7 : check the frame buffer status whether successfull or not 
        if (GLES32.glCheckFramebufferStatus(GLES32.GL_FRAMEBUFFER) != GLES32.GL_FRAMEBUFFER_COMPLETE)
        {
            System.out.println("Frame Buffer status is not complete\n");
            return(false);
        }

        // step 8 : unbind frame buffer
        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0);

        return(true);
        
    }

    private void printGLInfo(GL10 gl)
    {
        // code
        System.out.println("SGK: OpenGL-ES Renderer : "+ gl.glGetString(GL10.GL_RENDERER));
        System.out.println("SGK: OpenGL-ES Version : "+ gl.glGetString(GL10.GL_VERSION));
        System.out.println("SGK: OpenGL-ES Shading Language Version : "+ gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION));
    }

    private void resize_Cube(int width, int height)
    {
        // code
        if(height<=0)
        {
            height = 1;
        }

        winWidth = width;
        winHeight = height;

        GLES32.glViewport(0,0,width,height);

        // set perspective projection matrix
        Matrix.perspectiveM(perspectiveProjectionMatrix_Cube,0,45.0f,(float)width/(float)height,0.1f,100.0f);
    }

    private void resize_Sphere(int width, int height)
    {
        // code
        if(height<=0)
        {
            height = 1;
        }

        GLES32.glViewport(0,0,width,height);

        // set perspective projection matrix
        Matrix.perspectiveM(perspectiveProjectionMatrix_Sphere,0,45.0f,(float)width/(float)height,0.1f,100.0f);
    }

    private void display_Cube()
    {
        // render FBO(Frame Buffer Object) scene
        if(bFBOResult == true)
        {
            display_Sphere(FBO_WIDTH, FBO_HEIGHT);

            update_Sphere();
        }

        resize_Cube(winWidth, winHeight);

        GLES32.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // code
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT|GLES32.GL_DEPTH_BUFFER_BIT);

        GLES32.glUseProgram(shaderProgramObject_Cube);

        // transformation
        float[] modelViewMatrix = new float[16];
        float[] translationMatrix = new float[16];
        float[] rotationMatrix = new float[16];
        float[] rotationMatrix_X = new float[16];
        float[] rotationMatrix_Y = new float[16];
        float[] rotationMatrix_Z = new float[16];
        float[] modelViewProjectionMatrix = new float[16];

        Matrix.setIdentityM(modelViewMatrix,0);
        Matrix.setIdentityM(translationMatrix,0);
        Matrix.setIdentityM(rotationMatrix,0);
        Matrix.setIdentityM(rotationMatrix_X,0);
        Matrix.setIdentityM(rotationMatrix_Y,0);
        Matrix.setIdentityM(rotationMatrix_Z,0);
        Matrix.setIdentityM(modelViewProjectionMatrix,0);

        Matrix.translateM(translationMatrix,0,0.0f,0.0f,-5.0f);
        Matrix.rotateM(rotationMatrix_X,0,cAngle,1.0f,0.0f,0.0f);
        Matrix.rotateM(rotationMatrix_Y,0,cAngle,0.0f,1.0f,0.0f);
        Matrix.rotateM(rotationMatrix_Z,0,cAngle,0.0f,0.0f,1.0f);

        Matrix.multiplyMM(rotationMatrix,0,rotationMatrix_X,0,rotationMatrix_Y,0);

        Matrix.multiplyMM(rotationMatrix,0,rotationMatrix,0,rotationMatrix_Z,0);
        
        Matrix.multiplyMM(modelViewMatrix,0,translationMatrix,0,rotationMatrix,0);

        Matrix.multiplyMM(modelViewProjectionMatrix,0,perspectiveProjectionMatrix_Cube,0,modelViewMatrix,0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform_Cube,1,false,modelViewProjectionMatrix,0);

        //for texture
        GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
        
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_FBO[0]);

        GLES32.glUniform1i(textureSamplerUniform_Cube, 0);

        // bind with vao_cube
        GLES32.glBindVertexArray(vao_cube[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,0,4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,4,4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,8,4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,12,4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,16,4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,20,4);

        // unbind vao_cube
        GLES32.glBindVertexArray(0);

        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

        GLES32.glUseProgram(0);

        // update for animation
        update_Cube();

        // render
        requestRender();
    }

    
    private void display_Sphere(int textureWidth, int textureHeight)
    {
        // code
      	// Bind with FBO
        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, FBO[0]);

        // call resize_Sphere() to change size of sphere side code
       resize_Sphere(textureWidth, textureHeight);

        // set  glClearColor to black to compenset the change done by display Cube
        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT|GLES32.GL_DEPTH_BUFFER_BIT);

        if(chooseShader == 0)
        {
            GLES32.glUseProgram(shaderProgramObject_PV);

            // transformation
            float[] modelMatrix = new float[16];
            float[] translationMatrix = new float[16];

            float[] viewMatrix = new float[16];

            Matrix.setIdentityM(modelMatrix,0);
            Matrix.setIdentityM(translationMatrix,0);

            Matrix.setIdentityM(viewMatrix,0);

            Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);

            Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

            GLES32.glUniformMatrix4fv(modelMatrixUniform_PV,1,false,modelMatrix,0);

            GLES32.glUniformMatrix4fv(viewMatrixUniform_PV,1,false,viewMatrix,0);

            GLES32.glUniformMatrix4fv(projectionMatrixUniform_PV,1,false,perspectiveProjectionMatrix_Sphere,0);

            if (bLightingEnable == true)
            {
                GLES32.glUniform1i(keyPressedUniform_PV, 1);

                // light 0
                // for light ambient
                ByteBuffer byteBuffer = ByteBuffer.allocateDirect(light[0].ambient.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                FloatBuffer ambientBuffer = byteBuffer.asFloatBuffer();

                ambientBuffer.put(light[0].ambient);
                ambientBuffer.position(0);

                GLES32.glUniform3fv(lightAmbientUniform_PV[0], 1, ambientBuffer);

                // for light diffuse
                byteBuffer = ByteBuffer.allocateDirect(light[0].diffuse.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                FloatBuffer diffuseBuffer = byteBuffer.asFloatBuffer();

                diffuseBuffer.put(light[0].diffuse);
                diffuseBuffer.position(0);

                GLES32.glUniform3fv(lightDiffuseUniform_PV[0], 1, diffuseBuffer);

                // for light specular
                byteBuffer = ByteBuffer.allocateDirect(light[0].specular.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                FloatBuffer specularBuffer = byteBuffer.asFloatBuffer();

                specularBuffer.put(light[0].specular);
                specularBuffer.position(0);

                GLES32.glUniform3fv(lightSpecularUniform_PV[0], 1, specularBuffer);

                // for light position
                byteBuffer = ByteBuffer.allocateDirect(light[0].position.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

                positionBuffer.put(light[0].position);
                positionBuffer.position(0);

                GLES32.glUniform4fv(lightPositionUniform_PV[0], 1, positionBuffer);

                // light 1
                // for light ambient
                byteBuffer = ByteBuffer.allocateDirect(light[0].ambient.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                ambientBuffer = byteBuffer.asFloatBuffer();

                ambientBuffer.put(light[1].ambient);
                ambientBuffer.position(0);

                GLES32.glUniform3fv(lightAmbientUniform_PV[1], 1, ambientBuffer);

                // for light diffuse
                byteBuffer = ByteBuffer.allocateDirect(light[1].diffuse.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                diffuseBuffer = byteBuffer.asFloatBuffer();

                diffuseBuffer.put(light[1].diffuse);
                diffuseBuffer.position(0);

                GLES32.glUniform3fv(lightDiffuseUniform_PV[1], 1, diffuseBuffer);

                // for light specular
                byteBuffer = ByteBuffer.allocateDirect(light[1].specular.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                specularBuffer = byteBuffer.asFloatBuffer();

                specularBuffer.put(light[1].specular);
                specularBuffer.position(0);

                GLES32.glUniform3fv(lightSpecularUniform_PV[1], 1, specularBuffer);

                // for light position
                byteBuffer = ByteBuffer.allocateDirect(light[1].position.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                positionBuffer = byteBuffer.asFloatBuffer();

                positionBuffer.put(light[1].position);
                positionBuffer.position(0);

                GLES32.glUniform4fv(lightPositionUniform_PV[1], 1, positionBuffer);

                // light 1
                // for light ambient
                byteBuffer = ByteBuffer.allocateDirect(light[2].ambient.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                ambientBuffer = byteBuffer.asFloatBuffer();

                ambientBuffer.put(light[2].ambient);
                ambientBuffer.position(0);

                GLES32.glUniform3fv(lightAmbientUniform_PV[2], 1, ambientBuffer);

                // for light diffuse
                byteBuffer = ByteBuffer.allocateDirect(light[2].diffuse.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                diffuseBuffer = byteBuffer.asFloatBuffer();

                diffuseBuffer.put(light[2].diffuse);
                diffuseBuffer.position(0);

                GLES32.glUniform3fv(lightDiffuseUniform_PV[2], 1, diffuseBuffer);

                // for light specular
                byteBuffer = ByteBuffer.allocateDirect(light[2].specular.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                specularBuffer = byteBuffer.asFloatBuffer();

                specularBuffer.put(light[2].specular);
                specularBuffer.position(0);

                GLES32.glUniform3fv(lightSpecularUniform_PV[2], 1, specularBuffer);

                // for light position
                byteBuffer = ByteBuffer.allocateDirect(light[2].position.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                positionBuffer = byteBuffer.asFloatBuffer();

                positionBuffer.put(light[2].position);
                positionBuffer.position(0);

                GLES32.glUniform4fv(lightPositionUniform_PV[2], 1, positionBuffer);

                // for material ambient
                byteBuffer = ByteBuffer.allocateDirect(materialAmbient_Sphere.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                ambientBuffer = byteBuffer.asFloatBuffer();

                ambientBuffer.put(materialAmbient_Sphere);
                ambientBuffer.position(0);

                GLES32.glUniform3fv(materialAmbientUniform_PV, 1, ambientBuffer);

                // for material diffuse
                byteBuffer = ByteBuffer.allocateDirect(materialDiffuse_Sphere.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                diffuseBuffer = byteBuffer.asFloatBuffer();

                diffuseBuffer.put(materialDiffuse_Sphere);
                diffuseBuffer.position(0);

                GLES32.glUniform3fv(materialDiffuseUniform_PV, 1, diffuseBuffer);

                // for material specular
                byteBuffer = ByteBuffer.allocateDirect(materialSpecular_Sphere.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                specularBuffer = byteBuffer.asFloatBuffer();

                specularBuffer.put(materialSpecular_Sphere);
                specularBuffer.position(0);

                GLES32.glUniform3fv(materialSpecularUniform_PV, 1, specularBuffer);

                GLES32.glUniform1f(materialShininessUniform_PV,materialShininess_Sphere);
    
            }
            else
            {
                GLES32.glUniform1i(keyPressedUniform_PV, 0);
            }


            // bind with vao_pyramid
            GLES32.glBindVertexArray(vao_sphere[0]);

            // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
            GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

            GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

            // unbind vao_pyramid
            GLES32.glBindVertexArray(0);

            GLES32.glUseProgram(0);
        }
        else
        {
            GLES32.glUseProgram(shaderProgramObject_PF);

            // transformation
            float[] modelMatrix = new float[16];
            float[] translationMatrix = new float[16];

            float[] viewMatrix = new float[16];

            Matrix.setIdentityM(modelMatrix,0);
            Matrix.setIdentityM(translationMatrix,0);

            Matrix.setIdentityM(viewMatrix,0);

            Matrix.translateM(translationMatrix,0,0.0f,0.0f,-2.0f);

            Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

            GLES32.glUniformMatrix4fv(modelMatrixUniform_PF,1,false,modelMatrix,0);

            GLES32.glUniformMatrix4fv(viewMatrixUniform_PF,1,false,viewMatrix,0);

            GLES32.glUniformMatrix4fv(projectionMatrixUniform_PF,1,false,perspectiveProjectionMatrix_Sphere,0);

            if (bLightingEnable == true)
            {
                GLES32.glUniform1i(keyPressedUniform_PF, 1);

                // light 0
                // for light ambient
                ByteBuffer byteBuffer = ByteBuffer.allocateDirect(light[0].ambient.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                FloatBuffer ambientBuffer = byteBuffer.asFloatBuffer();

                ambientBuffer.put(light[0].ambient);
                ambientBuffer.position(0);

                GLES32.glUniform3fv(lightAmbientUniform_PF[0], 1, ambientBuffer);

                // for light diffuse
                byteBuffer = ByteBuffer.allocateDirect(light[0].diffuse.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                FloatBuffer diffuseBuffer = byteBuffer.asFloatBuffer();

                diffuseBuffer.put(light[0].diffuse);
                diffuseBuffer.position(0);

                GLES32.glUniform3fv(lightDiffuseUniform_PF[0], 1, diffuseBuffer);

                // for light specular
                byteBuffer = ByteBuffer.allocateDirect(light[0].specular.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                FloatBuffer specularBuffer = byteBuffer.asFloatBuffer();

                specularBuffer.put(light[0].specular);
                specularBuffer.position(0);

                GLES32.glUniform3fv(lightSpecularUniform_PF[0], 1, specularBuffer);

                // for light position
                byteBuffer = ByteBuffer.allocateDirect(light[0].position.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

                positionBuffer.put(light[0].position);
                positionBuffer.position(0);

                GLES32.glUniform4fv(lightPositionUniform_PF[0], 1, positionBuffer);

                // light 1
                // for light ambient
                byteBuffer = ByteBuffer.allocateDirect(light[0].ambient.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                ambientBuffer = byteBuffer.asFloatBuffer();

                ambientBuffer.put(light[1].ambient);
                ambientBuffer.position(0);

                GLES32.glUniform3fv(lightAmbientUniform_PF[1], 1, ambientBuffer);

                // for light diffuse
                byteBuffer = ByteBuffer.allocateDirect(light[1].diffuse.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                diffuseBuffer = byteBuffer.asFloatBuffer();

                diffuseBuffer.put(light[1].diffuse);
                diffuseBuffer.position(0);

                GLES32.glUniform3fv(lightDiffuseUniform_PF[1], 1, diffuseBuffer);

                // for light specular
                byteBuffer = ByteBuffer.allocateDirect(light[1].specular.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                specularBuffer = byteBuffer.asFloatBuffer();

                specularBuffer.put(light[1].specular);
                specularBuffer.position(0);

                GLES32.glUniform3fv(lightSpecularUniform_PF[1], 1, specularBuffer);

                // for light position
                byteBuffer = ByteBuffer.allocateDirect(light[1].position.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                positionBuffer = byteBuffer.asFloatBuffer();

                positionBuffer.put(light[1].position);
                positionBuffer.position(0);

                GLES32.glUniform4fv(lightPositionUniform_PF[1], 1, positionBuffer);

                // light 1
                // for light ambient
                byteBuffer = ByteBuffer.allocateDirect(light[2].ambient.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                ambientBuffer = byteBuffer.asFloatBuffer();

                ambientBuffer.put(light[2].ambient);
                ambientBuffer.position(0);

                GLES32.glUniform3fv(lightAmbientUniform_PF[2], 1, ambientBuffer);

                // for light diffuse
                byteBuffer = ByteBuffer.allocateDirect(light[2].diffuse.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                diffuseBuffer = byteBuffer.asFloatBuffer();

                diffuseBuffer.put(light[2].diffuse);
                diffuseBuffer.position(0);

                GLES32.glUniform3fv(lightDiffuseUniform_PF[2], 1, diffuseBuffer);

                // for light specular
                byteBuffer = ByteBuffer.allocateDirect(light[2].specular.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                specularBuffer = byteBuffer.asFloatBuffer();

                specularBuffer.put(light[2].specular);
                specularBuffer.position(0);

                GLES32.glUniform3fv(lightSpecularUniform_PF[2], 1, specularBuffer);

                // for light position
                byteBuffer = ByteBuffer.allocateDirect(light[2].position.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                positionBuffer = byteBuffer.asFloatBuffer();

                positionBuffer.put(light[2].position);
                positionBuffer.position(0);

                GLES32.glUniform4fv(lightPositionUniform_PF[2], 1, positionBuffer);

                // for material ambient
                byteBuffer = ByteBuffer.allocateDirect(materialAmbient_Sphere.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                ambientBuffer = byteBuffer.asFloatBuffer();

                ambientBuffer.put(materialAmbient_Sphere);
                ambientBuffer.position(0);

                GLES32.glUniform3fv(materialAmbientUniform_PF, 1, ambientBuffer);

                // for material diffuse
                byteBuffer = ByteBuffer.allocateDirect(materialDiffuse_Sphere.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                diffuseBuffer = byteBuffer.asFloatBuffer();

                diffuseBuffer.put(materialDiffuse_Sphere);
                diffuseBuffer.position(0);

                GLES32.glUniform3fv(materialDiffuseUniform_PF, 1, diffuseBuffer);

                // for material specular
                byteBuffer = ByteBuffer.allocateDirect(materialSpecular_Sphere.length * 4);

                byteBuffer.order(ByteOrder.nativeOrder());

                specularBuffer = byteBuffer.asFloatBuffer();

                specularBuffer.put(materialSpecular_Sphere);
                specularBuffer.position(0);

                GLES32.glUniform3fv(materialSpecularUniform_PF, 1, specularBuffer);

                GLES32.glUniform1f(materialShininessUniform_PF,materialShininess_Sphere);
    
            }
            else
            {
                GLES32.glUniform1i(keyPressedUniform_PF, 0);
            }


            // bind with vao_pyramid
            GLES32.glBindVertexArray(vao_sphere[0]);

            // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
            GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

            GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

            // unbind vao_pyramid
            GLES32.glBindVertexArray(0);

            GLES32.glUseProgram(0);
        }

        // unBind frame buffer
        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0);
    }


    private void update_Cube()
    {
        // code
        cAngle = cAngle + 1.0f;
        if(cAngle >= 360.0f)
        {
            cAngle = cAngle - 360.0f;
        }
    }

    private void update_Sphere()
    {
        //code
        // 

        if (bLightingEnable)
        {
            // animating light zero
            light[0].position[0] = 0.0f;
            light[0].position[1] = 5.0f * (float)Math.sin(lightAngleZero_Sphere);
            light[0].position[2] = 5.0f * (float)Math.cos(lightAngleZero_Sphere);
            light[0].position[3] = 1.0f;
    
        
            lightAngleZero_Sphere = lightAngleZero_Sphere + 0.05f;
            if (lightAngleZero_Sphere >= 360.0f)
            {
                lightAngleZero_Sphere = lightAngleZero_Sphere - 360.0f;
            }
    
            // animating light one
            light[1].position[0] = 5.0f * (float)Math.sin(lightAngleOne_Sphere);
            light[1].position[1] = 0.0f;
            light[1].position[2] = 5.0f * (float)Math.cos(lightAngleOne_Sphere);
            light[1].position[3] = 1.0f;
    
        
            lightAngleOne_Sphere = lightAngleOne_Sphere + 0.05f;
            if (lightAngleOne_Sphere >= 360.0f)
            {
                lightAngleOne_Sphere = lightAngleOne_Sphere - 360.0f;
            }
    
            // animating light two
            light[2].position[0] = 5.0f * (float)Math.sin(lightAngleTwo_Sphere);
            light[2].position[1] = 5.0f * (float)Math.cos(lightAngleTwo_Sphere);
            light[2].position[2] = 0.0f;
            light[2].position[3] = 1.0f;
    
            lightAngleTwo_Sphere = lightAngleTwo_Sphere + 0.05f;
            if (lightAngleTwo_Sphere >= 360.0f)
            {
                lightAngleTwo_Sphere = lightAngleTwo_Sphere - 360.0f;
            }
        }

    }


    private void uninitialize_Cube()
    {
        // code

        if(shaderProgramObject_Cube > 0)
        {
            GLES32.glUseProgram(shaderProgramObject_Cube);

            int[] returnValue = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject_Cube,GLES32.GL_ATTACHED_SHADERS,returnValue,0);

            if(returnValue[0] > 0)
            {
                int numAttachedShader = returnValue[0];

                int[] shaderObjects = new int[numAttachedShader];

                GLES32.glGetAttachedShaders(shaderProgramObject_Cube,numAttachedShader,returnValue,0,shaderObjects,0);

                for(int i = 0; i<numAttachedShader; i++)
                {
                    GLES32.glDetachShader(shaderProgramObject_Cube,shaderObjects[i]);

                    GLES32.glDeleteShader(shaderObjects[i]);

                    shaderObjects[i] = 0;
                }
            }
            GLES32.glUseProgram(0);

            GLES32.glDeleteProgram(shaderProgramObject_Cube);

            shaderProgramObject_Cube = 0;
        }

        // delete vbo_texcoordCube 
        if(vbo_texcoordCube[0]>0)
        {
            GLES32.glDeleteBuffers(1,vbo_texcoordCube,0);
            vbo_texcoordCube[0] = 0;
        }

        // delete vbo_positionCube 
        if(vbo_positionCube[0]>0)
        {
            GLES32.glDeleteBuffers(1,vbo_positionCube,0);
            vbo_positionCube[0] = 0;
        }

        // delete texture
        if (texture_FBO[0]>0)
        {
            GLES32.glDeleteTextures(1, texture_FBO,0);
            texture_FBO[0] = 0;
        }

        // delete vao_cube 
        if(vao_cube[0]>0)
        {
            GLES32.glDeleteVertexArrays(1,vao_cube,0);
            vao_cube[0] = 0;
        }
    
    }

    private void uninitialize_Sphere()
    {
        // code

        if(shaderProgramObject_PV > 0)
        {
            GLES32.glUseProgram(shaderProgramObject_PV);

            int[] returnValue = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject_PV,GLES32.GL_ATTACHED_SHADERS,returnValue,0);

            if(returnValue[0] > 0)
            {
                int numAttachedShader = returnValue[0];

                int[] shaderObjects = new int[numAttachedShader];

                GLES32.glGetAttachedShaders(shaderProgramObject_PV,numAttachedShader,returnValue,0,shaderObjects,0);

                for(int i = 0; i<numAttachedShader; i++)
                {
                    GLES32.glDetachShader(shaderProgramObject_PV,shaderObjects[i]);

                    GLES32.glDeleteShader(shaderObjects[i]);

                    shaderObjects[i] = 0;
                }
            }
            GLES32.glUseProgram(0);

            GLES32.glDeleteProgram(shaderProgramObject_PV);

            shaderProgramObject_PV = 0;
        }

        // destroy element vbo
        if(vbo_sphere_element[0] != 0)
        {
            GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
            vbo_sphere_element[0]=0;
        }

        // delete vbo_sphere_normal 
        if(vbo_sphere_normal[0] != 0)
        {
            GLES32.glDeleteBuffers(1,vbo_sphere_normal,0);
            vbo_sphere_normal[0] = 0;
        }

        // delete vbo_sphere_position 
        if(vbo_sphere_position[0] != 0)
        {
            GLES32.glDeleteBuffers(1,vbo_sphere_position,0);
            vbo_sphere_position[0] = 0;
        }

        // delete vao_sphere 
        if(vao_sphere[0] != 0)
        {
            GLES32.glDeleteVertexArrays(1,vao_sphere,0);
            vao_sphere[0] = 0;
        }
    
    }

}



