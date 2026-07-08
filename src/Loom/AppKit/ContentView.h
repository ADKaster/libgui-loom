/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "CocoaWrapper.h"

@class ContentView;

@protocol ContentViewDelegate <NSObject>

- (void)contentView:(ContentView*)contentView didReceiveEvent:(NSEvent*)event;

@end

@interface ContentView : NSView

@property (nonatomic, weak) id<ContentViewDelegate> eventDelegate;

@end
