/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Shm.h>

namespace Loom::Wayland::Protocol {

Shm::Shm(wl_shm* shm)
    : m_shm(shm)
{
    VERIFY(m_shm != nullptr);
}

Shm::~Shm()
{
    wl_shm_destroy(m_shm);
}

}
