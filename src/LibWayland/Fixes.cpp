/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <LibWayland/Fixes.h>
#include <LibWayland/Registry.h>

namespace Wayland {
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

void Fixes::ack_global_remove([[maybe_unused]] Registry& registry, [[maybe_unused]] u32 name)
{
#if defined(WL_FIXES_ACK_GLOBAL_REMOVE)
    if (wl_fixes_get_version(m_fixes) >= WL_FIXES_ACK_GLOBAL_REMOVE_SINCE_VERSION)
        wl_fixes_ack_global_remove(m_fixes, registry.ptr(), name);
#endif
}

}
