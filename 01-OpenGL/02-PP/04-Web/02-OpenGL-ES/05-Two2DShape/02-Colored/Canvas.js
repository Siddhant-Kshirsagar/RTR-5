
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
    AMC_ATTRIBUTE_COLOR: 1
};

var shaderProgramObject = null;

var vao_triangle = null;
var vbo_positionTriangle = null;
var vbo_colorTriangle = null;

var vao_square = null;
var vbo_positionSquare = null;
var vbo_colorSquare = null;

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
        "uniform mat4 uMVPMatrix;" +
        "in vec4 aPosition;" +
        "in vec4 aColor;" +
        "out vec4 oColor;" +
        "void main(void)" +
        "{" +
        "gl_Position= uMVPMatrix * aPosition;" +
        "oColor = aColor;" +
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
        "in vec4 oColor;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = oColor;" +
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

    // geometry attribute declaration
    var triangle_position = new Float32Array([
        0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0]);

    var triangle_color = new Float32Array([
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0]);

    // geometry attribute declaration
    var square_position = new Float32Array([
        1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0]);

    // vao_triangle
    vao_triangle = gl.createVertexArray();

    gl.bindVertexArray(vao_triangle);

    // vbo_positionTriangle
    vbo_positionTriangle = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionTriangle);

    gl.bufferData(gl.ARRAY_BUFFER, triangle_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    // vbo_colorTriangle
    vbo_colorTriangle = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_colorTriangle);

    gl.bufferData(gl.ARRAY_BUFFER, triangle_color, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_COLOR);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    // vao_square
    vao_square = gl.createVertexArray();

    gl.bindVertexArray(vao_square);

    // vbo_position
    vbo_positionSquare = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionSquare);

    gl.bufferData(gl.ARRAY_BUFFER, square_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    // vbo_color
    gl.vertexAttrib3f(VertexAttributeEnum.AMC_ATTRIBUTE_COLOR, 0.0, 0.0, 1.0);

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
    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [-1.5, 0.0, -5.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    gl.bindVertexArray(vao_triangle);

    gl.drawArrays(gl.TRIANGLES, 0, 3);

    gl.bindVertexArray(null);

    modelViewMatrix = mat4.create();
    modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [1.5, 0.0, -5.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);


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

    if (vbo_colorTriangle != null) {
        gl.deleteBuffer(vbo_colorTriangle);
        vbo_colorTriangle = null;
    }

    if (vbo_positionTriangle != null) {
        gl.deleteBuffer(vbo_positionTriangle);
        vbo_positionTriangle = null;
    }

    if (vao_triangle != null) {
        gl.deleteVertexArrray(vao_triangle);
        vao_triangle = null;
    }
}
