#import"MyView.h"

@implementation MyView
{
    NSString* string;
}

-(id)initWithFrame:(CGRect)frame
{
    // code
    self = [super initWithFrame:frame];
    if(self)
    {
        // set the background to black
        [self setBackgroundColor:[UIColor blackColor]];
        
        // set the string
        string = @"Landscape";
        
        // // Single Tap
        // // 1 : create single tap gesture recognizer object
        // UITapGestureRecognizer* singleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onSingleTap:)];
        
        // // 2 : set number of tap
        // [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        
        // // 3 : set number of finger
        // [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        
        // // 4 : set the delegate as self
        // [singleTapGestureRecognizer setDelegate:self];
        
        // // 5 : add this gesture recognizer to the self
        // [self addGestureRecognizer:singleTapGestureRecognizer];
        
        
        // // Double Tap
        // // same step as single tap
        // // 1
        // UITapGestureRecognizer* doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onDoubleTap:)];
        
        // // 2
        // [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        
        // // 3
        // [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        
        // // 4
        // [doubleTapGestureRecognizer setDelegate:self];
        
        // // 5
        // [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        // // double should for single tap failed
        // [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        // // Swipe
        // // 1 : create Object
        // UISwipeGestureRecognizer* swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(onSwipe:)];
        
        // // 2 : set delegate
        // [swipeGestureRecognizer setDelegate:self];

        // // 3 : add gesture recognizer
        // [self addGestureRecognizer:swipeGestureRecognizer];
        
        // // Long press
        // // 1 : create object
        // UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(onLongPress:)];
        
        // // 2 : set delegate
        // [longPressGestureRecognizer setDelegate:self];

        // // 3 : add gesture recognizer
        // [self addGestureRecognizer:longPressGestureRecognizer];
        
        
    }
    return(self);
}

-(void)drawRect:(CGRect)rect
{
    // declare background color black
    UIColor *bgColor  = [UIColor blackColor];
    
    // set this color
    [bgColor set];
    
    // fill the window/dirty rectangle with above black color
    UIRectFill(rect);
    
    // create the font for our string
    UIFont *stringFont = [UIFont fontWithName:@"Helvetica" size:24];
    
    // create the string color
    UIColor *fgColor = [UIColor greenColor];
    
    // create dictionary for our string attribute from above 2
    NSDictionary *stringDictionary = [NSDictionary dictionaryWithObjectsAndKeys : stringFont, NSFontAttributeName, fgColor, NSForegroundColorAttributeName, nil ];
    
    // create the
    CGSize stringSize = [string sizeWithAttributes: stringDictionary];
    
    //CGPoint stringPoint = CGMakePoint((rect.size.width/2 - stringSize.width/2),(rect.size.height/2 - stringSize.height/2));
    
    CGPoint point;
    point.x = (rect.size.width/2) - (stringSize.width/2);
    point.y = (rect.size.height/2) - (stringSize.height/2);
    
    
    // draw the string hello World
    [string drawAtPoint:point withAttributes:stringDictionary];
    
}

-(BOOL)becomeFirstResponder
{
    // code
    return(YES);
}

-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent *)event
{
    
}

// -(void)onSingleTap:(UITapGestureRecognizer*) gestureRecognizer
// {
//     // code
//     string = @"Single Tap";
        
//     [self setNeedsDisplay];
// }

// -(void)onDoubleTap:(UITapGestureRecognizer*) gestureRecognizer
// {
//     // code
//     string = @"Double Tap";
    
//     [self setNeedsDisplay];
// }

// -(void)onSwipe:(UISwipeGestureRecognizer*) gestureRecognizer
// {
//     // code
//     string = @"Swipe";
    
//     [self setNeedsDisplay];
// }

// -(void)onLongPress:(UILongPressGestureRecognizer*) gestureRecognizer
// {
//     // code
//     string = @"Long Press";
    
//     [self setNeedsDisplay];
    
// }

-(void)dealloc
{
    // code
    [super dealloc];
    
}

@end
