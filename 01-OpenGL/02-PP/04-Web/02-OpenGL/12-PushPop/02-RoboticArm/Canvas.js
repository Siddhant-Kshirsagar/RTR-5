
// global variable 
var canvas = null;
var gl = null;
var gbFullScreen = false;
var canvas_original_width;
var canvas_original_height;

// WebGL related variable
const VertexAttributeEnum =
{
    AMC_ATTRIBUTE_POSITION: 0,
    AMC_ATTRIBUTE_COLOR: 1,
    AMC_ATTRIBUTE_TEXCOORD: 2,
    AMC_ATTRIBUTE_NORMAL:3,
};

var shaderProgramObject = null;

var sphere = null;

var mvpMatrixUniform;
var colorUniform;

var perspectiveProjectionMatrix;

var matrixStackTop = -1;

var MODEL_VIEW_MATRIX_STACK = 32;

var matrixStack = [];

var sholder = 0;
var elbow = 0;
var wrist = 0;


var requestAnimationFrame =
    window.requestAnimationFrame || // google chrome
    window.webkitRequestAnimationFrame || // safari
    window.mozRequestAnimationFrame || // firefox
    window.oRequestAnimationFrame || // opera
    window.msRequestAnimationFrame; // edge

// main function
function main()
{
    // get canvas 
    canvas = document.getElementById("sgk");

    if (canvas == null) {
        console.log("Getting canvas failed\n");
    }
    else{
        console.log("Getting canvas succeeded.\n");
    }

    // set canvas width and height for future use
    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;

    // register for event
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);
    window.addEventListener("resize", resize, false);

    // initialize 
    initialize();

    // resize 
    resize();

    // display
    display();

}

function keyDown(event) {
    // code
    switch (event.keyCode) {
        case 81: // ascii for Q
        case 113: // ascii for q
            uninitialize();
            window.close(); // exit 
            break;

        case 70: // ascii for F
        case 102: // ascii for f
            toggleFullscreen();
            break;

        case 83:
        case 115:
            sholder = (sholder + 3) % 360;
            break;

        case 69:
        case 101:
            elbow = (elbow + 3) % 360;
            break;

        case 87:
        case 119:
            wrist = (wrist + 3) % 360;
            break;

    }
}

function mouseDown() {
    // code

}

function toggleFullscreen() {

    var fullscreen_element = document.fullscreenElement ||
                                document.webkitFullscreenElement ||
                                document.mozFullScreenElement ||
                                document.msFullscreenElement ||
                                null;

    // if not full screen
    if (fullscreen_element == null) {
        if (canvas.requestFullscreen) {
            canvas.requestFullscreen();
        }
        else if (canvas.webkitFullscreenElement) {
            canvas.webkitFullscreenElement();
        }
        else if (canvas.mozRequestFullScreen) {
            canvas.mozRequestFullScreen();
        }
        else if (canvas.msRequestFullscreen) {
            canvas.msRequestFullscreen();
        }
        gbFullScreen = true;
    }
    else // if not full screen
    {
        if (document.exitFullscreen) {
            document.exitFullscreen();
        }
        else if (document.webkitExitFullscreen) {
            document.webkitExitFullscreen();
        }
        else if (document.mozCancelFullScreen) {
            document.mozCancelFullScreen();
        }
        else if (document.msExitFullscreen) {
            document.msExitFullscreen();
        }
        gbFullScreen = false;
    }
}

function initialize() {
    // code
    gl = canvas.getContext("webgl2");
    if (gl == null) {
        console.log("Getting WebGL2 context failed.\n");
    }
    else {
        console.log("Getting WebGL2 context succeeded.\n");
    }

    // set WebGL2 context's viewportWidth and viewportHeight of window
    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;

    // vertex shader
    var vertexShaderSourceCode =
        "#version 300 es" +
        "\n" +
        "uniform mat4 uMVPMatrix;" +
        "in vec4 aPosition;" +
        "void main(void)" +
        "{" +
        "gl_Position= uMVPMatrix * aPosition;" +
        "}";

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject);

        if (error.length > 0) {
            var log = "Vertex Shader Compilation Error : " + error;
            alert(log);
            uninitialize();
        }
    }
    else {
        console.log("Vertex Shader Compile Successfully.\n");
    }

    // fragment shader
    var fragmentShaderSourceCode =
        "#version 300 es" +
        "\n" +
        "precision highp float;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = vec4(0.8,0.6,0.4,1.0);" +
        "}";

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject);

        if (error.length > 0) {
            var log = "Fragment Shader Compilation Error : " + error;
            alert(log);
            uninitialize();
        }
    }
    else {
        console.log("Fragment Shader Compile Successfully.\n");
    }

    // shader program
    shaderProgramObject = gl.createProgram();

    gl.attachShader(shaderProgramObject, vertexShaderObject);
    gl.attachShader(shaderProgramObject, fragmentShaderObject);

    gl.bindAttribLocation(shaderProgramObject, VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.linkProgram(shaderProgramObject);

    if (gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject);

        if (error.length > 0) {
            var log = "Shader Program Linking Error : " + error;
            alert(log);
            uninitialize();
        }
    }
    else {
        console.log("Shader Program linked successfully.\n");
    }

    // get uniform
    mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uMVPMatrix");


    sphere = new Mesh();
	makeSphere(sphere, 1.0, 30, 30);

    // depth initialization
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // set clear color
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    // initialize projection matrix
    perspectiveProjectionMatrix = mat4.create();

    initiailzeMatrixStack();
}

function resize() {
    // code
    if (gbFullScreen == true) {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    // set viewport 
    gl.viewport(0, 0, canvas.width, canvas.height);

    // set projection
    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width)/parseFloat(canvas.height), 0.1, 100.0);
}

function display() {
    // code

    gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    // transformation
    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();
    var translationMatrix = mat4.create();
    var rotationMatrix = mat4.create();
    var modelMatrix = mat4.create();
    var scaleMatrix = mat4.create();


    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -15.0]);

    mat4.multiply(modelMatrix,modelMatrix,translationMatrix);
   
    pushMatrix(modelMatrix);
    {
        // do transformation for arm
        mat4.rotateZ(rotationMatrix,rotationMatrix,degToRad(sholder));

        translationMatrix = mat4.create();
        mat4.translate(translationMatrix, translationMatrix, [1.0, 0.0, 0.0]);

        mat4.multiply(modelMatrix,modelMatrix,rotationMatrix);

        mat4.multiply(modelMatrix,modelMatrix,translationMatrix);
    
        pushMatrix(modelMatrix);
        {
            scaleMatrix = mat4.create();
            mat4.scale(scaleMatrix,scaleMatrix,[2.0,0.5,1.0]);
            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);
            
            mat4.multiply(modelViewMatrix, modelViewMatrix, modelMatrix);

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

            sphere.draw();
        }
        modelMatrix = popMatrix();

        // do transformation for elbow
        translationMatrix = mat4.create();
        mat4.translate(translationMatrix, translationMatrix, [1.0, 0.0, 0.0]);

        rotationMatrix = mat4.create();
        mat4.rotateZ(rotationMatrix,rotationMatrix,degToRad(elbow));

        mat4.multiply(modelMatrix,modelMatrix,translationMatrix);

        mat4.multiply(modelMatrix,modelMatrix,rotationMatrix);

        translationMatrix = mat4.create();
        mat4.translate(translationMatrix, translationMatrix, [1.0, 0.0, 0.0]);

        mat4.multiply(modelMatrix,modelMatrix,translationMatrix);
    
        pushMatrix(modelMatrix);
        {
            scaleMatrix = mat4.create();
            mat4.scale(scaleMatrix,scaleMatrix,[2.0,0.5,1.0]);
            
            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);
            
            mat4.multiply(modelViewMatrix, modelViewMatrix, modelMatrix);

            mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

            gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

            sphere.draw();
        }
        modelMatrix = popMatrix();

    }
    modelMatrix = popMatrix();

    gl.useProgram(null);

    // update for animation 
    update();
    // do the double buffering
    requestAnimationFrame(display, canvas);
}

function update() {
    // code
}

function degToRad(degrees) {
    return ((degrees * Math.PI) / 180.0);
}

function initiailzeMatrixStack()
{
    // code  
    matrixStackTop = 0;
    for(var i = 0; i < MODEL_VIEW_MATRIX_STACK; i++)
    {
        matrixStack[i]  = mat4.create();
    }
  
}

function pushMatrix(matrix) {
    matrixStack.push(matrix.slice(0));
}

function popMatrix() {
    return matrixStack.pop();
}

// function pushMatrix(matrix)
// {
//     if(matrixStackTop >= MODEL_VIEW_MATRIX_STACK - 1)
//     {
//         console.log("Error - Execeeded matrix stack limit \n");
//         uninitialize();
//     }
//     else
//     {
//         matrixStack[matrixStackTop] = matrix;
//         matrixStackTop++;
//     }
// }

// function popMatrix()
// {
//     if(matrixStackTop<0)
//     {
//         console.log("Error - Matrix stack is empty\n");
//         uninitialize();
//     }
//     else
//     {
//         matrixStack[matrixStackTop] = mat4.create();
//         matrixStackTop--;
    
//         // var matrix = matrixStack[matrixStackTop];
    
//         return(matrixStack[matrixStackTop]);
//     }

// }

function uninitialize() {
    //code

    if (shaderProgramObject) {

        gl.useProgram(shaderProgramObject);

        var shaderObject = gl.getAttachedShaders(shaderProgramObject);

        if (shaderObject && shaderObject.length > 0) {
            for (let i = 0; i < shaderObject.length; i++) {

                gl.detachShader(shaderProgramObject, shaderObject[i]);

                gl.deleteShader(shaderObject[i]);

                shaderObject[i] = null;
            }
        }

        gl.useProgram(null);

        gl.deleteProgram(shaderProgramObject);

        shaderProgramObject = null;
    }

    if (sphere)
    {
        sphere.deallocate();
        sphere = null;
    }
}
