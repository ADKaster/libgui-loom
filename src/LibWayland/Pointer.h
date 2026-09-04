/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <AK/Time.h>
#include <AK/Variant.h>
#include <AK/Vector.h>
#include <LibGfx/Point.h>
#include <LibWayland/Forward.h>
#include <LibWayland/Interface.h>
#include <LibWayland/MouseButtons.h>
#include <wayland-client.h>

namespace Wayland {

class Pointer {
    AK_MAKE_NONCOPYABLE(Pointer);
    AK_MAKE_NONMOVABLE(Pointer);
public:
    WAYLAND_INTERFACE(wl_pointer);

    enum class ButtonState : u32 {
        Released = WL_POINTER_BUTTON_STATE_RELEASED,
        Pressed = WL_POINTER_BUTTON_STATE_PRESSED
    };

    enum class Axis : u32 {
        VerticalScroll = WL_POINTER_AXIS_VERTICAL_SCROLL,
        HorizontalScroll = WL_POINTER_AXIS_HORIZONTAL_SCROLL
    };

    enum class AxisSource : u32 {
        Wheel = WL_POINTER_AXIS_SOURCE_WHEEL,
        Finger = WL_POINTER_AXIS_SOURCE_FINGER,
        Continuous = WL_POINTER_AXIS_SOURCE_CONTINUOUS,
        WheelTilt = WL_POINTER_AXIS_SOURCE_WHEEL_TILT
    };

    enum class AxisRelativeDirection : u32 {
        Identical = WL_POINTER_AXIS_RELATIVE_DIRECTION_IDENTICAL,
        Inverted = WL_POINTER_AXIS_RELATIVE_DIRECTION_INVERTED
    };

    explicit Pointer(wl_pointer* pointer);
    ~Pointer();

    RETURNS_NONNULL [[nodiscard]] wl_pointer* ptr() const { return m_pointer; }

    void set_cursor(i32 serial, Surface& surface, Gfx::IntPoint hotspot);

private:

    static void pointer_enter(void* data, wl_pointer* pointer, u32 serial, wl_surface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y);
    static void pointer_leave(void* data, wl_pointer* pointer, u32 serial, wl_surface* surface);
    static void pointer_motion(void* data, wl_pointer* pointer, u32 time, wl_fixed_t surface_x, wl_fixed_t surface_y);
    static void pointer_button(void* data, wl_pointer* pointer, u32 serial, u32 time, u32 button, u32 state);
    static void pointer_axis(void* data, wl_pointer* pointer, u32 time, u32 axis, wl_fixed_t value);
    static void pointer_frame(void* data, wl_pointer* pointer);
    static void pointer_axis_source(void* data, wl_pointer* pointer, u32 axis_source);
    static void pointer_axis_stop(void* data, wl_pointer* pointer, u32 time, u32 axis);
    static void pointer_axis_discrete(void* data, wl_pointer* pointer, u32 axis, i32 discrete);
    static void pointer_axis_value120(void* data, wl_pointer* pointer, u32 axis, i32 value120);
    static void pointer_axis_relative_direction(void* data, wl_pointer* pointer, u32 axis, u32 direction);
    static void pointer_warp(void* data, wl_pointer* pointer, wl_fixed_t surface_x, wl_fixed_t surface_y);

    static const wl_pointer_listener s_pointer_listener;

    void dispatch_pending_events();

    wl_pointer* m_pointer;

    struct EnterEvent {
        Surface* surface { nullptr };
        u32 serial { 0 };
        Gfx::IntPoint position;
    };

    struct LeaveEvent {
        Surface* surface { nullptr };
        u32 serial { 0 };
    };

    struct MotionEvent {
        Surface* surface { nullptr };
        Duration time;
        Gfx::IntPoint position;
    };

    struct ButtonEvent {
        Surface* surface { nullptr };
        u32 serial { 0 };
        Duration time;
        RawMouseButton button { RawMouseButton::Left };
        ButtonState state { ButtonState::Released };
    };

    struct AxisEvent {
        Surface* surface { nullptr };
        // FIXME: Handle mouse wheel etc.
    };

    using Event = Variant<EnterEvent, LeaveEvent, MotionEvent, ButtonEvent, AxisEvent>;
    Vector<Event, 4> m_pending_events;
};

}
