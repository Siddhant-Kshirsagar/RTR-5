
// global variable 
var canvas = null;
var context = null;

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

    context = canvas.getContext("2d");
    if (context == null) {
        console.log("Getting 2d context failed.\n");
    }
    else {
        console.log("Getting 2d context succeeded.\n");
    }

    // set canvas background color to black
    context.fillStyle = "black";

    // paint the canvas rectangle with above color 
    context.fillRect(0, 0, canvas.width, canvas.height);

    // draw text
    drawText("Hello World !!!");

    // register for event
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);

}

function keyDown(event) {
    // code
    switch (event.keyCode) {
        case 70: // ascii for F
        case 102: // ascii for f
            toggleFullscreen();
            drawText("Hello World!!!");
            break;
    }
}

function mouseDown() {
    // code

}

function drawText(text) {
    // code

    // set the text in the middle of the canvas
    context.textAlign = "center"; // centering text horizontally

    context.textBaseline = "middle"; // centering text vertically

    //set font style
    context.font = "48px sans-serif";
    // set text color to white
    context.fillStyle = "lime";

    // paint the text with above color
    context.fillText(text, canvas.width / 2, canvas.height / 2);
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
    }
}
