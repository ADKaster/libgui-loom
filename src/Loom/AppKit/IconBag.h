/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>

#include "CocoaWrapper.h"

namespace Loom {

class IconBag {
    AK_MAKE_NONCOPYABLE(IconBag);
    AK_MAKE_NONMOVABLE(IconBag);
public:
    static IconBag& the();

    NSImage* minimize { nil };
    NSImage* minimize_hover { nil };
    NSImage* maximize { nil };
    NSImage* maximize_hover { nil };
    NSImage* restore { nil };
    NSImage* restore_hover { nil };
    NSImage* close { nil };
    NSImage* close_hover { nil };

private:
    IconBag();
};

}
