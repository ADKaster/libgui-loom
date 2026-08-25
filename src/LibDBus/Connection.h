/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <LibDBus/Forward.h>

#include <dbus/dbus.h>

namespace DBus {

class Connection {
    AK_MAKE_NONCOPYABLE(Connection);
    AK_MAKE_NONMOVABLE(Connection);
public:

    static Connection& the();

    [[nodiscard]] DBusConnection* ptr() { return m_connection; }

private:
    static Connection create();

    explicit Connection(DBusConnection*);
    ~Connection();

    DBusConnection* m_connection { nullptr };
};

}
