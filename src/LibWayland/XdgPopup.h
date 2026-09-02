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
#include <LibGfx/Rect.h>
#include <LibWayland/Interface.h>
#include <xdg-shell-client.h>

namespace Wayland {

class XdgSurface;

class XdgPopup {
    AK_MAKE_NONCOPYABLE(XdgPopup);
    AK_MAKE_NONMOVABLE(XdgPopup);
public:
    WAYLAND_INTERFACE(xdg_popup);

    explicit XdgPopup(xdg_popup*, NonnullOwnPtr<XdgSurface>);
    ~XdgPopup();

    RETURNS_NONNULL [[nodiscard]] xdg_popup* ptr() const { return m_xdg_popup; }

    XdgSurface& surface() const { return *m_xdg_surface; }

    Function<void(Gfx::IntRect)> on_configure;
    Function<void()> on_done;
    Function<void(u32 token)> on_repositioned;

    // TODO: grab needs Seat
    // TODO: reposition needs XdgPositioner
    // Reposition should probably return an object to respond to the reposition callback

private:
    NonnullOwnPtr<XdgSurface> m_xdg_surface;
    xdg_popup* m_xdg_popup;
};

}
