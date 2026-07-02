/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowController.h"
#include "WindowViewController.h"

@interface WindowController () <NSToolbarDelegate>;

@property (nonatomic, strong) NSToolbar* toolbar;

@end

@implementation WindowController

- (instancetype)initWithContentRect:(NSRect)contentRect windowID:(int)id
{
    if (self = [super init]) {
        self.windowID = id;
        self.toolbar = [[NSToolbar alloc] initWithIdentifier:@"Toolbar"];
        [self.toolbar setDelegate:self];
        [self.toolbar setDisplayMode:NSToolbarDisplayModeIconOnly];
        if (@available(macOS 15, *)) {
            if ([self.toolbar respondsToSelector:@selector(setAllowsDisplayModeCustomization:)]) {
                [self.toolbar performSelector:@selector(setAllowsDisplayModeCustomization:) withObject:nil];
            }
        }
        [self.toolbar setAllowsUserCustomization:NO];
        [self.toolbar setSizeMode:NSToolbarSizeModeRegular];

        auto* viewController = [[WindowViewController alloc] initWithFrame:contentRect];

        self.window = [NSWindow windowWithContentViewController:viewController];
        [self.window setDelegate:self];

        [self.window setToolbar:self.toolbar];
        [self.window setToolbarStyle:NSWindowToolbarStyleUnified];
    }
    return self;
}

@end
