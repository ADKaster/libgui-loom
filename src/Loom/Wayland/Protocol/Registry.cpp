/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Fixes.h>
#include <Loom/Wayland/Protocol/Registry.h>
#include <Loom/Wayland/Protocol/Shm.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>

namespace Loom::Wayland::Protocol {

#define WAYLAND_REGISTRY_DEBUG 0

const wl_registry_listener Registry::s_listener {
    &Registry::global_callback,
    &Registry::global_removed_callback,
};

Registry::Registry(wl_display* display, wl_registry* registry)
    : m_display(display)
    , m_registry(registry)
{
}

Registry::~Registry()
{
    if (m_fixes)
        wl_fixes_destroy_registry(m_fixes->ptr(), m_registry);

    wl_registry_destroy(m_registry);
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
OwnPtr<T> Registry::bind(u32 name, u32 version)
{
    return make<T>(static_cast<T::InterfaceType*>(wl_registry_bind(m_registry, name, T::interface, version)));
}

void Registry::global_callback(void* data, wl_registry* registry, u32 name, const char* interface, u32 version)
{
    auto* that = static_cast<Registry*>(data);
    VERIFY(that->m_registry == registry);

    if (interface == Compositor::interface_name) {
        that->m_compositor =that->bind<Compositor>(name, version);
    } else if (interface == XdgWmBase::interface_name) {
        that->m_xdg_wm_base = that->bind<XdgWmBase>(name, version);
        that->m_xdg_wm_base->set_default_listener();
    } else if (interface == Fixes::interface_name) {
        that->m_fixes = that->bind<Fixes>(name, min(version, 2));
    } else if (interface == Shm::interface_name) {
        that->m_shm = that->bind<Shm>(name, version);
    } else {
        dbgln_if(WAYLAND_REGISTRY_DEBUG, "Registry: Unknown interface: {}, version {}, name {}", interface, version, name);
    }
}

void Registry::global_removed_callback(void* data, wl_registry* registry, u32 name)
{
    auto* that = static_cast<Registry*>(data);
    VERIFY(that->m_registry == registry);

    if (that->m_fixes && wl_fixes_get_version(that->m_fixes->ptr()) >= WL_FIXES_ACK_GLOBAL_REMOVE_SINCE_VERSION)
        wl_fixes_ack_global_remove(that->m_fixes->ptr(), registry, name);
}

}
