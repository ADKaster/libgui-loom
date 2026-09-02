/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/ByteString.h>
#include <AK/Vector.h>
#include <LibWayland/XdgToplevel.h>
#include <LibWayland/XdgSurface.h>

#define XDG_TOPLEVEL_DEBUG 1

namespace Wayland {

static Span<u32> to_span(wl_array* array)
{
    return { static_cast<u32*>(array->data), array->size / sizeof(u32) };
}

static void xdg_toplevel_configure(void* data, xdg_toplevel* xdg_toplevel, i32 width, i32 height, wl_array* states)
{
    auto const& toplevel = *static_cast<XdgToplevel*>(data);
    VERIFY(toplevel.ptr() == xdg_toplevel);
    dbgln_if(XDG_TOPLEVEL_DEBUG, "XdgToplevel::xdg_toplevel_configure: width={}, height={}, states={}", width, height, to_span(states));
    if (toplevel.on_configure)
        toplevel.on_configure(width, height, to_span(states));
}

static void xdg_toplevel_close(void* data, xdg_toplevel* xdg_toplevel)
{
    auto const& toplevel = *static_cast<XdgToplevel*>(data);
    VERIFY(toplevel.ptr() == xdg_toplevel);
    dbgln_if(XDG_TOPLEVEL_DEBUG, "XdgToplevel::xdg_toplevel_close");
    if (toplevel.on_close)
        toplevel.on_close();
}

static void xdg_toplevel_configure_bounds(void* data, xdg_toplevel* xdg_toplevel, i32 width, i32 height)
{
    auto const& toplevel = *static_cast<XdgToplevel*>(data);
    VERIFY(toplevel.ptr() == xdg_toplevel);
    dbgln_if(XDG_TOPLEVEL_DEBUG, "XdgToplevel::xdg_toplevel_configure_bounds: width={}, height={}", width, height);
    if (toplevel.on_configure_bounds)
        toplevel.on_configure_bounds(width, height);
}

static void xdg_toplevel_wm_capabilities(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities)
{
    auto const& toplevel = *static_cast<XdgToplevel*>(data);
    VERIFY(toplevel.ptr() == xdg_toplevel);
    dbgln_if(XDG_TOPLEVEL_DEBUG, "XdgToplevel::xdg_toplevel_wm_capabilities: capabilities={}", to_span(capabilities));
    if (toplevel.on_wm_capabilities)
        toplevel.on_wm_capabilities(to_span(capabilities));
}

static constexpr xdg_toplevel_listener s_toplevel_listener {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close,
    .configure_bounds = xdg_toplevel_configure_bounds,
    .wm_capabilities = xdg_toplevel_wm_capabilities,
};

XdgToplevel::XdgToplevel(xdg_toplevel* xdg_toplevel, NonnullOwnPtr<XdgSurface> xdg_surface)
    : m_xdg_surface(move(xdg_surface))
    , m_xdg_toplevel(xdg_toplevel)
{
    VERIFY(m_xdg_toplevel != nullptr);
    xdg_toplevel_add_listener(m_xdg_toplevel, &s_toplevel_listener, this);
}

XdgToplevel::~XdgToplevel()
{
    xdg_toplevel_destroy(m_xdg_toplevel);
}

void XdgToplevel::set_title(ByteString const& title)
{
    xdg_toplevel_set_title(m_xdg_toplevel, title.characters());
}

void XdgToplevel::set_app_id(ByteString const& app_id)
{
    xdg_toplevel_set_app_id(m_xdg_toplevel, app_id.characters());
}

}
