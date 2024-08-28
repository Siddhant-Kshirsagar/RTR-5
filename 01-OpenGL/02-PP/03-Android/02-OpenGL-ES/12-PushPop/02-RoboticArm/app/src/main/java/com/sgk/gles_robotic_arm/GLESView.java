package com.sgk.gles_robotic_arm;

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
    private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

    private int numVertices = 0;
    private int numElements = 0;

    private int modelViewMatrixUniform = 0;
    private int projectionMatrixUniform = 0;
    private int colorUniform = 0;

    private int rotateBodyPart = 0;

    private int sholder = 0;
    private int elbow = 0;
    private int wrist = 0;

    private final int MODEL_VIEW_MATRIX_STACK = 32;
    private float matrixStack[][] = new float[MODEL_VIEW_MATRIX_STACK][16];
    int matrixStackTop = -1;

    private float[] perspectiveProjectionMatrix = new float
    [16];


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
        rotateBodyPart = rotateBodyPart + 1;
        if(rotateBodyPart >= 3)
        {
            rotateBodyPart = 0;
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
        if(rotateBodyPart==0)
        {
            sholder = (sholder + 10) % 360;
        }
        else if(rotateBodyPart == 1)
        {
            elbow = (elbow + 10) % 360;
        }
        else if(rotateBodyPart == 2)
        {
            wrist = (wrist + 10) % 360;
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
            "uniform mat4 uModelViewMatrix;"+
            "uniform mat4 uProjectionMatrix;"+
            "void main(void)"+
            "{"+
            "gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;"+
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
            "uniform vec4 uColor;"+
            "out vec4 FragColor;"+
            "void main(void)"+
            "{"+
            "FragColor = uColor;"+
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
        modelViewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject,"uModelViewMatrix");

        projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject,"uProjectionMatrix");

        colorUniform = GLES32.glGetUniformLocation(shaderProgramObject,"uColor");

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

        initializeMatrixStack();

        Matrix.setIdentityM(perspectiveProjectionMatrix,0);
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
        float[] translationMatrix = new float[16];
        float[] rotationMatrix = new float[16];
        float[] scaleMatrix = new float[16];
        float[] modelMatrix = new float[16];
        float[] modelViewMatrix = new float[16];
        float[] viewMatrix = new float[16];
        

        Matrix.setIdentityM(translationMatrix,0);
        Matrix.setIdentityM(rotationMatrix,0);
        Matrix.setIdentityM(scaleMatrix,0);
        Matrix.setIdentityM(modelMatrix,0);
        Matrix.setIdentityM(modelViewMatrix,0);
        Matrix.setIdentityM(viewMatrix,0);

        

        pushMatrix(modelMatrix);
        {
            Matrix.translateM(translationMatrix,0,0.0f,0.0f,-10.0f);

            Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

            pushMatrix(modelMatrix);
            {
                // draw sholder
                Matrix.rotateM(rotationMatrix,0,sholder,0.0f,0.0f,1.0f);

                Matrix.translateM(translationMatrix,0,1.0f,0.0f,0.0f);

                Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,rotationMatrix,0);

                Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                pushMatrix(modelMatrix);
                {
                    Matrix.scaleM(scaleMatrix,0,2.0f,0.5f,1.0f);
                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,scaleMatrix,0);

                    GLES32.glUniform4f(colorUniform,0.8f, 0.6f, 0.4f,1.0f);

                    Matrix.setIdentityM(modelViewMatrix,0);
                    Matrix.multiplyMM(modelViewMatrix,0,viewMatrix,0,modelMatrix,0);

                    GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

                    GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

                    // GLES32.glPolygonMode(GLES32.GL_FRONT_AND_BACK, GLES32.GL_FILL);
                    
                    // bind with vao_sphere
                    GLES32.glBindVertexArray(vao_sphere[0]);

                    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
                    GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

                    GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

                    // unbind vao_sphere
                    GLES32.glBindVertexArray(0);
                }
                Matrix.setIdentityM(modelMatrix,0);
                modelMatrix = popMatrix();

                // draw elbow
                Matrix.setIdentityM(translationMatrix,0);
                Matrix.translateM(translationMatrix,0,1.0f,0.0f,0.0f);

                Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                Matrix.setIdentityM(rotationMatrix,0);
                Matrix.rotateM(rotationMatrix,0,elbow,0.0f,0.0f,1.0f);

                Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,rotationMatrix,0);

                Matrix.setIdentityM(translationMatrix,0);
                Matrix.translateM(translationMatrix,0,1.0f,0.0f,0.0f);

                Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                pushMatrix(modelMatrix);
                {
                    Matrix.setIdentityM(scaleMatrix,0);
                    Matrix.scaleM(scaleMatrix,0,2.0f, 0.5f, 1.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,scaleMatrix,0);

                    GLES32.glUniform4f(colorUniform,0.8f, 0.6f, 0.4f,1.0f);

                    Matrix.setIdentityM(modelViewMatrix,0);
                    Matrix.multiplyMM(modelViewMatrix,0,viewMatrix,0,modelMatrix,0);

                    GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

                    GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

                    // GLES32.glPolygonMode(GLES32.GL_FRONT_AND_BACK, GLES32.GL_LINE);

                    // step 2 : bind with VAO(vertex array object)
                    // bind with vao_sphere
                    GLES32.glBindVertexArray(vao_sphere[0]);

                    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
                    GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

                    GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

                    // unbind vao_sphere
                    GLES32.glBindVertexArray(0);
                }
                Matrix.setIdentityM(modelMatrix,0);
                modelMatrix = popMatrix();

                // draw palm of hand
                Matrix.setIdentityM(translationMatrix,0);
                Matrix.translateM(translationMatrix,0,1.0f,0.0f,0.0f);

                Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                Matrix.setIdentityM(rotationMatrix,0);
                Matrix.rotateM(rotationMatrix,0,wrist,0.0f,0.0f,1.0f);

                Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,rotationMatrix,0);

                Matrix.setIdentityM(translationMatrix,0);
                Matrix.translateM(translationMatrix,0,0.25f,0.0f,0.0f);

                Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                
                pushMatrix(modelMatrix);
                {
                    Matrix.setIdentityM(scaleMatrix,0);
                    Matrix.scaleM(scaleMatrix,0,0.5f, 0.5f, 0.5f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,scaleMatrix,0);

                    GLES32.glUniform4f(colorUniform,0.8f, 0.6f, 0.4f,1.0f);

                    Matrix.setIdentityM(modelViewMatrix,0);
                    Matrix.multiplyMM(modelViewMatrix,0,viewMatrix,0,modelMatrix,0);

                    GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

                    GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

                    // GLES32.glPolygonMode(GLES32.GL_FRONT_AND_BACK, GLES32.GL_LINE);

                    // step 2 : bind with VAO(vertex array object)
                    // bind with vao_sphere
                    GLES32.glBindVertexArray(vao_sphere[0]);

                    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
                    GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

                    GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

                    // unbind vao_sphere
                    GLES32.glBindVertexArray(0);
                }
                Matrix.setIdentityM(modelMatrix,0);
                modelMatrix = popMatrix();

                // draw thumb
                pushMatrix(modelMatrix);
                {
                    Matrix.setIdentityM(rotationMatrix,0);
                    Matrix.rotateM(rotationMatrix,0,90.0f,0.0f,0.0f,1.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,rotationMatrix,0);

                    Matrix.setIdentityM(translationMatrix,0);
                    Matrix.translateM(translationMatrix,0,0.45f,0.0f,0.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                    Matrix.setIdentityM(scaleMatrix,0);
                    Matrix.scaleM(scaleMatrix,0,0.35f, 0.2f, 0.25f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,scaleMatrix,0);

                    GLES32.glUniform4f(colorUniform,0.8f, 0.6f, 0.4f,1.0f);

                    Matrix.setIdentityM(modelViewMatrix,0);
                    Matrix.multiplyMM(modelViewMatrix,0,viewMatrix,0,modelMatrix,0);

                    GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

                    GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

                    // GLES32.glPolygonMode(GLES32.GL_FRONT_AND_BACK, GLES32.GL_LINE);

                    // step 2 : bind with VAO(vertex array object)
                    // bind with vao_sphere
                    GLES32.glBindVertexArray(vao_sphere[0]);

                    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
                    GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

                    GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

                    // unbind vao_sphere
                    GLES32.glBindVertexArray(0);
                }
                Matrix.setIdentityM(modelMatrix,0);
                modelMatrix = popMatrix();

                // draw index finger
                pushMatrix(modelMatrix);
                {
                    Matrix.setIdentityM(rotationMatrix,0);
                    Matrix.rotateM(rotationMatrix,0,10.0f,0.0f,0.0f,1.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,rotationMatrix,0);

                    Matrix.setIdentityM(translationMatrix,0);
                    Matrix.translateM(translationMatrix,0,0.45f,0.15f,0.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                    Matrix.setIdentityM(scaleMatrix,0);
                    Matrix.scaleM(scaleMatrix,0,0.45f, 0.15f, 0.25f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,scaleMatrix,0);

                    GLES32.glUniform4f(colorUniform,0.8f, 0.6f, 0.4f,1.0f);

                    Matrix.setIdentityM(modelViewMatrix,0);
                    Matrix.multiplyMM(modelViewMatrix,0,viewMatrix,0,modelMatrix,0);

                    GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

                    GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

                    // GLES32.glPolygonMode(GLES32.GL_FRONT_AND_BACK, GLES32.GL_LINE);

                    // step 2 : bind with VAO(vertex array object)
                    // bind with vao_sphere
                    GLES32.glBindVertexArray(vao_sphere[0]);

                    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
                    GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

                    GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

                    // unbind vao_sphere
                    GLES32.glBindVertexArray(0);
                }
                Matrix.setIdentityM(modelMatrix,0);
                modelMatrix = popMatrix();

                // draw middle finger
                pushMatrix(modelMatrix);
                {
                    Matrix.setIdentityM(rotationMatrix,0);
                    Matrix.rotateM(rotationMatrix,0,0.0f,0.0f,0.0f,1.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,rotationMatrix,0);

                    Matrix.setIdentityM(translationMatrix,0);
                    Matrix.translateM(translationMatrix,0,0.525f,0.05f,0.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                    Matrix.setIdentityM(scaleMatrix,0);
                    Matrix.scaleM(scaleMatrix,0,0.5f, 0.15f, 0.25f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,scaleMatrix,0);

                    GLES32.glUniform4f(colorUniform,0.8f, 0.6f, 0.4f,1.0f);

                    Matrix.setIdentityM(modelViewMatrix,0);
                    Matrix.multiplyMM(modelViewMatrix,0,viewMatrix,0,modelMatrix,0);

                    GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

                    GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

                    // GLES32.glPolygonMode(GLES32.GL_FRONT_AND_BACK, GLES32.GL_LINE);

                    // step 2 : bind with VAO(vertex array object)
                    // bind with vao_sphere
                    GLES32.glBindVertexArray(vao_sphere[0]);

                    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
                    GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

                    GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

                    // unbind vao_sphere
                    GLES32.glBindVertexArray(0);
                }
                Matrix.setIdentityM(modelMatrix,0);
                modelMatrix = popMatrix();

                // draw ring finger
                pushMatrix(modelMatrix);
                {
                    Matrix.setIdentityM(rotationMatrix,0);
                    Matrix.rotateM(rotationMatrix,0,-10.0f,0.0f,0.0f,1.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,rotationMatrix,0);

                    Matrix.setIdentityM(translationMatrix,0);
                    Matrix.translateM(translationMatrix,0,0.475f,-0.05f,0.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                    Matrix.setIdentityM(scaleMatrix,0);
                    Matrix.scaleM(scaleMatrix,0,0.5f, 0.15f, 0.25f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,scaleMatrix,0);

                    GLES32.glUniform4f(colorUniform,0.8f, 0.6f, 0.4f,1.0f);

                    Matrix.setIdentityM(modelViewMatrix,0);
                    Matrix.multiplyMM(modelViewMatrix,0,viewMatrix,0,modelMatrix,0);

                    GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

                    GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

                    // GLES32.glPolygonMode(GLES32.GL_FRONT_AND_BACK, GLES32.GL_LINE);

                    // step 2 : bind with VAO(vertex array object)
                    // bind with vao_sphere
                    GLES32.glBindVertexArray(vao_sphere[0]);

                    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
                    GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

                    GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

                    // unbind vao_sphere
                    GLES32.glBindVertexArray(0);
                }
                Matrix.setIdentityM(modelMatrix,0);
                modelMatrix = popMatrix();

                // draw pinky finger
                pushMatrix(modelMatrix);
                {
                    Matrix.setIdentityM(rotationMatrix,0);
                    Matrix.rotateM(rotationMatrix,0,-20.0f,0.0f,0.0f,1.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,rotationMatrix,0);

                    Matrix.setIdentityM(translationMatrix,0);
                    Matrix.translateM(translationMatrix,0,0.4f,-0.15f,0.0f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,translationMatrix,0);

                    Matrix.setIdentityM(scaleMatrix,0);
                    Matrix.scaleM(scaleMatrix,0,0.3f, 0.1f, 0.2f);

                    Matrix.multiplyMM(modelMatrix,0,modelMatrix,0,scaleMatrix,0);

                    GLES32.glUniform4f(colorUniform,0.8f, 0.6f, 0.4f,1.0f);

                    Matrix.setIdentityM(modelViewMatrix,0);
                    Matrix.multiplyMM(modelViewMatrix,0,viewMatrix,0,modelMatrix,0);

                    GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

                    GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

                    // GLES32.glPolygonMode(GLES32.GL_FRONT_AND_BACK, GLES32.GL_LINE);

                    // step 2 : bind with VAO(vertex array object)
                    // bind with vao_sphere
                    GLES32.glBindVertexArray(vao_sphere[0]);

                    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
                    GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

                    GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);

                    // unbind vao_sphere
                    GLES32.glBindVertexArray(0);
                }
                Matrix.setIdentityM(modelMatrix,0);
                modelMatrix = popMatrix();

            }
            modelViewMatrix = popMatrix();
        }
        modelViewMatrix = popMatrix();



        GLES32.glUseProgram(0);

        update();
        // render
        requestRender();
    }

    void update()
    {
        // code
        if(rotateBodyPart==0)
        {
            sholder = (sholder + 2) % 360;
        }
        else if(rotateBodyPart == 1)
        {
            elbow = (elbow + 2) % 360;
        }
        else if(rotateBodyPart == 2)
        {
            wrist = (wrist + 2) % 360;
        }

    }

    void initializeMatrixStack()
    {
        //code
        matrixStackTop = 0;

        for (int i = 0; i < MODEL_VIEW_MATRIX_STACK; i++)
        {
            Matrix.setIdentityM(matrixStack[i],0);

        }
    }

    void pushMatrix(float[] matrix)
    {
        // code
        if(matrixStackTop >= MODEL_VIEW_MATRIX_STACK)
        {
            System.out.println("SGK: ERROR : Exceeded matrix stack limit\n");
            uninitialize();
        }

        matrixStack[matrixStackTop] = matrix.clone();

        System.out.println("SGK: Current Matrix length: "+matrixStack[matrixStackTop]);

        matrixStackTop++;
    }

    float[] popMatrix()
    {
        // code
        if (matrixStackTop < 0)
        {
            System.out.println("ERROR : Matrix Stack is empty\n");

            uninitialize();
        }

        // matrixStack[matrixStackTop] = Matrix.setIdentityM(matrixStack[matrixStackTop],0);

        Matrix.setIdentityM(matrixStack[matrixStackTop],0);

        matrixStackTop--;

        float[] matrix = matrixStack[matrixStackTop].clone();

        return matrix;
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



