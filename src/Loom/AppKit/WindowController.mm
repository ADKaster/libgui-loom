/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowController.h"
#include "Window.h"
#include "WindowView.h"
#include "WindowViewController.h"

@implementation WindowController

- (instancetype)initWithContentRect:(NSRect)contentRect windowID:(int)id
{
    if (self = [super init]) {
        self.windowID = id;

        auto* viewController = [[WindowViewController alloc] initWithFrame:contentRect];

        self.window = [Window windowWithContentViewController:viewController];
        [self.window setDelegate:self];
        [self.window setTitlebarAppearsTransparent:YES];
        [self.window setTitleVisibility:NSWindowTitleHidden];
        self.window.styleMask = NSWindowStyleMaskBorderless;
        self.window.styleMask |= NSWindowStyleMaskResizable;
        [self.window setMovableByWindowBackground:YES];

        [[self.window standardWindowButton:NSWindowCloseButton] setHidden:YES];
        [[self.window standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
        [[self.window standardWindowButton:NSWindowZoomButton] setHidden:YES];

        [(WindowView*)viewController.view configureTitlebarButtons];
        [viewController.view setNeedsLayout:YES];
    }
    return self;
}

@end
