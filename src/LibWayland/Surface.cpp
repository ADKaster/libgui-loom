/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/Format.h>
#include <LibWayland/Buffer.h>
#include <LibWayland/Callback.h>
#include <LibWayland/Output.h>
#include <LibWayland/Region.h>
#include <LibWayland/Surface.h>
#include <LibGfx/Rect.h>

#define SURFACE_DEBUG 1

namespace Wayland {

static void surface_enter(void* data, wl_surface* surface, wl_output* output)
{
    auto& self = *static_cast<Surface*>(data);
    VERIFY(self.ptr() == surface);
    auto& out = *static_cast<Output*>(wl_output_get_user_data(output));
    dbgln_if(SURFACE_DEBUG, "Surface::surface_enter({})", out.name());
    if (self.on_enter)
        self.on_enter(out);
}

static void surface_leave(void* data, wl_surface* surface, wl_output* output)
{
    auto& self = *static_cast<Surface*>(data);
    VERIFY(self.ptr() == surface);
    auto& out = *static_cast<Output*>(wl_output_get_user_data(output));
    dbgln_if(SURFACE_DEBUG, "Surface::surface_leave({})", out.name());
    if (self.on_leave)
        self.on_leave(out);
}

void Surface::surface_preferred_buffer_scale(void* data, wl_surface* surface, i32 scale)
{
    auto& self = *static_cast<Surface*>(data);
    VERIFY(self.ptr() == surface);
    VERIFY(scale > 0);
    dbgln_if(SURFACE_DEBUG, "Surface::surface_preferred_buffer_scale: scale={}", scale);
    self.m_preferred_buffer_scale = scale;
}

void Surface::surface_preferred_transform(void* data, wl_surface* surface, u32 transform)
{
    auto& self = *static_cast<Surface*>(data);
    VERIFY(self.ptr() == surface);
    dbgln_if(SURFACE_DEBUG, "Surface::surface_preferred_transform: transform={}", transform);
    // FIXME: Validate that transform fits w/in wl_output.transform enum
    self.m_preferred_transform = transform;
}

const wl_surface_listener Surface::s_surface_listener = {
    .enter = surface_enter,
    .leave = surface_leave,
    .preferred_buffer_scale = surface_preferred_buffer_scale,
    .preferred_buffer_transform = surface_preferred_transform,
};

Surface::Surface(wl_surface* surface, u32 version)
    : m_surface(surface)
    , m_version(version)
{
    VERIFY(m_surface != nullptr);
    wl_surface_add_listener(m_surface, &s_surface_listener, this);
    wl_surface_set_user_data(m_surface, this);
}

Surface::~Surface()
{
    wl_surface_set_user_data(m_surface, nullptr);
    wl_surface_destroy(m_surface);
}

void Surface::attach(Buffer& buffer, i32 x, i32 y)
{
    wl_surface_attach(m_surface, buffer.ptr(), x, y);
}

void Surface::damage(Gfx::IntRect const& rect)
{
    wl_surface_damage(m_surface, rect.x(), rect.y(), rect.width(), rect.height());
}

void Surface::damage_buffer(Gfx::IntRect const& rect)
{
#if defined(WL_SURFACE_DAMAGE_BUFFER)
    if (m_version >= WL_SURFACE_DAMAGE_BUFFER_SINCE_VERSION)
        wl_surface_damage_buffer(m_surface, rect.x(), rect.y(), rect.width(), rect.height());
    else
#endif
        damage(rect);
}

NonnullOwnPtr<Callback> Surface::frame()
{
    return make<Callback>(wl_surface_frame(m_surface));
}

void Surface::commit()
{
    wl_surface_commit(m_surface);
}

void Surface::set_input_region(Region* region)
{
    wl_surface_set_input_region(m_surface, region ? region->ptr() : nullptr);
}

}
