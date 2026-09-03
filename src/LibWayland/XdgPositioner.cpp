/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWayland/XdgPositioner.h>

namespace Wayland {
XdgPositioner::XdgPositioner(xdg_positioner* xdg_positioner)
    : m_xdg_positioner(xdg_positioner)
{
    VERIFY(m_xdg_positioner != nullptr);
}

XdgPositioner::~XdgPositioner()
{
    xdg_positioner_destroy(m_xdg_positioner);
}

void XdgPositioner::set_size(Gfx::IntSize size)
{
    xdg_positioner_set_size(m_xdg_positioner, size.width(), size.height());
}

void XdgPositioner::set_anchor_rect(Gfx::IntRect rect)
{
    xdg_positioner_set_anchor_rect(m_xdg_positioner, rect.x(), rect.y(), rect.width(), rect.height());
}

void XdgPositioner::set_anchor(Anchor anchor)
{
    xdg_positioner_set_anchor(m_xdg_positioner, to_underlying(anchor));
}

void XdgPositioner::set_gravity(Gravity gravity)
{
    xdg_positioner_set_gravity(m_xdg_positioner, to_underlying(gravity));
}

void XdgPositioner::set_constraint_adjustment(ConstraintAdjustment adjustment)
{
    xdg_positioner_set_constraint_adjustment(m_xdg_positioner, to_underlying(adjustment));
}

void XdgPositioner::set_offset(Gfx::IntPoint offset)
{
    xdg_positioner_set_offset(m_xdg_positioner, offset.x(), offset.y());
}

void XdgPositioner::set_reactive()
{
    xdg_positioner_set_reactive(m_xdg_positioner);
}

void XdgPositioner::set_parent_size(Gfx::IntSize size)
{
    xdg_positioner_set_parent_size(m_xdg_positioner, size.width(), size.height());
}

void XdgPositioner::set_parent_configure(i32 serial)
{
    xdg_positioner_set_parent_configure(m_xdg_positioner, serial);
}

}
