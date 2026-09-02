/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <LibCore/AnonymousBuffer.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/Rect.h>

namespace Loom::Wayland {

namespace Protocol {
class Shm;
}

class Window;

class WindowFrame {
    AK_MAKE_NONCOPYABLE(WindowFrame);
    AK_MAKE_NONMOVABLE(WindowFrame);
public:

    WindowFrame(Window& window, Protocol::Shm& shm);
    ~WindowFrame();

    static void load_theme_config();

    void window_content_changed(Badge<Window>);

    [[nodiscard]] Window& window() const { return m_window; }

    [[nodiscard]] Gfx::IntRect menubar_rect() const;
    [[nodiscard]] Gfx::IntRect titlebar_rect() const;
    [[nodiscard]] Gfx::IntRect titlebar_icon_rect() const;
    [[nodiscard]] Gfx::IntRect titlebar_text_rect() const;

    [[nodiscard]] Gfx::IntRect frame_rect() const;
    [[nodiscard]] Gfx::IntRect inflated_for_shadow(Gfx::IntRect const& frame_rect) const;

    [[nodiscard]] Gfx::IntRect leftmost_titlebar_button_rect() const;

private:
    Window& m_window;
    Protocol::Shm& m_shm;

    Core::AnonymousBuffer m_render_buffer;
    RefPtr<Gfx::Bitmap> m_render_bitmap;

};

}
