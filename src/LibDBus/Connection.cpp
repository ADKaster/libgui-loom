/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <LibCore/EventLoop.h>
#include <LibCore/Notifier.h>
#include <LibCore/Timer.h>
#include <LibDBus/Connection.h>
#include <LibDBus/Error.h>

#include <dbus/dbus.h>

namespace DBus {

Connection& Connection::the()
{
    static Connection s_connection = create();
    return s_connection;
}

Connection Connection::create()
{
    Error err;

    auto* dbus_connection = dbus_bus_get_private(DBUS_BUS_SESSION, err.ptr());

    if (err.is_error()) {
        dbgln("Unable to get DBUS_BUS_SESSION: {}", err.message());
        VERIFY_NOT_REACHED();
    }

    return Connection { dbus_connection };
}

Connection::Connection(DBusConnection* connection)
    : m_connection(connection)
{
    VERIFY(m_connection);
}

Connection::~Connection()
{
    dbus_connection_close(m_connection);
    dbus_connection_unref(m_connection);
}

static void queue_connection_dispatch(DBusConnection* connection, DBusDispatchStatus status, void*)
{
    if (status == DBUS_DISPATCH_DATA_REMAINS) {
        Core::deferred_invoke([c = dbus_connection_ref(connection)] {
            while (dbus_connection_dispatch(c) == DBUS_DISPATCH_DATA_REMAINS)
                ;
            dbus_connection_unref(c);
        });
    }
}

struct WatchEntry {
    RefPtr<Core::Notifier> read_notifier;
    RefPtr<Core::Notifier> write_notifier;
    RefPtr<Core::Notifier> hangup_notifier;
    RefPtr<Core::Notifier> error_notifier;
};

static dbus_bool_t add_dbus_watch(DBusWatch* watch, void*)
{
    auto fd = dbus_watch_get_unix_fd(watch);
    unsigned flags = dbus_watch_get_flags(watch);
    bool enabled = dbus_watch_get_enabled(watch);

    auto* entry = new WatchEntry();

    if (flags & DBUS_WATCH_READABLE) {
        auto notifier = Core::Notifier::construct(fd, Core::NotificationType::Read);
        notifier->on_activation = [watch] {
            dbus_watch_handle(watch, DBUS_WATCH_READABLE);
        };
        notifier->set_enabled(enabled);
        entry->read_notifier = move(notifier);
    }

    if (flags & DBUS_WATCH_WRITABLE) {
        auto notifier = Core::Notifier::construct(fd, Core::NotificationType::Write);
        notifier->on_activation = [watch] {
            dbus_watch_handle(watch, DBUS_WATCH_WRITABLE);
        };
        notifier->set_enabled(enabled);
        entry->write_notifier = move(notifier);
    }

    // Always set error and hangup notifiers
    {
        auto notifier = Core::Notifier::construct(fd, Core::NotificationType::HangUp);
        notifier->on_activation = [watch] {
            dbus_watch_handle(watch, DBUS_WATCH_HANGUP);
        };
        notifier->set_enabled(enabled);
        entry->hangup_notifier = move(notifier);
    }

    {
        auto notifier = Core::Notifier::construct(fd, Core::NotificationType::Error);
        notifier->on_activation = [watch] {
            dbus_watch_handle(watch, DBUS_WATCH_ERROR);
        };
        notifier->set_enabled(enabled);
        entry->error_notifier = move(notifier);
    }

    dbus_watch_set_data(watch, entry, [](void* data) { delete static_cast<WatchEntry*>(data); });
    return TRUE;
}

static void remove_dbus_watch(DBusWatch* watch, void*)
{
    dbus_watch_set_data(watch, nullptr, nullptr);
}

static void toggle_dbus_watch(DBusWatch* watch, void*)
{
    auto* entry = static_cast<WatchEntry*>(dbus_watch_get_data(watch));

    bool enabled = dbus_watch_get_enabled(watch);
    if (entry->read_notifier)
        entry->read_notifier->set_enabled(enabled);
    if (entry->write_notifier)
        entry->write_notifier->set_enabled(enabled);
    if (entry->hangup_notifier)
        entry->hangup_notifier->set_enabled(enabled);
    if (entry->error_notifier)
        entry->error_notifier->set_enabled(enabled);
}

static dbus_bool_t add_dbus_timeout(DBusTimeout* timeout, void*)
{
    auto timer = Core::Timer::create_repeating(dbus_timeout_get_interval(timeout), [timeout] {
        dbus_timeout_handle(timeout);
    });

    if (dbus_timeout_get_enabled(timeout))
        timer->start();

    dbus_timeout_set_data(timeout, &timer.leak_ref(), [](void* data) {
        static_cast<Core::Timer*>(data)->unref();
    });
    return TRUE;
}

static void remove_dbus_timeout(DBusTimeout* timeout, void*)
{
    dbus_timeout_set_data(timeout, nullptr, nullptr);
}

static void toggle_dbus_timeout(DBusTimeout* timeout, void*)
{
    auto* timer = static_cast<Core::Timer*>(dbus_timeout_get_data(timeout));

    if (dbus_timeout_get_enabled(timeout))
        timer->restart();
    else
        timer->stop();
}

void Connection::install_event_loop_hooks()
{
    dbus_connection_set_watch_functions(m_connection, add_dbus_watch, remove_dbus_watch, toggle_dbus_watch, nullptr, nullptr);
    dbus_connection_set_timeout_functions(m_connection, add_dbus_timeout, remove_dbus_timeout, toggle_dbus_timeout, nullptr, nullptr);
    dbus_connection_set_dispatch_status_function(m_connection, queue_connection_dispatch, nullptr, nullptr);

    auto const current_status = dbus_connection_get_dispatch_status(m_connection);
    queue_connection_dispatch(m_connection, current_status, nullptr);
}

}
