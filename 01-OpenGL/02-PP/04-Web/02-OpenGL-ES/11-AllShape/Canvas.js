
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

var vao = null;
var vbo_position = null;
var vbo_color = null;

// for line
var vao_line;
var vbo_positionLine;

// for triangle
var vao_triangle;
var vbo_positionTriangle;

// for square
var vao_square;
var vbo_positionSquare;

// for circle
var vao_circle;
var vbo_positionCircle;

var mvpMatrixUniform;
var colorUniform;
var pointSizeUniform;

var iKeyPressed = 0;

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

        case 49:
            iKeyPressed = 1;
            break;

        case 50:
            iKeyPressed = 2;
            break;

        case 51:
            iKeyPressed = 3;
            break;

        case 52:
            iKeyPressed = 4;
            break;

        default:
            iKeyPressed = 0;
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
        "uniform mat4 uMVPMatrix;"+
        "uniform float uPointSize;"+
        "in vec4 aPosition;"+
        "void main(void)"+
        "{"+
        "gl_Position= uMVPMatrix * aPosition;"+
        "gl_PointSize = uPointSize;"+
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
        "uniform vec4 uColor;"+
        "out vec4 FragColor;"+
        "void main(void)"+
        "{"+
        "FragColor = uColor;"+
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

    gl.bindAttribLocation(shaderProgramObject, VertexAttributeEnum.AMC_ATTRIBUTE_COLOR, "oColor");

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
    colorUniform = gl.getUniformLocation(shaderProgramObject,"uColor");
    pointSizeUniform = gl.getUniformLocation(shaderProgramObject,"uPointSize");

    // geometry attribute declaration
    var square_position = new Float32Array([
        1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0]);

    // geometry attribute declaration
    var triangle_position = new Float32Array([
        0.0,1.0,0.0, // glVertex3f() 1 st call for triangle 
        -1.0,-1.0,0.0, // glVertex3f() 2nd call for triangle
        1.0,-1.0,0.0 // glVertex3f() 3rd  call for triangle
    ]);

    var circle_position = [];

    var i = 0;
    for (var iAngle = 0; iAngle < 360; iAngle = iAngle + 1)
    {
        var x = 0.0 + 1.0 * Math.cos((iAngle * Math.PI) / 180.0);
        var y = 0.0 + 1.0 * Math.sin((iAngle * Math.PI) / 180.0);
        var z = 0.0;

        circle_position.push(x);
        i++;
        circle_position.push(y);
        i++;
        circle_position.push(z);
        i++;

        // circle_position[i]= x;
        // i++;
        // circle_position[i] = y;
        // i++;
        // circle_position[i] = z;
        // i++;
    }

    console.log(circle_position.length);

    // geometry attribute declaration
    var line_position = new Float32Array([
        // horizontal line
        -2.0,0.0,0.0,
        2.0,0.0,0.0,

        // vertical line
        0.0,2.0,0.0,
        0.0,-2.0,0.0
    ]);

    // vao for triangle
    vao_triangle = gl.createVertexArray();

    gl.bindVertexArray(vao_triangle);

    // vbo_position
    vbo_positionTriangle = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionTriangle);

    gl.bufferData(gl.ARRAY_BUFFER, triangle_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);


    // vao for square
    vao_square = gl.createVertexArray();

    gl.bindVertexArray(vao_square);

    // vbo_position
    vbo_positionSquare = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionSquare);

    gl.bufferData(gl.ARRAY_BUFFER, square_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);


    // vao for circle
    vao_circle = gl.createVertexArray();

    gl.bindVertexArray(vao_circle);

    // vbo_position
    vbo_positionCircle = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionCircle);

    gl.bufferData(gl.ARRAY_BUFFER, circle_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);


    // vao for line
    vao_line = gl.createVertexArray();

    gl.bindVertexArray(vao_line);

    // vbo_position
    vbo_positionLine = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_positionLine);

    gl.bufferData(gl.ARRAY_BUFFER, line_position, gl.STATIC_DRAW);

    gl.vertexAttribPointer(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(VertexAttributeEnum.AMC_ATTRIBUTE_POSITION);

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

    if (iKeyPressed == 1)
    {
        drawLine();
        drawTriangle();
    }
    else if (iKeyPressed == 2)
    {
        drawLine();
        drawSquare();
    }
    else if (iKeyPressed == 3)
    {
        drawLine();
        drawCircle();
    }
    else if (iKeyPressed == 4)
    {
        drawLine();
    }

    // update for animation 
    update();
    // do the double buffering
    requestAnimationFrame(display, canvas);
}

function drawTriangle()
{
    // code
    gl.useProgram(shaderProgramObject);

    // transformation
    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -3.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
    gl.uniform4f(colorUniform,1.0,1.0,0.0,1.0);

    gl.bindVertexArray(vao_triangle);

    gl.drawArrays(gl.LINE_LOOP, 0, 3);

    gl.bindVertexArray(null);

    gl.useProgram(null);
}

function drawSquare()
{
    // code
    gl.useProgram(shaderProgramObject);

    // transformation
    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -3.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
    gl.uniform4f(colorUniform,1.0,1.0,0.0,1.0);

    gl.bindVertexArray(vao_square);

    gl.drawArrays(gl.LINE_LOOP, 0, 4);

    gl.bindVertexArray(null);

    gl.useProgram(null);
}

function drawCircle()
{
    // code
    gl.useProgram(shaderProgramObject);

    // transformation
    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -3.0]);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
    gl.uniform4f(colorUniform,1.0,1.0,0.0,1.0);

    gl.bindVertexArray(vao_circle);

    gl.drawArrays(gl.LINE_LOOP, 0, 360);

    gl.bindVertexArray(null);

    gl.useProgram(null);
}

function drawLine()
{
    // code
    gl.useProgram(shaderProgramObject);

    // transformation
    var modelViewMatrix = mat4.create();
    var translationMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();
    var lineCount = 0;

    // positive x axis (Vertical Line)
    for(var xAxis = 0.05; xAxis <= 2.0; xAxis = xAxis + 0.05)
    {
        lineCount = lineCount + 1;
        if (lineCount % 5 == 0)
        {
            // gl.lineWidth(2.0);
            gl.uniform1f(pointSizeUniform,2.0);
        }
        else
        {
            // gl.lineWidth(1.0);
            gl.uniform1f(pointSizeUniform,1.0);
        }

        modelViewMatrix = mat4.create();
        translationMatrix = mat4.create();
        modelViewProjectionMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [xAxis, 0.0, -3.0]);
        mat4.multiply(modelViewMatrix, modelViewMatrix, translationMatrix);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
        gl.uniform4f(colorUniform,0.0,0.0,1.0,1.0);

        gl.bindVertexArray(vao_line);

        gl.drawArrays(gl.LINES, 2, 2);

        gl.bindVertexArray(null);
    }

    // negative x axis (Vertical Line)
    for(var xAxis = -0.05; xAxis >= -2.0; xAxis = xAxis - 0.05)
    {
        lineCount = lineCount + 1;
        if (lineCount % 5 == 0)
        {
            gl.lineWidth(2.0);
        }
        else
        {
            gl.lineWidth(1.0);
        }

        modelViewMatrix = mat4.create();
        translationMatrix = mat4.create();
        modelViewProjectionMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [xAxis, 0.0, -3.0]);
        mat4.multiply(modelViewMatrix, modelViewMatrix, translationMatrix);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
        gl.uniform4f(colorUniform,0.0,0.0,1.0,1.0);

        gl.bindVertexArray(vao_line);

        gl.drawArrays(gl.LINES, 2, 2);

        gl.bindVertexArray(null);
    }

      // positive y axis (Horizontal Line)
    for(var yAxis = 0.05; yAxis <= 2.0; yAxis = yAxis + 0.05)
    {
        lineCount = lineCount + 1;
        if (lineCount % 5 == 0)
        {
            gl.lineWidth(2.0);
        }
        else
        {
            gl.lineWidth(1.0);
        }

        modelViewMatrix = mat4.create();
        translationMatrix = mat4.create();
        modelViewProjectionMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.0, yAxis, -3.0]);
        mat4.multiply(modelViewMatrix, modelViewMatrix, translationMatrix);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
        gl.uniform4f(colorUniform,0.0,0.0,1.0,1.0);

        gl.bindVertexArray(vao_line);

        gl.drawArrays(gl.LINES, 0, 2);

        gl.bindVertexArray(null);
    }

    // positive y axis (Horizontal Line)
    for(var yAxis = -0.05; yAxis >= -2.0; yAxis = yAxis - 0.05)
    {
        lineCount = lineCount + 1;
        if (lineCount % 5 == 0)
        {
            gl.lineWidth(2.0);
        }
        else
        {
            gl.lineWidth(1.0);
        }

        modelViewMatrix = mat4.create();
        translationMatrix = mat4.create();
        modelViewProjectionMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.0, yAxis, -3.0]);
        mat4.multiply(modelViewMatrix, modelViewMatrix, translationMatrix);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
        gl.uniform4f(colorUniform,0.0,0.0,1.0,1.0);

        gl.bindVertexArray(vao_line);

        gl.drawArrays(gl.LINES, 0, 2);

        gl.bindVertexArray(null);
    }

    // center horizontal line
    {
        modelViewMatrix = mat4.create();
        translationMatrix = mat4.create();
        modelViewProjectionMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -3.0]);
        mat4.multiply(modelViewMatrix, modelViewMatrix, translationMatrix);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
        gl.uniform4f(colorUniform,1.0,0.0,0.0,1.0);

        gl.bindVertexArray(vao_line);

        gl.drawArrays(gl.LINES, 2, 2);

        gl.bindVertexArray(null);
    }

    // center vertical line
    {
        modelViewMatrix = mat4.create();
        translationMatrix = mat4.create();
        modelViewProjectionMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -3.0]);
        mat4.multiply(modelViewMatrix, modelViewMatrix, translationMatrix);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
        gl.uniform4f(colorUniform,0.0,1.0,0.0,1.0);

        gl.bindVertexArray(vao_line);

        gl.drawArrays(gl.LINES, 0, 2);

        gl.bindVertexArray(null);
    }

    gl.useProgram(null);
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

    if (vbo_color != null) {
        gl.deleteBuffer(vbo_color);
        vbo_color = null;
    }

    if (vbo_position != null) {
        gl.deleteBuffer(vbo_position);
        vbo_position = null;
    }

    if (vao != null) {
        gl.deleteVertexArrray(vao);
        vao = null;
    }
}
