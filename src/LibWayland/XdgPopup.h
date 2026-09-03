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
#include <LibWayland/Forward.h>
#include <LibWayland/Interface.h>
#include <xdg-shell-client.h>

namespace Wayland {

class XdgPopup {
    AK_MAKE_NONCOPYABLE(XdgPopup);
    AK_MAKE_NONMOVABLE(XdgPopup);
public:
    WAYLAND_INTERFACE(xdg_popup);

    explicit XdgPopup(xdg_popup*, NonnullOwnPtr<XdgSurface>, XdgSurface* parent_surface, NonnullOwnPtr<XdgPositioner>);
    ~XdgPopup();

    RETURNS_NONNULL [[nodiscard]] xdg_popup* ptr() const { return m_xdg_popup; }

    XdgSurface& surface() const { return *m_xdg_surface; }
    XdgSurface* parent_surface() const { return m_parent_surface; }
    XdgPositioner& positioner() const { return *m_positioner; }

    Function<void(Gfx::IntRect)> on_configure;
    Function<void()> on_done;
    Function<void(u32 token)> on_repositioned;

    // TODO: grab
    void reposition(NonnullOwnPtr<XdgPositioner>, u32 token);

private:
    NonnullOwnPtr<XdgSurface> m_xdg_surface;
    XdgSurface* m_parent_surface { nullptr };
    NonnullOwnPtr<XdgPositioner> m_positioner;
    xdg_popup* m_xdg_popup;
};

}
