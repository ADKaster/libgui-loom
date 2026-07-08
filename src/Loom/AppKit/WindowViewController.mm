/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowViewController.h"
#include "ContentView.h"

#include "Conversions.h"
#include <AK/Format.h>

@interface WindowViewController ()
@end

@implementation WindowViewController

- (instancetype)initWithFrame:(NSRect)frameRect
{
    if (self = [super init]) {
        self.view = [[ContentView alloc] initWithFrame:frameRect];
        dbgln("input rect: {} view frame: {}", Loom::ns_rect_to_gfx_rect(frameRect), Loom::ns_rect_to_gfx_rect([self.view frame]));
    }
    return self;
}

@end
