/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/WaylandRegistry.h>

namespace Loom {

#define WAYLAND_REGISTRY_DEBUG 0

const wl_registry_listener WaylandRegistry::s_listener {
    &WaylandRegistry::global_callback,
    &WaylandRegistry::global_removed_callback,
};

static void wm_base_ping(void*, xdg_wm_base *xdg_wm_base, uint32_t serial) { xdg_wm_base_pong(xdg_wm_base, serial); }

static const xdg_wm_base_listener wm_base_listener = { wm_base_ping };

WaylandRegistry::WaylandRegistry(wl_display* display, wl_registry* registry)
    : m_display(display)
    , m_registry(registry)
{
}

WaylandRegistry::~WaylandRegistry()
{
    wl_registry_destroy(m_registry);
}

ErrorOr<NonnullOwnPtr<WaylandRegistry>> WaylandRegistry::try_create(wl_display* display)
{
    auto* wayland_registry = wl_display_get_registry(display);
    VERIFY(wayland_registry);
    auto registry = TRY(adopt_nonnull_own_or_enomem(new (nothrow) WaylandRegistry(display, wayland_registry)));

    wl_registry_add_listener(registry->m_registry, &s_listener, registry.ptr());

    wl_display_roundtrip(registry->m_display);

    VERIFY(registry->m_compositor);
    VERIFY(registry->m_xdg_wm_base);

    return registry;
}

void WaylandRegistry::global_callback(void* data, wl_registry* registry, u32 name, const char* interface, u32 version)
{
    auto* that = static_cast<WaylandRegistry*>(data);

    static StringView compositor_name = { wl_compositor_interface.name, strlen(wl_compositor_interface.name) };
    static StringView xdg_wm_base_name = { xdg_wm_base_interface.name, strlen(xdg_wm_base_interface.name) };

    if (interface == compositor_name) {
        that->m_compositor = static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, version));
    } else if (interface == xdg_wm_base_name) {
        that->m_xdg_wm_base = static_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, version));
        xdg_wm_base_add_listener(that->m_xdg_wm_base, &wm_base_listener, nullptr);
    }
    else {
        dbgln_if(WAYLAND_REGISTRY_DEBUG, "WaylandRegistry: Unknown interface: {}, version {}, name {}", interface, version, name);
    }
}

void WaylandRegistry::global_removed_callback(void*, wl_registry*, u32)
{
}

}
