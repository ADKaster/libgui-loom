/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <LibWayland/Output.h>

namespace Wayland {

#define OUTPUT_DEBUG 1

void Output::output_geometry(void* data, wl_output* output, i32 x, i32 y, i32 physical_width, i32 physical_height, i32 subpixel, char const* make, char const* model, i32 transform)
{
    auto& self = *static_cast<Output*>(data);
    VERIFY(self.ptr() == output);
    dbgln_if(OUTPUT_DEBUG, "Output::output_geometry: x={}, y={}, physical_width={}, physical_height={}, subpixel={}, make={}, model={}, transform={}", x, y, physical_width, physical_height, subpixel, make, model, transform);
    self.m_geometry = Geometry {
        .x = x,
        .y = y,
        .physical_width = physical_width,
        .physical_height = physical_height,
        .subpixel = static_cast<Subpixel>(subpixel),
        .make = MUST(String::from_utf8({ make, strlen(make) })),
        .model = MUST(String::from_utf8({ model, strlen(model) })),
        .transform = static_cast<Transform>(transform),
    };
}

void Output::output_mode(void* data, wl_output* output, u32 flags, i32 width, i32 height, i32 refresh)
{
    auto& self = *static_cast<Output*>(data);
    VERIFY(self.ptr() == output);
    dbgln_if(OUTPUT_DEBUG, "Output::output_mode: flags={}, width={}, height={}, refresh={}", flags, width, height, refresh);
    if (flags & WL_OUTPUT_MODE_CURRENT) {
        self.m_current_mode = Mode { width, height, refresh };
    }
    else {
        dbgln("Output::output_mode: Ignoring mode for: {}", flags);
    }
}

void Output::output_done(void*, wl_output*)
{
    // No-op
    // FIXME: Handle 'transactions' of incoming events.
    dbgln_if(OUTPUT_DEBUG, "Output::output_done");
}

void Output::output_scale(void* data, wl_output* output, i32 scale)
{
    auto& self = *static_cast<Output*>(data);
    VERIFY(self.ptr() == output);
    VERIFY(scale > 0);
    dbgln_if(OUTPUT_DEBUG, "Output::output_scale: scale={}", scale);
    self.m_scale = scale;
}

void Output::output_name(void* data, wl_output* output, const char* name)
{
    auto& self = *static_cast<Output*>(data);
    VERIFY(self.ptr() == output);
    dbgln_if(OUTPUT_DEBUG, "Output::output_name: name={}", name);
    self.m_name = MUST(String::from_utf8({ name, strlen(name) }));
}

void Output::output_description(void* data, wl_output* output, const char* description)
{
    auto& self = *static_cast<Output*>(data);
    VERIFY(self.ptr() == output);
    dbgln_if(OUTPUT_DEBUG, "Output::output_description: description={}", description);
    self.m_description = MUST(String::from_utf8({ description, strlen(description) }));
}

const wl_output_listener Output::s_output_listener = {
    .geometry = output_geometry,
    .mode = output_mode,
    .done = output_done,
    .scale = output_scale,
    .name = output_name,
    .description = output_description,
};

Output::Output(wl_output* output, u32 global_name)
    : m_output(output)
    , m_global_name(global_name)
{
    VERIFY(m_output != nullptr);
    wl_output_add_listener(m_output, &s_output_listener, this);
    wl_output_set_user_data(m_output, this);
}

Output::~Output()
{
    wl_output_set_user_data(m_output, nullptr);
    wl_output_destroy(m_output);
}

}
