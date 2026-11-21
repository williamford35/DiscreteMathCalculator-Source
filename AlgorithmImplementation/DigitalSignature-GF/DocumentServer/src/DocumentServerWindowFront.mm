#import <Cocoa/Cocoa.h>

extern "C++" void BringDocumentServerWindowToFront(void* handle)
{
    if (handle == nullptr)
        return;

    NSView* view = (__bridge NSView*)handle;

    if (![view isKindOfClass:[NSView class]])
        return;

    NSWindow* window = [view window];

    if (window != nil)
        [window orderFrontRegardless];
}
