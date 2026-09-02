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

private:
    NonnullOwnPtr<XdgSurface> m_xdg_surface;
    xdg_toplevel* m_xdg_toplevel;
};

}
