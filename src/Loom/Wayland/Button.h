/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGfx/Bitmap.h>

namespace Loom {
class Button {
public:
    struct Icon {
        RefPtr<Gfx::Bitmap> bitmap { nullptr };
        RefPtr<Gfx::Bitmap> hover_bitmap { nullptr };
    };
};
}
