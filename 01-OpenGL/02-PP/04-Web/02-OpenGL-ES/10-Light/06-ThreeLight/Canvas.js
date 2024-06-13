
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

var shaderProgramObject_PV = null;
var shaderProgramObject_PF = null;

var sphere = null;

// per vertex
var modelMatrixUniform_PV = 0;
var viewMatrixUniform_PV = 0;
var projectionMatrixUniform_PV = 0;

var lightAmbientUniform_PV = [3];
var lightDiffuseUniform_PV = [3];
var lightSpecularUniform_PV = [3];
var lightPositionUniform_PV = [3];

var materialDiffuseUniform_PV = 0;  
var materialAmbientUniform_PV = 0;
var materialSpecularUniform_PV = 0;
var materialShininessUniform_PV = 0;

var keyPressedUniform_PV = 0;

// per fragment 
var modelMatrixUniform_PF = 0;
var viewMatrixUniform_PF = 0;
var projectionMatrixUniform_PF = 0;

var lightAmbientUniform_PF = [3];
var lightDiffuseUniform_PF = [3];
var lightSpecularUniform_PF = [3];
var lightPositionUniform_PF = [3];

var materialDiffuseUniform_PF = 0;  
var materialAmbientUniform_PF = 0;
var materialSpecularUniform_PF = 0;
var materialShininessUniform_PF = 0;

var keyPressedUniform_PF = 0;

var lightAmbient = [
    new Float32Array([ 0.0,0.0,0.0 ]),
    new Float32Array([ 0.0,0.0,0.0 ]),
    new Float32Array([ 0.0,0.0,0.0 ])
    ];
var lightDiffuse = [
    new Float32Array([ 1.0,0.0,0.0 ]),
    new Float32Array([ 0.0,1.0,0.0 ]),
    new Float32Array([ 0.0,0.0,1.0 ])
];
var lightSpecular = [
    new Float32Array([ 1.0,0.0,0.0 ]),
    new Float32Array([ 0.0,1.0,0.0 ]),
    new Float32Array([ 0.0,0.0,1.0 ])
];
var lightPosition = [
    new Float32Array([ -2.0,0.0,0.0,1.0 ]),
    new Float32Array([ 0.0,0.0,3.0,1.0 ]),
    new Float32Array([ 2.0,0.0,0.0,1.0 ])
];

var materialAmbient = new Float32Array([ 0.0,0.0,0.0 ]);
var materialDiffuse = new Float32Array([ 1.0,1.0,1.0 ]);
var materialSpecular = new Float32Array([ 1.0, 1.0, 1.0 ]);
var materialShininess = 50.0;

var bLightingEnable = false;

var mvpMatrixUniform;

var perspectiveProjectionMatrix;

var lightAngleZero = 0.0;
var lightAngleOne = 0.0;
var lightAngleTwo = 0.0;

var chooseShader = 'v';



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
            chooseShader = 'f';
            break;
        case 86:
        case 118:
            chooseShader = 'v';
            break;

        case 27:
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

    // per vertex lighting 
    {
        // vertex shader
        var vertexShaderSourceCode_PV =
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
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
            "}";

        var vertexShaderObject_PV = gl.createShader(gl.VERTEX_SHADER);

        gl.shaderSource(vertexShaderObject_PV, vertexShaderSourceCode_PV);

        gl.compileShader(vertexShaderObject_PV);

        if (gl.getShaderParameter(vertexShaderObject_PV, gl.COMPILE_STATUS) == false) {
            var error = gl.getShaderInfoLog(vertexShaderObject_PV);

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
        var fragmentShaderSourceCode_PV =
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
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
            "}";

        var fragmentShaderObject_PV = gl.createShader(gl.FRAGMENT_SHADER);

        gl.shaderSource(fragmentShaderObject_PV, fragmentShaderSourceCode_PV);

        gl.compileShader(fragmentShaderObject_PV);

        if (gl.getShaderParameter(fragmentShaderObject_PV, gl.COMPILE_STATUS) == false) {
            var error = gl.getShaderInfoLog(fragmentShaderObject_PV);

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
        shaderProgramObject_PV = gl.createProgram();

        gl.attachShader(shaderProgramObject_PV, vertexShaderObject_PV);
        gl.attachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

        gl.bindAttribLocation(shaderProgramObject_PV, VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

        gl.bindAttribLocation(shaderProgramObject_PV, VertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

        gl.linkProgram(shaderProgramObject_PV);

        if (gl.getProgramParameter(shaderProgramObject_PV, gl.LINK_STATUS) == false) {
            var error = gl.getProgramInfoLog(shaderProgramObject_PV);

            if (error.length > 0) {
                var log = "Shader Program Linking Error : " + error;
                alert(log);
                uninitialize();
            }_PV
        }
        else {
            console.log("Shader Program linked successfully.\n");
        }

        // get shader uniform location
        modelMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uModelMatrix");
        viewMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uViewMatrix");
        projectionMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uProjectionMatrix");

        lightAmbientUniform_PV[0] = gl.getUniformLocation(shaderProgramObject_PV, "uLightAmbient[0]");
        lightDiffuseUniform_PV[0] = gl.getUniformLocation(shaderProgramObject_PV, "uLightDiffuse[0]");
        lightSpecularUniform_PV[0] = gl.getUniformLocation(shaderProgramObject_PV, "uLightSpecular[0]");
        lightPositionUniform_PV[0] = gl.getUniformLocation(shaderProgramObject_PV, "uLightPosition[0]");

        lightAmbientUniform_PV[1] = gl.getUniformLocation(shaderProgramObject_PV, "uLightAmbient[1]");
        lightDiffuseUniform_PV[1] = gl.getUniformLocation(shaderProgramObject_PV, "uLightDiffuse[1]");
        lightSpecularUniform_PV[1] = gl.getUniformLocation(shaderProgramObject_PV, "uLightSpecular[1]");
        lightPositionUniform_PV[1] = gl.getUniformLocation(shaderProgramObject_PV, "uLightPosition[1]");

        lightAmbientUniform_PV[2] = gl.getUniformLocation(shaderProgramObject_PV, "uLightAmbient[2]");
        lightDiffuseUniform_PV[2] = gl.getUniformLocation(shaderProgramObject_PV, "uLightDiffuse[2]");
        lightSpecularUniform_PV[2] = gl.getUniformLocation(shaderProgramObject_PV, "uLightSpecular[2]");
        lightPositionUniform_PV[2] = gl.getUniformLocation(shaderProgramObject_PV, "uLightPosition[2]");

        materialAmbientUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uMaterialAmbient");
        materialDiffuseUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uMaterialDiffuse");
        materialSpecularUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uMaterialSpecular");
        materialShininessUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uMaterialShineness");
        keyPressedUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uKeyPressed");
    }
    // per fragment lighting 
    {
        // vertex shader
        var vertexShaderSourceCode_PF =
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
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
            "}";

        var vertexShaderObject_PF = gl.createShader(gl.VERTEX_SHADER);

        gl.shaderSource(vertexShaderObject_PF, vertexShaderSourceCode_PF);

        gl.compileShader(vertexShaderObject_PF);

        if (gl.getShaderParameter(vertexShaderObject_PF, gl.COMPILE_STATUS) == false) {
            var error = gl.getShaderInfoLog(vertexShaderObject_PF);

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
        var fragmentShaderSourceCode_PF =
            "#version 300 es" +
            "\n" +
            "precision highp float;" +
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
            "}";

        var fragmentShaderObject_PF = gl.createShader(gl.FRAGMENT_SHADER);

        gl.shaderSource(fragmentShaderObject_PF, fragmentShaderSourceCode_PF);

        gl.compileShader(fragmentShaderObject_PF);

        if (gl.getShaderParameter(fragmentShaderObject_PF, gl.COMPILE_STATUS) == false) {
            var error = gl.getShaderInfoLog(fragmentShaderObject_PF);

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
        shaderProgramObject_PF = gl.createProgram();

        gl.attachShader(shaderProgramObject_PF, vertexShaderObject_PF);
        gl.attachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

        gl.bindAttribLocation(shaderProgramObject_PF, VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

        gl.bindAttribLocation(shaderProgramObject_PF, VertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

        gl.linkProgram(shaderProgramObject_PF);

        if (gl.getProgramParameter(shaderProgramObject_PF, gl.LINK_STATUS) == false) {
            var error = gl.getProgramInfoLog(shaderProgramObject_PF);

            if (error.length > 0) {
                var log = "Shader Program Linking Error : " + error;
                alert(log);
                uninitialize();
            }_PV
        }
        else {
            console.log("Shader Program linked successfully.\n");
        }

        // get shader uniform location
        modelMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uModelMatrix");
        viewMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uViewMatrix");
        projectionMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");

        lightAmbientUniform_PF[0] = gl.getUniformLocation(shaderProgramObject_PF, "uLightAmbient[0]");
        lightDiffuseUniform_PF[0] = gl.getUniformLocation(shaderProgramObject_PF, "uLightDiffuse[0]");
        lightSpecularUniform_PF[0] = gl.getUniformLocation(shaderProgramObject_PF, "uLightSpecular[0]");
        lightPositionUniform_PF[0] = gl.getUniformLocation(shaderProgramObject_PF, "uLightPosition[0]");

        lightAmbientUniform_PF[1] = gl.getUniformLocation(shaderProgramObject_PF, "uLightAmbient[1]");
        lightDiffuseUniform_PF[1] = gl.getUniformLocation(shaderProgramObject_PF, "uLightDiffuse[1]");
        lightSpecularUniform_PF[1] = gl.getUniformLocation(shaderProgramObject_PF, "uLightSpecular[1]");
        lightPositionUniform_PF[1] = gl.getUniformLocation(shaderProgramObject_PF, "uLightPosition[1]");

        lightAmbientUniform_PF[2] = gl.getUniformLocation(shaderProgramObject_PF, "uLightAmbient[2]");
        lightDiffuseUniform_PF[2] = gl.getUniformLocation(shaderProgramObject_PF, "uLightDiffuse[2]");
        lightSpecularUniform_PF[2] = gl.getUniformLocation(shaderProgramObject_PF, "uLightSpecular[2]");
        lightPositionUniform_PF[2] = gl.getUniformLocation(shaderProgramObject_PF, "uLightPosition[2]");

        materialAmbientUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uMaterialAmbient");
        materialDiffuseUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uMaterialDiffuse");
        materialSpecularUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uMaterialSpecular");
        materialShininessUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uMaterialShineness");
        keyPressedUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uKeyPressed");
    }

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

    if(chooseShader == 'v')
    {
        gl.useProgram(shaderProgramObject_PV);

        // transformation
        var modelMatrix = mat4.create();
        var viewMatrix = mat4.create();

        var translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        // push above mvp(model view projection) into vertex shader's mvp uniform
        gl.uniformMatrix4fv(modelMatrixUniform_PV, false, modelMatrix);

        gl.uniformMatrix4fv(viewMatrixUniform_PV, false, viewMatrix);

        gl.uniformMatrix4fv(projectionMatrixUniform_PV, false, perspectiveProjectionMatrix);

        if (bLightingEnable == true)
        {
            gl.uniform1i(keyPressedUniform_PV, 1);

            gl.uniform3fv(lightAmbientUniform_PV[0], lightAmbient[0]);
            gl.uniform3fv(lightDiffuseUniform_PV[0], lightDiffuse[0]);
            gl.uniform3fv(lightSpecularUniform_PV[0], lightSpecular[0]);
            gl.uniform4fv(lightPositionUniform_PV[0], lightPosition[0]);

            gl.uniform3fv(lightAmbientUniform_PV[1], lightAmbient[1]);
            gl.uniform3fv(lightDiffuseUniform_PV[1], lightDiffuse[1]);
            gl.uniform3fv(lightSpecularUniform_PV[1], lightSpecular[1]);
            gl.uniform4fv(lightPositionUniform_PV[1], lightPosition[1]);

            gl.uniform3fv(lightAmbientUniform_PV[2], lightAmbient[2]);
            gl.uniform3fv(lightDiffuseUniform_PV[2], lightDiffuse[2]);
            gl.uniform3fv(lightSpecularUniform_PV[2], lightSpecular[2]);
            gl.uniform4fv(lightPositionUniform_PV[2], lightPosition[2]);

            gl.uniform3fv(materialAmbientUniform_PV, materialAmbient);
            gl.uniform3fv(materialDiffuseUniform_PV, materialDiffuse);
            gl.uniform3fv(materialSpecularUniform_PV, materialSpecular);
            gl.uniform1f(materialShininessUniform_PV, materialShininess);
        }
        else
        {
            gl.uniform1i(keyPressedUniform_PV, 0);
        }

        sphere.draw();

        gl.useProgram(null);
    }
    else if(chooseShader == 'f')
    {
        gl.useProgram(shaderProgramObject_PF);

        // transformation
        var modelMatrix = mat4.create();
        var viewMatrix = mat4.create();

        var translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        // push above mvp(model view projection) into vertex shader's mvp uniform
        gl.uniformMatrix4fv(modelMatrixUniform_PF, false, modelMatrix);

        gl.uniformMatrix4fv(viewMatrixUniform_PF, false, viewMatrix);

        gl.uniformMatrix4fv(projectionMatrixUniform_PF, false, perspectiveProjectionMatrix);

        if (bLightingEnable == true)
        {
            gl.uniform1i(keyPressedUniform_PF, 1);

            gl.uniform3fv(lightAmbientUniform_PF[0], lightAmbient[0]);
            gl.uniform3fv(lightDiffuseUniform_PF[0], lightDiffuse[0]);
            gl.uniform3fv(lightSpecularUniform_PF[0], lightSpecular[0]);
            gl.uniform4fv(lightPositionUniform_PF[0], lightPosition[0]);

            gl.uniform3fv(lightAmbientUniform_PF[1], lightAmbient[1]);
            gl.uniform3fv(lightDiffuseUniform_PF[1], lightDiffuse[1]);
            gl.uniform3fv(lightSpecularUniform_PF[1], lightSpecular[1]);
            gl.uniform4fv(lightPositionUniform_PF[1], lightPosition[1]);

            gl.uniform3fv(lightAmbientUniform_PF[2], lightAmbient[2]);
            gl.uniform3fv(lightDiffuseUniform_PF[2], lightDiffuse[2]);
            gl.uniform3fv(lightSpecularUniform_PF[2], lightSpecular[2]);
            gl.uniform4fv(lightPositionUniform_PF[2], lightPosition[2]);

            gl.uniform3fv(materialAmbientUniform_PF, materialAmbient);
            gl.uniform3fv(materialDiffuseUniform_PF, materialDiffuse);
            gl.uniform3fv(materialSpecularUniform_PF, materialSpecular);
            gl.uniform1f(materialShininessUniform_PF, materialShininess);
        }
        else
        {
            gl.uniform1i(keyPressedUniform_PF, 0);
        }

        sphere.draw();

        gl.useProgram(null);
    }
    // update for animation 
    update();
    // do the double buffering
    requestAnimationFrame(display, canvas);
}

function update() {
    // code
    if (bLightingEnable)
        {
            // animating light zero
            lightPosition[0][0] = 0.0;
            lightPosition[0][1] = 5.0 *  Math.sin(lightAngleZero);
            lightPosition[0][2] = 5.0 *  Math.cos(lightAngleZero);
            lightPosition[0][3] = 1.0;
            lightAngleZero = lightAngleZero + 0.05;
            if (lightAngleZero >= 360.0)
            {
                lightAngleZero = lightAngleZero - 360.0;
            }
    
            // animating light one
            lightPosition[1][0] = 5.0 * Math.sin(lightAngleOne);
            lightPosition[1][1] = 0.0;
            lightPosition[1][2] = 5.0 * Math.cos(lightAngleOne);
            lightPosition[1][3] = 1.0;
    
        
            lightAngleOne = lightAngleOne + 0.05;
            if (lightAngleOne >= 360.0)
            {
                lightAngleOne = lightAngleOne - 360.0;
            }
    
            // animating light two
            lightPosition[2][0] = 5.0 * Math.sin(lightAngleTwo);
            lightPosition[2][1] = 5.0 * Math.cos(lightAngleTwo);
            lightPosition[2][2] = 0.0;
            lightPosition[2][3] = 1.0;
    
            lightAngleTwo = lightAngleTwo + 0.05;
            if (lightAngleTwo >= 360.0)
            {
                lightAngleTwo = lightAngleTwo - 360.0;
            }
        }
}

function uninitialize() {
    //code

    if (shaderProgramObject_PV) {

        gl.useProgram(shaderProgramObject_PV);

        var shaderObject = gl.getAttachedShader();

        if (shaderObject && shaderObject.length > 0) {
            for (let i = 0; i < shaderObject.length; i++) {

                gl.detachShader(shaderProgramObject_PV, shaderObject[i]);

                gl.deleteShader(shaderObject[i]);

                shaderObject[i] = null;
            }
        }

        gl.useProgram(null);

        gl.deleteProgram(shaderProgramObject_PV);

        shaderProgramObject_PV = null;
    }

    if (shaderProgramObject_PF) {

        gl.useProgram(shaderProgramObject_PF);

        var shaderObject = gl.getAttachedShader();

        if (shaderObject && shaderObject.length > 0) {
            for (let i = 0; i < shaderObject.length; i++) {

                gl.detachShader(shaderProgramObject_PF, shaderObject[i]);

                gl.deleteShader(shaderObject[i]);

                shaderObject[i] = null;
            }
        }

        gl.useProgram(null);

        gl.deleteProgram(shaderProgramObject_PF);

        shaderProgramObject_PV = null;
    }

    if (sphere)
    {
        sphere.deallocate();
        sphere = null;
    }
}
