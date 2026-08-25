/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibDBus/Connection.h>
#include <LibDBus/Error.h>
#include <LibDBus/ObjectRegistration.h>

namespace DBus {

ErrorOr<NonnullOwnPtr<ObjectRegistration>> ObjectRegistration::create(Connection& connection, ByteString path)
{
    Error err;

    auto registration = adopt_own(*new ObjectRegistration(move(path)));

    auto const res = dbus_connection_try_register_object_path(connection.ptr(),
                                                                registration->path().characters(),
                                                                &registration->vtable(),
                                                                registration.ptr(),
                                                                err.ptr());

    if (res) {
        registration->m_valid = true;
        return registration;
    }

    return err;
}

ObjectRegistration::ObjectRegistration(ByteString path)
    : m_path(move(path))
    , m_vtable {}
{
    m_vtable.unregister_function = &ObjectRegistration::path_unregister;
    m_vtable.message_function = &ObjectRegistration::path_message;
}

ObjectRegistration::~ObjectRegistration()
{
    if (m_valid)
        (void)dbus_connection_unregister_object_path(DBus::Connection::the().ptr(), m_path.characters());
}

void ObjectRegistration::path_unregister(DBusConnection*, void*)
{
    // No-op
}

DBusHandlerResult ObjectRegistration::path_message(DBusConnection*, DBusMessage* message, void* user_data)
{
    auto* registration = static_cast<ObjectRegistration*>(user_data);
    if (registration->on_message)
        return registration->on_message(message);

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

}
