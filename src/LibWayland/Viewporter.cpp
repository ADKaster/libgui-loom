/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <LibWayland/Surface.h>
#include <LibWayland/Viewport.h>
#include <LibWayland/Viewporter.h>

namespace Wayland {

Viewporter::Viewporter(wp_viewporter* viewporter)
    : m_viewporter(viewporter)
{
    VERIFY(m_viewporter);
}

Viewporter::~Viewporter()
{
    wp_viewporter_destroy(m_viewporter);
}

NonnullOwnPtr<Viewport> Viewporter::get_viewport(Surface* surface)
{
    return make<Viewport>(wp_viewporter_get_viewport(m_viewporter, surface->ptr()));
}

}
