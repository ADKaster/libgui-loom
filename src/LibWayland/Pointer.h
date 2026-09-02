/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <LibWayland/Interface.h>
#include <wayland-client.h>

namespace Wayland {

class Pointer {
    AK_MAKE_NONCOPYABLE(Pointer);
    AK_MAKE_NONMOVABLE(Pointer);
public:
    WAYLAND_INTERFACE(wl_pointer);

    explicit Pointer(wl_pointer* pointer);
    ~Pointer();

    RETURNS_NONNULL [[nodiscard]] wl_pointer* ptr() const { return m_pointer; }

private:
    wl_pointer* m_pointer;
};

}
