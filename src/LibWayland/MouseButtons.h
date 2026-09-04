/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Platform.h>
#include <AK/Types.h>

#if __has_include(<linux/input-event-codes.h>)
#    include <linux/input-event-codes.h>
#elif __has_include(<libevdev/libevdev.h>)
#    include <libevdev/libevdev.h>
#elif defined(AK_OS_FREEBSD) || defined(AK_OS_DRAGONFLY)
#    include <dev/evdev/input-event-codes.h>
#else
#    define BTN_LEFT 0x110
#    define BTN_RIGHT 0x111
#    define BTN_MIDDLE 0x112
#    define BTN_SIDE 0x113
#    define BTN_EXTRA 0x114
#    define BTN_FORWARD 0x115
#    define BTN_BACK 0x116
#    define BTN_TASK 0x117
#endif

namespace Wayland {

enum class RawMouseButton : u32 {
    Left = BTN_LEFT,
    Right = BTN_RIGHT,
    Middle = BTN_MIDDLE,
    Side = BTN_SIDE,
    Extra = BTN_EXTRA,
    Forward = BTN_FORWARD,
    Backward = BTN_BACK,
    Task = BTN_TASK,
};

enum class MouseButtonState : u32 {
    Released = 0,
    Pressed = 1,
};

}
