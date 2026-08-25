/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Error.h>
#include <AK/Noncopyable.h>
#include <AK/StringView.h>
#include <LibDBus/Forward.h>

#include <dbus/dbus.h>

namespace DBus {

class Error {
    AK_MAKE_NONCOPYABLE(Error);
public:
    Error();
    ~Error();
    Error(Error&&) noexcept;
    Error& operator=(Error&&) noexcept;

    DBusError* ptr() { return &m_error; }
    [[nodiscard]] bool is_error() const;

    StringView name() const;
    StringView message() const;

private:
    DBusError m_error;
};

template<typename T>
using ErrorOr = AK::ErrorOr<T, Error>;

}
