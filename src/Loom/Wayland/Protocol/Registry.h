/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>
#include <AK/Error.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/OwnPtr.h>
#include <wayland-client.h>
#include <xdg-shell-client.h>

namespace Loom::Wayland::Protocol {

class Compositor;
class Fixes;
class Shm;
class XdgWmBase;

class Registry {

public:
    ~Registry();

    static ErrorOr<NonnullOwnPtr<Registry>> try_create(wl_display*);

    [[nodiscard]] Compositor& compositor() const { return *m_compositor; }
    [[nodiscard]] Shm& shm() const { return *m_shm; }
    [[nodiscard]] Fixes& fixes() const { return *m_fixes; }
    [[nodiscard]] XdgWmBase& wm_base() const { return *m_xdg_wm_base; }

private:
    Registry(wl_display*, wl_registry*);

    static void global_callback(void* data, wl_registry* registry, u32 name, const char* interface, u32 version);
    static void global_removed_callback(void* data, wl_registry* registry, u32 name);
    static const wl_registry_listener s_listener;

    template<typename T>
    OwnPtr<T> bind(u32 name, u32 version);

    wl_display* m_display { nullptr };
    wl_registry* m_registry { nullptr };

    OwnPtr<Fixes> m_fixes;

    OwnPtr<Compositor> m_compositor;
    OwnPtr<Shm> m_shm;
    OwnPtr<XdgWmBase> m_xdg_wm_base;
};

}
