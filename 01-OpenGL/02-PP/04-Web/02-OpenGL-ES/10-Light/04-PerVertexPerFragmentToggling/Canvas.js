
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

var modelMatrixUniform_PV = 0;
var viewMatrixUniform_PV = 0;
var projectionMatrixUniform_PV = 0;

var lightAmbientUniform_PV = 0;
var lightDiffuseUniform_PV = 0;
var lightSpecularUniform_PV = 0;
var lightPositionUniform_PV = 0;

var materialDiffuseUniform_PV = 0;  
var materialAmbientUniform_PV = 0;
var materialSpecularUniform_PV = 0;
var materialShininessUniform_PV = 0;

var keyPressedUniform_PV = 0;

// for per fragment light
var modelMatrixUniform_PF = 0;
var viewMatrixUniform_PF = 0;
var projectionMatrixUniform_PF = 0;

var lightAmbientUniform_PF = 0;
var lightDiffuseUniform_PF = 0;
var lightSpecularUniform_PF = 0;
var lightPositionUniform_PF = 0;

var materialDiffuseUniform_PF = 0;  
var materialAmbientUniform_PF = 0;
var materialSpecularUniform_PF = 0;
var materialShininessUniform_PF = 0;

var keyPressedUniform_PF = 0;

var lightAmbient = new Float32Array([ 0.1,0.1,0.1 ]);
var lightDiffuse = new Float32Array([ 1.0,1.0,1.0 ]);
var lightSpecular = new Float32Array([ 1.0,1.0,1.0 ]);
var lightPosition = new Float32Array([ 100.0,100.0,100.0,1.0 ]);

var materialAmbient = new Float32Array([ 0.0,0.0,0.0 ]);
var materialDiffuse = new Float32Array([ 0.5,0.2,0.7 ]);
var materialSpecular = new Float32Array([ 0.7, 0.7, 0.7 ]);
var materialShininess = 128.0;

var bLightingEnable = false;

var perspectiveProjectionMatrix;

var chooseShader = 0;


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
        case 76: // ascii for L
        case 108: // ascii for l
            if (bLightingEnable == false) {
                bLightingEnable = true;
            }
            else {
                bLightingEnable = false;
            }
            break;

        case 27: // ascii for ESC
            toggleFullscreen();
            break;

        case 70: // ascii for F
        case 102: // ascii for f
           chooseShader = 1;
            break;

        case 86:
        case 118:
            chooseShader = 0;
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

    // per Vertex Light
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
                "uniform vec3 uLightAmbient;"+
                "uniform vec3 uLightDiffuse;"+
                "uniform vec3 uLightSpecular;"+
                "uniform vec4 uLightPosition;"+
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
                "vec3 lightDirection = normalize(vec3(uLightPosition-eyeCoordinates));"+
                "vec3 reflectionVector = reflect(-lightDirection,transformedNormals);"+
                "vec3 viewerVector = normalize(-eyeCoordinates.xyz);"+
                "vec3 lightAmbient = uLightAmbient * uMaterialAmbient;"+
                "vec3 lightDiffuse = uLightDiffuse * uMaterialDiffuse * max(dot(lightDirection,transformedNormals),0.0);"+
                "vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector,viewerVector),0.0),uMaterialShineness);"+
                "oPhongADSLight = lightAmbient + lightDiffuse + lightSpecular;"+
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
            }
        }
        else {
            console.log("Shader Program linked successfully.\n");
        }

        // get shader uniform location
        modelMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uModelMatrix");
        viewMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uViewMatrix");
        projectionMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uProjectionMatrix");
        lightAmbientUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uLightAmbient");
        lightDiffuseUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uLightDiffuse");
        lightSpecularUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uLightSpecular");
        lightPositionUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uLightPosition");
        materialAmbientUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uMaterialAmbient");
        materialDiffuseUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uMaterialDiffuse");
        materialSpecularUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uMaterialSpecular");
        materialShininessUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uMaterialShineness");
        keyPressedUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "uKeyPressed");
    }

    // per Fragment Light
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
                "uniform vec3 uLightAmbient;"+
                "uniform vec3 uLightDiffuse;"+
                "uniform vec3 uLightSpecular;"+
                "uniform vec4 uLightPosition;"+
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
                "vec3 lightDirection = normalize(vec3(uLightPosition-eyeCoordinates));"+
                "vec3 reflectionVector = reflect(-lightDirection,transformedNormals);"+
                "vec3 viewerVector = normalize(-eyeCoordinates.xyz);"+
                "vec3 lightAmbient = uLightAmbient * uMaterialAmbient;"+
                "vec3 lightDiffuse = uLightDiffuse * uMaterialDiffuse * max(dot(lightDirection,transformedNormals),0.0);"+
                "vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector,viewerVector),0.0),uMaterialShineness);"+
                "oPhongADSLight = lightAmbient + lightDiffuse + lightSpecular;"+
                "}"+
                "else"+
                "{"+
                "oPhongADSLight = vec3(0.0,0.0,0.0);"+
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
            }
        }
        else {
            console.log("Shader Program linked successfully.\n");
        }

        // get shader uniform location
        modelMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uModelMatrix");
        viewMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uViewMatrix");
        projectionMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");
        lightAmbientUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uLightAmbient");
        lightDiffuseUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uLightDiffuse");
        lightSpecularUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uLightSpecular");
        lightPositionUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "uLightPosition");
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
    gl.clearColor(0.0, 0.0, 1.0, 1.0);

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

    if(chooseShader == 0)
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

            gl.uniform3fv(lightAmbientUniform_PV, lightAmbient);
            gl.uniform3fv(lightDiffuseUniform_PV, lightDiffuse);
            gl.uniform3fv(lightSpecularUniform_PV, lightSpecular);
            gl.uniform4fv(lightPositionUniform_PV, lightPosition);

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
    else
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

            gl.uniform3fv(lightAmbientUniform_PF, lightAmbient);
            gl.uniform3fv(lightDiffuseUniform_PF, lightDiffuse);
            gl.uniform3fv(lightSpecularUniform_PF, lightSpecular);
            gl.uniform4fv(lightPositionUniform_PF, lightPosition);

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

        shaderProgramObject_PF = null;
    }

    if (sphere)
    {
        sphere.deallocate();
        sphere = null;
    }
}
