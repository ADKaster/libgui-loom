/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/Platform.h>
#include <Loom/Wayland/Protocol/Interface.h>
#include <xdg-shell-client.h>

namespace Loom::Wayland::Protocol {

class Surface;

class XdgSurface {
    AK_MAKE_NONCOPYABLE(XdgSurface);
    AK_MAKE_NONMOVABLE(XdgSurface);
public:
    WAYLAND_INTERFACE(xdg_surface);

    explicit XdgSurface(xdg_surface*, NonnullOwnPtr<Surface>);
    ~XdgSurface();

    RETURNS_NONNULL [[nodiscard]] xdg_surface* ptr() const { return m_xdg_surface; }

private:
    NonnullOwnPtr<Surface> m_surface;
    xdg_surface* m_xdg_surface;
};

}
