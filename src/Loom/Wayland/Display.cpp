/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/Display.h>
#include <Loom/Wayland/Protocol/Callback.h>
#include <Loom/Wayland/Protocol/Registry.h>

namespace Loom::Wayland {

Display::Display(wl_display* display, NonnullRefPtr<Core::Notifier> read_notifier, NonnullRefPtr<Core::Notifier> write_notifier)
    : m_display(display)
    , m_read_notifier(move(read_notifier))
    , m_write_notifier(move(write_notifier))
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
    m_read_notifier->set_enabled(false);
    m_write_notifier->set_enabled(false);
    m_registry = nullptr;
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

NonnullOwnPtr<Protocol::Callback> Display::sync()
{
    auto* callback = wl_display_sync(m_display);
    return adopt_own(*new Protocol::Callback(callback));
}

Protocol::Registry& Display::registry() const
{
    if (!m_registry) {
        auto* wayland_registry = wl_display_get_registry(m_display);
        m_registry = adopt_own(*new Protocol::Registry(wayland_registry));
    }
    return *m_registry;
}

}
