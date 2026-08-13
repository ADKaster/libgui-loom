/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Surface.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>

namespace Loom::Wayland::Protocol {

XdgSurface::XdgSurface(xdg_surface* xdg_surface, NonnullOwnPtr<Surface> surface)
    : m_surface(move(surface))
    , m_xdg_surface(xdg_surface)
{
    VERIFY(m_xdg_surface != nullptr);
}

XdgSurface::~XdgSurface()
{
    xdg_surface_destroy(m_xdg_surface);
}

}
