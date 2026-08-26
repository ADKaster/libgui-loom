/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <LibCore/ArgsParser.h>
#include <LibDBus/Bus.h>
#include <LibDBus/Connection.h>
#include <LibDBus/ObjectRegistration.h>
#include <Loom/IPCBridge.h>
#include <Loom/Wayland/Application.h>
#include <Loom/Wayland/Display.h>
#include <Loom/Wayland/Protocol/Callback.h>

namespace Loom {

Application::Application() = default;
Application::~Application()
{
    DBus::Connection::the().uninstall_event_loop_hooks();
}

bool Application::request_single_instance()
{
    auto dbus_result = MUST(DBus::Bus::request_name(DBus::Connection::the(), "org.serenityos.Loom", DBus::Bus::RequestNameFlags::DoNotQueue));
    if (dbus_result != DBus::Bus::RequestNameResult::PrimaryOwner) {
        warnln("Failed to request Loom DBus name: {}", to_underlying(dbus_result));
        return false;
    }
    return true;
}

void Application::register_dbus_handlers()
{
    auto& dbus_connection = DBus::Connection::the();
    dbus_connection.install_event_loop_hooks();

    m_dbus_interface_registration = MUST(DBus::ObjectRegistration::create(dbus_connection, "/org/serenityos/Loom"));
    m_dbus_interface_registration->on_message = [&dbus_connection](DBusMessage* message) {
        if (!dbus_message_is_method_call(message, "org.serenityos.Loom", "Ping"))
            return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

        dbgln("DBUS: Got a ping! Ponging...");

        auto* reply = dbus_message_new_method_return(message);
        if (!reply)
            return DBUS_HANDLER_RESULT_NEED_MEMORY;

        (void)dbus_connection_send(dbus_connection.ptr(), reply, nullptr);
        dbus_message_unref(reply);

        return DBUS_HANDLER_RESULT_HANDLED;
    };
}

ErrorOr<void> Application::initialize(Main::Arguments arguments)
{
    StringView display_name;

    Core::ArgsParser parser;
    parser.set_general_help("Loom compositor bridge service");
    parser.add_option(display_name, "Wayland display name", "display", 'd', "name");

    if (!parser.parse(arguments, Core::ArgsParser::FailureBehavior::PrintUsage))
        return Error::from_string_literal("Failed to parse arguments");

    m_event_loop = make<Core::EventLoop>();

    (void)Core::EventLoop::register_signal(SIGINT, [this](int) {
        dbgln("SIGINT received, exiting...");
        m_event_loop->quit(0);
    });

    m_display = Wayland::Display::create(display_name);

    // Ensure globals are bound before we start processing events
    [[maybe_unused]] auto& registry = m_display->registry();
    auto sync_cb = m_display->sync();
    TRY(sync_cb->promise().await());

    m_ipc_bridge = IPCBridge::create();
    m_ipc_bridge->on_new_window_server_client = [this](auto& client) {
        client.set_wayland_display(*m_display);
    };

    register_dbus_handlers();

    return {};
}

int Application::exec()
{
    VERIFY(m_event_loop);

    return m_event_loop->exec();
}

}
