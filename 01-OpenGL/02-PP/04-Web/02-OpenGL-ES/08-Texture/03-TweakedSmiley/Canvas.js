
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
    AMC_ATTRIBUTE_TEXCOORD: 1,
};

var shaderProgramObject = null;

var vao_square = null;
var vbo_positionSquare = null;
var vbo_texcoordSquare = null;

var mvpMatrixUniform;
var keyPressedUniform;
var textureSamplerUniform;

var glPressedKey = 0;

var perspectiveProjectionMatrix;

var smiley_texture;




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
            
        case 31:
            glKeyPressed = 1;
            break;

        case 32:
            glKeyPressed = 2;
            break;

        case 33:
            glKeyPressed = 3;
            break;

        case 34:
            glKeyPressed = 4;
            break;

        default :
            glKeyPressed = 0;
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
        "precision highp float;"+
        "in vec2 oTexCoord;"+
        "uniform highp sampler2D uTextureSampler;"+
        "uniform int uKeyPressed;"+
        "out vec4 FragColor;"+
        "void main(void)"+
        "{"+
        "if(uKeyPressed == 0)"+
        "{"+
        "FragColor = vec4(1.0,1.0,1.0,1.0);"+
        "}"+
        "else"+
        "{"+
        "FragColor = texture(uTextureSampler,oTexCoord);"+
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

    gl.bindAttribLocation(shaderProgramObject, VertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

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

    textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "uTextureSampler");

    keyPressedUniform = gl.getUniformLocation(shaderProgramObject,"uKeyPressed");

    // geometry attribute declaration
    var square_position = new Float32Array([
        1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0]);

    // vao_square
    vao_square = gl.createVertexArray();

    gl.bindVertexArray(vao_square);

    // vbo_positionSquare
    vbo_positionSquare = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionSquare);

    gl.bufferData(gl.ARRAY_BUFFER, square_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    // vbo_texcoordSquare
    vbo_texcoordSquare = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texcoordSquare);

    gl.bufferData(gl.ARRAY_BUFFER, 8*4, gl.DYNAMIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_TEXCOORD);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    // depth initialization
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // set clear color
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    loadGLTexture();

    // initialize projection matrix
    perspectiveProjectionMatrix = mat4.create();
}

function loadGLTexture() {

    smiley_texture = gl.createTexture();

    smiley_texture.image = new Image();

    smiley_texture.image.src = "Smiley.png";

    smiley_texture.image.onload = function () {
        gl.bindTexture(gl.TEXTURE_2D, smiley_texture);

        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, smiley_texture.image);

        gl.generateMipmap(gl.TEXTURE_2D);

        gl.bindTexture(gl.TEXTURE_2D, null);
    }
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

    //variable declaration
    var squareTexCoords = [8];

    // code

    gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    // transformation
    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -3.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    // for texture
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, smiley_texture);
    gl.uniform1i(textureSamplerUniform, 0);

    if (glPressedKey == 1)
    {
        squareTexCoords[0] = 1.0;
        squareTexCoords[1] = 1.0;
        squareTexCoords[2] = 0.0;
        squareTexCoords[3] = 1.0;
        squareTexCoords[4] = 0.0;
        squareTexCoords[5] = 0.0;
        squareTexCoords[6] = 1.0;
        squareTexCoords[7] = 0.0;

        gl.uniform1i(keyPressedUniform, 1);
    }
    else if (glPressedKey == 2)
    {
        squareTexCoords[0] = 0.5;
        squareTexCoords[1] = 0.5;
        squareTexCoords[2] = 0.0;
        squareTexCoords[3] = 0.5;
        squareTexCoords[4] = 0.0;
        squareTexCoords[5] = 0.0;
        squareTexCoords[6] = 0.5;
        squareTexCoords[7] = 0.0;

        gl.uniform1i(keyPressedUniform, 1);
    }
    else if (glPressedKey == 3)
    {
        squareTexCoords[0] = 2.0;
        squareTexCoords[1] = 2.0;
        squareTexCoords[2] = 0.0;
        squareTexCoords[3] = 2.0;
        squareTexCoords[4] = 0.0;
        squareTexCoords[5] = 0.0;
        squareTexCoords[6] = 2.0;
        squareTexCoords[7] = 0.0;

        gl.uniform1i(keyPressedUniform, 1);
    }
    else if (glPressedKey == 4)
    {
        squareTexCoords[0] = 0.5;
        squareTexCoords[1] = 0.5;
        squareTexCoords[2] = 0.5;
        squareTexCoords[3] = 0.5;
        squareTexCoords[4] = 0.5;
        squareTexCoords[5] = 0.5;
        squareTexCoords[6] = 0.5;
        squareTexCoords[7] = 0.5;

        glUniform1i(keyPressedUniform, 1);
    }
    else
    {
        gl.uniform1i(keyPressedUniform, 0);
    }

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texcoordSquare);

    gl.bufferData(gl.ARRAY_BUFFER, squareTexCoords, gl.DYNAMIC_DRAW);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(vao_square);

    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);

    gl.bindVertexArray(null);

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

    if (vbo_texcoordSquare != null) {
        gl.deleteBuffer(vbo_texcoordSquare);
        vbo_texcoordSquare = null;
    }

    if (vbo_positionSquare != null) {
        gl.deleteBuffer(vbo_positionSquare);
        vbo_positionSquare = null;
    }

    if (vao_square != null) {
        gl.deleteVertexArrray(vao_square);
        vao_square = null;
    }

    if (smiley_texture != 0) {
        gl.deleteTextures(1, smiley_texture);
        smiley_texture = 0;
    }
}
