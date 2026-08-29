/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <Loom/Wayland/Protocol/Buffer.h>
#include <Loom/Wayland/Protocol/ShmPool.h>

namespace Loom::Wayland::Protocol {

ShmPool::ShmPool(wl_shm_pool* shm, Core::AnonymousBuffer buffer)
    : m_shm_pool(shm)
    , m_buffer(move(buffer))
{
    VERIFY(m_shm_pool != nullptr);
}

ShmPool::~ShmPool()
{
    wl_shm_pool_destroy(m_shm_pool);
}

static wl_shm_format to_shm_format(Gfx::BitmapFormat format)
{
    switch (format) {
    case Gfx::BitmapFormat::BGRA8888:
        return WL_SHM_FORMAT_ARGB8888;
    case Gfx::BitmapFormat::BGRx8888:
        return WL_SHM_FORMAT_XRGB8888;
    case Gfx::BitmapFormat::RGBA8888:
        return WL_SHM_FORMAT_ABGR8888;
    default:
        VERIFY_NOT_REACHED();
    }
}

NonnullOwnPtr<Buffer> ShmPool::create_buffer(Gfx::IntSize size, i32 pitch, Gfx::BitmapFormat format)
{
    return make<Buffer>(wl_shm_pool_create_buffer(m_shm_pool, 0, size.width(), size.height(), pitch, to_shm_format(format)));
}

}
