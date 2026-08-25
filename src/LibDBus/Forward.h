/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Forward.h>

namespace DBus {
class Connection;
class Error;
class ObjectRegistration;

template<typename T>
using ErrorOr = AK::ErrorOr<T, Error>;
}
