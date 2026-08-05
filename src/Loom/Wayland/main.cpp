/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibCore/EventLoop.h>
#include <LibCore/Timer.h>
#include <Loom/IPCBridge.h>
#include <Loom/Wayland/Protocol/Registry.h>

#include <wayland-client.h>

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

    auto registry = MUST(Loom::Wayland::Protocol::Registry::try_create(display));

    auto timer = Core::Timer::create_single_shot(1000, [display] {
        Core::EventLoop::current().quit(0);
    });
    timer->start();

   return event_loop.exec();
}
