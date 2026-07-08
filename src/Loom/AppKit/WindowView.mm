/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowView.h"

#include "IconBag.h"
#include "Window.h"
#include "Conversions.h"
#include <LibGfx/Palette.h>
#include <LibGfx/SystemTheme.h>
#include <objc/message.h>

static Gfx::Palette current_palette()
{
    return Gfx::Palette(Gfx::PaletteImpl::create_with_anonymous_buffer(Gfx::current_system_theme_buffer()));
}

static NSTextAlignment ns_text_alignment_from_serenity(Gfx::TextAlignment alignment)
{
    switch (alignment) {
    case Gfx::TextAlignment::CenterLeft:
        return NSTextAlignmentLeft;
    case Gfx::TextAlignment::CenterRight:
        return NSTextAlignmentRight;
    case Gfx::TextAlignment::Center:
        return NSTextAlignmentCenter;
    default:
        return NSTextAlignmentLeft;
    }
}

@interface SerenityTitlebarButton : NSView

- (instancetype)initWithFrame:(NSRect)frame normalImage:(NSImage*)normalImage hoverImage:(NSImage*)hoverImage action:(SEL)action target:(id)target;
- (void)setNormalImage:(NSImage*)normalImage hoverImage:(NSImage*)hoverImage;

@end

@interface SerenityTitlebarButton ()

@property (nonatomic, strong) NSImage* normalImage;
@property (nonatomic, strong) NSImage* hoverImage;
@property (nonatomic, strong) NSTrackingArea* trackingArea;
@property (nonatomic, assign) SEL action;
@property (nonatomic, weak) id target;
@property (nonatomic, assign) BOOL hovered;
@property (nonatomic, assign) BOOL pressed;

@end

@implementation SerenityTitlebarButton

- (instancetype)initWithFrame:(NSRect)frame normalImage:(NSImage*)normalImage hoverImage:(NSImage*)hoverImage action:(SEL)action target:(id)target
{
    if (self = [super initWithFrame:frame]) {
        self.wantsLayer = NO;
        self.target = target;
        self.action = action;
        [self setNormalImage:normalImage hoverImage:hoverImage];
    }
    return self;
}

- (void)setNormalImage:(NSImage*)normalImage hoverImage:(NSImage*)hoverImage
{
    _normalImage = normalImage;
    _hoverImage = hoverImage;
}

- (void)updateTrackingAreas
{
    [super updateTrackingAreas];

    if (self.trackingArea)
        [self removeTrackingArea:self.trackingArea];

    self.trackingArea = [[NSTrackingArea alloc] initWithRect:self.bounds options:(NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveAlways | NSTrackingInVisibleRect) owner:self userInfo:nil];
    [self addTrackingArea:self.trackingArea];
}

- (void)mouseEntered:(NSEvent*)event
{
    (void)event;
    self.hovered = YES;
    [self setNeedsDisplay:YES];
}

- (void)mouseExited:(NSEvent*)event
{
    (void)event;
    self.hovered = NO;
    [self setNeedsDisplay:YES];
}

- (void)mouseMoved:(NSEvent*)event
{
    auto location = [self convertPoint:event.locationInWindow fromView:nil];
    BOOL inside = NSPointInRect(location, self.bounds);
    if (self.hovered != inside) {
        self.hovered = inside;
        [self setNeedsDisplay:YES];
    }
}

- (void)mouseDown:(NSEvent*)event
{
    self.pressed = YES;
    [self setNeedsDisplay:YES];

    while (true) {
        auto* next_event = [self.window nextEventMatchingMask:(NSEventMaskLeftMouseDragged | NSEventMaskLeftMouseUp)];
        auto location = [self convertPoint:next_event.locationInWindow fromView:nil];
        BOOL inside = NSPointInRect(location, self.bounds);
        if (self.pressed != inside) {
            self.pressed = inside;
            [self setNeedsDisplay:YES];
        }

        if (next_event.type == NSEventTypeLeftMouseUp) {
            if (inside && self.target && self.action) {
                using ActionMessage = void (*)(id, SEL, id);
                auto invoke = (ActionMessage)objc_msgSend;
                invoke(self.target, self.action, self);
            }
            break;
        }
    }

    self.pressed = NO;
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect
{
    auto palette = current_palette();
    auto rect = self.bounds;
    auto icon_only = palette.title_buttons_icon_only();

    if (icon_only) {
        auto* image = self.hovered && self.hoverImage ? self.hoverImage : self.normalImage;
        if (!image)
            return;

        auto size = image.size;
        auto bounds = self.bounds;
        auto offset = self.pressed ? 1.0 : 0.0;
        auto draw_rect = NSMakeRect((bounds.size.width - size.width) / 2.0,
            (bounds.size.height - size.height) / 2.0,
            size.width,
            size.height);
        draw_rect.origin.x += offset;
        draw_rect.origin.y += offset;

        auto inactive_alpha = palette.window_title_button_inactive_alpha() / 255.0f;
        auto active = self.window.isKeyWindow || self.window.isMainWindow;
        [image drawInRect:draw_rect fromRect:NSMakeRect(0, 0, size.width, size.height) operation:NSCompositingOperationSourceOver fraction:active ? 1.0 : inactive_alpha];
        return;
    }

    auto fill_color = self.hovered ? palette.hover_highlight() : palette.button();
    if (self.pressed)
        fill_color = palette.button();

    [(Loom::gfx_color_to_ns_color(fill_color) ?: NSColor.clearColor) setFill];
    NSRectFill(rect);

    auto highlight_color = Loom::gfx_color_to_ns_color(palette.threed_highlight());
    auto shadow1_color = Loom::gfx_color_to_ns_color(palette.threed_shadow1());
    auto shadow2_color = Loom::gfx_color_to_ns_color(palette.threed_shadow2());

    [highlight_color setFill];
    NSRectFill(NSMakeRect(rect.origin.x, rect.origin.y, rect.size.width - 1, 1));
    NSRectFill(NSMakeRect(rect.origin.x, rect.origin.y, 1, rect.size.height - 1));

    if (self.pressed) {
        [shadow2_color setFill];
        NSRectFill(NSMakeRect(rect.origin.x, rect.origin.y + 1, rect.size.width - 2, 1));
        NSRectFill(NSMakeRect(rect.origin.x + 1, rect.origin.y + 2, 1, rect.size.height - 3));

        [shadow1_color setFill];
        NSRectFill(NSMakeRect(rect.origin.x + 1, rect.origin.y + rect.size.height - 2, rect.size.width - 2, 1));
        NSRectFill(NSMakeRect(rect.origin.x + rect.size.width - 2, rect.origin.y + 1, 1, rect.size.height - 2));
    } else {
        [shadow2_color setFill];
        NSRectFill(NSMakeRect(rect.origin.x, rect.origin.y + rect.size.height - 1, rect.size.width, 1));
        NSRectFill(NSMakeRect(rect.origin.x + rect.size.width - 1, rect.origin.y, 1, rect.size.height - 1));

        [shadow1_color setFill];
        NSRectFill(NSMakeRect(rect.origin.x + 1, rect.origin.y + rect.size.height - 2, rect.size.width - 2, 1));
        NSRectFill(NSMakeRect(rect.origin.x + rect.size.width - 2, rect.origin.y + 1, 1, rect.size.height - 2));
    }

    auto* image = self.hovered && self.hoverImage ? self.hoverImage : self.normalImage;
    if (!image)
        return;

    auto size = image.size;
    auto bounds = self.bounds;
    auto offset = self.pressed ? 1.0 : 0.0;
    auto draw_rect = NSMakeRect((bounds.size.width - size.width) / 2.0,
        (bounds.size.height - size.height) / 2.0,
        size.width,
        size.height);
    draw_rect.origin.x += offset;
    draw_rect.origin.y += offset;
    auto inactive_alpha = palette.window_title_button_inactive_alpha() / 255.0f;
    auto active = self.window.isKeyWindow || self.window.isMainWindow;
    [image drawInRect:draw_rect fromRect:NSMakeRect(0, 0, size.width, size.height) operation:NSCompositingOperationSourceOver fraction:active ? 1.0 : inactive_alpha];
}

@end

@interface WindowView ()

@property (nonatomic, strong) SerenityTitlebarButton* closeButton;
@property (nonatomic, strong) SerenityTitlebarButton* minimizeButton;
@property (nonatomic, strong) SerenityTitlebarButton* zoomButton;
@property (nonatomic, strong) NSImage* windowIconImage;

@end

@implementation WindowView

- (instancetype)initWithFrame:(NSRect)frameRect
{
    if (self = [super initWithFrame:frameRect]) {
        self.wantsLayer = YES;
    }
    return self;
}

- (void)viewDidMoveToWindow
{
    [super viewDidMoveToWindow];
    self.window.acceptsMouseMovedEvents = YES;
    [self updateChromeAppearance];
    [self configureTitlebarButtons];
    [self layoutTitlebarButtons];
    [self setNeedsDisplay:YES];
}

- (void)updateChromeAppearance
{
    auto palette = current_palette();
    auto border_radius = static_cast<CGFloat>(palette.window_border_radius());
    self.window.hasShadow = YES;

    if (border_radius > 0.0) {
        self.window.opaque = NO;
        self.window.backgroundColor = NSColor.clearColor;
        self.layer.cornerRadius = border_radius;
        self.layer.masksToBounds = YES;
    } else {
        self.window.backgroundColor = nil;
        self.layer.cornerRadius = 0.0;
        self.layer.masksToBounds = NO;
    }
}

- (void)configureTitlebarButtons
{
    if (self.closeButton)
        return;

    auto& icons = Loom::IconBag::the();
    auto palette = current_palette();
    auto button_width = static_cast<CGFloat>(palette.window_title_button_width());
    auto button_height = static_cast<CGFloat>(palette.window_title_button_height());

    self.closeButton = [[SerenityTitlebarButton alloc] initWithFrame:NSMakeRect(0, 0, button_width, button_height) normalImage:icons.close hoverImage:icons.close_hover action:@selector(serenityCloseWindow:) target:self.window];
    self.minimizeButton = [[SerenityTitlebarButton alloc] initWithFrame:NSMakeRect(0, 0, button_width, button_height) normalImage:icons.minimize hoverImage:icons.minimize_hover action:@selector(serenityToggleMiniaturize:) target:self.window];
    self.zoomButton = [[SerenityTitlebarButton alloc] initWithFrame:NSMakeRect(0, 0, button_width, button_height) normalImage:icons.maximize hoverImage:icons.maximize_hover action:@selector(serenityToggleZoom:) target:self.window];

    [self addSubview:self.closeButton];
    [self addSubview:self.minimizeButton];
    [self addSubview:self.zoomButton];
    [self updateZoomButtonImage];
    [self layoutTitlebarButtons];
}

- (void)updateZoomButtonImage
{
    if (!self.zoomButton)
        return;

    auto& icons = Loom::IconBag::the();
    self.zoomButton.normalImage = [(Window*)self.window serenityIsZoomed] ? icons.restore : icons.maximize;
    self.zoomButton.hoverImage = [(Window*)self.window serenityIsZoomed] ? icons.restore_hover : icons.maximize_hover;
    [self.zoomButton setNeedsDisplay:YES];
}

- (void)layout
{
    [super layout];
    [self layoutTitlebarButtons];
}

- (void)setFrameSize:(NSSize)newSize
{
    [super setFrameSize:newSize];
    [self layoutTitlebarButtons];
}

- (void)layoutTitlebarButtons
{
    if (!self.closeButton)
        return;

    auto palette = current_palette();
    auto const border_thickness = static_cast<CGFloat>(palette.window_border_thickness());
    auto const border_radius = static_cast<CGFloat>(palette.window_border_radius());
    auto const button_width = static_cast<CGFloat>(palette.window_title_button_width());
    auto const button_height = static_cast<CGFloat>(palette.window_title_button_height());
    auto const titlebar_height = static_cast<CGFloat>(palette.window_title_height());
    auto const y = self.bounds.size.height - border_thickness - titlebar_height + floorf((titlebar_height - button_height) / 2.f);
    auto const right_inset = max(border_thickness, border_radius) + 1.f;

    auto close_x = self.bounds.size.width - button_width - right_inset;
    auto zoom_x = close_x - button_width;
    auto minimize_x = zoom_x - button_width;

    self.minimizeButton.frame = NSMakeRect(minimize_x, y, button_width, button_height);
    self.zoomButton.frame = NSMakeRect(zoom_x, y, button_width, button_height);
    self.closeButton.frame = NSMakeRect(close_x, y, button_width, button_height);
    [self.closeButton setNeedsDisplay:YES];
    [self.minimizeButton setNeedsDisplay:YES];
    [self.zoomButton setNeedsDisplay:YES];
}

- (void)setWindowIconImage:(NSImage*)iconImage
{
    _windowIconImage = iconImage;
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];

    auto palette = current_palette();
    auto const titlebar_height = static_cast<CGFloat>(palette.window_title_height());
    auto const border_thickness = static_cast<CGFloat>(palette.window_border_thickness());
    auto titlebar_rect = NSMakeRect(border_thickness, self.bounds.size.height - border_thickness - titlebar_height, self.bounds.size.width - border_thickness * 2, titlebar_height);

    auto base_color = palette.window();

    [(Loom::gfx_color_to_ns_color(base_color) ?: NSColor.clearColor) setFill];
    NSRectFill(self.bounds);

    auto highlight_color = Loom::gfx_color_to_ns_color(palette.threed_highlight());
    auto shadow2_color = Loom::gfx_color_to_ns_color(palette.threed_shadow2());
    [highlight_color setFill];
    NSRectFill(NSMakeRect(0, 0, self.bounds.size.width, border_thickness));
    NSRectFill(NSMakeRect(0, 0, border_thickness, self.bounds.size.height));
    [shadow2_color setFill];
    NSRectFill(NSMakeRect(0, self.bounds.size.height - border_thickness, self.bounds.size.width, border_thickness));
    NSRectFill(NSMakeRect(self.bounds.size.width - border_thickness, 0, border_thickness, self.bounds.size.height));

    auto active = self.window.isKeyWindow || self.window.isMainWindow;
    auto title_left = border_thickness + (self.windowIconImage ? 21.0 : 3.0);
    auto top_color = active ? palette.active_window_border1() : palette.inactive_window_border1();
    auto bottom_color = active ? palette.active_window_border2() : palette.inactive_window_border2();
    auto gradient = [[NSGradient alloc] initWithStartingColor:Loom::gfx_color_to_ns_color(top_color)
                                                  endingColor:Loom::gfx_color_to_ns_color(bottom_color)];
    [gradient drawInRect:titlebar_rect angle:90];
    auto stripes_color = Loom::gfx_color_to_ns_color(active ? palette.active_window_title_stripes() : palette.inactive_window_title_stripes());
    if (stripes_color.alphaComponent > 0.0) {
        [stripes_color setFill];
        auto title_width = [self.window.title sizeWithAttributes:@{ NSFontAttributeName : [NSFont systemFontOfSize:13 weight:NSFontWeightSemibold] }].width;
        auto title_right_edge = title_left + title_width + 4.0;
        auto button_left = self.closeButton.frame.origin.x;

        auto stripes_left = title_right_edge + 4.0;
        auto stripes_right = button_left - 3.0;
        if (stripes_left < stripes_right) {
            for (CGFloat y = titlebar_rect.origin.y + 2; y <= titlebar_rect.origin.y + titlebar_height - 3; y += 2)
                NSRectFill(NSMakeRect(stripes_left, y, stripes_right - stripes_left, 1));
        }
    }

    [Loom::gfx_color_to_ns_color(palette.button()) setFill];
    NSRectFill(NSMakeRect(border_thickness, titlebar_rect.origin.y + titlebar_height - 1, titlebar_rect.size.width, 1));

    auto* title = self.window.title ?: @"";
    auto title_color = active ? palette.active_window_title() : palette.inactive_window_title();
    auto title_right = self.closeButton.frame.origin.x - 3.0;
    auto title_rect = NSMakeRect(title_left,
        self.bounds.size.height - border_thickness - titlebar_height + (titlebar_height - 13.0) / 2.0,
        max(0.0, title_right - title_left),
        13.0);
    NSMutableDictionary* attributes = [@{
        NSFontAttributeName : [NSFont systemFontOfSize:13 weight:NSFontWeightSemibold],
        NSForegroundColorAttributeName : Loom::gfx_color_to_ns_color(title_color),
    } mutableCopy];
    auto paragraph_style = [[NSMutableParagraphStyle alloc] init];
    paragraph_style.lineBreakMode = NSLineBreakByTruncatingTail;
    paragraph_style.alignment = ns_text_alignment_from_serenity(palette.title_alignment());
    attributes[NSParagraphStyleAttributeName] = paragraph_style;
    auto title_shadow = Loom::gfx_color_to_ns_color(active ? palette.active_window_title_shadow() : palette.inactive_window_title_shadow());
    NSDictionary* shadow_attributes = @{
        NSFontAttributeName : [NSFont systemFontOfSize:13 weight:NSFontWeightSemibold],
        NSForegroundColorAttributeName : title_shadow,
        NSParagraphStyleAttributeName : paragraph_style,
    };
    [title drawInRect:NSOffsetRect(title_rect, 1, 2) withAttributes:shadow_attributes];
    [title drawInRect:NSOffsetRect(title_rect, 0, 1) withAttributes:attributes];

    auto* icon = self.windowIconImage;
    if (icon) {
        auto icon_rect = NSMakeRect(border_thickness + 2.0, titlebar_rect.origin.y + floorf((titlebar_height - 16.0) / 2.f), 16.0, 16.0);
        [icon drawInRect:icon_rect];
    }

}

@end
