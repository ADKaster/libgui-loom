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
#include <LibGfx/Rect.h>
#include <xdg-shell-client.h>

namespace Wayland {

class XdgSurface {
    AK_MAKE_NONCOPYABLE(XdgSurface);
    AK_MAKE_NONMOVABLE(XdgSurface);
public:
    WAYLAND_INTERFACE(xdg_surface);

    explicit XdgSurface(xdg_surface*, NonnullOwnPtr<Surface>);
    ~XdgSurface();

    RETURNS_NONNULL [[nodiscard]] xdg_surface* ptr() const { return m_xdg_surface; }

    Surface& surface() const { return *m_surface; }

    static NonnullOwnPtr<XdgToplevel> get_xdg_toplevel(NonnullOwnPtr<XdgSurface>);
    void set_window_geometry(Gfx::IntRect const& rect);

    Function<void()> on_configure;

private:
    NonnullOwnPtr<Surface> m_surface;
    xdg_surface* m_xdg_surface;
};

}
