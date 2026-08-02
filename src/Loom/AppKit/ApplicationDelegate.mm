/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ApplicationDelegate.h"
#include <AK/OwnPtr.h>
#include <Loom/IPCBridge.h>

@implementation LoomAppDelegate {
    OwnPtr<Loom::IPCBridge> m_ipc_bridge;
}

- (instancetype)init
{
    self = [super init];
    if (self == nil)
        return nil;

    m_ipc_bridge = Loom::IPCBridge::create();
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    (void)notification;
    [NSApp activateIgnoringOtherApps:YES];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
    (void)sender;
    return NO;
}

@end
