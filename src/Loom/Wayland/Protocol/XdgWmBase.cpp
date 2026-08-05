/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>

namespace Loom::Wayland::Protocol {

XdgWmBase::XdgWmBase(xdg_wm_base* wm_base)
    : m_wm_base(wm_base)
{
    VERIFY(m_wm_base != nullptr);
}

XdgWmBase::~XdgWmBase()
{
    xdg_wm_base_destroy(m_wm_base);
}

}

