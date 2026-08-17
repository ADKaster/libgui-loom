/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/Format.h>
#include <Loom/Wayland/Protocol/Buffer.h>
#include <Loom/Wayland/Protocol/Surface.h>

#define SURFACE_DEBUG 1

namespace Loom::Wayland::Protocol {

static void surface_enter(void* data, wl_surface* surface, wl_output*)
{
    auto& self = *static_cast<Surface*>(data);
    VERIFY(self.ptr() == surface);
    dbgln_if(SURFACE_DEBUG, "Surface::surface_enter");
    if (self.on_enter)
        self.on_enter(/* FIXME: Output */);
}

static void surface_leave(void* data, wl_surface* surface, wl_output*)
{
    auto& self = *static_cast<Surface*>(data);
    VERIFY(self.ptr() == surface);
    dbgln_if(SURFACE_DEBUG, "Surface::surface_leave");
    if (self.on_leave)
        self.on_leave(/* FIXME: Output */);
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

Surface::Surface(wl_surface* surface)
    : m_surface(surface)
{
    VERIFY(m_surface != nullptr);
    wl_surface_add_listener(m_surface, &s_surface_listener, this);
}

Surface::~Surface()
{
    wl_surface_destroy(m_surface);
}

void Surface::attach(OwnPtr<Buffer> buffer, i32 x, i32 y)
{
    wl_surface_attach(m_surface, buffer->ptr(), x, y);
    m_pending_buffer = move(buffer);
}

void Surface::commit()
{
    wl_surface_commit(m_surface);
}

}
