/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <Loom/Wayland/Protocol/Interface.h>
#include <wayland-client.h>

namespace Loom::Wayland::Protocol {

class Surface {
    AK_MAKE_NONCOPYABLE(Surface);
    AK_MAKE_NONMOVABLE(Surface);
public:
    WAYLAND_INTERFACE(wl_surface);

    explicit Surface(wl_surface* surface);
    ~Surface();

    RETURNS_NONNULL [[nodiscard]] wl_surface* ptr() const { return m_surface; }

private:
    wl_surface* m_surface;
};

}
