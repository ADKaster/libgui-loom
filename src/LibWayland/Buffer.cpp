/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <LibWayland/Buffer.h>

namespace Wayland {

void Buffer::buffer_release(void* data, wl_buffer* buffer)
{
    auto& self = *static_cast<Buffer*>(data);
    VERIFY(self.ptr() == buffer);
    if (self.on_release)
        self.on_release();
}

wl_buffer_listener const Buffer::s_buffer_listener = {
    .release = buffer_release,
};

Buffer::Buffer(wl_buffer* buffer)
    : m_buffer(buffer)
{
    VERIFY(m_buffer != nullptr);
    wl_buffer_add_listener(m_buffer, &s_buffer_listener, this);
}

Buffer::~Buffer()
{
    wl_buffer_destroy(m_buffer);
}

}
