#import"AppDelegate.h"

#import"ViewController.h"

#import"MyView.h"

@implementation AppDelegate
{
 @private
    UIWindow *window;
    ViewController *viewController;
    MyView *myView;
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
    myView = [[MyView alloc] initWithFrame:screeenRect];
    
    // set this view as view of ViewController
    [viewController setView:myView];
    
    // decrement the reference of count of view which is incremented by setView
    [myView release];
    
    // set focus on this window and make int visible
    [window makeKeyAndVisible];
    
    return(YES);
}

-(void)applicationWillResignActive:(UIApplication *)application
{
    // code
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
    
}

-(void)applicationWillTerminate:(UIApplication *)application
{
    // code
    
}

-(void)dealloc
{
    // code
    [super dealloc];
    [myView release];
    [viewController release];
    [window release];
}
@end
