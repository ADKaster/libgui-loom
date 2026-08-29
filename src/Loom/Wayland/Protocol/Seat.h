/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/EnumBits.h>
#include <AK/Noncopyable.h>
#include <AK/OwnPtr.h>
#include <AK/Platform.h>
#include <AK/ByteString.h>
#include <Loom/Wayland/Protocol/Interface.h>
#include <wayland-client.h>

namespace Loom::Wayland::Protocol {

class Keyboard;
class Pointer;

class Seat {
    AK_MAKE_NONCOPYABLE(Seat);
    AK_MAKE_NONMOVABLE(Seat);
public:
    WAYLAND_INTERFACE(wl_seat);

    enum class Capabilities : u32 {
        None = 0,
        Pointer = WL_SEAT_CAPABILITY_POINTER,
        Keyboard = WL_SEAT_CAPABILITY_KEYBOARD,
        Touch = WL_SEAT_CAPABILITY_TOUCH,
    };

    explicit Seat(wl_seat* seat);
    ~Seat();

    RETURNS_NONNULL [[nodiscard]] wl_seat* ptr() const { return m_seat; }

    [[nodiscard]] ByteString const& name() const { return m_name; }
    [[nodiscard]] Keyboard const* keyboard() const { return m_keyboard.ptr(); }
    [[nodiscard]] Pointer const* pointer() const { return m_pointer.ptr(); }

private:
    wl_seat* m_seat;

    static void seat_capabilities(void* data, wl_seat*, u32 capabilities);
    static void seat_name(void* data, wl_seat*, const char*);
    static const wl_seat_listener s_seat_listener;

    ByteString m_name;

    OwnPtr<Keyboard> m_keyboard;
    OwnPtr<Pointer> m_pointer;
    // FIXME: Touch
};

AK_ENUM_BITWISE_OPERATORS(Seat::Capabilities);

}
