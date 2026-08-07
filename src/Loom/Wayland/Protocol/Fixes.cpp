/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Fixes.h>
#include <Loom/Wayland/Protocol/Registry.h>

namespace Loom::Wayland::Protocol {
Fixes::Fixes(wl_fixes* fixes)
    : m_fixes(fixes)
{
    VERIFY(m_fixes != nullptr);
}

Fixes::~Fixes()
{
    wl_fixes_destroy(m_fixes);
}

void Fixes::destroy_registry(Registry& registry)
{
    wl_fixes_destroy_registry(m_fixes, registry.ptr());
}

void Fixes::ack_global_remove(Registry& registry, u32 name)
{
    wl_fixes_ack_global_remove(m_fixes, registry.ptr(), name);
}

}
