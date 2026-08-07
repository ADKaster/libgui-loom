/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Fixes.h>

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

}
