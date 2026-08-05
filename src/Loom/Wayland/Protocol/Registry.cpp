/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Registry.h>

namespace Loom::Wayland::Protocol {

#define WAYLAND_REGISTRY_DEBUG 0

const wl_registry_listener Registry::s_listener {
    &Registry::global_callback,
    &Registry::global_removed_callback,
};

static void wm_base_ping(void*, xdg_wm_base *xdg_wm_base, uint32_t serial) { xdg_wm_base_pong(xdg_wm_base, serial); }

static const constinit xdg_wm_base_listener s_wm_base_listener = { wm_base_ping };

Registry::Registry(wl_display* display, wl_registry* registry)
    : m_display(display)
    , m_registry(registry)
{
}

Registry::~Registry()
{
    if (m_fixes)
        wl_fixes_destroy_registry(m_fixes, m_registry);

    wl_registry_destroy(m_registry);

    if (m_fixes)
        wl_fixes_destroy(m_fixes);

    wl_shm_destroy(m_shm);
    xdg_wm_base_destroy(m_xdg_wm_base);
}

ErrorOr<NonnullOwnPtr<Registry>> Registry::try_create(wl_display* display)
{
    auto* wayland_registry = wl_display_get_registry(display);
    VERIFY(wayland_registry);
    auto registry = TRY(adopt_nonnull_own_or_enomem(new (nothrow) Registry(display, wayland_registry)));

    wl_registry_add_listener(registry->m_registry, &s_listener, registry.ptr());

    wl_display_roundtrip(registry->m_display);

    VERIFY(registry->m_compositor);
    VERIFY(registry->m_shm);
    VERIFY(registry->m_xdg_wm_base);

    return registry;
}

template<typename T>
T* Registry::bind(u32 name, wl_interface const* interface, u32 version)
{
    return static_cast<T*>(wl_registry_bind(m_registry, name, interface, version));
}

void Registry::global_callback(void* data, wl_registry* registry, u32 name, const char* interface, u32 version)
{
    auto* that = static_cast<Registry*>(data);
    VERIFY(that->m_registry == registry);

    static StringView compositor_name = { wl_compositor_interface.name, strlen(wl_compositor_interface.name) };
    static StringView xdg_wm_base_name = { xdg_wm_base_interface.name, strlen(xdg_wm_base_interface.name) };
    static StringView fixes_name = { wl_fixes_interface.name, strlen(wl_fixes_interface.name) };
    static StringView shm_name = { wl_shm_interface.name, strlen(wl_shm_interface.name) };

    if (interface == compositor_name) {
        that->m_compositor = make<Compositor>(that->bind<wl_compositor>(name, &wl_compositor_interface, version));
    } else if (interface == xdg_wm_base_name) {
        that->m_xdg_wm_base = that->bind<xdg_wm_base>(name, &xdg_wm_base_interface, version);
        xdg_wm_base_add_listener(that->m_xdg_wm_base, &s_wm_base_listener, nullptr);
    } else if (interface == fixes_name) {
        that->m_fixes = that->bind<wl_fixes>(name, &wl_fixes_interface, min(version, 2));
    } else if (interface == shm_name) {
        that->m_shm = that->bind<wl_shm>(name, &wl_shm_interface, version);
    } else {
        dbgln_if(WAYLAND_REGISTRY_DEBUG, "Registry: Unknown interface: {}, version {}, name {}", interface, version, name);
    }
}

void Registry::global_removed_callback(void* data, wl_registry* registry, u32 name)
{
    auto* that = static_cast<Registry*>(data);
    VERIFY(that->m_registry == registry);

    if (that->m_fixes && wl_fixes_get_version(that->m_fixes) >= WL_FIXES_ACK_GLOBAL_REMOVE_SINCE_VERSION)
        wl_fixes_ack_global_remove(that->m_fixes, registry, name);
}

}
