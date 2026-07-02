/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "CocoaWrapper.h"

@interface WindowController : NSWindowController <NSWindowDelegate>

@property (nonatomic, assign) int windowID;

- (instancetype)initWithContentRect:(NSRect)contentRect windowID:(int)id;

@end
