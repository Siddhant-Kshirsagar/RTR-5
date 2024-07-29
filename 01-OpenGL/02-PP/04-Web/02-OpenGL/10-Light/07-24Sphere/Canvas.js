
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
var lightPosition = new Float32Array([ 0.0,0.0,5.0,1.0 ]);

var materialAmbient = new Float32Array([ 0.0,0.0,0.0 ]);
var materialDiffuse = new Float32Array([ 1.0,1.0,1.0 ]);
var materialSpecular = new Float32Array([ 1.0, 1.0, 1.0 ]);
var materialShininess = 50.0;

var lightAngle = 0.0;

var rotationAxis = 0;

var currentWinWidth = 0;
var currentWinHeight = 0;

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
        case 88:
        case 120:
            rotationAxis = 1;
            lightAngle = 0;
        break;

        case 89:
        case 121:
            rotationAxis = 2;
            lightAngle = 0;
        break;

        case 90:
        case 122:
            rotationAxis = 3;
            lightAngle = 0;
        break;

        case 70: // ascii for F
        case 102: // ascii for f
            toggleFullscreen();
            break;

        default:
            rotationAxis = 0;
            lightAngle = 0;
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
    gl.clearColor(0.75, 0.75, 0.75, 1.0);

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

    currentWinWidth = canvas.width;
    currentWinHeight = canvas.height;

    // set viewport 
    gl.viewport(0, 0, canvas.width, canvas.height);

    // set projection
    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width)/parseFloat(canvas.height), 0.1, 100.0);
}

function display() {

    // variable declaration
    var row1_Y, row2_Y, row3_Y, row4_Y, row5_Y, row6_Y;
    var col1_X, col2_X, col3_X, col4_X;

    var difference_X = currentWinWidth / 6; // for column
    var difference_Y = currentWinHeight / 6; // for rows

    col1_X = 0;
    col2_X = col1_X + difference_X;
    col3_X = col2_X + difference_X;
    col4_X = col3_X + difference_X;

    row1_Y = currentWinHeight - difference_Y;
    row2_Y = row1_Y - difference_Y;
    row3_Y = row2_Y - difference_Y;
    row4_Y = row3_Y - difference_Y;
    row5_Y = row4_Y - difference_Y;
    row6_Y = row5_Y - difference_Y;
    // code

    gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    // 1st column
    {
        // first column of Precious Stone
        // emerad
        // ambient material
        materialAmbient[0] = 0.0215; // r
        materialAmbient[1] = 0.1745; // g
        materialAmbient[2] = 0.0215; // b
        materialAmbient[3] = 1.0; // a

        // diffuse material
        materialDiffuse[0] = 0.07568; // r
        materialDiffuse[1] = 0.61424; // g
        materialDiffuse[2] = 0.07568; // b
        materialDiffuse[3] = 1.0; // a

        // specular material
        materialSpecular[0] = 0.0633; // r
        materialSpecular[1] = 0.727811; // g
        materialSpecular[2] = 0.633; // b
        materialSpecular[3] = 1.0; // a

        // shininess
        materialShininess = 0.6 * 128;

        gl.viewport(col1_X, row1_Y, difference_X, difference_Y);

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
    }

    {
        // 2nd sphere on 1st column, jade
        // ambient material
        materialAmbient[0] = 0.135; // r
        materialAmbient[1] = 0.2225; // g
        materialAmbient[2] = 0.1575; // b
        materialAmbient[3] = 1.0; // a

        // diffuse material
        materialDiffuse[0] = 0.54; // r
        materialDiffuse[1] = 0.89; // g
        materialDiffuse[2] = 0.63; // b
        materialDiffuse[3] = 1.0; // a

        // specular material
        materialSpecular[0] = 0.316228; // r
        materialSpecular[1] = 0.316228; // g
        materialSpecular[2] = 0.316228; // b
        materialSpecular[3] = 1.0; // a

        // shininess
        materialShininess = 0.1 * 128; 

        gl.viewport(col1_X, row2_Y, difference_X, difference_Y);

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
    }

    {
        // 3rd sphere on 1st column, obsidian
        // ambient material
        materialAmbient[0] = 0.05375; // r
        materialAmbient[1] = 0.05; // g
        materialAmbient[2] = 0.06625; // b
        materialAmbient[3] = 1.0; // a

        // diffuse material
        materialDiffuse[0] = 0.18275; // r
        materialDiffuse[1] = 0.17; // g
        materialDiffuse[2] = 0.22525; // b
        materialDiffuse[3] = 1.0; // a

        // specular material
        materialSpecular[0] = 0.332741; // r
        materialSpecular[1] = 0.328634; // g
        materialSpecular[2] = 0.346435; // b
        materialSpecular[3] = 1.0; // a

        // shininess
        materialShininess = 0.3 * 128;

        gl.viewport(col1_X, row3_Y, difference_X, difference_Y);

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
    }

    {
        // 4th sphere on 1st column, pearl
        // ambient material
        materialAmbient[0] = 0.25; // r
        materialAmbient[1] = 0.20725; // g
        materialAmbient[2] = 0.20725; // b
        materialAmbient[3] = 1.0; // a

        // diffuse material
        materialDiffuse[0] = 1.0; // r
        materialDiffuse[1] = 0.829; // g
        materialDiffuse[2] = 0.829; // b
        materialDiffuse[3] = 1.0; // a

        // specular material
        materialSpecular[0] = 0.296648; // r
        materialSpecular[1] = 0.296648; // g
        materialSpecular[2] = 0.296648; // b
        materialSpecular[3] = 1.0; // a

        // shininess
        materialShininess = 0.088 * 128;

        gl.viewport(col1_X, row4_Y, difference_X, difference_Y);

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
    }

    {
        // 5th sphere on 1st column, ruby
		// ambient material
		materialAmbient[0] = 0.1745; // r
		materialAmbient[1] = 0.01175; // g
		materialAmbient[2] = 0.01175; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.61424; // r
		materialDiffuse[1] = 0.04136; // g
		materialDiffuse[2] = 0.04136; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.727811; // r
		materialSpecular[1] = 0.626959; // g
		materialSpecular[2] = 0.626959; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.6 * 128;

        gl.viewport(col1_X, row5_Y, difference_X, difference_Y);

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
    }

    {
        // 6th shpere on 1st column, turquoise
		// ambient material
		materialAmbient[0] = 0.1; // r
		materialAmbient[1] = 0.18725; // g
		materialAmbient[2] = 0.1745; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.396; // r
		materialDiffuse[1] = 0.74151; // g
		materialDiffuse[2] = 0.69102; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.297254; // r
		materialSpecular[1] = 0.30829; // g
		materialSpecular[2] = 0.306678; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.1 * 128;

        gl.viewport(col1_X, row6_Y, difference_X, difference_Y);

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
    }

    // 2nd column
    {
        // 2nd column metal
		// 1st sphere on 2nd column, brass
		// ambient material
		materialAmbient[0] = 0.329412; // r
		materialAmbient[1] = 0.223529; // g
		materialAmbient[2] = 0.027451; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.780392; // r
		materialDiffuse[1] = 0.568627; // g
		materialDiffuse[2] = 0.113725; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.992157; // r
		materialSpecular[1] = 0.941176; // g
		materialSpecular[2] = 0.807843; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.21794872 * 128;

        gl.viewport(col2_X, row1_Y, difference_X, difference_Y);

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
    }

    {
        // 2nd sphere
		// ambient material
		materialAmbient[0] = 0.2125; // r
		materialAmbient[1] = 0.1275; // g
		materialAmbient[2] = 0.054; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.714; // r
		materialDiffuse[1] = 0.4284; // g
		materialDiffuse[2] = 0.18144; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.393548; // r
		materialSpecular[1] = 0.271906; // g
		materialSpecular[2] = 0.166721; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.2 * 128;

        gl.viewport(col2_X, row2_Y, difference_X, difference_Y);

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
    }

    {
        // 3rd sphere on 2nd column, chrome
		// ambient material
		materialAmbient[0] = 0.25; // r
		materialAmbient[1] = 0.25; // g
		materialAmbient[2] = 0.25; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.4; // r
		materialDiffuse[1] = 0.4; // g
		materialDiffuse[2] = 0.4; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.774597; // r
		materialSpecular[1] = 0.774597; // g
		materialSpecular[2] = 0.774597; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.6 * 128;

        gl.viewport(col2_X, row3_Y, difference_X, difference_Y);

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
    }

    {
        // 4th sphere on 2nd column, copper
		// ambient material
		materialAmbient[0] = 0.19125; // r
		materialAmbient[1] = 0.0735; // g
		materialAmbient[2] = 0.0225; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.7038; // r
		materialDiffuse[1] = 0.27048; // g
		materialDiffuse[2] = 0.0828; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.256777; // r
		materialSpecular[1] = 0.137622; // g
		materialSpecular[2] = 0.086014; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.1 * 128;

        gl.viewport(col2_X, row4_Y, difference_X, difference_Y);

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
    }

    {
        // 5th sphere on 2nd column, gold
		// ambient material
		materialAmbient[0] = 0.24725; // r
		materialAmbient[1] = 0.1995; // g
		materialAmbient[2] = 0.0745; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.75164; // r
		materialDiffuse[1] = 0.60648; // g
		materialDiffuse[2] = 0.22648; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.628281; // r
		materialSpecular[1] = 0.555802; // g
		materialSpecular[2] = 0.366065; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.4 * 128;

        gl.viewport(col2_X, row5_Y, difference_X, difference_Y);

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
    }

    {
        // 6th sphere on 2nd column, silver
		// ambient material
		materialAmbient[0] = 0.19225; // r
		materialAmbient[1] = 0.19225; // g
		materialAmbient[2] = 0.19225; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.50754; // r
		materialDiffuse[1] = 0.50754; // g
		materialDiffuse[2] = 0.50754; // b
		materialDiffuse[3] = 1.0; // a


		// specular material
		materialSpecular[0] = 0.508273; // r
		materialSpecular[1] = 0.508273; // g
		materialSpecular[2] = 0.508273; // b
		materialSpecular[3] = 1.0; // a


		// shininess
		materialShininess = 0.4 * 128;

        gl.viewport(col2_X, row6_Y, difference_X, difference_Y);

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
    }

    // 3rd column
    {
        // 1st sphere on 3rd column, black
		// ambient material
		materialAmbient[0] = 0.0; // r
		materialAmbient[1] = 0.0; // g
		materialAmbient[2] = 0.0; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.01; // r
		materialDiffuse[1] = 0.01; // g
		materialDiffuse[2] = 0.01; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.50; // r
		materialSpecular[1] = 0.50; // g
		materialSpecular[2] = 0.50; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.25 * 128;

        gl.viewport(col3_X, row1_Y, difference_X, difference_Y);

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
    }

    {
        // 2nd sphere on 3rd column, cyan 
		// ambient material
		materialAmbient[0] = 0.0; // r
		materialAmbient[1] = 0.1; // g
		materialAmbient[2] = 0.06; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.0; // r
		materialDiffuse[1] = 0.50980392; // g
		materialDiffuse[2] = 0.50980392; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.50196078; // r
		materialSpecular[1] = 0.50196078; // g
		materialSpecular[2] = 0.50196078; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.25 * 128;

        gl.viewport(col3_X, row2_Y, difference_X, difference_Y);

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
    }

    {
        // 3rd sphere on 2nd column, green
		// ambient material
		materialAmbient[0] = 0.0; // r
		materialAmbient[1] = 0.0; // g
		materialAmbient[2] = 0.0; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.1; // r
		materialDiffuse[1] = 0.35; // g
		materialDiffuse[2] = 0.1; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.45; // r
		materialSpecular[1] = 0.55; // g
		materialSpecular[2] = 0.45; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.25 * 128;

        gl.viewport(col3_X, row3_Y, difference_X, difference_Y);

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
    }

    {
        // 4th sphere on 3rd column, red
		// ambient material
		materialAmbient[0] = 0.0; // r
		materialAmbient[1] = 0.0; // g
		materialAmbient[2] = 0.0; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.5; // r
		materialDiffuse[1] = 0.0; // g
		materialDiffuse[2] = 0.0; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.7; // r
		materialSpecular[1] = 0.6; // g
		materialSpecular[2] = 0.6; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.25 * 128;

        gl.viewport(col3_X, row4_Y, difference_X, difference_Y);

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
    }

    {
       	// 5th sphere on 3rd column, white
		// ambient material
		materialAmbient[0] = 0.0; // r
		materialAmbient[1] = 0.0; // g
		materialAmbient[2] = 0.0; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.55; // r
		materialDiffuse[1] = 0.55; // g
		materialDiffuse[2] = 0.55; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.70; // r
		materialSpecular[1] = 0.70; // g
		materialSpecular[2] = 0.70; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.25 * 128;

        gl.viewport(col3_X, row5_Y, difference_X, difference_Y);

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
    }

    {
        // 6th sphere on 3rd column, yellow
		// ambient material
		materialAmbient[0] = 0.0; // r
		materialAmbient[1] = 0.0; // g
		materialAmbient[2] = 0.0; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.5; // r
		materialDiffuse[1] = 0.5; // g
		materialDiffuse[2] = 0.0; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.60; // r
		materialSpecular[1] = 0.60; // g
		materialSpecular[2] = 0.50; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.25 * 128;

        gl.viewport(col3_X, row6_Y, difference_X, difference_Y);

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
    }

    // 4th column
    {
       // 1st sphere on 4th column, black
		// ambient material
		materialAmbient[0] = 0.02; // r
		materialAmbient[1] = 0.02; // g
		materialAmbient[2] = 0.02; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.1; // r
		materialDiffuse[1] = 0.1; // g
		materialDiffuse[2] = 0.1; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.4; // r
		materialSpecular[1] = 0.4; // g
		materialSpecular[2] = 0.4; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.078125 * 128;

        gl.viewport(col4_X, row1_Y, difference_X, difference_Y);

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
    }

    {
		// 2nd sphere on 4th column, cyan
		// ambient material
		materialAmbient[0] = 0.0; // r
		materialAmbient[1] = 0.05; // g
		materialAmbient[2] = 0.05; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.4; // r
		materialDiffuse[1] = 0.5; // g
		materialDiffuse[2] = 0.5; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.04; // r
		materialSpecular[1] = 0.7; // g
		materialSpecular[2] = 0.7; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.078125 * 128;

        gl.viewport(col4_X, row2_Y, difference_X, difference_Y);

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
    }

    {
 		// 3rd sphere on 4th column, green
		// ambient material
		materialAmbient[0] = 0.0; // r
		materialAmbient[1] = 0.05; // g
		materialAmbient[2] = 0.0; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.4; // r
		materialDiffuse[1] = 0.5; // g
		materialDiffuse[2] = 0.4; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.04; // r
		materialSpecular[1] = 0.7; // g
		materialSpecular[2] = 0.04; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.078125 * 128;

        gl.viewport(col4_X, row3_Y, difference_X, difference_Y);

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
    }

    {
       // 4th sphere on 4th column, red
		// ambient material
		materialAmbient[0] = 0.05; // r
		materialAmbient[1] = 0.0; // g
		materialAmbient[2] = 0.0; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.5; // r
		materialDiffuse[1] = 0.4; // g
		materialDiffuse[2] = 0.4; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.7; // r
		materialSpecular[1] = 0.04; // g
		materialSpecular[2] = 0.04; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.078125 * 128;

        gl.viewport(col4_X, row4_Y, difference_X, difference_Y);

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
    }

    {
       	// 5th sphere on 4th column, white
		// ambient material
		materialAmbient[0] = 0.05; // r
		materialAmbient[1] = 0.05; // g
		materialAmbient[2] = 0.05; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.5; // r
		materialDiffuse[1] = 0.5; // g
		materialDiffuse[2] = 0.5; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.7; // r
		materialSpecular[1] = 0.7; // g
		materialSpecular[2] = 0.7; // b
		materialSpecular[3] = 1.0; // a


		// shininess
		materialShininess = 0.078125 * 128;

        gl.viewport(col4_X, row5_Y, difference_X, difference_Y);

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
    }

    {
        // 6th sphere on 4th column, yellow
		// ambient material
		materialAmbient[0] = 0.05; // r
		materialAmbient[1] = 0.05; // g
		materialAmbient[2] = 0.0; // b
		materialAmbient[3] = 1.0; // a

		// diffuse material
		materialDiffuse[0] = 0.5; // r
		materialDiffuse[1] = 0.5; // g
		materialDiffuse[2] = 0.4; // b
		materialDiffuse[3] = 1.0; // a

		// specular material
		materialSpecular[0] = 0.7; // r
		materialSpecular[1] = 0.7; // g
		materialSpecular[2] = 0.04; // b
		materialSpecular[3] = 1.0; // a

		// shininess
		materialShininess = 0.078125 * 128;

        gl.viewport(col4_X, row6_Y, difference_X, difference_Y);

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
    }



    gl.useProgram(null);

    // update for animation 
    update();
    // do the double buffering
    requestAnimationFrame(display, canvas);
}

function update() {
    // code

    if (bLightingEnable)
        {
            if(rotationAxis == 1)
            {
                // animating light zero
                lightPosition[0] = 0.0;
                lightPosition[1] = 5.0 * Math.sin(lightAngle);
                lightPosition[2] = 5.0 * Math.cos(lightAngle);
                lightPosition[3] = 1.0;

                lightAngle = lightAngle + 0.05;
                if (lightAngle >= 360.0)
                {
                    lightAngle = lightAngle - 360.0;
                }
            }

            if(rotationAxis == 2)
            {
                // animating light zero
                lightPosition[0] = 5.0 * Math.sin(lightAngle);
                lightPosition[1] = 0.0;
                lightPosition[2] = 5.0 * Math.cos(lightAngle);
                lightPosition[3] = 1.0;

                lightAngle = lightAngle + 0.05;
                if (lightAngle >= 360.0)
                {
                    lightAngle = lightAngle - 360.0;
                }
            }
        
            if(rotationAxis == 3)
            {
                // animating light zero
                lightPosition[0] = 5.0 * Math.sin(lightAngle);
                lightPosition[1] = 5.0 * Math.cos(lightAngle);
                lightPosition[2] = 0.0;
                lightPosition[3] = 1.0;

                lightAngle = lightAngle + 0.05;
                if (lightAngle >= 360.0)
                {
                    lightAngle = lightAngle - 360.0;
                }
            }
        
        }
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
