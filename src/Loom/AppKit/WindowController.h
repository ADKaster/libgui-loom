/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "CocoaWrapper.h"
#include "ContentView.h"

@interface WindowController : NSWindowController <NSWindowDelegate, ContentViewDelegate>

@property (nonatomic, assign) int windowID;
@property (nonatomic, assign) int clientID;
@property (nonatomic, assign) NSSize serenityContentSize;
@property (nonatomic, strong) NSImage* windowIconImage;
@property (nonatomic, strong) NSImage* backingStoreImage;
@property (nonatomic, strong) NSImage* lastBackingStoreImage;
@property (nonatomic, assign) NSSize backingStoreVisibleSize;
@property (nonatomic, assign) int backingStoreSerial;
@property (nonatomic, assign) int lastBackingStoreSerial;
@property (nonatomic, assign) BOOL backingStoreHasAlpha;

- (instancetype)initWithContentRect:(NSRect)contentRect windowID:(int)id clientID:(int)clientID;

@end
