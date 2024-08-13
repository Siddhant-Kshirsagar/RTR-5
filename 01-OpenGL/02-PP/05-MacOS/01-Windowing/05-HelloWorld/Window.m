#import<Foundation/Foundation.h>
#import<Cocoa/Cocoa.h>

@interface AppDelegate:NSObject <NSApplicationDelegate,NSWindowDelegate>
@end

@interface View:NSView
@end

// main function
int main(int argc, char* argv[])
{
    //code

    // Create auto release pool for memory management
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

    // Create global shared NSApplication object
    NSApp = [NSApplication sharedApplication];

    // Set it's delegate to our own custom app delegate
    [NSApp setDelegate: [[AppDelegate alloc]init]];

    // Start NSApp run loop
    [NSApp run];

    // Let auto release pool release all pending object in our application
    [pool release];

    return(0);
}

// Implementation of our custom AppDelegate interface
@implementation AppDelegate
{
    NSWindow *window;
    View *view;
}

-(void)applicationDidFinishLaunching:(NSNotification *)notification
{
    // code
    // Step 1: Declare rectangle for frame/border of window
    NSRect win_rect = NSMakeRect(0.0,0.0,800.0,600.0);

    // Step 2: Create window
    window = [[NSWindow alloc]initWithContentRect: win_rect
                                        styleMask: NSWindowStyleMaskTitled|
                                                   NSWindowStyleMaskClosable|
                                                   NSWindowStyleMaskMiniaturizable| 
                                                   NSWindowStyleMaskResizable
                                         backing : NSBackingStoreBuffered
                                           defer : NO];

    // Give title to the window
    [window setTitle: @"SGK: Cocoa Window"];

    // center the window
    [window center];

    // create the custom view
    view = [[View alloc]initWithFrame: win_rect];

    // set this newly created custom view as view of our newly created custom windows
    [window setContentView: view];

    //
    [window setDelegate:self];

    // Actually show the focus, give it keyboard focus and make it top on Z-order
    [window makeKeyAndOrderFront:self];
}
    // Destroy application
    -(void)applicationWillTerminate:(NSNotification *)notification
    {

    }

    -(void)windowWillClose:(NSNotification *)notification
    {
        // code
        [NSApp terminate:self];
    }                                            

    -(void)dealloc
    {
        [super dealloc];
        [view release];
        [window release];
    }

@end

// Implementation of custom view interface
@implementation View
{
    NSString *string;
}

-(id)initWithFrame:(NSRect)frame
{
    // code
    self = [super initWithFrame:frame];
    if(self)
    {
        string = @"Hello World!!!";
    }

    return(self);
}

-(void)drawRect:(NSRect)dirtyRect
{
    // declare background color black
    NSColor *bgColor  = [NSColor blackColor];

    // set this color 
    [bgColor set];

    // fill the window/dirty rectangle with above black color
    NSRectFill(dirtyRect);

    // create the font for our string 
    NSFont *stringFont = [NSFont fontWithName:@"Helvetica" size:32];

    // create the string color 
    NSColor *fgColor = [NSColor greenColor];

    // create dictionary for our string attribute from above 2
    //NSDictionary *stringDictionary = [NSDictionary dictionaryWithObjectAndKeys: stringFont,NSFontAttributeName,        fgColor, NSForegroundColorAttributeName,nil];
      NSDictionary *stringDictionary = [NSDictionary dictionaryWithObjectsAndKeys : stringFont, NSFontAttributeName, fgColor, NSForegroundColorAttributeName, nil ];

    // create the 
    NSSize stringSize = [string sizeWithAttributes: stringDictionary];

    NSPoint stringPoint = NSMakePoint((dirtyRect.size.width/2 - stringSize.width/2),(dirtyRect.size.height/2 - stringSize.height/2));

    // draw the string hello World
    [string drawAtPoint:stringPoint withAttributes:stringDictionary];
}

-(BOOL) acceptsFirstResponder
{
    // code
    [[self window]makeFirstResponder:self];
    return(YES);
}

-(void)keyDown:(NSEvent *)event
{
    // code
    int key = (int)[[event characters] characterAtIndex: 0];

    switch(key)
    {
        case 27:
        [self release];
        [NSApp terminate:self];

        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self];
            break;

        default:
            break;
    }
}

// -(void)mouseDown:(NSEvent *)event
// {
//     // code
//     string=@"Left Mouse Button is clicked";
//     [self setNeedsDisplay:YES];
// }

// -(void)rightMouseDown:(NSEvent *)event
// {
//     // code
//     string=@"Right Mouse Button is clicked";
//     [self setNeedsDisplay:YES];
// }

-(void)dealloc
{
    [super dealloc];
}   

@end
/*
clang -c -o Window.o Window.m

mkdir -p Window.app /Contents/MacOS

clang -o Window.app /content/MacOS/Window Window.o -Framework Cocoa

*/