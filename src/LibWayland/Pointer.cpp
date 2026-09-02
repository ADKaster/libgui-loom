/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <LibWayland/Pointer.h>

namespace Wayland {

Pointer::Pointer(wl_pointer* pointer)
    : m_pointer(pointer)
{
    VERIFY(m_pointer != nullptr);
}

Pointer::~Pointer()
{
    wl_pointer_destroy(m_pointer);
}

}
