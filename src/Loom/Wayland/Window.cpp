/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#include <Loom/Wayland/Window.h>
#include <Loom/Wayland/Display.h>
#include <Loom/Wayland/Protocol/Buffer.h>
#include <Loom/Wayland/Protocol/Registry.h>
#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Shm.h>
#include <Loom/Wayland/Protocol/ShmPool.h>
#include <Loom/Wayland/Protocol/Surface.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>
#include <Loom/Wayland/Protocol/XdgToplevel.h>

namespace Loom::Wayland {

Window::Window(NonnullOwnPtr<Protocol::XdgToplevel> toplevel)
    : m_toplevel(move(toplevel))
{
}

Window::~Window() = default;

NonnullOwnPtr<Window> Window::create(Display& display)
{
    auto& registry = display.registry();

    auto surface = registry.compositor().create_surface();
    auto xdg_surface = registry.wm_base().get_xdg_surface(move(surface));
    auto xdg_toplevel = Protocol::XdgSurface::get_xdg_toplevel(move(xdg_surface));

    return adopt_own(*new Window(move(xdg_toplevel)));
}

}
