/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullRefPtr.h>
#include <LibCore/Notifier.h>
#include <wayland-client-core.h>

namespace Loom::Wayland {

namespace Protocol {
class Callback;
class Registry;
}

class Display {
    AK_MAKE_NONCOPYABLE(Display);
    AK_MAKE_NONMOVABLE(Display);
public:
    static NonnullOwnPtr<Display> create(StringView name);
    static NonnullOwnPtr<Display> create(int fd);

    ~Display();

    RETURNS_NONNULL [[nodiscard]] wl_display* ptr() const { return m_display; }

    NonnullOwnPtr<Protocol::Callback> sync();

    Protocol::Registry& registry() const;

private:
    explicit Display(wl_display* display, NonnullRefPtr<Core::Notifier> read_notifier, NonnullRefPtr<Core::Notifier> write_notifier);

    wl_display* m_display;

    mutable OwnPtr<Protocol::Registry> m_registry;

    NonnullRefPtr<Core::Notifier> m_read_notifier;
    NonnullRefPtr<Core::Notifier> m_write_notifier;
};

}
