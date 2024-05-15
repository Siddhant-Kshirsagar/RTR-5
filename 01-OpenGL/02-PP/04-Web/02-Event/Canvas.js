// main function
function main()
{
    // get canvas 
    var canvas = document.getElementById("sgk");

    if (canvas == null) {
        console.log("Getting canvas failed\n");
    }
    else{
        console.log("Getting canvas succeeded.\n");
    }

    var context = canvas.getContext("2d");
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

    // declare the text
    var text = "Hello World !!!";

    // set the text in the middle of the canvas
    context.textAlign = "center"; // centering text horizontally

    context.textBaseline = "middle"; // centering text vertically

    //set font style
    context.font = "48px sans-serif";
    // set text color to white
    context.fillStyle = "lime";

    // paint the text with above color
    context.fillText(text, canvas.width / 2, canvas.height / 2);

    // register for event
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);

}

function keyDown(event) {
    alert("key is pressed");
}

function mouseDown() {
    alert("mouse is clicked");
}
