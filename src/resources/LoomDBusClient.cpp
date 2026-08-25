/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Format.h>
#include <LibDBus/Connection.h>
#include <LibDBus/Error.h>
#include <dbus/dbus.h>

[[gnu::used]] static bool s_dbus_client_initialized = []{
    auto& dbus_connection = DBus::Connection::the();

    auto* message = dbus_message_new_method_call(
        "org.serenityos.Loom",
        "/org/serenityos/Loom",
        "org.serenityos.Loom",
        "Ping");
    VERIFY(message);

    DBus::Error err;

    auto* reply = dbus_connection_send_with_reply_and_block(dbus_connection.ptr(), message, 1000, err.ptr());
    dbus_message_unref(message);

    if (!reply) {
        warnln("Failed to ping Loom DBus service: {}: {}", err.name(), err.message());
        VERIFY_NOT_REACHED();
        return false;
    }
    dbus_message_unref(reply);
    return true;
}();
