/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Error.h"

namespace DBus {

Error::Error()
{
    dbus_error_init(&m_error);
}

Error::~Error()
{
    dbus_error_free(&m_error);
}

Error::Error(Error&& error) noexcept
{
    dbus_move_error(&error.m_error, &m_error);
}

Error& Error::operator=(Error&& error) noexcept
{
    if (this != &error)
        dbus_move_error(&error.m_error, &m_error);
    return *this;
}

bool Error::is_error() const
{
    return dbus_error_is_set(&m_error);
}

StringView Error::name() const
{
    return { m_error.name, strlen(m_error.name) };
}

StringView Error::message() const
{
    return { m_error.message, strlen(m_error.message) };
}

}
