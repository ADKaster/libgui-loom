/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/Format.h>
#include <LibCore/System.h>
#include <LibWayland/Keyboard.h>

#include <xkbcommon/xkbcommon.h>
#include <sys/mman.h>

namespace Wayland {

static KeyState to_keystate(wl_keyboard_key_state key_state)
{
    switch (key_state) {
    case WL_KEYBOARD_KEY_STATE_RELEASED:
        return KeyState::Released;
    case WL_KEYBOARD_KEY_STATE_PRESSED:
        return KeyState::Pressed;
    case WL_KEYBOARD_KEY_STATE_REPEATED:
        return KeyState::Repeated;
    default:
        VERIFY_NOT_REACHED();
    }
}

void Keyboard::keyboard_keymap(void* data, wl_keyboard* keyboard, u32 format, i32 fd, u32 size)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);

    if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
        warnln("Keyboard::keyboard_keymap: Unsupported keymap format: {}", format);
        ::close(fd);
        return;
    }

    if (self.m_keymap)
        xkb_keymap_unref(self.m_keymap);
    if (self.m_state)
        xkb_state_unref(self.m_state);

    auto* keymap_ptr = MUST(Core::System::mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0));

    self.m_keymap = xkb_keymap_new_from_string(self.m_context, static_cast<char const*>(keymap_ptr), XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
    self.m_state = xkb_state_new(self.m_keymap);

    MUST(Core::System::munmap(keymap_ptr, size));
    ::close(fd);
}

void Keyboard::keyboard_enter(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface, wl_array* keys)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);
    dbgln("Keyboard::keyboard_enter: serial={}, surface={}, keys={}", serial, surface, keys->size / sizeof(u32));

    self.m_focused_surface = static_cast<Surface*>(wl_surface_get_user_data(surface));

    if (!self.m_delegate)
        return;

    self.m_delegate->on_keyboard_enter(self, self.m_focused_surface, serial);
}

void Keyboard::keyboard_leave(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);
    dbgln("Keyboard::keyboard_leave: serial={}, surface={}", serial, surface);

    auto* left_surface = static_cast<Surface*>(wl_surface_get_user_data(surface));
    VERIFY(self.m_focused_surface == left_surface);
    self.m_focused_surface = nullptr;

    if (!self.m_delegate)
        return;

    self.m_delegate->on_keyboard_leave(self, left_surface, serial);
}

void Keyboard::keyboard_key(void* data, wl_keyboard* keyboard, u32 serial, u32 time, u32 key, u32 state)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);
    dbgln("Keyboard::keyboard_key: serial={}, time={}, key={}, state={}", serial, time, key, state);

    auto key_state = to_keystate(static_cast<wl_keyboard_key_state>(state));
    auto xkb_keycode = key + 8; // XKB keycodes are offset by 8 from raw edev keycodes from Compositor

    auto const keysym = xkb_state_key_get_one_sym(self.m_state, xkb_keycode);
    auto const code_point = xkb_state_key_get_utf32(self.m_state, xkb_keycode);

    if (!self.m_delegate)
        return;

    self.m_delegate->on_keyboard_key(self, self.m_focused_surface, serial, Duration::from_milliseconds(time), key, key_state, keysym, code_point, self.modifiers());
}

void Keyboard::keyboard_modifiers(void* data, wl_keyboard* keyboard, u32 serial, u32 mods_depressed, u32 mods_latched, u32 mods_locked, u32 group)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);
    dbgln("Keyboard::keyboard_modifiers: serial={}, mods_depressed={}, mods_latched={}, mods_locked={}, group={}", serial, mods_depressed, mods_latched, mods_locked, group);

    xkb_state_update_mask(self.m_state, mods_depressed, mods_latched, mods_locked, 0, 0, group);
}

void Keyboard::keyboard_repeat_info(void* data, wl_keyboard* keyboard, i32 rate, i32 delay)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);
    dbgln("Keyboard::keyboard_repeat_info: rate={}, delay={}", rate, delay);
}

const wl_keyboard_listener Keyboard::s_keyboard_listener
{
    .keymap = keyboard_keymap,
    .enter =  keyboard_enter,
    .leave = keyboard_leave,
    .key = keyboard_key,
    .modifiers = keyboard_modifiers,
    .repeat_info = keyboard_repeat_info,
};

Keyboard::Keyboard(wl_keyboard* keyboard)
    : m_keyboard(keyboard)
{
    VERIFY(m_keyboard != nullptr);

    m_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

    wl_keyboard_add_listener(m_keyboard, &s_keyboard_listener, this);
}

Keyboard::~Keyboard()
{
    wl_keyboard_destroy(m_keyboard);

    xkb_context_unref(m_context);
    if (m_keymap)
        xkb_keymap_unref(m_keymap);
    if (m_state)
        xkb_state_unref(m_state);
}

KeyboardModifier Keyboard::modifiers() const
{
    if (!m_state)
        return KeyboardModifier::None;

    auto modifiers = KeyboardModifier::None;

    if (xkb_state_mod_name_is_active(m_state, XKB_VMOD_NAME_ALT, XKB_STATE_MODS_EFFECTIVE))
        modifiers |= KeyboardModifier::Alt;
    if (xkb_state_mod_name_is_active(m_state, XKB_MOD_NAME_CTRL, XKB_STATE_MODS_EFFECTIVE))
        modifiers |= KeyboardModifier::Ctrl;
    if (xkb_state_mod_name_is_active(m_state, XKB_MOD_NAME_SHIFT, XKB_STATE_MODS_EFFECTIVE))
        modifiers |= KeyboardModifier::Shift;
    if (xkb_state_mod_name_is_active(m_state, XKB_VMOD_NAME_SUPER, XKB_STATE_MODS_EFFECTIVE))
        modifiers |= KeyboardModifier::Super;
    if (xkb_state_mod_name_is_active(m_state, XKB_VMOD_NAME_LEVEL3, XKB_STATE_MODS_EFFECTIVE))
        modifiers |= KeyboardModifier::AltGr;

    return modifiers;
}

}
