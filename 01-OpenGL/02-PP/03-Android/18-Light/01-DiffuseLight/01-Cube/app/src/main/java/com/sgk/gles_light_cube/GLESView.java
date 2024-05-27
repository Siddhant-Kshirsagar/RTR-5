package com.sgk.gles_light_cube;

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

import com.sgk.gles_light_cube.VertexAttributesEnum;

import javax.microedition.khronos.egl.EGLConfig;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,OnDoubleTapListener,OnGestureListener
{
    // global variable
    private GestureDetector gestureDetector;

    // OpenGL related variable 
    private int shaderProgramObject = 0;
    private int[] vao_cube = new int[1];
    private int[] vbo_positionCube = new int[1];
    private int[] vbo_normalCube = new int[1];

    private int modelViewMatrixUniform = 0;
    private int projectionMatrixUniform = 0;
    private int ldUniform = 0;
    private int kdUniform = 0;
    private int lightPositionUniform = 0;
    private int keyPressedUniform = 0;

    private float[] lightDiffuse = {1.0f,1.0f,1.0f,1.0f};
    private float[] materialDiffuse = {0.5f,0.5f,0.5f,1.0f};
    private float[] lightPosition = {0.0f,0.0f,2.0f,1.0f};

    private float[] perspectiveProjectionMatrix = new float[16];

    private float cAngle = 0.0f;
    private boolean bAnimationEnable = false;
    private boolean bLightingEnable = false;


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
        if (bLightingEnable == false)
        {
            bLightingEnable = true;
        }
        else
        {
            bLightingEnable = false;
        }
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
            "uniform vec3 uld;"+
            "uniform vec3 ukd;"+
            "uniform vec4 uLightPosition;"+
            "uniform int uKeyPressed;"+
            "out vec3 oDiffuseLight;"+
            "void main(void)"+
            "{"+
            "if(uKeyPressed == 1)"+
            "{"+
            "vec4 eyePosition = uModelViewMatrix * aPosition;"+
            "mat3 normalMatrix = mat3(transpose(inverse(uModelViewMatrix)));"+
            "vec3 n = normalize(normalMatrix * aNormal);"+
            "vec3 s = normalize(vec3(uLightPosition-eyePosition));"+
            "oDiffuseLight = uld * ukd * dot(s,n);"+
            "}"+
            "else"+
            "{"+
            "oDiffuseLight = vec3(0.0,0.0,0.0);"+
            "}"+
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
            "in vec3 oDiffuseLight;"+
            "uniform highp int uKeyPressed;"+
            "out vec4 FragColor;"+
            "void main(void)"+
            "{"+
            "if(uKeyPressed == 1)"+
            "{"+
            "FragColor = vec4(oDiffuseLight,1.0);"+
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
        modelViewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject,"uModelViewMatrix");

        projectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject,"uProjectionMatrix");

        ldUniform = GLES32.glGetUniformLocation(shaderProgramObject,"uld");

        kdUniform = GLES32.glGetUniformLocation(shaderProgramObject,"ukd");

        lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject,"uLightPosition");

        keyPressedUniform = GLES32.glGetUniformLocation(shaderProgramObject,"uKeyPressed");

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

        final float[] cube_Normals = {
        // top
        // front surface
		0.0f,  0.0f,  1.0f, // top-right of front
		0.0f,  0.0f,  1.0f, // top-left of front
		0.0f,  0.0f,  1.0f, // bottom-left of front
		0.0f,  0.0f,  1.0f, // bottom-right of front

		// right surface
		1.0f,  0.0f,  0.0f, // top-right of right
		1.0f,  0.0f,  0.0f, // top-left of right
		1.0f,  0.0f,  0.0f, // bottom-left of right
		1.0f,  0.0f,  0.0f, // bottom-right of right

		// back surface
		0.0f,  0.0f, -1.0f, // top-right of back
		0.0f,  0.0f, -1.0f, // top-left of back
		0.0f,  0.0f, -1.0f, // bottom-left of back
		0.0f,  0.0f, -1.0f, // bottom-right of back

		// left surface
	   -1.0f,  0.0f,  0.0f, // top-right of left
	   -1.0f,  0.0f,  0.0f, // top-left of left
	   -1.0f,  0.0f,  0.0f, // bottom-left of left
	   -1.0f,  0.0f,  0.0f, // bottom-right of left

		// top surface
		0.0f,  1.0f,  0.0f, // top-right of top
		0.0f,  1.0f,  0.0f, // top-left of top
		0.0f,  1.0f,  0.0f, // bottom-left of top
		0.0f,  1.0f,  0.0f, // bottom-right of top

		// bottom surface
		0.0f, -1.0f,  0.0f, // top-right of bottom
		0.0f, -1.0f,  0.0f, // top-left of bottom
		0.0f, -1.0f,  0.0f, // bottom-left of bottom
		0.0f, -1.0f,  0.0f, // bottom-right of bottom
    };

        // vao_cube(vertex array object)
        GLES32.glGenVertexArrays(1,vao_cube,0);

        GLES32.glBindVertexArray(vao_cube[0]);

        GLES32.glGenBuffers(1,vbo_positionCube,0);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_positionCube[0]);

        // prepare triangle vertices array for glBufferData
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cube_position.length * 4);

        byteBuffer.order(ByteOrder.nativeOrder());

        FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

        positionBuffer.put(cube_position);
        positionBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,cube_position.length * 4,positionBuffer,GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION,3,GLES32.GL_FLOAT,false,0,0);

        GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_POSITION);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

        // vbo normal
        GLES32.glGenBuffers(1,vbo_normalCube,0);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_normalCube[0]);

        // prepare triangle vertices array for glBufferData
        byteBuffer = ByteBuffer.allocateDirect(cube_Normals.length * 4);

        byteBuffer.order(ByteOrder.nativeOrder());

        FloatBuffer normalBuffer = byteBuffer.asFloatBuffer();

        normalBuffer.put(cube_Normals);
        normalBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,cube_Normals.length * 4,normalBuffer,GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL,3,GLES32.GL_FLOAT,false,0,0);

        GLES32.glEnableVertexAttribArray(VertexAttributesEnum.AMC_ATTRIBUTE_NORMAL);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

        GLES32.glBindVertexArray(0);

        // for depth enable setting
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // clear color
        GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

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
        float[] modelViewMatrix = new float[16];
        float[] translationMatrix = new float[16];
        float[] rotationMatrix = new float[16];
        float[] rotationMatrix_X = new float[16];
        float[] rotationMatrix_Y = new float[16];
        float[] rotationMatrix_Z = new float[16];

        Matrix.setIdentityM(modelViewMatrix,0);
        Matrix.setIdentityM(translationMatrix,0);
        Matrix.setIdentityM(rotationMatrix,0);
        Matrix.setIdentityM(rotationMatrix_X,0);
        Matrix.setIdentityM(rotationMatrix_Y,0);
        Matrix.setIdentityM(rotationMatrix_Z,0);

        Matrix.translateM(translationMatrix,0,0.0f,0.0f,-5.0f);
        Matrix.rotateM(rotationMatrix_X,0,cAngle,1.0f,0.0f,0.0f);
        Matrix.rotateM(rotationMatrix_Y,0,cAngle,0.0f,1.0f,0.0f);
        Matrix.rotateM(rotationMatrix_Z,0,cAngle,0.0f,0.0f,1.0f);

        Matrix.multiplyMM(rotationMatrix,0,rotationMatrix_X,0,rotationMatrix_Y,0);

        Matrix.multiplyMM(rotationMatrix,0,rotationMatrix,0,rotationMatrix_Z,0);

        Matrix.multiplyMM(modelViewMatrix,0,translationMatrix,0,rotationMatrix,0);

        // push above mvp(model view projection) into vertex shader's mvp uniform
        GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);

        GLES32.glUniformMatrix4fv(projectionMatrixUniform,1,false,perspectiveProjectionMatrix,0);

        if (bLightingEnable == true)
        {
            GLES32.glUniform1i(keyPressedUniform, 1);

            // for light diffuse
            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(lightDiffuse.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            FloatBuffer diffuseBuffer = byteBuffer.asFloatBuffer();

            diffuseBuffer.put(lightDiffuse);
            diffuseBuffer.position(0);

            GLES32.glUniform3fv(ldUniform, 1, diffuseBuffer);

            // for material diffuse
            byteBuffer = ByteBuffer.allocateDirect(materialDiffuse.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            diffuseBuffer = byteBuffer.asFloatBuffer();

            diffuseBuffer.put(materialDiffuse);
            diffuseBuffer.position(0);

            GLES32.glUniform3fv(kdUniform, 1, diffuseBuffer);

            // for light position
            byteBuffer = ByteBuffer.allocateDirect(lightPosition.length * 4);

            byteBuffer.order(ByteOrder.nativeOrder());

            FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();

            positionBuffer.put(lightPosition);
            positionBuffer.position(0);
            GLES32.glUniform4fv(lightPositionUniform, 1, positionBuffer);
        }
        else
        {
            GLES32.glUniform1i(keyPressedUniform, 0);
        }

        // bind with vao_cube
        GLES32.glBindVertexArray(vao_cube[0]);

        GLES32.glDrawArrays( GLES32.GL_TRIANGLE_FAN, 0, 4);
        GLES32.glDrawArrays( GLES32.GL_TRIANGLE_FAN, 4, 4);
        GLES32.glDrawArrays( GLES32.GL_TRIANGLE_FAN, 8, 4);
        GLES32.glDrawArrays( GLES32.GL_TRIANGLE_FAN, 12, 4);
        GLES32.glDrawArrays( GLES32.GL_TRIANGLE_FAN, 16, 4);
        GLES32.glDrawArrays( GLES32.GL_TRIANGLE_FAN, 20, 4);

        // unbind vao_cube
        GLES32.glBindVertexArray(0);

        GLES32.glUseProgram(0);

        
        // update for animation
        if(bAnimationEnable)
        {
            cAngle = cAngle + 1.0f;
            if(cAngle >= 360.0f)
            {
                cAngle = cAngle - 360.0f;
            }
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

        // delete vbo_normalCube 
        if(vbo_normalCube[0]!=0)
        {
            GLES32.glDeleteBuffers(1,vbo_normalCube,0);
            vbo_normalCube[0] = 0;
        }

        // delete vbo_positionCube 
        if(vbo_positionCube[0]!=0)
        {
            GLES32.glDeleteBuffers(1,vbo_positionCube,0);
            vbo_positionCube[0] = 0;
        }

        // delete vao_cube 
        if(vao_cube[0]!=0)
        {
            GLES32.glDeleteVertexArrays(1,vao_cube,0);
            vao_cube[0] = 0;
        }
    
    }
}



