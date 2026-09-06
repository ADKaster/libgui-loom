/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/HashMap.h>
#include <AK/Noncopyable.h>
#include <LibWayland/KeyboardDelegate.h>
#include <LibWayland/PointerDelegate.h>

namespace Loom {

class Window;

class SeatDelegate : public Wayland::KeyboardDelegate, public Wayland::PointerDelegate {
    AK_MAKE_NONCOPYABLE(SeatDelegate);
    AK_MAKE_NONMOVABLE(SeatDelegate);
public:

    SeatDelegate();

    void register_surface_owner(Wayland::Surface&, Window&);
    void unregister_surface_owner(Wayland::Surface&);

private:

    Window* get_window(Wayland::Surface*);

    HashMap<Wayland::Surface*, Window*> m_surface_owners;
};

}
