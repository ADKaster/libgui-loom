/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <AK/OwnPtr.h>
#include <LibWayland/Forward.h>
#include <LibWayland/Interface.h>
#include <xdg-shell-client.h>

namespace Wayland {

class XdgWmBase {
    AK_MAKE_NONCOPYABLE(XdgWmBase);
    AK_MAKE_NONMOVABLE(XdgWmBase);
public:
    WAYLAND_INTERFACE(xdg_wm_base);

    explicit XdgWmBase(xdg_wm_base* wm_base);
    ~XdgWmBase();

    RETURNS_NONNULL [[nodiscard]] xdg_wm_base* ptr() const { return m_wm_base; }

    void set_default_listener();

    NonnullOwnPtr<XdgSurface> get_xdg_surface(NonnullOwnPtr<Surface> surface);

private:
    xdg_wm_base* m_wm_base;
};

}
