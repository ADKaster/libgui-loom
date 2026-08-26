/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/OwnPtr.h>
#include <AK/NonnullRefPtr.h>
#include <AK/RefPtr.h>
#include <LibCore/Forward.h>
#include <LibGfx/Forward.h>
#include <LibMain/Main.h>

namespace DBus {
class ObjectRegistration;
}

namespace Loom {

namespace Wayland {
class Display;
}

class IPCBridge;

class Application {
public:
    Application();
    ~Application();

    static Application& the();

    [[nodiscard]] static bool request_single_instance();

    ErrorOr<void> initialize(Main::Arguments);
    int exec();

    [[nodiscard]] Wayland::Display& display() { return *m_display; }
    [[nodiscard]] NonnullRefPtr<Gfx::PaletteImpl> palette_impl();

private:
    void register_dbus_handlers();

    RefPtr<Gfx::PaletteImpl> m_palette_impl;
    OwnPtr<Wayland::Display> m_display;
    OwnPtr<IPCBridge> m_ipc_bridge;
    OwnPtr<Core::EventLoop> m_event_loop;
    OwnPtr<DBus::ObjectRegistration> m_dbus_interface_registration;
};

}
