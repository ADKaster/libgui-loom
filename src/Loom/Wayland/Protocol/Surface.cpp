/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Surface.h>

namespace Loom::Wayland::Protocol {

Surface::Surface(wl_surface* surface)
    : m_surface(surface)
{
    VERIFY(m_surface != nullptr);
}

Surface::~Surface()
{
    wl_surface_destroy(m_surface);
}

}
