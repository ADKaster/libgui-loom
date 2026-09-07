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
#include <wp-viewporter-client.h>

namespace Wayland {

class Viewport {
    AK_MAKE_NONCOPYABLE(Viewport);
    AK_MAKE_NONMOVABLE(Viewport);
public:
    WAYLAND_INTERFACE(wp_viewport);

    explicit Viewport(wp_viewport* viewport);
    ~Viewport();

    RETURNS_NONNULL [[nodiscard]] wp_viewport* ptr() const { return m_viewport; }

    void set_source(Gfx::IntRect);
    void set_destination(Gfx::IntSize);

private:
    wp_viewport* m_viewport;

    // FIXME: Weak/ref ptr to Surface?
};

}
