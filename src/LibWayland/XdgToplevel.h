/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Function.h>
#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/Platform.h>
#include <LibWayland/Forward.h>
#include <LibWayland/Interface.h>
#include <xdg-shell-client.h>

namespace Wayland {

class XdgToplevel {
    AK_MAKE_NONCOPYABLE(XdgToplevel);
    AK_MAKE_NONMOVABLE(XdgToplevel);
public:
    WAYLAND_INTERFACE(xdg_toplevel);

    enum class ResizeEdge : u32 {
        None = XDG_TOPLEVEL_RESIZE_EDGE_NONE,
        Top = XDG_TOPLEVEL_RESIZE_EDGE_TOP,
        Bottom = XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM,
        Left = XDG_TOPLEVEL_RESIZE_EDGE_LEFT,
        TopLeft = XDG_TOPLEVEL_RESIZE_EDGE_TOP_LEFT,
        BottomLeft = XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_LEFT,
        Right = XDG_TOPLEVEL_RESIZE_EDGE_RIGHT,
        TopRight = XDG_TOPLEVEL_RESIZE_EDGE_TOP_RIGHT,
        BottomRight = XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT,
    };

    explicit XdgToplevel(xdg_toplevel*, NonnullOwnPtr<XdgSurface>);
    ~XdgToplevel();

    RETURNS_NONNULL [[nodiscard]] xdg_toplevel* ptr() const { return m_xdg_toplevel; }

    XdgSurface& surface() const { return *m_xdg_surface; }

    Function<void(i32, i32, Span<u32>)> on_configure;
    Function<void()> on_close;
    Function<void(i32, i32)> on_configure_bounds;
    Function<void(Span<u32>)> on_wm_capabilities;

    void set_title(ByteString const&);
    void set_app_id(ByteString const&);
    void set_minimized();
    void move(Seat&, u32 serial);
    void resize(Seat&, u32 serial, ResizeEdge);

private:
    NonnullOwnPtr<XdgSurface> m_xdg_surface;
    xdg_toplevel* m_xdg_toplevel;
};

}
