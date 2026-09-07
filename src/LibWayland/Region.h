/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <LibGfx/Rect.h>
#include <LibWayland/Forward.h>
#include <LibWayland/Interface.h>
#include <wayland-client.h>

namespace Wayland {

class Region {
    AK_MAKE_NONCOPYABLE(Region);
    AK_MAKE_NONMOVABLE(Region);
public:
    WAYLAND_INTERFACE(wl_region);

    explicit Region(wl_region* region);
    ~Region();

    RETURNS_NONNULL [[nodiscard]] wl_region* ptr() const { return m_region; }

    void add(Gfx::IntRect const&);
    void subtract(Gfx::IntRect const&);

private:
    wl_region* m_region;
};

}
