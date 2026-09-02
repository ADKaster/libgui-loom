/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Surface.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>
#include <Loom/Wayland/Protocol/XdgToplevel.h>

#define XDG_SURFACE_DEBUG 1

namespace Loom::Wayland::Protocol {

static void xdg_surface_configure(void* data, xdg_surface* xdg_surface, uint32_t serial)
{
    auto const& surface = *static_cast<XdgSurface*>(data);
    VERIFY(surface.ptr() == xdg_surface);
    dbgln_if(XDG_SURFACE_DEBUG, "XdgSurface::xdg_surface_configure: serial={}", serial);
    if (surface.on_configure)
        surface.on_configure();
    xdg_surface_ack_configure(xdg_surface, serial);
}

static constexpr xdg_surface_listener s_surface_listener {
    .configure = xdg_surface_configure
};

XdgSurface::XdgSurface(xdg_surface* xdg_surface, NonnullOwnPtr<Surface> surface)
    : m_surface(move(surface))
    , m_xdg_surface(xdg_surface)
{
    VERIFY(m_xdg_surface != nullptr);
    xdg_surface_add_listener(m_xdg_surface, &s_surface_listener, this);
}

XdgSurface::~XdgSurface()
{
    xdg_surface_destroy(m_xdg_surface);
}

NonnullOwnPtr<XdgToplevel> XdgSurface::get_xdg_toplevel(NonnullOwnPtr<XdgSurface> xdg_surface)
{
    return make<XdgToplevel>(xdg_surface_get_toplevel(xdg_surface->ptr()), move(xdg_surface));
}

void XdgSurface::set_window_geometry(Gfx::IntRect const& rect)
{
    dbgln_if(XDG_SURFACE_DEBUG, "XdgSurface::set_window_geometry: rect={}", rect);
    xdg_surface_set_window_geometry(m_xdg_surface, rect.x(), rect.y(), rect.width(), rect.height());
}

}
