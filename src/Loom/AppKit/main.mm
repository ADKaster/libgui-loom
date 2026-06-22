/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibCore/EventLoop.h>
#include "ApplicationDelegate.h"
#include "CocoaWrapper.h"
#include "EventLoopImplementation.h"

int main(int argc, const char* argv[])
{
    Core::EventLoopManager::install(*new Mac::CFEventLoopManager);
    Core::EventLoop event_loop;

    NSApplication* app = [NSApplication sharedApplication];
    LoomAppDelegate* delegate = [[LoomAppDelegate alloc] init];
    [app setActivationPolicy:NSApplicationActivationPolicyRegular];
    [app setDelegate:delegate];

    (void)argc;
    (void)argv;

    return event_loop.exec();
}
