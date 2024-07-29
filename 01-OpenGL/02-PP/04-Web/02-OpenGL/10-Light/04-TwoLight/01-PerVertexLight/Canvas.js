
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

var vao_pyramid = null;
var vbo_positionPyramid = null;
var vbo_normalPyramid = null;

var pAngle = 0.0;

var modelMatrixUniform = 0;
var viewMatrixUniform = 0;
var projectionMatrixUniform = 0;

var lightAmbientUniform = [2];
var lightDiffuseUniform = [2];
var lightSpecularUniform = [2];
var lightPositionUniform = [2];

var materialDiffuseUniform = 0;  
var materialAmbientUniform = 0;
var materialSpecularUniform = 0;
var materialShininessUniform = 0;

var keyPressedUniform = 0;

var lightAmbient = [
    new Float32Array([ 0.0,0.0,0.0 ]),
    new Float32Array([ 0.0,0.0,0.0 ])
    ];
var lightDiffuse = [
    new Float32Array([ 1.0,0.0,0.0 ]),
    new Float32Array([ 0.0,0.0,1.0 ])];
var lightSpecular = [
    new Float32Array([ 1.0,0.0,0.0 ]),
    new Float32Array([ 0.0,0.0,1.0 ])
];
var lightPosition = [
    new Float32Array([ -2.0,0.0,0.0,1.0 ]),
    new Float32Array([ 2.0,0.0,0.0,1.0 ])
];

var materialAmbient = new Float32Array([ 0.0,0.0,0.0 ]);
var materialDiffuse = new Float32Array([ 0.5,0.2,0.7 ]);
var materialSpecular = new Float32Array([ 0.7, 0.7, 0.7 ]);
var materialShininess = 128.0;

var bLightingEnable = false;
var bAnimationEnable = false;

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

        case 65:
        case 97:
            if(bAnimationEnable == false){
                bAnimationEnable = true;
            }
            else{
                bAnimationEnable = false;
            }
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
            "uniform vec3 uLightAmbient[2];"+
            "uniform vec3 uLightDiffuse[2];"+
            "uniform vec3 uLightSpecular[2];"+
            "uniform vec4 uLightPosition[2];"+
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
            "vec3 lightAmbient[2];"+
            "vec3 lightDirection[2];"+
            "vec3 lightDiffuse[2];"+
            "vec3 reflectionVector[2];"+
            "vec3 lightSpecular[2];"+
            "for(int i = 0; i<2; i++)"+
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

    lightAmbientUniform[0] = gl.getUniformLocation(shaderProgramObject, "uLightAmbient[0]");
    lightDiffuseUniform[0] = gl.getUniformLocation(shaderProgramObject, "uLightDiffuse[0]");
    lightSpecularUniform[0] = gl.getUniformLocation(shaderProgramObject, "uLightSpecular[0]");
    lightPositionUniform[0] = gl.getUniformLocation(shaderProgramObject, "uLightPosition[0]");

    lightAmbientUniform[1] = gl.getUniformLocation(shaderProgramObject, "uLightAmbient[1]");
    lightDiffuseUniform[1] = gl.getUniformLocation(shaderProgramObject, "uLightDiffuse[1]");
    lightSpecularUniform[1] = gl.getUniformLocation(shaderProgramObject, "uLightSpecular[1]");
    lightPositionUniform[1] = gl.getUniformLocation(shaderProgramObject, "uLightPosition[1]");

    materialAmbientUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialAmbient");
    materialDiffuseUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialDiffuse");
    materialSpecularUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialSpecular");
    materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialShineness");
    keyPressedUniform = gl.getUniformLocation(shaderProgramObject, "uKeyPressed");

    // geometry attribute declaration
    var pyramid_position = new Float32Array([
    // front
    0.0, 1.0, 0.0,
    -1.0, -1.0, 1.0,
    1.0, -1.0, 1.0,

    // right
    0.0, 1.0, 0.0,
    1.0, -1.0, 1.0,
    1.0, -1.0, -1.0,

    // back
    0.0, 1.0, 0.0,
    1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,

    // left
    0.0, 1.0, 0.0,
    -1.0, -1.0, -1.0,
    -1.0, -1.0, 1.0]);

    var pyramid_normal = new Float32Array([
        
		0.000000, 0.447214,  0.894427,
		0.000000, 0.447214,  0.894427, 
		0.000000, 0.447214,  0.894427, 
		
		0.894427, 0.447214,  0.000000, 
		0.894427, 0.447214,  0.000000, 
		0.894427, 0.447214,  0.000000, 
		
		0.000000, 0.447214, -0.894427, 
		0.000000, 0.447214, -0.894427, 
		0.000000, 0.447214, -0.894427, 
		
	   -0.894427, 0.447214,  0.000000, 
	   -0.894427, 0.447214,  0.000000, 
	   -0.894427, 0.447214,  0.000000, 
    ]);

    // vao_pyramid
    vao_pyramid = gl.createVertexArray();

    gl.bindVertexArray(vao_pyramid);

    // vbo_positionPyramid
    vbo_positionPyramid = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionPyramid);

    gl.bufferData(gl.ARRAY_BUFFER, pyramid_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    // vbo_normalPyramid
    vbo_normalPyramid = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_normalPyramid);

    gl.bufferData(gl.ARRAY_BUFFER, pyramid_normal, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_NORMAL);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

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
    var rotationMatrix = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);

    mat4.rotateY(rotationMatrix, rotationMatrix,degToRad(pAngle));

    mat4.multiply(modelMatrix, translationMatrix, rotationMatrix);

    // push above mvp(model view projection) into vertex shader's mvp uniform
    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);

    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    if (bLightingEnable == true)
    {
        gl.uniform1i(keyPressedUniform, 1);

        gl.uniform3fv(lightAmbientUniform[0], lightAmbient[0]);
        gl.uniform3fv(lightDiffuseUniform[0], lightDiffuse[0]);
        gl.uniform3fv(lightSpecularUniform[0], lightSpecular[0]);
        gl.uniform4fv(lightPositionUniform[0], lightPosition[0]);

        gl.uniform3fv(lightAmbientUniform[1], lightAmbient[1]);
        gl.uniform3fv(lightDiffuseUniform[1], lightDiffuse[1]);
        gl.uniform3fv(lightSpecularUniform[1], lightSpecular[1]);
        gl.uniform4fv(lightPositionUniform[1], lightPosition[1]);

        gl.uniform3fv(materialAmbientUniform, materialAmbient);
        gl.uniform3fv(materialDiffuseUniform, materialDiffuse);
        gl.uniform3fv(materialSpecularUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShininess);
    }
    else
    {
        gl.uniform1i(keyPressedUniform, 0);
    }

    gl.bindVertexArray(vao_pyramid);

    gl.drawArrays(gl.TRIANGLES, 0, 12);

    gl.bindVertexArray(null);

    gl.useProgram(null);

    // update for animation 
    if(bAnimationEnable)
    {
        update();
    }
    
    // do the double buffering
    requestAnimationFrame(display, canvas);
}


function degToRad(degrees) {
    return ((degrees * Math.PI) / 180.0);
}

function update() {
    // code
    //pyramid rotate
    pAngle = pAngle + 1.0;
    if (pAngle >= 360.0) {
        pAngle = pAngle - 360.0;
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

    if (vbo_positionPyramid != null) {
        gl.deleteBuffer(vbo_positionPyramid);
        vbo_positionPyramid = null;
    }

    if (vao_pyramid != null) {
        gl.deleteVertexArrray(vao_pyramid);
        vao_pyramid = null;
    }

}
