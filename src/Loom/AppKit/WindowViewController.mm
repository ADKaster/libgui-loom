/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowViewController.h"
#include "ContentView.h"
#include "WindowView.h"

#include "Conversions.h"
#include <AK/Format.h>
#include <LibGfx/Palette.h>

static NSRect full_frame_rect_for_content_rect(NSRect content_rect)
{
    auto palette = Gfx::Palette(Gfx::PaletteImpl::create_with_anonymous_buffer(Gfx::current_system_theme_buffer()));
    auto const border_thickness = static_cast<CGFloat>(palette.window_border_thickness());
    auto const titlebar_height = static_cast<CGFloat>(palette.window_title_height());

    return NSMakeRect(0, 0,
        content_rect.size.width + border_thickness * 2.0,
        content_rect.size.height + border_thickness * 2.0 + titlebar_height);
}

@interface WindowViewController ()
@end

@implementation WindowViewController

- (instancetype)initWithFrame:(NSRect)frameRect
{
    if (self = [super init]) {
        auto* window_view = [[WindowView alloc] initWithFrame:full_frame_rect_for_content_rect(frameRect)];
        auto* content_view = [[ContentView alloc] initWithFrame:[window_view contentRect]];
        window_view.contentView = content_view;
        self.view = window_view;
        dbgln("input rect: {} view frame: {}", Loom::ns_rect_to_gfx_rect(frameRect), Loom::ns_rect_to_gfx_rect([self.view frame]));
    }
    return self;
}

@end
