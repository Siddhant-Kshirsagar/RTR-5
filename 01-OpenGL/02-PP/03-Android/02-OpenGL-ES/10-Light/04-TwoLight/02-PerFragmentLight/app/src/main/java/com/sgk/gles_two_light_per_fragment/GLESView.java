package com.sgk.gles_two_light_per_fragment;

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

// for sphere
import java.nio.ShortBuffer;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,OnDoubleTapListener,OnGestureListener
{
    // global variable
    private GestureDetector gestureDetector;

    // OpenGL related variable 
    private int shaderProgramObject = 0;
    private int[] vao_pyramid = new int[1];
    private int[] vbo_position_pyramid = new int[1];
    private int[] vbo_normal = new int[1];

    private int modelMatrixUniform = 0;
    private int viewMatrixUniform = 0;
    private int projectionMatrixUniform = 0;

    private int[] lightAmbientUniform = new int[2];
    private int[] lightDiffuseUniform = new int[2];
    private int[] lightSpecularUniform = new int[2];
    private int[] lightPositionUniform = new int[2];

   private int materialAmbientUniform = 0;
    private int materialDiffuseUniform = 0;
    private int materialSpecularUniform = 0;
    private int materialShininessUniform = 0;

    private int keyPressedUniform = 0;

    private float[] materialAmbient = {0.0f,0.0f,0.0f,1.0f};
    private float[] materialDiffuse = {1.0f,1.0f,1.0f,1.0f};
    private float[] materialSpecular = {1.0f,1.0f,1.0f,1.0f};
    private float materialShininess = 50.0f;


    private float[] perspectiveProjectionMatrix = new float
    [16];

    private boolean bLightingEnable = false;
    private boolean bAnimationEnable = false;

    private class Light{
        float[] ambient = new float[3];
        float[] diffuse = new float[3];
        float[] specular = new float[3];
        float[] position = new float[4];
    }

    Light []light = new Light[2];

    private float pAngle = 0.0f;

    public GLESView(Context context)
    {
        super(context);
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
        initialize(gl);
    }

    @Override
    public void onSurfaceChanged(GL10 gl,int width,int height)
    {
        // code
        resize(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        //code
        display();
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
        if (bAnimationEnable == false)
        {
            bAnimationEnable = true;
        }
        else
        {
            bAnimationEnable = false;
        }
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
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
    {
        // code
        uninitialize();

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
    private void initialize(GL10 gl)
    {
        // code 
        // print OpenGLES Info
        printGLInfo(gl);

        // Vertex Shader
        final String vertexShaderSourceCode = String.format
        (
            "#version 320 es"+
            "\n"+
            "in vec4 aPosition;"+
            "in vec3 aNormal;"+
            "uniform mat4 uModelMatrix;"+
            "uniform mat4 uViewMatrix;"+
            "uniform mat4 uProjectionMatrix;"+
            "uniform vec4 uLightPosition[2];"+
            "uniform int uKeyPressed;"+
            "out vec3 oTransformedNormals;"+
            "out vec3 oLightDirection[2];"+
            "out vec3 oViewerVector;"+
            "void main(void)"+
            "{"+
            "if(uKeyPressed == 1)"+
            "{"+
            "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
            "oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;"+
            "oLightDirection[0] = vec3(uLightPosition[0]-eyeCoordinates);"+
            "oLightDirection[1] = vec3(uLightPosition[1]-eyeCoordinates);"+
            "oViewerVector = -eyeCoordinates.xyz;"+
            "}"+
            "else"+
            "{"+
            "oTransformedNormals = vec3(0.0,0.0,0.0);"+
            "oLightDirection[0] = vec3(0.0,0.0,0.0);"+
            "oLightDirection[1] = vec3(0.0,0.0,0.0);"+
            "oViewerVector = vec3(0.0,0.0,0.0);"+
            "}"+
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"+
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

                    uninitialize();
                    System.exit(0);
                }
            }
        }

        // Fragment shader
        final String fragmentShaderSourceCode = String.format(
            "#version 320 es"+
            "\n"+
            "precision highp float;"+
            "in vec3 oTransformedNormals;"+
            "in vec3 oLightDirection[2];"+
            "in vec3 oViewerVector;"+
            "uniform vec3 uLightAmbient[2];"+
            "uniform vec3 uLightDiffuse[2];"+
            "uniform vec3 uLightSpecular[2];"+
            "uniform vec3 uMaterialAmbient;"+
            "uniform vec3 uMaterialDiffuse;"+
            "uniform vec3 uMaterialSpecular;"+
            "uniform float uMaterialShineness;"+
            "uniform highp int uKeyPressed;"+
            "out vec4 FragColor;"+
            "void main(void)"+
            "{"+
            "vec3 phongADSLight;"+
            "vec3 normalizeLightDirection[2];"+
            "vec3 lightAmbient[2];"+
            "vec3 lightDirection[2];"+
            "vec3 lightDiffuse[2];"+
            "vec3 reflectionVector[2];"+
            "vec3 lightSpecular[2];"+
            "if(uKeyPressed == 1)"+
            "{"+
            "vec3 normalizeTransformedNormals = normalize(oTransformedNormals);"+
            "normalizeLightDirection[0] = normalize(oLightDirection[0]);"+
            "normalizeLightDirection[1] = normalize(oLightDirection[1]);"+
            "vec3 normalizeViewerVector = normalize(oViewerVector);"+
            "for(int i = 0; i<2; i++)"+
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

                    uninitialize();
                    System.exit(0);
                }
            }
        }

        // shader program
        shaderProgramObject = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject,vertexShaderObject);
        GLES32.glAttachShader(shaderProgramObject,fragmentShaderObject);

        GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,"aPosition");

        GLES32.glBindAttribLocation(shaderProgramObject,VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL,"aNormal");

        GLES32.glLinkProgram(shaderProgramObject);

        int[] programLinkStatus = new int[1];
        infoLogLength[0] = 0;
        infoLog = null;

        GLES32.glGetProgramiv(shaderProgramObject,GLES32.GL_INFO_LOG_LENGTH,programLinkStatus,0);

        // linking error check
        if(programLinkStatus[0] == 0)
        {
            GLES32.glGetShaderiv(shaderProgramObject,GLES32.GL_LINK_STATUS,infoLogLength,0);

            if(infoLogLength[0]>0)
            {
                infoLog = GLES32.glGetShaderInfoLog(shaderProgramObject);
                if(infoLog!=null)
                {
                System.out.println("SGK: Shader program link error log : "+infoLog);

                uninitialize();
                System.exit(0);
                }
            }
        }

        // get shader uniform location
        modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uModelMatrix");
        viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uViewMatrix");
        projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");

        lightAmbientUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "uLightAmbient[0]");
        lightDiffuseUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "uLightDiffuse[0]");
        lightSpecularUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "uLightSpecular[0]");
        lightPositionUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "uLightPosition[0]");

        lightAmbientUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "uLightAmbient[1]");
        lightDiffuseUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "uLightDiffuse[1]");
        lightSpecularUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "uLightSpecular[1]");
        lightPositionUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "uLightPosition[1]");
        


        materialAmbientUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");
        materialDiffuseUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialDiffuse");
        materialSpecularUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");
        materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMaterialShineness");
        keyPressedUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uKeyPressed");

        // geometry / shape
        final float pyramid_position[] = {
            // front
            0.0f,  1.0f,  0.0f, // front-top
            -1.0f, -1.0f,  1.0f, // front-left
            1.0f, -1.0f,  1.0f, // front-right

            // right
            0.0f,  1.0f,  0.0f, // right-top
            1.0f, -1.0f,  1.0f, // right-left
            1.0f, -1.0f, -1.0f, // right-right

            // back
            0.0f,  1.0f,  0.0f, // back-top
            1.0f, -1.0f, -1.0f, // back-left
            -1.0f, -1.0f, -1.0f, // back-right

            // left
            0.0f,  1.0f,  0.0f, // left-top
            -1.0f, -1.0f, -1.0f, // left-left
            -1.0f, -1.0f,  1.0f, // left-right
        };

        final float pyramid_normals[]={
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

        // vao_pyramid(vertex array object)
        GLES32.glGenVertexArrays(1,vao_pyramid,0);

        GLES32.glBindVertexArray(vao_pyramid[0]);

        GLES32.glGenBuffers(1,vbo_position_pyramid,0);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_position_pyramid[0]);

        // prepare vertices array for glBufferData
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pyramid_position.length * 4);

        byteBuffer.order(ByteOrder.nativeOrder());

        FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();

        verticesBuffer.put(pyramid_position);
        verticesBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,pyramid_position.length * 4,verticesBuffer,GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,3,GLES32.GL_FLOAT,false,0,0);

        GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

       // normal vbo
       GLES32.glGenBuffers(1,vbo_normal,0);
       GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_normal[0]);
       
       byteBuffer=ByteBuffer.allocateDirect(pyramid_normals.length * 4);
       byteBuffer.order(ByteOrder.nativeOrder());
       FloatBuffer normalBuffer = byteBuffer.asFloatBuffer();
       normalBuffer.put(pyramid_normals);
       normalBuffer.position(0);
       
       GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
        pyramid_normals.length * 4,
        normalBuffer,
        GLES32.GL_STATIC_DRAW);
       
       GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL,
        3,
        GLES32.GL_FLOAT,
        false,0,0);
       
       GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL);
       
       GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
    

        GLES32.glBindVertexArray(0);

        // for depth enable setting
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // Enable culling 
        GLES32.glEnable(GLES32.GL_CULL_FACE);

        // clear color
        GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

        Matrix.setIdentityM(perspectiveProjectionMatrix,0);

        light[0] = new Light();
        light[1] = new Light();

        light[0].ambient[0] = 0.0f;
        light[0].ambient[1] = 0.0f;
        light[0].ambient[2] = 0.0f;

        light[1].ambient[0] = 0.0f;
        light[1].ambient[1] = 0.0f;
        light[1].ambient[2] = 0.0f;

        light[0].diffuse[0] = 1.0f;
        light[0].diffuse[1] = 0.0f;
        light[0].diffuse[2] = 0.0f;

        light[1].diffuse[0] = 0.0f;
        light[1].diffuse[1] = 0.0f;
        light[1].diffuse[2] = 1.0f;


        light[0].specular[0] = 1.0f;
        light[0].specular[1] = 0.0f;
        light[0].specular[2] = 0.0f;

        light[1].specular[0] = 0.0f;
        light[1].specular[1] = 0.0f;
        light[1].specular[2] = 1.0f;

        light[0].position[0] = -2.0f;
        light[0].position[1] = 0.0f;
        light[0].position[2] = 0.0f;
        light[0].position[3] = 1.0f;

        light[1].position[0] = 2.0f;
        light[1].position[1] = 0.0f;
        light[1].position[2] = 0.0f;
        light[1].position[3] = 1.0f;

    }

    private void printGLInfo(GL10 gl)
    {
        // code
        System.out.println("SGK: OpenGL-ES Renderer : "+ gl.glGetString(GL10.GL_RENDERER));
        System.out.println("SGK: OpenGL-ES Version : "+ gl.glGetString(GL10.GL_VERSION));
        System.out.println("SGK: OpenGL-ES Shading Language Version : "+ gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION));
    }

    private void resize(int width, int height)
    {
        // code
        if(height<=0)
        {
            height = 1;
        }

        GLES32.glViewport(0,0,width,height);

        // set perspective projection matrix
        Matrix.perspectiveM(perspectiveProjectionMatrix,0,45.0f,(float)width/(float)height,0.1f,100.0f);
    }

    private void display()
    {
        // code
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT|GLES32.GL_DEPTH_BUFFER_BIT);

        GLES32.glUseProgram(shaderProgramObject);

        // transformation
        float[] modelMatrix = new float[16];
        float[] translationMatrix = new float[16];
        float[] rotationMatrix = new float[16];

        float[] viewMatrix = new float[16];

        Matrix.setIdentityM(modelMatrix,0);
        Matrix.setIdentityM(translationMatrix,0);
        Matrix.setIdentityM(rotationMatrix,0);

        Matrix.setIdentityM(viewMatrix,0);

        Matrix.translateM(translationMatrix,0,0.0f,0.0f,-5.0f);
        Matrix.rotateM(rotationMatrix,0,pAngle,0.0f,1.0f,0.0f);

        Matrix.multiplyMM(modelMatrix,0,translationMatrix,0,rotationMatrix,0);

        GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);

        GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);

        GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

        if (bLightingEnable == true)
        {
            GLES32.glUniform1i(keyPressedUniform, 1);

            // light 0
            // for light ambient
            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(light[0].ambient.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            FloatBuffer ambientBuffer = byteBuffer.asFloatBuffer();

            ambientBuffer.put(light[0].ambient);
            ambientBuffer.position(0);

            GLES32.glUniform3fv(lightAmbientUniform[0], 1, ambientBuffer);

            // for light diffuse
            byteBuffer = ByteBuffer.allocateDirect(light[0].diffuse.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            FloatBuffer diffuseBuffer = byteBuffer.asFloatBuffer();

            diffuseBuffer.put(light[0].diffuse);
            diffuseBuffer.position(0);

            GLES32.glUniform3fv(lightDiffuseUniform[0], 1, diffuseBuffer);

            // for light specular
            byteBuffer = ByteBuffer.allocateDirect(light[0].specular.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            FloatBuffer specularBuffer = byteBuffer.asFloatBuffer();

            specularBuffer.put(light[0].specular);
            specularBuffer.position(0);

            GLES32.glUniform3fv(lightSpecularUniform[0], 1, specularBuffer);

            // for light position
            byteBuffer = ByteBuffer.allocateDirect(light[0].position.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

            positionBuffer.put(light[0].position);
            positionBuffer.position(0);

            GLES32.glUniform4fv(lightPositionUniform[0], 1, positionBuffer);

            // light 1
            // for light ambient
             byteBuffer = ByteBuffer.allocateDirect(light[0].ambient.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            ambientBuffer = byteBuffer.asFloatBuffer();

            ambientBuffer.put(light[1].ambient);
            ambientBuffer.position(0);

            GLES32.glUniform3fv(lightAmbientUniform[1], 1, ambientBuffer);

            // for light diffuse
            byteBuffer = ByteBuffer.allocateDirect(light[1].diffuse.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            diffuseBuffer = byteBuffer.asFloatBuffer();

            diffuseBuffer.put(light[1].diffuse);
            diffuseBuffer.position(0);

            GLES32.glUniform3fv(lightDiffuseUniform[1], 1, diffuseBuffer);

            // for light specular
            byteBuffer = ByteBuffer.allocateDirect(light[1].specular.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            specularBuffer = byteBuffer.asFloatBuffer();

            specularBuffer.put(light[1].specular);
            specularBuffer.position(0);

            GLES32.glUniform3fv(lightSpecularUniform[1], 1, specularBuffer);

            // for light position
            byteBuffer = ByteBuffer.allocateDirect(light[1].position.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            positionBuffer = byteBuffer.asFloatBuffer();

            positionBuffer.put(light[1].position);
            positionBuffer.position(0);

            GLES32.glUniform4fv(lightPositionUniform[1], 1, positionBuffer);

            // for material ambient
            byteBuffer = ByteBuffer.allocateDirect(materialAmbient.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            ambientBuffer = byteBuffer.asFloatBuffer();

            ambientBuffer.put(materialAmbient);
            ambientBuffer.position(0);

            GLES32.glUniform3fv(materialAmbientUniform, 1, ambientBuffer);

            // for material diffuse
            byteBuffer = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            diffuseBuffer = byteBuffer.asFloatBuffer();

            diffuseBuffer.put(materialDiffuse);
            diffuseBuffer.position(0);

            GLES32.glUniform3fv(materialDiffuseUniform, 1, diffuseBuffer);

            // for material specular
            byteBuffer = ByteBuffer.allocateDirect(materialSpecular.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            specularBuffer = byteBuffer.asFloatBuffer();

            specularBuffer.put(materialSpecular);
            specularBuffer.position(0);

            GLES32.glUniform3fv(materialSpecularUniform, 1, specularBuffer);

            GLES32.glUniform1f(materialShininessUniform,materialShininess);
   
        }
        else
        {
            GLES32.glUniform1i(keyPressedUniform, 0);
        }


        // bind with vao_pyramid
        GLES32.glBindVertexArray(vao_pyramid[0]);

        // step 3 : draw geometry / shape / model /scene
        GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 12);

        // unbind vao_pyramid
        GLES32.glBindVertexArray(0);

        GLES32.glUseProgram(0);

        // update for animation
        pAngle = pAngle + 1.0f;
        if(pAngle >= 360.0f)
        {
            pAngle = pAngle - 360.0f;
        }
        // render
        requestRender();
    }

    private void uninitialize()
    {
        // code

        if(shaderProgramObject > 0)
        {
            GLES32.glUseProgram(shaderProgramObject);

            int[] returnValue = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject,GLES32.GL_ATTACHED_SHADERS,returnValue,0);

            if(returnValue[0] > 0)
            {
                int numAttachedShader = returnValue[0];

                int[] shaderObjects = new int[numAttachedShader];

                GLES32.glGetAttachedShaders(shaderProgramObject,numAttachedShader,returnValue,0,shaderObjects,0);

                for(int i = 0; i<numAttachedShader; i++)
                {
                    GLES32.glDetachShader(shaderProgramObject,shaderObjects[i]);

                    GLES32.glDeleteShader(shaderObjects[i]);

                    shaderObjects[i] = 0;
                }
            }
            GLES32.glUseProgram(0);

            GLES32.glDeleteProgram(shaderProgramObject);

            shaderProgramObject = 0;
        }

        // delete vbo_normal 
        if(vbo_normal[0] != 0)
        {
            GLES32.glDeleteBuffers(1,vbo_normal,0);
            vbo_normal[0] = 0;
        }

        // delete vbo_position_pyramid 
        if(vbo_position_pyramid[0] != 0)
        {
            GLES32.glDeleteBuffers(1,vbo_position_pyramid,0);
            vbo_position_pyramid[0] = 0;
        }

        // delete vao_pyramid 
        if(vao_pyramid[0] != 0)
        {
            GLES32.glDeleteVertexArrays(1,vao_pyramid,0);
            vao_pyramid[0] = 0;
        }
    
    }
}



