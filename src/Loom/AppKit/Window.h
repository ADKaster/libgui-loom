/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "CocoaWrapper.h"

@interface Window : NSWindow

+ (instancetype)windowWithContentViewController:(NSViewController*)contentViewController;
- (void)serenityCloseWindow:(id)sender;
- (void)serenityToggleMiniaturize:(id)sender;
- (void)serenitySetMiniaturized:(BOOL)miniaturized;
- (BOOL)serenityIsMiniaturized;
- (void)serenityToggleZoom:(id)sender;
- (void)serenitySetZoomed:(BOOL)zoomed;
- (BOOL)serenityIsZoomed;

@end
