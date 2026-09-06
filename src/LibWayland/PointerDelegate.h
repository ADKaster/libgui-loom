/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Function.h>
#include <AK/Time.h>
#include <LibWayland/Forward.h>
#include <LibWayland/MouseButtons.h>
#include <LibGfx/Point.h>

namespace Wayland {

struct PointerDelegate {
    Function<void(Pointer&, Surface*, u32 serial, Gfx::IntPoint position)> on_pointer_enter;
    Function<void(Pointer&, Surface*, u32 serial)> on_pointer_leave;
    Function<void(Pointer&, Surface*, Duration time, Gfx::IntPoint position)> on_pointer_motion;
    Function<void(Pointer&, Surface*, u32 serial, Duration time, RawMouseButton button, MouseButtonState state)> on_pointer_button;
    Function<void(Pointer&)> on_pointer_axis;
};

}
