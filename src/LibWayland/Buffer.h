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

class Buffer {
    AK_MAKE_NONCOPYABLE(Buffer);
    AK_MAKE_NONMOVABLE(Buffer);
public:
    WAYLAND_INTERFACE(wl_buffer);

    explicit Buffer(wl_buffer* buffer);
    ~Buffer();

    RETURNS_NONNULL [[nodiscard]] wl_buffer* ptr() const { return m_buffer; }

private:
    wl_buffer* m_buffer;
};

}
