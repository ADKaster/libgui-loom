/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/StringView.h>
#include <string.h>

#define WAYLAND_INTERFACE(interface_type) \
    using InterfaceType = interface_type; \
    static inline wl_interface const* interface = &interface_type##_interface; \
    static inline StringView const interface_name = { interface_type##_interface.name, strlen(interface_type##_interface.name) }; \
    inline u32 version() const { return interface_type##_get_version(ptr()); }
