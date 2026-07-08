/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ContentView.h"
#include "Conversions.h"
#include "WindowController.h"

#include <LibGfx/Palette.h>

@interface ContentView ()

@property (nonatomic, strong) NSTrackingArea* trackingArea;

@end

@implementation ContentView

- (void)viewDidMoveToWindow
{
    [super viewDidMoveToWindow];
    [self.window makeFirstResponder:self];
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)becomeFirstResponder
{
    return YES;
}

- (BOOL)resignFirstResponder
{
    return YES;
}

- (void)updateTrackingAreas
{
    [super updateTrackingAreas];

    if (self.trackingArea)
        [self removeTrackingArea:self.trackingArea];

    self.trackingArea = [[NSTrackingArea alloc] initWithRect:self.bounds options:(NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveAlways | NSTrackingInVisibleRect) owner:self userInfo:nil];
    [self addTrackingArea:self.trackingArea];
}

- (void)forwardEvent:(NSEvent*)event
{
    [self.eventDelegate contentView:self didReceiveEvent:event];
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];

    auto* controller = (WindowController*)self.window.windowController;
    auto* image = controller.backingStoreImage;
    if (!image)
        return;

    auto draw_size = controller.backingStoreVisibleSize;
    if (draw_size.width <= 0.0 || draw_size.height <= 0.0)
        draw_size = image.size;

    if (draw_size.width <= 0.0 || draw_size.height <= 0.0)
        return;

    auto draw_rect = NSMakeRect(0.0, 0.0, draw_size.width, draw_size.height);
    [image drawInRect:draw_rect fromRect:NSMakeRect(0, 0, draw_size.width, draw_size.height) operation:NSCompositingOperationSourceOver fraction:1.0];
}

- (void)mouseDown:(NSEvent*)event { [self forwardEvent:event]; }
- (void)mouseDragged:(NSEvent*)event { [self forwardEvent:event]; }
- (void)mouseUp:(NSEvent*)event { [self forwardEvent:event]; }
- (void)rightMouseDown:(NSEvent*)event { [self forwardEvent:event]; }
- (void)rightMouseDragged:(NSEvent*)event { [self forwardEvent:event]; }
- (void)rightMouseUp:(NSEvent*)event { [self forwardEvent:event]; }
- (void)otherMouseDown:(NSEvent*)event { [self forwardEvent:event]; }
- (void)otherMouseDragged:(NSEvent*)event { [self forwardEvent:event]; }
- (void)otherMouseUp:(NSEvent*)event { [self forwardEvent:event]; }
- (void)mouseMoved:(NSEvent*)event { [self forwardEvent:event]; }
- (void)mouseEntered:(NSEvent*)event { [self forwardEvent:event]; }
- (void)mouseExited:(NSEvent*)event { [self forwardEvent:event]; }
- (void)scrollWheel:(NSEvent*)event { [self forwardEvent:event]; }
- (void)keyDown:(NSEvent*)event { [self forwardEvent:event]; }
- (void)keyUp:(NSEvent*)event { [self forwardEvent:event]; }
- (void)flagsChanged:(NSEvent*)event { [self forwardEvent:event]; }

@end
