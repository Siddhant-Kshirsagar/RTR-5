
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

var shaderProgramObject_Cube = null;

var vao_cube = null;
var vbo_positionCube = null;
var vbo_texcoordCube = null;

var mvpMatrixUniform_Cube;

var perspectiveProjectionMatrix_Cube;

var textureSamplerUniform_Cube;

var cAngle = 0.0;

var WIN_WIDTH = 800;
var WIN_HEIGHT = 600;

var FBO_HEIGHT = 512;
var FBO_WIDTH = 512;

// for three light

var shaderProgramObject_PV_Sphere = null;
var shaderProgramObject_PF_Sphere = null;

var sphere = null;

// per vertex
var modelMatrixUniform_PV_Sphere = 0;
var viewMatrixUniform_PV_Sphere = 0;
var projectionMatrixUniform_PV_Sphere = 0;

var lightAmbientUniform_PV_Sphere = [3];
var lightDiffuseUniform_PV_Sphere = [3];
var lightSpecularUniform_PV_Sphere = [3];
var lightPositionUniform_PV_Sphere = [3];

var materialDiffuseUniform_PV_Sphere = 0;  
var materialAmbientUniform_PV_Sphere = 0;
var materialSpecularUniform_PV_Sphere = 0;
var materialShininessUniform_PV_Sphere = 0;

var keyPressedUniform_PV_Sphere = 0;

// per fragment 
var modelMatrixUniform_PF_Sphere = 0;
var viewMatrixUniform_PF_Sphere = 0;
var projectionMatrixUniform_PF_Sphere = 0;

var lightAmbientUniform_PF_Sphere = [3];
var lightDiffuseUniform_PF_Sphere = [3];
var lightSpecularUniform_PF_Sphere = [3];
var lightPositionUniform_PF_Sphere = [3];

var materialDiffuseUniform_PF_Sphere = 0;  
var materialAmbientUniform_PF_Sphere = 0;
var materialSpecularUniform_PF_Sphere = 0;
var materialShininessUniform_PF_Sphere = 0;

var keyPressedUniform_PF_Sphere = 0;

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

var mvpMatrixUniform_Sphere;

var perspectiveProjectionMatrix_Sphere;

var lightAngleZero = 0.0;
var lightAngleOne = 0.0;
var lightAngleTwo = 0.0;

var chooseShader = 'v';

// FBO(Frame Buffer Object) related variable 
var winWidth = 0;
var winHeight = 0;

var FBO = 0; // Frame Buffer Object
var RBO = 0; // Render Buffer Object
var texture_FBO = 0;
var bFBOResult = false;

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
    window.addEventListener("resize_Cube", resize_Cube, false);

    // initialize_Cube 
    initialize_Cube();

    // resize_Cube 
    resize_Cube();

    // display_Cube
    display_Cube();

}

function keyDown(event) {
    // code
    switch (event.keyCode) {
        case 81: // ascii for Q
        case 113: // ascii for q
            uninitialize_Cube();
            window.close(); // exit 
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

function initialize_Cube() {
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
        "in vec2 aTexCoord;" +
        "out vec2 oTexCoord;" +
        "void main(void)" +
        "{" +
        "gl_Position= uMVPMatrix * aPosition;" +
        "oTexCoord = aTexCoord;" +
        "}";
    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject);

        if (error.length > 0) {
            var log = "Vertex Shader Compilation Error : " + error;
            alert(log);
            uninitialize_Cube();
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
        "in vec2 oTexCoord;" +
        "uniform sampler2D uTextureSampler;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = texture(uTextureSampler,oTexCoord);" +
        "}";

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject);

        if (error.length > 0) {
            var log = "Fragment Shader Compilation Error : " + error;
            alert(log);
            uninitialize_Cube();
        }
    }
    else {
        console.log("Fragment Shader Compile Successfully.\n");
    }

    // shader program
    shaderProgramObject_Cube = gl.createProgram();

    gl.attachShader(shaderProgramObject_Cube, vertexShaderObject);
    gl.attachShader(shaderProgramObject_Cube, fragmentShaderObject);

    gl.bindAttribLocation(shaderProgramObject_Cube, VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

    gl.bindAttribLocation(shaderProgramObject_Cube, VertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

    gl.linkProgram(shaderProgramObject_Cube);

    if (gl.getProgramParameter(shaderProgramObject_Cube, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject_Cube);

        if (error.length > 0) {
            var log = "Shader Program Linking Error : " + error;
            alert(log);
            uninitialize_Cube();
        }
    }
    else {
        console.log("Shader Program linked successfully.\n");
    }

    // get uniform
    mvpMatrixUniform_Cube = gl.getUniformLocation(shaderProgramObject_Cube, "uMVPMatrix");

    textureSamplerUniform_Cube = gl.getUniformLocation(shaderProgramObject_Cube, "uTextureSampler");

    // geometry attribute declaration
    var cube_position = new Float32Array([
        // top
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,

        // bottom
        1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,

        // front
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,

        // back
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,

        // right
        1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,

        // left
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0]);

    var Cube_texcoord = new Float32Array([
    
        1.0, 1.0, 
        0.0, 1.0, 
        0.0, 0.0, 
        1.0, 0.0, 
        
        1.0, 1.0, 
        0.0, 1.0, 
        0.0, 0.0, 
        1.0, 0.0, 
        
        1.0, 1.0, 
        0.0, 1.0, 
        0.0, 0.0, 
        1.0, 0.0, 
        
        1.0, 1.0, 
        0.0, 1.0, 
        0.0, 0.0, 
        1.0, 0.0, 
        
        1.0, 1.0, 
        0.0, 1.0, 
        0.0, 0.0, 
        1.0, 0.0, 
    
        1.0, 1.0, 
        0.0, 1.0, 
        0.0, 0.0, 
        1.0, 0.0, 
    ]);

    // vao_cube
    vao_cube = gl.createVertexArray();

    gl.bindVertexArray(vao_cube);

    // vbo_positionCube
    vbo_positionCube = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionCube);

    gl.bufferData(gl.ARRAY_BUFFER, cube_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    // vbo_texcoordSquare
    vbo_texcoordCube = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texcoordCube);

    gl.bufferData(gl.ARRAY_BUFFER, Cube_texcoord, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    // depth initialization
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // set clear color
    gl.clearColor(1.0, 1.0, 1.0, 1.0);

    // enable texture
	// gl.enable(gl.TEXTURE_2D);

    // initialize_Cube projection matrix
    perspectiveProjectionMatrix_Cube = mat4.create();

    resize_Cube();

    if(createFBO(FBO_WIDTH,FBO_HEIGHT) == true)
    {
        bFBOResult = initialize_Sphere(FBO_WIDTH,FBO_HEIGHT);
        console.log("initialize_Sphere Complete"+bFBOResult);
    }
}

function initialize_Sphere(textureWidth, textureHeight) {
    // code
    // per vertex lighting 
    {
        // vertex shader
        var vertexShaderSourceCode_PV_Sphere =
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

        var vertexShaderObject_PV_Sphere = gl.createShader(gl.VERTEX_SHADER);

        gl.shaderSource(vertexShaderObject_PV_Sphere, vertexShaderSourceCode_PV_Sphere);

        gl.compileShader(vertexShaderObject_PV_Sphere);

        if (gl.getShaderParameter(vertexShaderObject_PV_Sphere, gl.COMPILE_STATUS) == false) {
            var error = gl.getShaderInfoLog(vertexShaderObject_PV_Sphere);

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
        var fragmentShaderSourceCode_PV_Sphere =
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

        var fragmentShaderObject_PV_Sphere = gl.createShader(gl.FRAGMENT_SHADER);

        gl.shaderSource(fragmentShaderObject_PV_Sphere, fragmentShaderSourceCode_PV_Sphere);

        gl.compileShader(fragmentShaderObject_PV_Sphere);

        if (gl.getShaderParameter(fragmentShaderObject_PV_Sphere, gl.COMPILE_STATUS) == false) {
            var error = gl.getShaderInfoLog(fragmentShaderObject_PV_Sphere);

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
        shaderProgramObject_PV_Sphere = gl.createProgram();

        gl.attachShader(shaderProgramObject_PV_Sphere, vertexShaderObject_PV_Sphere);
        gl.attachShader(shaderProgramObject_PV_Sphere, fragmentShaderObject_PV_Sphere);

        gl.bindAttribLocation(shaderProgramObject_PV_Sphere, VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

        gl.bindAttribLocation(shaderProgramObject_PV_Sphere, VertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

        gl.linkProgram(shaderProgramObject_PV_Sphere);

        if (gl.getProgramParameter(shaderProgramObject_PV_Sphere, gl.LINK_STATUS) == false) {
            var error = gl.getProgramInfoLog(shaderProgramObject_PV_Sphere);

            if (error.length > 0) {
                var log = "Shader Program Linking Error : " + error;
                alert(log);
                uninitialize();
            }_PV_Sphere
        }
        else {
            console.log("Shader Program linked successfully.\n");
        }

        // get shader uniform location
        modelMatrixUniform_PV_Sphere = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uModelMatrix");
        viewMatrixUniform_PV_Sphere = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uViewMatrix");
        projectionMatrixUniform_PV_Sphere = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uProjectionMatrix");

        lightAmbientUniform_PV_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightAmbient[0]");
        lightDiffuseUniform_PV_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightDiffuse[0]");
        lightSpecularUniform_PV_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightSpecular[0]");
        lightPositionUniform_PV_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightPosition[0]");

        lightAmbientUniform_PV_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightAmbient[1]");
        lightDiffuseUniform_PV_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightDiffuse[1]");
        lightSpecularUniform_PV_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightSpecular[1]");
        lightPositionUniform_PV_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightPosition[1]");

        lightAmbientUniform_PV_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightAmbient[2]");
        lightDiffuseUniform_PV_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightDiffuse[2]");
        lightSpecularUniform_PV_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightSpecular[2]");
        lightPositionUniform_PV_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uLightPosition[2]");

        materialAmbientUniform_PV_Sphere = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialAmbient");
        materialDiffuseUniform_PV_Sphere = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialDiffuse");
        materialSpecularUniform_PV_Sphere = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialSpecular");
        materialShininessUniform_PV_Sphere = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialShineness");
        keyPressedUniform_PV_Sphere = gl.getUniformLocation(shaderProgramObject_PV_Sphere, "uKeyPressed");
    }
    // per fragment lighting 
    {
        // vertex shader
        var vertexShaderSourceCode_PF_Sphere =
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

        var vertexShaderObject_PF_Sphere = gl.createShader(gl.VERTEX_SHADER);

        gl.shaderSource(vertexShaderObject_PF_Sphere, vertexShaderSourceCode_PF_Sphere);

        gl.compileShader(vertexShaderObject_PF_Sphere);

        if (gl.getShaderParameter(vertexShaderObject_PF_Sphere, gl.COMPILE_STATUS) == false) {
            var error = gl.getShaderInfoLog(vertexShaderObject_PF_Sphere);

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
        var fragmentShaderSourceCode_PF_Sphere =
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

        var fragmentShaderObject_PF_Sphere = gl.createShader(gl.FRAGMENT_SHADER);

        gl.shaderSource(fragmentShaderObject_PF_Sphere, fragmentShaderSourceCode_PF_Sphere);

        gl.compileShader(fragmentShaderObject_PF_Sphere);

        if (gl.getShaderParameter(fragmentShaderObject_PF_Sphere, gl.COMPILE_STATUS) == false) {
            var error = gl.getShaderInfoLog(fragmentShaderObject_PF_Sphere);

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
        shaderProgramObject_PF_Sphere = gl.createProgram();

        gl.attachShader(shaderProgramObject_PF_Sphere, vertexShaderObject_PF_Sphere);
        gl.attachShader(shaderProgramObject_PF_Sphere, fragmentShaderObject_PF_Sphere);

        gl.bindAttribLocation(shaderProgramObject_PF_Sphere, VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, "aPosition");

        gl.bindAttribLocation(shaderProgramObject_PF_Sphere, VertexAttributeEnum.AMC_ATTRIBUTE_NORMAL, "aNormal");

        gl.linkProgram(shaderProgramObject_PF_Sphere);

        if (gl.getProgramParameter(shaderProgramObject_PF_Sphere, gl.LINK_STATUS) == false) {
            var error = gl.getProgramInfoLog(shaderProgramObject_PF_Sphere);

            if (error.length > 0) {
                var log = "Shader Program Linking Error : " + error;
                alert(log);
                uninitialize();
            }_PV_Sphere
        }
        else {
            console.log("Shader Program linked successfully.\n");
        }

        // get shader uniform location
        modelMatrixUniform_PF_Sphere = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uModelMatrix");
        viewMatrixUniform_PF_Sphere = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uViewMatrix");
        projectionMatrixUniform_PF_Sphere = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uProjectionMatrix");

        lightAmbientUniform_PF_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightAmbient[0]");
        lightDiffuseUniform_PF_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightDiffuse[0]");
        lightSpecularUniform_PF_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightSpecular[0]");
        lightPositionUniform_PF_Sphere[0] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightPosition[0]");

        lightAmbientUniform_PF_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightAmbient[1]");
        lightDiffuseUniform_PF_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightDiffuse[1]");
        lightSpecularUniform_PF_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightSpecular[1]");
        lightPositionUniform_PF_Sphere[1] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightPosition[1]");

        lightAmbientUniform_PF_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightAmbient[2]");
        lightDiffuseUniform_PF_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightDiffuse[2]");
        lightSpecularUniform_PF_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightSpecular[2]");
        lightPositionUniform_PF_Sphere[2] = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uLightPosition[2]");

        materialAmbientUniform_PF_Sphere = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialAmbient");
        materialDiffuseUniform_PF_Sphere = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialDiffuse");
        materialSpecularUniform_PF_Sphere = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialSpecular");
        materialShininessUniform_PF_Sphere = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialShineness");
        keyPressedUniform_PF_Sphere = gl.getUniformLocation(shaderProgramObject_PF_Sphere, "uKeyPressed");
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

    resize_Sphere(WIN_WIDTH, WIN_HEIGHT);

    return(true);
}

function createFBO(textureWidth,textureHeight)
{
    var maxRenderBufferSize = 0;

    // code 

    // step 1 : check capacity of render buffer
    // gl.getIntegerv(gl.MAX_RENDERBUFFER_SIZE, maxRenderBufferSize);
    maxRenderBufferSize =  gl.getParameter(gl.MAX_RENDERBUFFER_SIZE);
    console.log("max render buffer size"+maxRenderBufferSize);

    if(maxRenderBufferSize < textureWidth || maxRenderBufferSize <textureHeight)
    {
        console.log("Texture Size Overflow\n");
        return(false);
    }

    // step 2 : create custom frame buffer
    // gl.genFramebuffers(1,FBO);
    FBO = gl.createFramebuffer();

    // bind frame buffer
    gl.bindFramebuffer(gl.FRAMEBUFFER, FBO);

    // step 3 : create the texture for FBO in which we are going to render sphere
    texture_FBO = gl.createTexture();

    gl.bindTexture(gl.TEXTURE_2D, texture_FBO);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, textureWidth, textureHeight,0,gl.RGB,gl.UNSIGNED_SHORT_5_6_5, null);

    // step 4 : attach above texture to frame buffer at default color attachment 0
	gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, texture_FBO, 0);

    // step 5 : now create render buffer to hold depth of custom FBO
	// generate render buffer
	// gl.genRenderbuffers(1, RBO);
    RBO = gl.createRenderbuffer();

	// bind render buffer
	gl.bindRenderbuffer(gl.RENDERBUFFER, RBO);

	// set storage of above render buffer of texture size for depth
	gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, textureWidth, textureHeight);

	// step 6 : attach above depth related render buffer to FBO(Frame Buffer Object) as depth attachment
	gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, gl.RENDERBUFFER, RBO);

	// step 7 : check the frame buffer status whether successfull or not 
	if (gl.checkFramebufferStatus(gl.FRAMEBUFFER) != gl.FRAMEBUFFER_COMPLETE)
	{
		console.log("Frame Buffer status is not complete\n");
		return(false);
	}

	// step 8 : unbind frame buffer
	gl.bindFramebuffer(gl.FRAMEBUFFER, null);

	return(true);
}


function resize_Cube(width,height) {
    // code
    if (gbFullScreen == true) {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    // console.log("width="+width+"height="+height);
    winWidth = canvas.width;
    winHeight = canvas.height;

    // set viewport 
    gl.viewport(0, 0, winWidth, winHeight);

    // set projection
    mat4.perspective(perspectiveProjectionMatrix_Cube, 45.0, parseFloat(canvas.width)/parseFloat(canvas.height), 0.1, 100.0);
}

function resize_Sphere(width,height) {
    // code
    if (height <= 0)
    {
        height = 1;
    }
    
    // console.log("Width="+width+"Height="+height);
    // set viewport 
    gl.viewport(0, 0, width, height);

    // set projection
    mat4.perspective(perspectiveProjectionMatrix_Cube, 45.0, parseFloat(canvas.width)/parseFloat(canvas.height), 0.1, 100.0);
}

function display_Cube() {
    // code

    // render FBO(Frame Buffer Object) scene
    if(bFBOResult == true)
    {
        display_Sphere(FBO_WIDTH,FBO_HEIGHT);

        update_Sphere();
    }

    resize_Cube(winWidth,winHeight);

    // reset color to white to compenset the change by display_Sphere
	gl.clearColor(1.0, 1.0, 1.0, 1.0);

    gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject_Cube);

    // transformation
    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();
    var translationMatrix = mat4.create();
    var rotationMatrix = mat4.create();
    var rotationMatrix_X = mat4.create();
    var rotationMatrix_Y = mat4.create();
    var rotationMatrix_Z = mat4.create();

    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);

    mat4.rotateX(rotationMatrix_X, rotationMatrix_X, degToRad(cAngle));
    mat4.rotateY(rotationMatrix_Y, rotationMatrix_Y, degToRad(cAngle));
    mat4.rotateZ(rotationMatrix_Z, rotationMatrix_Z, degToRad(cAngle));

    mat4.multiply(rotationMatrix, rotationMatrix_X, rotationMatrix_Y);

    mat4.multiply(rotationMatrix, rotationMatrix, rotationMatrix_Z);

    mat4.multiply(modelViewMatrix, translationMatrix, rotationMatrix);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix_Cube, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform_Cube, false, modelViewProjectionMatrix);

    // texture related code
	gl.activeTexture(gl.TEXTURE0);

	gl.bindTexture(gl.TEXTURE_2D, texture_FBO);

	gl.uniform1i(textureSamplerUniform_Cube, 0);

    gl.bindVertexArray(vao_cube);

    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);

    gl.bindVertexArray(null);

    gl.bindTexture(gl.TEXTURE_2D, null);

    gl.useProgram(null);

    // update_Cube for animation 
    update_Cube();
    // do the double buffering
    requestAnimationFrame(display_Cube, canvas);
}
function display_Sphere(textureWidth,textureHeight)
{
    // code
    // Bind with FBO
	gl.bindFramebuffer(gl.FRAMEBUFFER, FBO);

	// call resize_Sphere() to change size of sphere side code
	resize_Sphere(textureWidth, textureHeight);

	// set  glClearColor to black to compenset the change done by display Cube
	gl.clearColor(0.0, 0.0, 0.0, 1.0);

    gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);

    if(chooseShader == 'v')
    {
        gl.useProgram(shaderProgramObject_PV_Sphere);

        // transformation
        var modelMatrix = mat4.create();
        var viewMatrix = mat4.create();

        var translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        // push above mvp(model view projection) into vertex shader's mvp uniform
        gl.uniformMatrix4fv(modelMatrixUniform_PV_Sphere, false, modelMatrix);

        gl.uniformMatrix4fv(viewMatrixUniform_PV_Sphere, false, viewMatrix);

        gl.uniformMatrix4fv(projectionMatrixUniform_PV_Sphere, false, perspectiveProjectionMatrix);

        if (bLightingEnable == true)
        {
            gl.uniform1i(keyPressedUniform_PV_Sphere, 1);

            gl.uniform3fv(lightAmbientUniform_PV_Sphere[0], lightAmbient[0]);
            gl.uniform3fv(lightDiffuseUniform_PV_Sphere[0], lightDiffuse[0]);
            gl.uniform3fv(lightSpecularUniform_PV_Sphere[0], lightSpecular[0]);
            gl.uniform4fv(lightPositionUniform_PV_Sphere[0], lightPosition[0]);

            gl.uniform3fv(lightAmbientUniform_PV_Sphere[1], lightAmbient[1]);
            gl.uniform3fv(lightDiffuseUniform_PV_Sphere[1], lightDiffuse[1]);
            gl.uniform3fv(lightSpecularUniform_PV_Sphere[1], lightSpecular[1]);
            gl.uniform4fv(lightPositionUniform_PV_Sphere[1], lightPosition[1]);

            gl.uniform3fv(lightAmbientUniform_PV_Sphere[2], lightAmbient[2]);
            gl.uniform3fv(lightDiffuseUniform_PV_Sphere[2], lightDiffuse[2]);
            gl.uniform3fv(lightSpecularUniform_PV_Sphere[2], lightSpecular[2]);
            gl.uniform4fv(lightPositionUniform_PV_Sphere[2], lightPosition[2]);

            gl.uniform3fv(materialAmbientUniform_PV_Sphere, materialAmbient);
            gl.uniform3fv(materialDiffuseUniform_PV_Sphere, materialDiffuse);
            gl.uniform3fv(materialSpecularUniform_PV_Sphere, materialSpecular);
            gl.uniform1f(materialShininessUniform_PV_Sphere, materialShininess);
        }
        else
        {
            gl.uniform1i(keyPressedUniform_PV_Sphere, 0);
        }

        sphere.draw();

        gl.useProgram(null);
    }
    else if(chooseShader == 'f')
    {
        gl.useProgram(shaderProgramObject_PF_Sphere);

        // transformation
        var modelMatrix = mat4.create();
        var viewMatrix = mat4.create();

        var translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -5.0]);

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        // push above mvp(model view projection) into vertex shader's mvp uniform
        gl.uniformMatrix4fv(modelMatrixUniform_PF_Sphere, false, modelMatrix);

        gl.uniformMatrix4fv(viewMatrixUniform_PF_Sphere, false, viewMatrix);

        gl.uniformMatrix4fv(projectionMatrixUniform_PF_Sphere, false, perspectiveProjectionMatrix);

        if (bLightingEnable == true)
        {
            gl.uniform1i(keyPressedUniform_PF_Sphere, 1);

            gl.uniform3fv(lightAmbientUniform_PF_Sphere[0], lightAmbient[0]);
            gl.uniform3fv(lightDiffuseUniform_PF_Sphere[0], lightDiffuse[0]);
            gl.uniform3fv(lightSpecularUniform_PF_Sphere[0], lightSpecular[0]);
            gl.uniform4fv(lightPositionUniform_PF_Sphere[0], lightPosition[0]);

            gl.uniform3fv(lightAmbientUniform_PF_Sphere[1], lightAmbient[1]);
            gl.uniform3fv(lightDiffuseUniform_PF_Sphere[1], lightDiffuse[1]);
            gl.uniform3fv(lightSpecularUniform_PF_Sphere[1], lightSpecular[1]);
            gl.uniform4fv(lightPositionUniform_PF_Sphere[1], lightPosition[1]);

            gl.uniform3fv(lightAmbientUniform_PF_Sphere[2], lightAmbient[2]);
            gl.uniform3fv(lightDiffuseUniform_PF_Sphere[2], lightDiffuse[2]);
            gl.uniform3fv(lightSpecularUniform_PF_Sphere[2], lightSpecular[2]);
            gl.uniform4fv(lightPositionUniform_PF_Sphere[2], lightPosition[2]);

            gl.uniform3fv(materialAmbientUniform_PF_Sphere, materialAmbient);
            gl.uniform3fv(materialDiffuseUniform_PF_Sphere, materialDiffuse);
            gl.uniform3fv(materialSpecularUniform_PF_Sphere, materialSpecular);
            gl.uniform1f(materialShininessUniform_PF_Sphere, materialShininess);
        }
        else
        {
            gl.uniform1i(keyPressedUniform_PF_Sphere, 0);
        }

        sphere.draw();

        gl.useProgram(null);
    }

    // unBind frame buffer
	gl.bindFramebuffer(gl.FRAMEBUFFER, null);

}

function degToRad(degrees) {
    return ((degrees * Math.PI) / 180.0);
}

function update_Cube() {
    // code
    //cube rotate
    cAngle = cAngle + 1.0;
    if (cAngle >= 360.0) {
        cAngle = cAngle - 360.0;
    }
}

function update_Sphere() {
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

function uninitialize_Cube() {
    //code

    if (shaderProgramObject_Cube) {

        gl.useProgram(shaderProgramObject_Cube);

        var shaderObject = gl.getAttachedShader();

        if (shaderObject && shaderObject.length > 0) {
            for (let i = 0; i < shaderObject.length; i++) {

                gl.detachShader(shaderProgramObject_Cube, shaderObject[i]);

                gl.deleteShader(shaderObject[i]);

                shaderObject[i] = null;
            }
        }

        gl.useProgram(null);

        gl.deleteProgram(shaderProgramObject_Cube);

        shaderProgramObject_Cube = null;
    }

    if (vbo_positionSquare != null) {
        gl.deleteBuffer(vbo_positionSquare);
        vbo_positionSquare = null;
    }

    if (vao_square != null) {
        gl.deleteVertexArrray(vao_square);
        vao_square = null;
    }

    if (vbo_positionCube != null) {
        gl.deleteBuffer(vbo_positionCube);
        vbo_positionCube = null;
    }

    if (vao_cube != null) {
        gl.deleteVertexArrray(vao_cube);
        vao_cube = null;
    }
}
