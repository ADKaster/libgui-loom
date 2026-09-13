/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Function.h>
#include <AK/Weakable.h>
#include <LibGfx/Bitmap.h>
#include <Loom/Wayland/Events.h>

namespace Loom {

class Window;
class WindowFrame;

class Button : public Weakable<Button> {
public:
    Button(WindowFrame&, Function<void()>&& on_click_handler);
    ~Button();

    Gfx::IntRect relative_rect() const { return m_relative_rect; }
    Gfx::IntRect surface_rect() const { return m_surface_rect; }
    void set_relative_rect(Gfx::IntRect const& rect) { m_relative_rect = rect; }
    void set_surface_rect(Gfx::IntRect const& rect) { m_surface_rect = rect; }

    Gfx::IntRect rect() const { return { {}, m_relative_rect.size() }; }

    void paint(Gfx::Painter&);
    void handle_mouse_event(MouseEvent const&);

    Function<void()> on_click;

    struct Icon {
        RefPtr<Gfx::Bitmap> bitmap { nullptr };
        RefPtr<Gfx::Bitmap> hover_bitmap { nullptr };
    };

    void set_icon(Icon const& icon) { m_icon = icon; }

    enum class Style : u8 {
        Normal,
        IconOnly
    };

    void set_style(Style style) { m_style = style; }

    Window const* window() const;

private:
    WindowFrame& m_frame;
    Gfx::IntRect m_relative_rect;
    Gfx::IntRect m_surface_rect;
    Icon m_icon;
    bool m_pressed { false };
    bool m_hovered { false };
    Style m_style { Style::Normal };
};

}
