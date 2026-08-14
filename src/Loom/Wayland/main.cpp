/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibCore/EventLoop.h>
#include <LibCore/Timer.h>
#include <Loom/IPCBridge.h>
#include <Loom/Wayland/Protocol/Registry.h>
#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Surface.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>
#include <Loom/Wayland/Protocol/XdgToplevel.h>

#include <wayland-client.h>

using namespace Loom::Wayland;

int main(int argc, char const* argv[])
{
    Core::EventLoop event_loop;

    auto ipc_bridge = Loom::IPCBridge::create();

    auto* display = wl_display_connect(nullptr);
    if (display)
        dbgln("connected!");
    else
        dbgln("failed to connect!");

    ScopeGuard disconnect = [display] {
        wl_display_disconnect(display);
    };

    (void)argc;
    (void)argv;

    auto read_notifier = MUST(Core::Notifier::try_create(wl_display_get_fd(display), Core::Notifier::Type::Read));
    read_notifier->on_activation = [&]() {
        wl_display_dispatch(display);
    };
    auto write_notifier = MUST(Core::Notifier::try_create(wl_display_get_fd(display), Core::Notifier::Type::Write));
    write_notifier->on_activation = [&]() {
        wl_display_flush(display);
    };

    (void)Core::EventLoop::register_signal(SIGINT, [&](int) {
        dbgln("SIGINT received, exiting...");
        event_loop.quit(0);
    });

    auto registry = MUST(Protocol::Registry::try_create(display));

    auto surface = registry->compositor().create_surface();
    auto xdg_surface = registry->wm_base().get_xdg_surface(move(surface));
    auto xdg_toplevel = Protocol::XdgSurface::get_xdg_toplevel(move(xdg_surface));

    xdg_toplevel->surface().surface().commit();

   return event_loop.exec();
}
