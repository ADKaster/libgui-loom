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

extern "C" {
struct xkb_context;
struct xkb_keymap;
struct xkb_state;
}

namespace Loom::Wayland::Protocol {

class Keyboard {
    AK_MAKE_NONCOPYABLE(Keyboard);
    AK_MAKE_NONMOVABLE(Keyboard);
public:
    WAYLAND_INTERFACE(wl_keyboard);

    explicit Keyboard(wl_keyboard* keyboard);
    ~Keyboard();

    RETURNS_NONNULL [[nodiscard]] wl_keyboard* ptr() const { return m_keyboard; }

    enum class KeyState : u32 {
        Released = WL_KEYBOARD_KEY_STATE_RELEASED,
        Pressed = WL_KEYBOARD_KEY_STATE_PRESSED,
        Repeated = WL_KEYBOARD_KEY_STATE_REPEATED,
    };

private:
    wl_keyboard* m_keyboard;
    xkb_context* m_context { nullptr };
    xkb_keymap* m_keymap { nullptr };
    xkb_state* m_state { nullptr };

    static void keyboard_keymap(void* data, wl_keyboard* keyboard, u32 format, i32 fd, u32 size);
    static void keyboard_enter(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface, wl_array* keys);
    static void keyboard_leave(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface);
    static void keyboard_key(void* data, wl_keyboard* keyboard, u32 serial, u32 time, u32 key, u32 state);
    static void keyboard_modifiers(void* data, wl_keyboard* keyboard, u32 serial, u32 mods_depressed, u32 mods_latched, u32 mods_locked, u32 group);
    static void keyboard_repeat_info(void* data, wl_keyboard* keyboard, i32 rate, i32 delay);
    static const wl_keyboard_listener s_keyboard_listener;
};

}
