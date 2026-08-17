/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Platform.h>
#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <LibCore/AnonymousBuffer.h>
#include <LibGfx/Bitmap.h>
#include <Loom/Wayland/Protocol/Interface.h>
#include <wayland-client.h>

namespace Loom::Wayland::Protocol {

class Buffer;

class ShmPool {
    AK_MAKE_NONCOPYABLE(ShmPool);
    AK_MAKE_NONMOVABLE(ShmPool);
public:
    WAYLAND_INTERFACE(wl_shm_pool);

    ShmPool(wl_shm_pool* shm, Core::AnonymousBuffer buffer);
    ~ShmPool();

    RETURNS_NONNULL [[nodiscard]] wl_shm_pool* ptr() const { return m_shm_pool; }

    [[nodiscard]] size_t buffer_size() const { return m_buffer.size(); }

    NonnullOwnPtr<Buffer> create_buffer(Gfx::IntSize, Gfx::BitmapFormat);

private:
    wl_shm_pool* m_shm_pool;

    Core::AnonymousBuffer m_buffer;
};

}
