/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <LibGfx/Point.h>

namespace Loom {

struct HitTestResult {
    Gfx::IntPoint surface_position;
    Gfx::IntPoint content_relative_position;
    bool is_frame_hit { false };
};

class MouseEvent {
    AK_MAKE_DEFAULT_COPYABLE(MouseEvent);
    AK_MAKE_DEFAULT_MOVABLE(MouseEvent);
public:

    enum class Type : u8 {
        MouseDown = 0,
        MouseUp = 1,
        MouseMove = 2,
        MouseWheel = 3,
    };

    MouseEvent(Type type, Gfx::IntPoint surface_position, u32 button, u32 buttons, u32 keyboard_modifiers)
        : m_type(type)
        , m_surface_position(surface_position)
        , m_button(button)
        , m_buttons(buttons)
        , m_keyboard_modifiers(keyboard_modifiers)
    {
    }

    [[nodiscard]] Type type() const { return m_type; }
    [[nodiscard]] Gfx::IntPoint surface_position() const { return m_surface_position; }
    [[nodiscard]] u32 button() const { return m_button; }
    [[nodiscard]] u32 buttons() const { return m_buttons; }
    [[nodiscard]] u32 keyboard_modifiers() const { return m_keyboard_modifiers; }

private:
    Type m_type;
    Gfx::IntPoint m_surface_position;
    u32 m_button { 0 };
    u32 m_buttons { 0};
    u32 m_keyboard_modifiers { 0 };
};

class KeyboardEvent {
    AK_MAKE_DEFAULT_COPYABLE(KeyboardEvent);
    AK_MAKE_DEFAULT_MOVABLE(KeyboardEvent);
public:

    enum class Type : u8 {
        KeyDown,
        KeyUp,
    };

    KeyboardEvent(Type type, u32 code_point, u32 key_code, u32 keyboard_modifiers, u32 raw_key)
        : m_type(type)
        , m_code_point(code_point)
        , m_key_code(key_code)
        , m_keyboard_modifiers(keyboard_modifiers)
        , m_raw_key(raw_key)
    {
    }

    [[nodiscard]] Type type() const { return m_type; }
    [[nodiscard]] u32 code_point() const { return m_code_point; }
    [[nodiscard]] u32 key_code() const { return m_key_code; }
    [[nodiscard]] u32 keyboard_modifiers() const { return m_keyboard_modifiers; }
    [[nodiscard]] u32 raw_key() const { return m_raw_key; }

private:
    Type m_type;
    u32 m_code_point { 0 };
    u32 m_key_code { 0 };
    u32 m_keyboard_modifiers { 0 };
    u32 m_raw_key { 0 };
};

}
