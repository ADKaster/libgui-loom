/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Function.h>
#include <AK/Noncopyable.h>
#include <AK/OwnPtr.h>
#include <AK/Platform.h>
#include <LibGfx/Point.h>
#include <LibWayland/Forward.h>
#include <LibWayland/MouseButtons.h>
#include <LibWayland/Interface.h>
#include <wayland-client.h>

namespace Wayland {

class Surface {
    AK_MAKE_NONCOPYABLE(Surface);
    AK_MAKE_NONMOVABLE(Surface);
public:
    WAYLAND_INTERFACE(wl_surface);

    Surface(wl_surface* surface, u32 version);
    ~Surface();

    RETURNS_NONNULL [[nodiscard]] wl_surface* ptr() const { return m_surface; }

    Function<void(Output&)> on_enter;
    Function<void(Output&)> on_leave;

    i32 preferred_buffer_scale() const { return m_preferred_buffer_scale; }
    u32 preferred_transform() const { return m_preferred_transform; }

    void attach(Buffer&, i32 x, i32 y);
    void damage(Gfx::IntRect const&);
    void damage_buffer(Gfx::IntRect const&);
    NonnullOwnPtr<Callback> frame();
    void commit();

private:
    wl_surface* m_surface;
    u32 m_version;

    static void surface_preferred_buffer_scale(void* data, wl_surface*, i32 scale);
    static void surface_preferred_transform(void* data, wl_surface*, u32 transform);
    static const wl_surface_listener s_surface_listener;

    i32 m_preferred_buffer_scale { 1 };
    u32 m_preferred_transform { WL_OUTPUT_TRANSFORM_NORMAL }; // FIXME: make own wl_output.transform enum
};

}
