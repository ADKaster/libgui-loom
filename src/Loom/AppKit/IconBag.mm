/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "IconBag.h"
#include "Conversions.h"

#include <AK/ByteString.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/Palette.h>
#include <LibGfx/SystemTheme.h>

namespace Loom {

static NSImage* load_icon(ByteString const& path)
{
    auto* image = gfx_bitmap_to_ns_image(MUST(Gfx::Bitmap::load_from_uri(path)));
    [image setTemplate:NO];
    return image;
}

static ByteString to_resource_uri(ByteString const& path)
{
    if (path.starts_with("/res/"sv))
        return ByteString::formatted("resource://{}", path.substring_view(5));
    return path;
}

static NSImage* load_icon_or_fallback(ByteString const& path, ByteString const& fallback_path)
{
    if (auto icon = Gfx::Bitmap::load_from_uri(path); !icon.is_error())
        return load_icon(path);
    return load_icon(fallback_path);
}

IconBag& IconBag::the()
{
    static IconBag s_the;
    return s_the;
}

IconBag::IconBag()
{
    auto palette = Gfx::Palette(Gfx::PaletteImpl::create_with_anonymous_buffer(Gfx::current_system_theme_buffer()));
    auto icon_path = to_resource_uri(ByteString(palette.title_button_icons_path()));

    minimize = load_icon_or_fallback(ByteString::formatted("{}/window-minimize.png", icon_path), "resource://icons/16x16/downward-triangle.png");
    minimize_hover = load_icon_or_fallback(ByteString::formatted("{}/window-minimize-hover.png", icon_path), "resource://icons/16x16/downward-triangle.png");
    maximize = load_icon_or_fallback(ByteString::formatted("{}/window-maximize.png", icon_path), "resource://icons/16x16/upward-triangle.png");
    maximize_hover = load_icon_or_fallback(ByteString::formatted("{}/window-maximize-hover.png", icon_path), "resource://icons/16x16/upward-triangle.png");
    restore = load_icon_or_fallback(ByteString::formatted("{}/window-restore.png", icon_path), "resource://icons/16x16/window-restore.png");
    restore_hover = load_icon_or_fallback(ByteString::formatted("{}/window-restore-hover.png", icon_path), "resource://icons/16x16/window-restore.png");
    close = load_icon_or_fallback(ByteString::formatted("{}/window-close.png", icon_path), "resource://icons/16x16/window-close.png");
    close_hover = load_icon_or_fallback(ByteString::formatted("{}/window-close-hover.png", icon_path), "resource://icons/16x16/window-close.png");
}

}
