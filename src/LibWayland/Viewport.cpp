/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <LibWayland/Viewport.h>

namespace Wayland {

Viewport::Viewport(wp_viewport* viewport)
    : m_viewport(viewport)
{
    VERIFY(m_viewport);
}

Viewport::~Viewport()
{
    wp_viewport_destroy(m_viewport);
}

void Viewport::set_source(Gfx::IntRect source)
{
    auto x = wl_fixed_from_int(source.x());
    auto y = wl_fixed_from_int(source.y());
    auto width = wl_fixed_from_int(source.width());
    auto height = wl_fixed_from_int(source.height());

    wp_viewport_set_source(m_viewport, x, y, width, height);
}

void Viewport::set_destination(Gfx::IntSize destination)
{
    wp_viewport_set_destination(m_viewport, destination.width(), destination.height());
}

}
