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
#include <LibWayland/Forward.h>
#include <LibWayland/Interface.h>
#include <wayland-client.h>

namespace Wayland {

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
    [[nodiscard]] Keyboard* keyboard() const { return m_keyboard.ptr(); }
    [[nodiscard]] Pointer* pointer() const { return m_pointer.ptr(); }

    void set_keyboard_delegate(KeyboardDelegate* delegate) { m_keyboard_delegate = delegate; }
    void set_pointer_delegate(PointerDelegate* delegate) { m_pointer_delegate = delegate; }

private:
    wl_seat* m_seat;

    static void seat_capabilities(void* data, wl_seat*, u32 capabilities);
    static void seat_name(void* data, wl_seat*, const char*);
    static const wl_seat_listener s_seat_listener;

    ByteString m_name;

    OwnPtr<Keyboard> m_keyboard;
    OwnPtr<Pointer> m_pointer;
    // FIXME: Touch

    KeyboardDelegate* m_keyboard_delegate { nullptr };
    PointerDelegate* m_pointer_delegate { nullptr };
};

AK_ENUM_BITWISE_OPERATORS(Seat::Capabilities);

}
