/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/Application.h>
#include <Loom/Wayland/Button.h>
#include <Loom/Wayland/SeatDelegate.h>
#include <Loom/Wayland/Window.h>
#include <Loom/Wayland/WindowFrame.h>
#include <LibGUI/Event.h>
#include <LibGfx/Painter.h>

namespace Loom {

Button::Button(WindowFrame& frame, Function<void()>&& on_click_handler)
    : on_click(move(on_click_handler))
    , m_frame(frame)
{
}

Button::~Button() = default;

Window const* Button::window() const
{
    return &m_frame.window();
}

void Button::handle_mouse_event(MouseEvent const& event)
{
    auto& seat_delegate = Application::the().seat_delegate();

    if (event.button() != GUI::MouseButton::Primary && event.type() != MouseEvent::Type::MouseMove)
        return;

    if (event.type() == MouseEvent::Type::MouseDown && event.button() == GUI::MouseButton::Primary) {
        m_pressed = true;
        seat_delegate.set_cursor_tracking_button(this);
        m_frame.invalidate_decorations();
        return;
    }

    if (event.type() == MouseEvent::Type::MouseUp && event.button() == GUI::MouseButton::Primary) {
        if (seat_delegate.cursor_tracking_button() != this)
            return;
        seat_delegate.set_cursor_tracking_button(nullptr);
        bool old_pressed = m_pressed;
        m_pressed = false;
        if (rect().contains(event.position())) {
            if (on_click)
                on_click();
        }
        if (old_pressed != m_pressed) {
            m_hovered = false;
            m_frame.invalidate_decorations();
        }
        return;
    }

    if (event.type() == MouseEvent::Type::MouseMove) {
        auto old_hovered = m_hovered;
        m_hovered = rect().contains(event.position());
        seat_delegate.set_hovered_button(m_hovered ? this : nullptr);
        if (old_hovered != m_hovered)
            m_frame.invalidate_decorations();
    }

    if (event.type() == MouseEvent::Type::MouseMove && event.buttons() & to_underlying(GUI::MouseButton::Primary)) {
        if (seat_delegate.cursor_tracking_button() != this)
            return;
        bool old_pressed = m_pressed;
        m_pressed = m_hovered;
        if (old_pressed != m_pressed)
            m_frame.invalidate_decorations();
    }

}

void Button::paint(Gfx::Painter& painter)
{
    auto palette = Application::the().palette();
    Gfx::PainterStateSaver saver(painter);
    painter.translate(relative_rect().location());

    if (m_style == Style::Normal)
        Gfx::StylePainter::paint_button(painter, rect(), palette, Gfx::ButtonStyle::Normal, m_pressed, m_hovered);

    auto paint_icon = [&](Gfx::Bitmap& bitmap) {
        auto icon_location = rect().center().translated(-(bitmap.width() / 2), -(bitmap.height() / 2));
        if (m_pressed)
            painter.translate(1, 1);
        auto inactive_opacity = palette.window_title_button_inactive_alpha() / 255.0f;
        bool is_inactive = m_frame.window().type() != WindowServer::WindowType::Notification
            && m_frame.window_state_for_theme() == Gfx::WindowTheme::WindowState::Inactive;
        painter.blit(icon_location, bitmap, bitmap.rect(), is_inactive ? inactive_opacity : 1.0f);
    };

    if (m_hovered && m_icon.hover_bitmap)
        paint_icon(*m_icon.hover_bitmap);
    else if (m_icon.bitmap)
        paint_icon(*m_icon.bitmap);
}

}
