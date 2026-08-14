/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/Display.h>
#include <Loom/Wayland/Protocol/Registry.h>

namespace Loom::Wayland {

Display::Display(wl_display* display, NonnullRefPtr<Core::Notifier> read_notifier, NonnullRefPtr<Core::Notifier> write_notifier)
    : m_display(display)
    , m_read_notifier(read_notifier)
    , m_write_notifier(write_notifier)
{
    VERIFY(m_display != nullptr);

    m_read_notifier->on_activation = [&]() {
        wl_display_dispatch(m_display);
    };

    m_write_notifier->on_activation = [&]() {
        wl_display_flush(m_display);
    };
}

Display::~Display()
{
    wl_display_disconnect(m_display);
}

NonnullOwnPtr<Display> Display::create(StringView name)
{
    auto* display = wl_display_connect(name.characters_without_null_termination());
    VERIFY(display);
    auto fd = wl_display_get_fd(display);

    auto read_notifier = MUST(Core::Notifier::try_create(fd, Core::Notifier::Type::Read));
    auto write_notifier = MUST(Core::Notifier::try_create(fd, Core::Notifier::Type::Write));

    return adopt_own(*new Display(display, move(read_notifier), move(write_notifier)));
}

NonnullOwnPtr<Display> Display::create(int fd)
{
    auto* display = wl_display_connect_to_fd(fd);
    VERIFY(display);

    auto read_notifier = MUST(Core::Notifier::try_create(fd, Core::Notifier::Type::Read));
    auto write_notifier = MUST(Core::Notifier::try_create(fd, Core::Notifier::Type::Write));

    return adopt_own(*new Display(display, move(read_notifier), move(write_notifier)));
}

NonnullOwnPtr<Protocol::Registry> Display::get_registry()
{
    auto* wayland_registry = wl_display_get_registry(m_display);
    VERIFY(wayland_registry);
    auto registry = adopt_own(*new Protocol::Registry(wayland_registry));

    wl_display_roundtrip(m_display);

    return registry;
}

}
