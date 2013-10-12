#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <OpenGL/OpenGL.h>
#import "ESOpenGLView.h"
#import "base.h"

@implementation ESOpenGLView

// pixel format definition
+ (NSOpenGLPixelFormat*) esPixelFormat
{
    NSOpenGLPixelFormatAttribute attributes [] = {
    //NSOpenGLPFAFullScreen,
    //NSOpenGLPFAScreenMask,
    //            CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
		NSOpenGLPFAWindow,
        NSOpenGLPFAAllRenderers,
        NSOpenGLPFAAccelerated,
		NSOpenGLPFANoRecovery,
		//NSOpenGLPFAMPSafe,
		NSOpenGLPFABackingStore,
		NSOpenGLPFADoubleBuffer,	// double buffered
        NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute)32, // 32 bit color buffer
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)16, // 32 bit depth buffer
//		NSOpenGLPFAAlphaSize, (NSOpenGLPixelFormatAttribute)8,
        (NSOpenGLPixelFormatAttribute)nil
    };
    return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
}

-(id) initWithFrame: (NSRect) frameRect
{
	NSOpenGLPixelFormat * pf = [ESOpenGLView esPixelFormat];
	
	self = [super initWithFrame: frameRect pixelFormat: pf];
		
    return self;
}

- (BOOL)needsDisplay
{
	return NO;
}

@end
