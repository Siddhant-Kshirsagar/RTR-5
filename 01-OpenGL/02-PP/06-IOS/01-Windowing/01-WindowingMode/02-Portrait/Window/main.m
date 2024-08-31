#import<UIKit/UIKit.h>

#import"AppDelegate.h"

int main(int argc, char* argv[])
{
    // code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    // get the AppDelegate class name in the form of string
    NSString* delegateClassName = NSStringFromClass([AppDelegate class]);
    
    UIApplicationMain(argc, argv, nil, delegateClassName);
    
    [pool release];
    
    return(0);
}
