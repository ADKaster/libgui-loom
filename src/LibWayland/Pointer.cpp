/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/Format.h>
#include <LibWayland/Pointer.h>
#include <LibWayland/Surface.h>

namespace Wayland {

#define WAYLAND_POINTER_DEBUG 0

void Pointer::pointer_enter(void* data, wl_pointer* pointer, u32 serial, wl_surface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_enter: serial={}, surface={}, surface_x={}, surface_y={}", serial, surface, wl_fixed_to_double(surface_x), wl_fixed_to_double(surface_y));

    if (!surface) // FIXME: Tell someone that there's no focused window?
        return;

    self.m_pending_events.empend(EnterEvent {
        .surface = static_cast<Surface*>(wl_surface_get_user_data(surface)),
        .serial = serial,
        .position = { wl_fixed_to_int(surface_x), wl_fixed_to_int(surface_y) }
    });
}

void Pointer::pointer_leave(void* data, wl_pointer* pointer, u32 serial, wl_surface* surface)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_leave: serial={}, surface={}", serial, surface);

    if (!surface) // FIXME: Tell someone that there's no focused window?
        return;

    self.m_pending_events.empend(LeaveEvent {
        .surface = static_cast<Surface*>(wl_surface_get_user_data(surface)),
        .serial = serial
    });
}

void Pointer::pointer_motion(void* data, wl_pointer* pointer, u32 time, wl_fixed_t surface_x, wl_fixed_t surface_y)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_motion: time={}, surface_x={}, surface_y={}", time, wl_fixed_to_double(surface_x), wl_fixed_to_double(surface_y));

    self.m_pending_events.empend(MotionEvent {
        .time = Duration::from_milliseconds(time),
        .position = { wl_fixed_to_int(surface_x), wl_fixed_to_int(surface_y) }
    });
}

void Pointer::pointer_button(void* data, wl_pointer* pointer, u32 serial, u32 time, u32 button, u32 state)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_button: serial={}, time={}, button={}, state={}", serial, time, button, state);

    if (button < to_underlying(RawMouseButton::Left) || button > to_underlying(RawMouseButton::Task)) {
        warnln("Pointer::pointer_button: unknown button code {}", button);
        return;
    }

    self.m_pending_events.empend(ButtonEvent {
        .serial = serial,
        .time = Duration::from_milliseconds(time),
        .button = static_cast<RawMouseButton>(button),
        .state = static_cast<ButtonState>(state)
    });
}

void Pointer::pointer_axis(void* data, wl_pointer* pointer, u32 time, u32 axis, wl_fixed_t value)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_axis: time={}, axis={}, value={}", time, axis, wl_fixed_to_double(value));
}

void Pointer::pointer_frame(void* data, wl_pointer* pointer)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_frame with {} pending events", self.m_pending_events.size());

    self.dispatch_pending_events();
}

void Pointer::pointer_axis_source(void* data, wl_pointer* pointer, u32 axis_source)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_axis_source: axis_source={}", axis_source);
}

void Pointer::pointer_axis_stop(void* data, wl_pointer* pointer, u32 time, u32 axis)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_axis_stop: time={}, axis={}", time, axis);
}

void Pointer::pointer_axis_discrete(void* data, wl_pointer* pointer, u32 axis, i32 discrete)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_axis_discrete: axis={}, discrete={}", axis, discrete);
}

void Pointer::pointer_axis_value120(void* data, wl_pointer* pointer, u32 axis, i32 value120)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_axis_value120: axis={}, value120={}", axis, value120);
}

void Pointer::pointer_axis_relative_direction(void* data, wl_pointer* pointer, u32 axis, u32 direction)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_axis_relative_direction: axis={}, direction={}", axis, direction);
}

void Pointer::pointer_warp(void* data, wl_pointer* pointer, wl_fixed_t surface_x, wl_fixed_t surface_y)
{
    auto& self = *static_cast<Pointer*>(data);
    VERIFY(self.ptr() == pointer);
    dbgln_if(WAYLAND_POINTER_DEBUG, "Pointer::pointer_warp: surface_x={}, surface_y={}", wl_fixed_to_double(surface_x), wl_fixed_to_double(surface_y));
}

const wl_pointer_listener Pointer::s_pointer_listener = {
    .enter = pointer_enter,
    .leave = pointer_leave,
    .motion = pointer_motion,
    .button = pointer_button,
    .axis = pointer_axis,
    .frame = pointer_frame,
    .axis_source = pointer_axis_source,
    .axis_stop = pointer_axis_stop,
    .axis_discrete = pointer_axis_discrete,
    .axis_value120 = pointer_axis_value120,
    .axis_relative_direction = pointer_axis_relative_direction,
    .warp = pointer_warp
};

Pointer::Pointer(wl_pointer* pointer)
    : m_pointer(pointer)
{
    VERIFY(m_pointer != nullptr);

    wl_pointer_set_user_data(m_pointer, this);
    wl_pointer_add_listener(m_pointer, &s_pointer_listener, this);
}

Pointer::~Pointer()
{
    wl_pointer_release(m_pointer);
}

void Pointer::set_cursor(i32 serial, Surface& surface, Gfx::IntPoint hotspot)
{
    wl_pointer_set_cursor(m_pointer, serial, surface.ptr(), hotspot.x(), hotspot.y());
}

static MouseButtonState to_mouse_button_state(Pointer::ButtonState state)
{
    switch (state) {
    case Pointer::ButtonState::Released:
        return MouseButtonState::Released;
    case Pointer::ButtonState::Pressed:
        return MouseButtonState::Pressed;
    default:
        VERIFY_NOT_REACHED();
    }
}

void Pointer::dispatch_pending_events()
{
    auto events = move(m_pending_events);
    for (auto& event : events) {
        event.visit(
            [&](EnterEvent const& enter) {
                if (!enter.surface)
                    return;
                if (enter.surface->on_pointer_enter)
                    enter.surface->on_pointer_enter(*this, enter.serial, enter.position);
            },
            [&](LeaveEvent const& leave) {
                if (!leave.surface)
                    return;
                if (leave.surface->on_pointer_leave)
                    leave.surface->on_pointer_leave(*this, leave.serial);
            },
            [&](MotionEvent const& motion) {
                if (!motion.surface)
                    return;
                if (motion.surface->on_pointer_motion)
                    motion.surface->on_pointer_motion(*this, motion.time, motion.position);
            },
            [&](ButtonEvent const& button) {
                if (!button.surface)
                    return;
                if (button.surface->on_pointer_button)
                    button.surface->on_pointer_button(*this, button.serial, button.time, button.button, to_mouse_button_state(button.state));
            },
            [&](AxisEvent const& axis) {
                if (!axis.surface)
                    return;
                if (axis.surface->on_pointer_axis)
                    axis.surface->on_pointer_axis(*this);
            });
    }
}

}
