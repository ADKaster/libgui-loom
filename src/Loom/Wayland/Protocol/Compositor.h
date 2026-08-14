/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Platform.h>
#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <Loom/Wayland/Protocol/Interface.h>
#include <wayland-client.h>

namespace Loom::Wayland::Protocol {

class Surface;

class Compositor {
    AK_MAKE_NONCOPYABLE(Compositor);
    AK_MAKE_NONMOVABLE(Compositor);
public:
    WAYLAND_INTERFACE(wl_compositor);

    explicit Compositor(wl_compositor* compositor);
    ~Compositor();

    RETURNS_NONNULL [[nodiscard]] wl_compositor* ptr() const { return m_compositor; }

    NonnullOwnPtr<Surface> create_surface();

private:
    wl_compositor* m_compositor;
};

}
