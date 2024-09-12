#import"AppDelegate.h"

#import"ViewController.h"

#import"GLESView.h"

@implementation AppDelegate
{
 @private
    UIWindow *window;
    ViewController *viewController;
    GLESView *glesView;
}

//-(BOOL)application:(UIApplication*)application : (NSDictionary )
-(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    // code
    // get the iPhoneScreen rectangle
    CGRect screeenRect = [[UIScreen mainScreen]bounds];
    
    // create window
    window = [[UIWindow alloc] initWithFrame:screeenRect];
    
    // create view controller
    viewController = [[ViewController alloc] init];
    
    // set this view controller as window view controller
    [window setRootViewController:viewController];
    
    // create our custom view
    glesView = [[GLESView alloc] initWithFrame:screeenRect];
    
    // set this view as view of ViewController
    [viewController setView:glesView];
    
    // decrement the reference of count of view which is incremented by setView
    [glesView release];
    
    // set focus on this window and make int visible
    [window makeKeyAndVisible];
    
    // start display link
    [glesView startDisplayLink];
    
    
    return(YES);
}

-(void)applicationWillResignActive:(UIApplication *)application
{
    // code
    // stop display link
    [glesView stopDisplayLink];
}

-(void)applicationDidEnterBackground:(UIApplication *)application
{
    // code
}

-(void)applicationWillEnterForeground:(UIApplication *)application
{
    // code
    
}

-(void)applicationDidBecomeActive:(UIApplication *)application
{
    // code
    // start display link
    [glesView startDisplayLink];
}

-(void)applicationWillTerminate:(UIApplication *)application
{
    // code
    // stop display link
    [glesView stopDisplayLink];
}

-(void)dealloc
{
    // code
    [super dealloc];
    [glesView release];
    [viewController release];
    [window release];
}
@end
