/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/EnumBits.h>
#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <LibGfx/Rect.h>
#include <LibWayland/Interface.h>
#include <xdg-shell-client.h>

namespace Wayland {

class XdgPositioner {
    AK_MAKE_NONCOPYABLE(XdgPositioner);
    AK_MAKE_NONMOVABLE(XdgPositioner);
public:
    WAYLAND_INTERFACE(xdg_positioner);

    enum class Anchor : u32 {
        None = XDG_POSITIONER_ANCHOR_NONE,
        Top = XDG_POSITIONER_ANCHOR_TOP,
        Bottom = XDG_POSITIONER_ANCHOR_BOTTOM,
        Left = XDG_POSITIONER_ANCHOR_LEFT,
        Right = XDG_POSITIONER_ANCHOR_RIGHT,
        TopLeft = XDG_POSITIONER_ANCHOR_TOP_LEFT,
        TopRight = XDG_POSITIONER_ANCHOR_TOP_RIGHT,
        BottomLeft = XDG_POSITIONER_ANCHOR_BOTTOM_LEFT,
        BottomRight = XDG_POSITIONER_ANCHOR_BOTTOM_RIGHT,
    };

    enum class Gravity : u32 {
        None = XDG_POSITIONER_GRAVITY_NONE,
        Top = XDG_POSITIONER_GRAVITY_TOP,
        Bottom = XDG_POSITIONER_GRAVITY_BOTTOM,
        Left = XDG_POSITIONER_GRAVITY_LEFT,
        Right = XDG_POSITIONER_GRAVITY_RIGHT,
        TopLeft = XDG_POSITIONER_GRAVITY_TOP_LEFT,
        TopRight = XDG_POSITIONER_GRAVITY_TOP_RIGHT,
        BottomLeft = XDG_POSITIONER_GRAVITY_BOTTOM_LEFT,
        BottomRight = XDG_POSITIONER_GRAVITY_BOTTOM_RIGHT,
    };

    enum class ConstraintAdjustment : u32 {
        None = XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_NONE,
        SlideX = XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_X,
        SlideY = XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_Y,
        FlipX = XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_X,
        FlipY = XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_Y,
        ResizeX = XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_RESIZE_X,
        ResizeY = XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_RESIZE_Y,
    };

    explicit XdgPositioner(xdg_positioner* xdg_positioner);
    ~XdgPositioner();

    RETURNS_NONNULL [[nodiscard]] xdg_positioner* ptr() const { return m_xdg_positioner; }

    void set_size(Gfx::IntSize);
    void set_anchor_rect(Gfx::IntRect);
    void set_anchor(Anchor);
    void set_gravity(Gravity);
    void set_constraint_adjustment(ConstraintAdjustment);
    void set_offset(Gfx::IntPoint);
    void set_reactive();
    void set_parent_size(Gfx::IntSize);
    void set_parent_configure(i32 serial);

private:
    xdg_positioner* m_xdg_positioner;
};

AK_ENUM_BITWISE_OPERATORS(XdgPositioner::ConstraintAdjustment);

}
