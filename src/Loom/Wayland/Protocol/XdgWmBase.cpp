/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Surface.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>

namespace Loom::Wayland::Protocol {
static void wm_base_ping(void*, xdg_wm_base *xdg_wm_base, uint32_t serial) { xdg_wm_base_pong(xdg_wm_base, serial); }

static const constinit xdg_wm_base_listener s_wm_base_listener = { wm_base_ping };

XdgWmBase::XdgWmBase(xdg_wm_base* wm_base)
    : m_wm_base(wm_base)
{
    VERIFY(m_wm_base != nullptr);
}

XdgWmBase::~XdgWmBase()
{
    xdg_wm_base_destroy(m_wm_base);
}

void XdgWmBase::set_default_listener()
{
    xdg_wm_base_add_listener(m_wm_base, &s_wm_base_listener, nullptr);
}

OwnPtr<XdgSurface> XdgWmBase::get_xdg_surface(NonnullOwnPtr<Surface> surface)
{
    return make<XdgSurface>(xdg_wm_base_get_xdg_surface(m_wm_base, surface->ptr()), move(surface));
}

}
