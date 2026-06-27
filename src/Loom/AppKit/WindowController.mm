/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowController.h"

@interface WindowController () <NSToolbarDelegate>;

@property (nonatomic, strong) NSToolbar* toolbar;

@end

@implementation WindowController

- (instancetype)init
{
    if (self = [super init]) {
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
    }
    return self;
}

#pragma mark - NSWindowController

- (IBAction)showWindow:(id)sender
{
    self.window = [[NSWindow alloc] init];

    [self.window setDelegate:self];

    [self.window setToolbar:self.toolbar];
    [self.window setToolbarStyle:NSWindowToolbarStyleUnified];

    [self.window makeKeyAndOrderFront:sender];
}

@end
