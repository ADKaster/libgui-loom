/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <LibCore/ArgsParser.h>
#include <LibCore/EventLoop.h>
#include <LibDBus/Bus.h>
#include <LibDBus/Connection.h>
#include <LibDBus/ObjectRegistration.h>
#include <LibGfx/Palette.h>
#include <LibGfx/Font/FontDatabase.h>
#include <LibGfx/SystemTheme.h>
#include <Loom/IPCBridge.h>
#include <Loom/Wayland/Application.h>
#include <Loom/Wayland/Display.h>
#include <Loom/Wayland/Protocol/Callback.h>
#include <Loom/Wayland/Protocol/Output.h>
#include <Loom/Wayland/Protocol/Registry.h>
#include <Services/WindowServer/ScreenLayout.h>

namespace Loom {

static Application* s_the = nullptr;

Application::Application()
{
    VERIFY(!s_the);
    s_the = this;
}
Application::~Application()
{
    s_the = nullptr;
    DBus::Connection::the().uninstall_event_loop_hooks();
}

Application& Application::the()
{
    VERIFY(s_the);
    return *s_the;
}

NonnullRefPtr<Gfx::PaletteImpl> Application::palette_impl()
{
    VERIFY(m_palette_impl);
    return *m_palette_impl;
}

WindowServer::ScreenLayout Application::screen_layout() const
{
    WindowServer::ScreenLayout screen_layout;

    auto& registry = m_display->registry();
    auto const outputs = registry.outputs();
    screen_layout.screens.ensure_capacity(outputs.size());

    for (auto const& output : outputs) {
        auto const& geometry = output->geometry();
        auto const& mode = output->current_mode();

        auto width = mode.width;
        auto height = mode.height;
        switch (geometry.transform) {
        case Wayland::Protocol::Output::Transform::Degrees90:
        case Wayland::Protocol::Output::Transform::Degrees270:
        case Wayland::Protocol::Output::Transform::FlippedDegrees90:
        case Wayland::Protocol::Output::Transform::FlippedDegrees270:
            AK::swap(width, height);
            break;
        default:
            break;
        }

        screen_layout.screens.append(WindowServer::ScreenLayout::Screen {
            .mode = WindowServer::ScreenLayout::Screen::Mode::Virtual,
            .device = {},
            .location = { geometry.x, geometry.y },
            .resolution = { width, height },
            .scale_factor = output->scale(),
        });
    }

    if (screen_layout.screens.is_empty()) {
        screen_layout.screens.append(WindowServer::ScreenLayout::Screen {
            .mode = WindowServer::ScreenLayout::Screen::Mode::Virtual,
            .device = {},
            .location = { 0, 0 },
            .resolution = { 1024, 768 },
            .scale_factor = 1,
        });
    }

    screen_layout.main_screen_index = 0;
    (void)screen_layout.normalize();
    VERIFY(screen_layout.is_valid());
    return screen_layout;
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

static NonnullRefPtr<Gfx::PaletteImpl> initialize_libgfx_globals(StringView theme_name)
{
    auto theme = MUST(Gfx::load_system_theme(ByteString::formatted("resource://themes/{}.ini", theme_name)));
    Gfx::set_system_theme(theme);
    auto palette = Gfx::PaletteImpl::create_with_anonymous_buffer(theme);

    auto default_font_query = "Katica 10 400 0"sv;
    auto fixed_width_font_query = "Csilla 10 400 0"sv;
    auto window_title_font_query = "Katica 10 700 0"sv;

    Gfx::FontDatabase::set_default_font_query(default_font_query);
    Gfx::FontDatabase::set_fixed_width_font_query(fixed_width_font_query);
    Gfx::FontDatabase::set_window_title_font_query(window_title_font_query);

    return palette;
}

ErrorOr<void> Application::initialize(Main::Arguments arguments)
{
    StringView display_name;
    StringView system_theme = "Default"sv;

    Core::ArgsParser parser;
    parser.set_general_help("Loom compositor bridge service");
    parser.add_option(display_name, "Wayland display name", "display", 'd', "name");
    parser.add_option(system_theme, "System theme to use", "theme", 't', "name");

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

    m_palette_impl = initialize_libgfx_globals(system_theme);

    TRY(sync_cb->promise().await());

    m_ipc_bridge = IPCBridge::create();

    register_dbus_handlers();

    return {};
}

int Application::exec()
{
    VERIFY(m_event_loop);

    return m_event_loop->exec();
}

}
