/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Platform.h>
#include <AK/Noncopyable.h>
#include <Loom/Wayland/Protocol/Interface.h>
#include <wayland-client.h>

namespace Loom::Wayland::Protocol {

class Shm {
    AK_MAKE_NONCOPYABLE(Shm);
    AK_MAKE_NONMOVABLE(Shm);
public:
    WAYLAND_INTERFACE(wl_shm);

    explicit Shm(wl_shm* shm);
    ~Shm();

    RETURNS_NONNULL [[nodiscard]] wl_shm* ptr() const { return m_shm; }

private:
    wl_shm* m_shm;
};

}
