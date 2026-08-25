/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibCore/EventLoop.h>
#include <LibCore/Timer.h>
#include <LibDBus/Bus.h>
#include <LibDBus/Connection.h>
#include <Loom/IPCBridge.h>
#include <Loom/Wayland/Display.h>
#include <Loom/Wayland/Protocol/Buffer.h>
#include <Loom/Wayland/Protocol/Callback.h>
#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Registry.h>
#include <Loom/Wayland/Protocol/Shm.h>
#include <Loom/Wayland/Protocol/ShmPool.h>
#include <Loom/Wayland/Protocol/Surface.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>
#include <Loom/Wayland/Protocol/XdgToplevel.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>

#include <wayland-client.h>

using namespace Loom::Wayland;

int main(int argc, char const* argv[])
{
    Core::EventLoop event_loop;

    auto dbus_result = MUST(DBus::Bus::request_name(DBus::Connection::the(), "org.serenityos.Loom", DBus::Bus::RequestNameFlags::DoNotQueue));
    if (dbus_result != DBus::Bus::RequestNameResult::PrimaryOwner) {
        warnln("Failed to acquire D-Bus name: {}, exiting...", to_underlying(dbus_result));
        return 1;
    }

    auto ipc_bridge = Loom::IPCBridge::create();

    auto display = Display::create(StringView {});

    ipc_bridge->on_new_window_server_client = [&display](auto& client) {
        client.set_wayland_display(*display);
    };

    (void)argc;
    (void)argv;

    (void)Core::EventLoop::register_signal(SIGINT, [&](int) {
        dbgln("SIGINT received, exiting...");
        event_loop.quit(0);
    });

    auto& registry = display->registry();

    auto sync_cb = display->sync();
    MUST(sync_cb->promise().await());

    auto surface = registry.compositor().create_surface();
    auto xdg_surface = registry.wm_base().get_xdg_surface(move(surface));
    auto xdg_toplevel = Protocol::XdgSurface::get_xdg_toplevel(move(xdg_surface));

    auto window_size = Gfx::IntSize { 200, 200 };
    auto buf = MUST(Core::AnonymousBuffer::create_with_size(window_size.area() * 4));
    auto shm_pool = registry.shm().create_pool(buf);
    auto buffer = shm_pool->create_buffer(window_size, Gfx::BitmapFormat::BGRA8888);

    auto bitmap = MUST(Gfx::Bitmap::create_with_anonymous_buffer(Gfx::BitmapFormat::BGRA8888, move(buf), window_size, 1));

    bitmap->fill(Color::Magenta);

    xdg_toplevel->surface().surface().commit();

    xdg_toplevel->surface().on_configure = [&]() {
        xdg_toplevel->surface().surface().attach(move(buffer), 0, 0);
        xdg_toplevel->surface().surface().commit();
        xdg_toplevel->surface().on_configure = nullptr;
    };

   return event_loop.exec();
}
