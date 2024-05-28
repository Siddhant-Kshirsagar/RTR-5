
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

var modelMatrixUniform = 0;
var viewMatrixUniform = 0;
var projectionMatrixUniform = 0;

var lightAmbientUniform = 0;
var lightDiffuseUniform = 0;
var lightSpecularUniform = 0;
var lightPositionUniform = 0;

var materialDiffuseUniform = 0;  
var materialAmbientUniform = 0;
var materialSpecularUniform = 0;
var materialShininessUniform = 0;

var keyPressedUniform = 0;

var lightAmbient = new Float32Array([ 0.1,0.1,0.1 ]);
var lightDiffuse = new Float32Array([ 1.0,1.0,1.0 ]);
var lightSpecular = new Float32Array([ 1.0,1.0,1.0 ]);
var lightPosition = new Float32Array([ 100.0,100.0,100.0,1.0 ]);

var materialAmbient = new Float32Array([ 0.0,0.0,0.0 ]);
var materialDiffuse = new Float32Array([ 0.5,0.2,0.7 ]);
var materialSpecular = new Float32Array([ 0.7, 0.7, 0.7 ]);
var materialShininess = 128.0;

var bLightingEnable = false;

var mvpMatrixUniform;

var perspectiveProjectionMatrix;


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
        case 76:
        case 108:
            if (bLightingEnable == false) {
                bLightingEnable = true;
            }
            else {
                bLightingEnable = false;
            }
            break;

        case 70: // ascii for F
        case 102: // ascii for f
            toggleFullscreen();
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
        "precision highp float;" +
        "in vec4 aPosition;"+
        "in vec3 aNormal;"+
        "uniform mat4 uModelMatrix;"+
        "uniform mat4 uViewMatrix;"+
        "uniform mat4 uProjectionMatrix;"+
        "uniform vec4 uLightPosition;"+
        "uniform int uKeyPressed;"+
        "out vec3 oTransformedNormals;"+
        "out vec3 oLightDirection;"+
        "out vec3 oViewerVector;"+
        "void main(void)"+
        "{"+
        "if(uKeyPressed == 1)"+
        "{"+
        "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;"+
        "oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;"+
        "oLightDirection = vec3(uLightPosition-eyeCoordinates);"+
        "oViewerVector = -eyeCoordinates.xyz;"+
        "}"+
        "else"+
        "{"+
        "oTransformedNormals = vec3(0.0,0.0,0.0);"+
        "oLightDirection = vec3(0.0,0.0,0.0);"+
        "oViewerVector = vec3(0.0,0.0,0.0);"+
        "}"+
        "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"+
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
        "in vec3 oTransformedNormals;"+
        "in vec3 oLightDirection;"+
        "in vec3 oViewerVector;"+
        "uniform vec3 uLightAmbient;"+
        "uniform vec3 uLightDiffuse;"+
        "uniform vec3 uLightSpecular;"+
        "uniform vec3 uMaterialAmbient;"+
        "uniform vec3 uMaterialDiffuse;"+
        "uniform vec3 uMaterialSpecular;"+
        "uniform float uMaterialShineness;"+
        "uniform highp int uKeyPressed;"+
        "out vec4 FragColor;"+
        "void main(void)"+
        "{"+
        "vec3 phongADSLight;"+
        "if(uKeyPressed == 1)"+
        "{"+
        "vec3 normalizeTransformedNormals = normalize(oTransformedNormals);"+
        "vec3 normalizeLightDirection = normalize(oLightDirection);"+
        "vec3 normalizeViewerVector = normalize(oViewerVector);"+
        "vec3 lightAmbient = uLightAmbient * uMaterialAmbient;"+
        "vec3 lightDiffuse = uLightDiffuse * uMaterialDiffuse * max(dot(normalizeLightDirection,normalizeTransformedNormals),0.0);"+
        "vec3 reflectionVector = reflect(-normalizeLightDirection,normalizeTransformedNormals);"+
        "vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector,normalizeViewerVector),0.0),uMaterialShineness);"+
        "phongADSLight = lightAmbient + lightDiffuse + lightSpecular;"+
        "}"+
        "else"+
        "{"+
        "phongADSLight = vec3(1.0,1.0,1.0);"+
        "}"+
        "FragColor = vec4(phongADSLight,1.0);"+
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

    gl.bindAttribLocation(shaderProgramObject, VertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

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

    // get shader uniform location
    modelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uModelMatrix");
    viewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uViewMatrix");
    projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uProjectionMatrix");
    lightAmbientUniform = gl.getUniformLocation(shaderProgramObject, "uLightAmbient");
    lightDiffuseUniform = gl.getUniformLocation(shaderProgramObject, "uLightDiffuse");
    lightSpecularUniform = gl.getUniformLocation(shaderProgramObject, "uLightSpecular");
    lightPositionUniform = gl.getUniformLocation(shaderProgramObject, "uLightPosition");
    materialAmbientUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialAmbient");
    materialDiffuseUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialDiffuse");
    materialSpecularUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialSpecular");
    materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialShineness");
    keyPressedUniform = gl.getUniformLocation(shaderProgramObject, "uKeyPressed");

    sphere = new Mesh();
	makeSphere(sphere, 2.0, 50, 50);

    // depth initialization
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // set clear color
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    // initialize projection matrix
    perspectiveProjectionMatrix = mat4.create();
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
    var modelMatrix = mat4.create();
    var viewMatrix = mat4.create();

    var translationMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);

    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

    // push above mvp(model view projection) into vertex shader's mvp uniform
    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
    {
        gl.uniform1i(keyPressedUniform, 1);

        gl.uniform3fv(lightAmbientUniform, lightAmbient);
        gl.uniform3fv(lightDiffuseUniform, lightDiffuse);
        gl.uniform3fv(lightSpecularUniform, lightSpecular);
        gl.uniform4fv(lightPositionUniform, lightPosition);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShininess);
    }
    else
    {
        gl.uniform1i(keyPressedUniform, 0);
    }

    sphere.draw();

    gl.useProgram(null);

    // update for animation 
    update();
    // do the double buffering
    requestAnimationFrame(display, canvas);
}

function update() {
    // code
}

function uninitialize() {
    //code

    if (shaderProgramObject) {

        gl.useProgram(shaderProgramObject);

        var shaderObject = gl.getAttachedShader();

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
