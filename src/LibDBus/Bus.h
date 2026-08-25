/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>
#include <AK/ByteString.h>
#include <LibDBus/Forward.h>
#include <LibDBus/Error.h>

#include <dbus/dbus.h>

namespace DBus::Bus {

enum class RequestNameFlags : u32 {
    None = 0,
    AllowReplacement = DBUS_NAME_FLAG_ALLOW_REPLACEMENT,
    ReplaceExisting = DBUS_NAME_FLAG_REPLACE_EXISTING,
    DoNotQueue = DBUS_NAME_FLAG_DO_NOT_QUEUE,
};

enum class RequestNameResult : i32 {
    PrimaryOwner = DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER,
    AlreadyOwner = DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER,
    InQueue = DBUS_REQUEST_NAME_REPLY_IN_QUEUE,
    Exists = DBUS_REQUEST_NAME_REPLY_EXISTS,
};

ErrorOr<RequestNameResult> request_name(Connection&, ByteString const& name, RequestNameFlags);

}
