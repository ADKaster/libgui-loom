/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <LibCore/Promise.h>
#include <LibWayland/Interface.h>
#include <wayland-client.h>

namespace Wayland {

class Callback {
    AK_MAKE_NONCOPYABLE(Callback);
    AK_MAKE_NONMOVABLE(Callback);
public:
    WAYLAND_INTERFACE(wl_callback);

    explicit Callback(wl_callback* callback);
    ~Callback();

    RETURNS_NONNULL [[nodiscard]] wl_callback* ptr() const { return m_callback; }

    [[nodiscard]] bool done() const { return m_promise->is_resolved(); }
    [[nodiscard]] Core::Promise<void>& promise() const { return *m_promise; }

private:

    static wl_callback_listener const s_callback_listener;
    static void callback_done(void* data, wl_callback*, u32);

    wl_callback* m_callback;
    NonnullRefPtr<Core::Promise<void>> m_promise;
};

}
