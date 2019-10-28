#import "AppDelegate.hpp"

int main(int argc, char** argv){
	int status = 0;
	@autoreleasepool {
		AppDelegate* delegate = [[[AppDelegate alloc] init] autorelease];
		[[NSApplication sharedApplication] setDelegate:delegate];
		status = NSApplicationMain(argc, (const char**)argv);
	}
	return status;
}
