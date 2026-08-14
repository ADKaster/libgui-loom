/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Surface.h>

namespace Loom::Wayland::Protocol {

Compositor::Compositor(wl_compositor* compositor)
    : m_compositor(compositor)
{
    VERIFY(m_compositor != nullptr);
}

Compositor::~Compositor()
{
    wl_compositor_destroy(m_compositor);
}

NonnullOwnPtr<Surface> Compositor::create_surface()
{
    return make<Surface>(wl_compositor_create_surface(m_compositor));
}

}
