//
// Created by Andrew Kaster on 6/22/26.
//

#include "Application.h"
#include <LibCore/EventLoop.h>
#include <LibCore/ThreadEventQueue.h>

@interface Application ()
@end

@implementation Application

#pragma mark - NSApplication

- (void)terminate:(id)sender
{
    Core::EventLoop::current().quit(0);
}

- (void)sendEvent:(NSEvent*)event
{
    if ([event type] == NSEventTypeApplicationDefined) {
        Core::ThreadEventQueue::current().process();
    } else {
        [super sendEvent:event];
    }
}

@end
