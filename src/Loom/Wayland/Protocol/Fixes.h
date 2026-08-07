/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <Loom/Wayland/Protocol/Interface.h>
#include <wayland-client.h>

namespace Loom::Wayland::Protocol {

class Registry;

class Fixes {
    AK_MAKE_NONCOPYABLE(Fixes);
    AK_MAKE_NONMOVABLE(Fixes);
public:
    WAYLAND_INTERFACE(wl_fixes);

    explicit Fixes(wl_fixes* fixes);
    ~Fixes();

    RETURNS_NONNULL [[nodiscard]] wl_fixes* ptr() const { return m_fixes; }

    void destroy_registry(Registry&);
    void ack_global_remove(Registry&, u32 name);

private:
    wl_fixes* m_fixes;
};

}
