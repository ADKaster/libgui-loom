/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/WeakPtr.h>
#include <AK/HashMap.h>
#include <AK/Noncopyable.h>
#include <LibWayland/KeyboardDelegate.h>
#include <LibWayland/PointerDelegate.h>

namespace Loom {

class Button;
class Window;

class SeatDelegate : public Wayland::KeyboardDelegate, public Wayland::PointerDelegate {
    AK_MAKE_NONCOPYABLE(SeatDelegate);
    AK_MAKE_NONMOVABLE(SeatDelegate);
public:

    SeatDelegate();

    void register_surface_owner(Wayland::Surface&, Window&);
    void unregister_surface_owner(Wayland::Surface&);

    void set_cursor_tracking_button(Button*);
    void set_hovered_button(Button*);
    WeakPtr<Button> cursor_tracking_button();
    WeakPtr<Button> hovered_button();

    // Note: This is only valid if there is a currently focused surface that
    //       has been receiving pointer events!
    Gfx::IntPoint cursor_position() const;

private:

    Window* get_window(Wayland::Surface*);

    HashMap<Wayland::Surface*, Window*> m_surface_owners;

    u32 m_key_modifiers { 0 }; // FIXME: Why does WindowServer send *keyboard* modifiers in MouseEvents :v
    u32 m_mouse_buttons { 0 };
    u32 m_last_pointer_serial { 0 };
    Gfx::IntPoint m_pointer_position { 0, 0 };

    WeakPtr<Button> m_cursor_tracking_button;
    WeakPtr<Button> m_hovered_button;
};

}
