/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibGfx/Bitmap.h>
#include <Loom/Wayland/Conversions.h>

namespace Loom {

ByteString to_resource_path(StringView path)
{
    if (path.starts_with("/res/"sv))
        return path.replace("/res/"sv, "resource://"sv, ReplaceMode::FirstOnly);
    return path;
}

void premultiply_alpha_channel(Gfx::Bitmap& bitmap)
{
    for (auto y = 0; y < bitmap.height(); ++y) {
        for (auto x = 0; x < bitmap.width(); ++x)
            bitmap.set_pixel(x, y, to_premultiplied_alpha(bitmap.get_pixel(x, y)));
    }
}

}
