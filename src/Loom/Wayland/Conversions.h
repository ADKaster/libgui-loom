/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/ByteString.h>
#include <LibGfx/Color.h>
#include <LibGfx/Forward.h>

namespace Loom {

ByteString to_resource_path(StringView);

inline Gfx::Color to_premultiplied_alpha(Gfx::Color color)
{
    return Gfx::Color(
        color.red() * color.alpha() / 255,
        color.green() * color.alpha() / 255,
        color.blue() * color.alpha() / 255,
        color.alpha()
    );
}

void premultiply_alpha_channel(Gfx::Bitmap&);

}
