/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/Platform.h>
#include <LibWayland/Forward.h>
#include <LibWayland/Interface.h>
#include <wp-viewporter-client.h>

namespace Wayland {

class Viewporter {
    AK_MAKE_NONCOPYABLE(Viewporter);
    AK_MAKE_NONMOVABLE(Viewporter);
public:
    WAYLAND_INTERFACE(wp_viewporter);

    explicit Viewporter(wp_viewporter* viewporter);
    ~Viewporter();

    RETURNS_NONNULL [[nodiscard]] wp_viewporter* ptr() const { return m_viewporter; }

    NonnullOwnPtr<Viewport> get_viewport(Surface*);

private:
    wp_viewporter* m_viewporter;
};

}
