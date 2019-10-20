#import "AppDelegate.h"

@implementation AppDelegate

- (void)dealloc{
    [super dealloc];
}
	
- (void)applicationDidBecomeActive:(NSNotification *)aNotification{
	printf("%s", "Test");
}

@end
