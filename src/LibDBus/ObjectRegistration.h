/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/ByteString.h>
#include <AK/Function.h>
#include <AK/NonnullOwnPtr.h>
#include <LibDBus/Forward.h>

#include <dbus/dbus.h>

namespace DBus {

class ObjectRegistration {
public:

    static ErrorOr<NonnullOwnPtr<ObjectRegistration>> create(Connection&, ByteString path);
    ~ObjectRegistration();

    Function<DBusHandlerResult(DBusMessage*)> on_message;

    DBusObjectPathVTable& vtable() { return m_vtable; }
    ByteString const& path() const { return m_path; }

private:
    explicit ObjectRegistration(ByteString path);


    static void path_unregister(DBusConnection* connection, void* user_data);
    static DBusHandlerResult path_message(DBusConnection* connection, DBusMessage* message, void* user_data);

    bool m_valid { false };
    ByteString m_path;
    DBusObjectPathVTable m_vtable;
};

}
