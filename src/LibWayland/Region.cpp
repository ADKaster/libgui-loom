/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <LibWayland/Region.h>

namespace Wayland {

Region::Region(wl_region* region)
    : m_region(region)
{
    VERIFY(m_region);
}

Region::~Region()
{
    wl_region_destroy(m_region);
}

void Region::add(Gfx::IntRect const& rect)
{
    wl_region_add(m_region, rect.x(), rect.y(), rect.width(), rect.height());
}

void Region::subtract(Gfx::IntRect const& rect)
{
    wl_region_subtract(m_region, rect.x(), rect.y(), rect.width(), rect.height());
}

}
