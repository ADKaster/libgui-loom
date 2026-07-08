/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "CocoaWrapper.h"

@class ContentView;

@interface WindowView : NSView

- (instancetype)initWithFrame:(NSRect)frameRect;
- (void)configureTitlebarButtons;
- (void)updateZoomButtonImage;
- (void)layoutTitlebarButtons;
- (void)setWindowIconImage:(NSImage*)iconImage;
- (NSRect)contentRect;
@property (nonatomic, strong) ContentView* contentView;

@end
