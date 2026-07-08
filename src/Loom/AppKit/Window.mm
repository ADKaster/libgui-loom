/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Window.h"
#include "WindowView.h"
#include <objc/runtime.h>

@implementation Window

static void const* s_serenity_restore_frame_key = &s_serenity_restore_frame_key;
static void const* s_serenity_zoomed_key = &s_serenity_zoomed_key;

+ (instancetype)windowWithContentViewController:(NSViewController*)contentViewController
{
    return [super windowWithContentViewController:contentViewController];
}

- (void)serenityCloseWindow:(id)sender
{
    (void)sender;
    [self close];
}

- (BOOL)canBecomeKeyWindow
{
    return YES;
}

- (BOOL)canBecomeMainWindow
{
    return YES;
}

- (void)becomeKeyWindow
{
    [super becomeKeyWindow];
    [self.contentView setNeedsDisplay:YES];
}

- (void)resignKeyWindow
{
    [super resignKeyWindow];
    [self.contentView setNeedsDisplay:YES];
}

- (void)becomeMainWindow
{
    [super becomeMainWindow];
    [self.contentView setNeedsDisplay:YES];
}

- (void)resignMainWindow
{
    [super resignMainWindow];
    [self.contentView setNeedsDisplay:YES];
}

- (void)serenityToggleMiniaturize:(id)sender
{
    if (self.isMiniaturized)
        [self deminiaturize:sender];
    else
        [self miniaturize:sender];
}

- (void)serenitySetMiniaturized:(BOOL)miniaturized
{
    if (miniaturized)
        [self miniaturize:nil];
    else if (self.isMiniaturized)
        [self deminiaturize:nil];
}

- (BOOL)serenityIsMiniaturized
{
    return self.isMiniaturized;
}

- (void)serenityToggleZoom:(id)sender
{
    [self serenitySetZoomed:![self serenityIsZoomed]];
    (void)sender;
}

- (void)serenitySetZoomed:(BOOL)zoomed
{
    if (zoomed == [self serenityIsZoomed])
        return;

    if (zoomed) {
        objc_setAssociatedObject(self, s_serenity_restore_frame_key, [NSValue valueWithRect:self.frame], OBJC_ASSOCIATION_RETAIN_NONATOMIC);
        auto* screen = self.screen ?: NSScreen.mainScreen;
        [self setFrame:screen.visibleFrame display:YES animate:NO];
        objc_setAssociatedObject(self, s_serenity_zoomed_key, @(YES), OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    } else {
        auto* value = (NSValue*)objc_getAssociatedObject(self, s_serenity_restore_frame_key);
        if (value)
            [self setFrame:value.rectValue display:YES animate:NO];
        objc_setAssociatedObject(self, s_serenity_zoomed_key, @(NO), OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }

    if ([self.contentView isKindOfClass:[WindowView class]])
        [(WindowView*)self.contentView updateZoomButtonImage];
}

- (BOOL)serenityIsZoomed
{
    return [objc_getAssociatedObject(self, s_serenity_zoomed_key) boolValue];
}

@end
