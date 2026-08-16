/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Fixes.h>
#include <Loom/Wayland/Protocol/Output.h>
#include <Loom/Wayland/Protocol/Registry.h>
#include <Loom/Wayland/Protocol/Shm.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>

namespace Loom::Wayland::Protocol {

#define WAYLAND_REGISTRY_DEBUG 0

const wl_registry_listener Registry::s_listener {
    &Registry::global_callback,
    &Registry::global_removed_callback,
};

Registry::Registry(wl_registry* registry)
    : m_registry(registry)
{
    VERIFY(m_registry);
    wl_registry_add_listener(m_registry, &s_listener, this);
}

Registry::~Registry()
{
    if (m_fixes)
        m_fixes->destroy_registry(*this);

    wl_registry_destroy(m_registry);
}

template<typename T, typename... Args>
OwnPtr<T> Registry::bind(u32 name, u32 version, Args&&... args)
{
    return make<T>(static_cast<T::InterfaceType*>(wl_registry_bind(m_registry, name, T::interface, version)), forward<Args>(args)...);
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
    } else if (interface == Output::interface_name) {
        that->m_outputs.append(that->bind<Output>(name, version, name).release_nonnull());
    } else {
        dbgln_if(WAYLAND_REGISTRY_DEBUG, "Registry: Unknown interface: {}, version {}, name {}", interface, version, name);
    }
}

void Registry::global_removed_callback(void* data, wl_registry* registry, u32 name)
{
    auto* that = static_cast<Registry*>(data);
    VERIFY(that->m_registry == registry);

    // FIXME: Notify someone that output is gone. Or just wait for crashes and then fix
    if (that->m_outputs.remove_first_matching([&](auto const& o) { return o->global_name() == name; })) {
        dbgln_if(WAYLAND_REGISTRY_DEBUG, "Registry: Output removed: {}", name);
    }

    if (that->m_fixes && that->m_fixes->version() >= WL_FIXES_ACK_GLOBAL_REMOVE_SINCE_VERSION)
        that->m_fixes->ack_global_remove(*that, name);
}

}
