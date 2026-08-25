/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/Vector.h>
#include <LibGfx/Rect.h>
#include <Loom/Wayland/Protocol/XdgPopup.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>

#define XDG_POPUP_DEBUG 1

namespace Loom::Wayland::Protocol {

static void xdg_popup_configure(void* data, xdg_popup* xdg_popup, i32 x, i32 y, i32 width, i32 height)
{
    auto const& popup = *static_cast<XdgPopup*>(data);
    VERIFY(popup.ptr() == xdg_popup);

    auto const rect = Gfx::IntRect(x, y, width, height);

    dbgln_if(XDG_POPUP_DEBUG, "XdgPopup::xdg_popup_configure: x={}, y={}, width={}, height={}", x, y, width, height);
    if (popup.on_configure)
        popup.on_configure(rect);
}

static void xdg_popup_done(void* data, xdg_popup* xdg_popup)
{
    auto const& popup = *static_cast<XdgPopup*>(data);
    VERIFY(popup.ptr() == xdg_popup);
    dbgln_if(XDG_POPUP_DEBUG, "XdgPopup::xdg_popup_done");
    if (popup.on_done)
        popup.on_done();
}

static void xdg_popup_repositioned(void* data, xdg_popup* xdg_popup, u32 token)
{
    auto const& popup = *static_cast<XdgPopup*>(data);
    VERIFY(popup.ptr() == xdg_popup);
    dbgln_if(XDG_POPUP_DEBUG, "XdgPopup::xdg_popup_repositioned: token={}", token);
    if (popup.on_repositioned)
        popup.on_repositioned(token);
}

static constexpr xdg_popup_listener s_popup_listener {
    .configure = xdg_popup_configure,
    .popup_done = xdg_popup_done,
    .repositioned = xdg_popup_repositioned
};

XdgPopup::XdgPopup(xdg_popup* xdg_popup, NonnullOwnPtr<XdgSurface> xdg_surface)
    : m_xdg_surface(move(xdg_surface))
    , m_xdg_popup(xdg_popup)
{
    VERIFY(m_xdg_popup != nullptr);
    xdg_popup_add_listener(m_xdg_popup, &s_popup_listener, this);
}

XdgPopup::~XdgPopup()
{
    xdg_popup_destroy(m_xdg_popup);
}

}
