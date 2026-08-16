/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/String.h>
#include <AK/Platform.h>
#include <AK/Noncopyable.h>
#include <Loom/Wayland/Protocol/Interface.h>
#include <wayland-client.h>

namespace Loom::Wayland::Protocol {

class Output {
    AK_MAKE_NONCOPYABLE(Output);
    AK_MAKE_NONMOVABLE(Output);
public:
    WAYLAND_INTERFACE(wl_output);

    enum class Subpixel {
        Unknown = WL_OUTPUT_SUBPIXEL_UNKNOWN,
        None = WL_OUTPUT_SUBPIXEL_NONE,
        HorizontalRGB = WL_OUTPUT_SUBPIXEL_HORIZONTAL_RGB,
        HorizontalBGR = WL_OUTPUT_SUBPIXEL_HORIZONTAL_BGR,
        VerticalRGB = WL_OUTPUT_SUBPIXEL_VERTICAL_RGB,
        VerticalBGR = WL_OUTPUT_SUBPIXEL_VERTICAL_BGR,
    };

    enum class Transform : u32 {
        Normal = WL_OUTPUT_TRANSFORM_NORMAL,
        Degrees90 = WL_OUTPUT_TRANSFORM_90,
        Degrees180 = WL_OUTPUT_TRANSFORM_180,
        Degrees270 = WL_OUTPUT_TRANSFORM_270,
        Flipped = WL_OUTPUT_TRANSFORM_FLIPPED,
        FlippedDegrees90 = WL_OUTPUT_TRANSFORM_FLIPPED_90,
        FlippedDegrees180 = WL_OUTPUT_TRANSFORM_FLIPPED_180,
        FlippedDegrees270 = WL_OUTPUT_TRANSFORM_FLIPPED_270,
    };

    struct Geometry {
        i32 x { 0 }; // prefer xdg-output-unstable-v1 logical_position.x
        i32 y { 0 }; // prefer xdg-output-unstable-v1 logical_position.y
        i32 physical_width { 0 }; // in millimeters
        i32 physical_height { 0 }; // in millimeters
        Subpixel subpixel { Subpixel::Unknown };
        String make; // prefer Output.name
        String model; // prefer Output.description
        Transform transform { Transform::Normal }; // prefer Surface.preferred_transform
    };

    struct Mode {
        i32 width { 0 };
        i32 height { 0 };
        i32 refresh { 0 };
    };

    Output(wl_output* output, u32 global_name);
    ~Output();

    RETURNS_NONNULL [[nodiscard]] wl_output* ptr() const { return m_output; }

    Geometry const& geometry() const { return m_geometry; }
    Mode const& current_mode() const { return m_current_mode; }
    i32 scale() const { return m_scale; }
    String const& name() const { return m_name; }
    String const& description() const { return m_description; }
    u32 global_name() const { return m_global_name; }

private:
    wl_output* m_output;

    static void output_geometry(void* data, wl_output*, i32 x, i32 y, i32 physical_width, i32 physical_height, i32 subpixel, const char* make, const char* model, i32 transform);
    static void output_mode(void* data, wl_output*, u32 flags, i32 width, i32 height, i32 refresh);
    static void output_done(void* data, wl_output*);
    static void output_scale(void* data, wl_output*, i32 scale);
    static void output_name(void* data, wl_output*, const char* name);
    static void output_description(void* data, wl_output*, const char* description);
    static const wl_output_listener s_output_listener;

    Geometry m_geometry;
    Mode m_current_mode;
    i32 m_scale { 1 };
    String m_name;
    String m_description;
    u32 m_global_name { 0 };
};

}
