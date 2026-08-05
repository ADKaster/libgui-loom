/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>
#include <AK/Error.h>
#include <AK/NonnullOwnPtr.h>
#include <wayland-client.h>
#include <xdg-shell-client.h>

namespace Loom::Wayland::Protocol {

class Registry {

public:
    ~Registry();

    static ErrorOr<NonnullOwnPtr<Registry>> try_create(wl_display*);

private:
    Registry(wl_display*, wl_registry*);

    static void global_callback(void* data, wl_registry* registry, u32 name, const char* interface, u32 version);
    static void global_removed_callback(void* data, wl_registry* registry, u32 name);
    static const wl_registry_listener s_listener;

    template<typename T>
    T* bind(u32 name, wl_interface const*, u32 version);

    wl_display* m_display { nullptr };
    wl_registry* m_registry { nullptr };

    wl_fixes* m_fixes { nullptr };

    wl_compositor* m_compositor { nullptr };
    wl_shm* m_shm { nullptr };
    xdg_wm_base* m_xdg_wm_base { nullptr };
};

}
