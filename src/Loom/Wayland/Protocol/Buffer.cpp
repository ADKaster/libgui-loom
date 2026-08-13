/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Buffer.h>

namespace Loom::Wayland::Protocol {

Buffer::Buffer(wl_buffer* buffer)
    : m_buffer(buffer)
{
    VERIFY(m_buffer != nullptr);
}

Buffer::~Buffer()
{
    wl_buffer_destroy(m_buffer);
}

}
