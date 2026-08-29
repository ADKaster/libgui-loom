/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/Format.h>
#include <Loom/Wayland/Protocol/Keyboard.h>
#include <Loom/Wayland/Protocol/Pointer.h>
#include <Loom/Wayland/Protocol/Seat.h>

namespace Loom::Wayland::Protocol {

void Seat::seat_capabilities(void* data, wl_seat* seat, u32 capabilities)
{
    auto& self = *static_cast<Seat*>(data);
    VERIFY(self.ptr() == seat);
    dbgln("Seat::seat_capabilities: capabilities={}", capabilities);
    auto caps = static_cast<Capabilities>(capabilities);
    if (!self.m_pointer && has_flag(caps, Capabilities::Pointer))
        self.m_pointer = make<Pointer>(wl_seat_get_pointer(seat));
    if (!self.m_keyboard && has_flag(caps, Capabilities::Keyboard))
        self.m_keyboard = make<Keyboard>(wl_seat_get_keyboard(seat));
}

void Seat::seat_name(void* data, wl_seat* seat, const char* name)
{
    auto& self = *static_cast<Seat*>(data);
    VERIFY(self.ptr() == seat);
    dbgln("Seat::seat_name: name={}", name);
    self.m_name = name;
}

const wl_seat_listener Seat::s_seat_listener
{
    .capabilities = seat_capabilities,
    .name = seat_name,
};

Seat::Seat(wl_seat* seat)
    : m_seat(seat)
{
    VERIFY(m_seat != nullptr);
    wl_seat_add_listener(m_seat, &s_seat_listener, this);
}

Seat::~Seat()
{
    wl_seat_release(m_seat);
}

}
