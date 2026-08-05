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

namespace Loom {

class WaylandRegistry {

public:
    ~WaylandRegistry();

    static ErrorOr<NonnullOwnPtr<WaylandRegistry>> try_create(wl_display*);

private:
    WaylandRegistry(wl_display*, wl_registry*);

    static void global_callback(void* data, wl_registry* registry, u32 name, const char* interface, u32 version);
    static void global_removed_callback(void* data, wl_registry* registry, u32 name);
    static const wl_registry_listener s_listener;

    wl_display* m_display { nullptr };
    wl_registry* m_registry { nullptr };

    wl_fixes* m_fixes { nullptr };

    wl_compositor* m_compositor { nullptr };
    wl_shm* m_shm { nullptr };
    xdg_wm_base* m_xdg_wm_base { nullptr };
};

}
