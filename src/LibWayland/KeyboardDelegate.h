/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/EnumBits.h>
#include <AK/Function.h>
#include <AK/Time.h>
#include <AK/Types.h>
#include <LibWayland/Forward.h>

namespace Wayland {

enum class KeyboardModifier : u8 {
    None = 0,
    Alt = 1 << 0,
    Ctrl = 1 << 1,
    Shift = 1 << 2,
    Super = 1 << 3,
    AltGr = 1 << 4,
};

AK_ENUM_BITWISE_OPERATORS(KeyboardModifier);

enum class KeyState : u8 {
    Released = 0,
    Pressed = 1,
    Repeated = 2,
};

struct KeyboardDelegate {
    Function<void(Keyboard&, Surface*, u32 serial)> on_keyboard_enter;
    Function<void(Keyboard&, Surface*, u32 serial)> on_keyboard_leave;
    Function<void(Keyboard&, Surface*, u32 serial, Duration, u32 raw_key, KeyState, u32 keysym, u32 code_point, KeyboardModifier)> on_keyboard_key;
};

}
