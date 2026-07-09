/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/RefPtr.h>
#include "CocoaWrapper.h"
#include "ContentView.h"
#include <LibGfx/Forward.h>

namespace Loom {
class WindowServerConnectionProxy;
}

@interface WindowController : NSWindowController <NSWindowDelegate, ContentViewDelegate>

@property (nonatomic, assign) int windowID;
@property (nonatomic, assign) NSSize serenityContentSize;
@property (nonatomic, strong) NSImage* windowIconImage;
@property (nonatomic, assign) NSSize backingStoreVisibleSize;
@property (nonatomic, assign) BOOL backingStoreHasAlpha;

- (instancetype)initWithContentRect:(NSRect)contentRect windowID:(int)id connection:(NonnullRefPtr<Loom::WindowServerConnectionProxy>)connection;
- (void)setBackingStoreBitmap:(RefPtr<Gfx::Bitmap>)bitmap;
- (NSImage*)backingStoreImage;
- (void)serenityRequestCloseFromTitlebarButton;
- (void)serenityCloseFromServer;

@end
