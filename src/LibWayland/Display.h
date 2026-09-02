/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/NonnullRefPtr.h>
#include <LibCore/Notifier.h>
#include <LibWayland/Forward.h>
#include <wayland-client-core.h>

namespace Wayland {

class Display {
    AK_MAKE_NONCOPYABLE(Display);
    AK_MAKE_NONMOVABLE(Display);
public:
    static NonnullOwnPtr<Display> create(StringView name);
    static NonnullOwnPtr<Display> create(int fd);

    ~Display();

    RETURNS_NONNULL [[nodiscard]] wl_display* ptr() const { return m_display; }

    NonnullOwnPtr<Callback> sync();

    Registry& registry() const;

private:
    explicit Display(wl_display* display, NonnullRefPtr<Core::Notifier> read_notifier, NonnullRefPtr<Core::Notifier> write_notifier);

    wl_display* m_display;

    mutable OwnPtr<Registry> m_registry;

    NonnullRefPtr<Core::Notifier> m_read_notifier;
    NonnullRefPtr<Core::Notifier> m_write_notifier;
};

}
