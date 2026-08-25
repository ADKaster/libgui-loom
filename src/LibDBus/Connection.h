/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <LibCore/Notifier.h>
#include <LibDBus/Forward.h>

#include <dbus/dbus.h>

namespace DBus {

class Connection {
    AK_MAKE_NONCOPYABLE(Connection);
    AK_MAKE_NONMOVABLE(Connection);
public:

    static Connection& the();
    void install_event_loop_hooks();

    [[nodiscard]] DBusConnection* ptr() { return m_connection; }

private:
    static Connection create();

    explicit Connection(DBusConnection*);
    ~Connection();

    DBusConnection* m_connection { nullptr };
};

}
