/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibDBus/Bus.h>
#include <LibDBus/Connection.h>

namespace DBus::Bus {

ErrorOr<RequestNameResult> request_name(Connection& connection, ByteString const& name, RequestNameFlags flags)
{
    Error err;

    auto res = dbus_bus_request_name(connection.ptr(), name.characters(), to_underlying(flags), err.ptr());

    if (res < 0) {
        VERIFY(err.is_error());
        return err;
    }

    return static_cast<RequestNameResult>(res);
}

}
