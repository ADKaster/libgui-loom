/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ApplicationDelegate.h"
#include <AK/NonnullRefPtr.h>
#include <AK/OwnPtr.h>
#include <Loom/IPCBridge.h>
#include "WindowServerCallbacksMacOS.h"

@implementation LoomAppDelegate {
    NSWindow* _window;
    OwnPtr<Loom::IPCBridge> m_ipc_bridge;
}

- (instancetype)init
{
    self = [super init];
    if (self == nil)
        return nil;

    auto callbacks = make_ref_counted<Loom::WindowServerCallbacksMacOS>();
    m_ipc_bridge = Loom::IPCBridge::create(move(callbacks));
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    (void)notification;

    NSRect frame = NSMakeRect(0, 0, 900, 600);
    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

    _window = [[NSWindow alloc] initWithContentRect:frame
                                          styleMask:style
                                            backing:NSBackingStoreBuffered
                                              defer:NO];
    [_window setTitle:@"Loom"];
    [_window center];
    [_window makeKeyAndOrderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
    (void)sender;
    return YES;
}

@end
