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

void Keyboard::keyboard_keymap(void* data, wl_keyboard* keyboard, u32 format, i32 fd, u32 size)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);

    if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
        warnln("Keyboard::keyboard_keymap: Unsupported keymap format: {}", format);
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
}

void Keyboard::keyboard_leave(void* data, wl_keyboard* keyboard, u32 serial, wl_surface* surface)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);
    dbgln("Keyboard::keyboard_leave: serial={}, surface={}", serial, surface);
}

void Keyboard::keyboard_key(void* data, wl_keyboard* keyboard, u32 serial, u32 time, u32 key, u32 state)
{
    auto& self = *static_cast<Keyboard*>(data);
    VERIFY(self.ptr() == keyboard);
    dbgln("Keyboard::keyboard_key: serial={}, time={}, key={}, state={}", serial, time, key, state);

    auto key_state = static_cast<KeyState>(state);
    auto keycode = key + 8; // XKB keycodes are offset by 8 from raw edev keycodes from Compositor

    xkb_keysym_t const* syms = nullptr;;
    auto nsyms = xkb_state_key_get_syms(self.m_state, keycode, &syms);

    (void)key_state;
    (void)nsyms;
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

}
