/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
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

}
