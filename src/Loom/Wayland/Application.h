/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullRefPtr.h>
#include <AK/OwnPtr.h>
#include <AK/RefPtr.h>
#include <LibCore/Forward.h>
#include <LibGfx/Palette.h>
#include <LibMain/Main.h>

namespace DBus {
class ObjectRegistration;
}

namespace WindowServer {
class ScreenLayout;
class SystemEffects;
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
    [[nodiscard]] StringView app_id() const;

    [[nodiscard]] Gfx::Palette palette() const { return Palette(*m_palette_impl); }
    [[nodiscard]] WindowServer::ScreenLayout screen_layout() const;
    [[nodiscard]] WindowServer::SystemEffects& system_effects() const;

private:
    void register_dbus_handlers();

    RefPtr<Gfx::PaletteImpl> m_palette_impl;
    OwnPtr<Wayland::Display> m_display;
    OwnPtr<IPCBridge> m_ipc_bridge;
    OwnPtr<Core::EventLoop> m_event_loop;
    OwnPtr<DBus::ObjectRegistration> m_dbus_interface_registration;
};

}
