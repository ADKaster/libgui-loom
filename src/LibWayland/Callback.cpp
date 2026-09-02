/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <LibWayland/Callback.h>

namespace Wayland {

void Callback::callback_done(void* data, wl_callback* callback, u32)
{
    auto* self = static_cast<Callback*>(data);
    VERIFY(self->m_callback == callback);
    self->m_promise->resolve();
}

wl_callback_listener const Callback::s_callback_listener = {
    .done = callback_done,
};

Callback::Callback(wl_callback* callback)
    : m_callback(callback)
    , m_promise(Core::Promise<void>::construct())
{
    VERIFY(m_callback != nullptr);

    wl_callback_add_listener(m_callback, &s_callback_listener, this);
}

Callback::~Callback()
{
    wl_callback_destroy(m_callback);
}

}
