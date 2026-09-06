/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/OwnPtr.h>
#include <AK/Vector.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/Rect.h>
#include <LibWayland/Forward.h>

namespace Loom {

class Window;

class WindowFrame {
    AK_MAKE_NONCOPYABLE(WindowFrame);
    AK_MAKE_NONMOVABLE(WindowFrame);
public:

    WindowFrame(Window& window, Wayland::Shm& shm);
    ~WindowFrame();

    static void load_theme_config();

    void content_paint_finished(Badge<Window>, Vector<Gfx::IntRect> const& damaged_rects);
    void content_rect_changed(Badge<Window>);
    void invalidate_decorations(Badge<Window>);
    void surface_configured(Badge<Window>);

    [[nodiscard]] Window& window() const { return m_window; }

    [[nodiscard]] Gfx::IntRect menubar_rect() const;
    [[nodiscard]] Gfx::IntRect titlebar_rect() const;
    [[nodiscard]] Gfx::IntRect titlebar_icon_rect() const;
    [[nodiscard]] Gfx::IntRect titlebar_text_rect() const;

    [[nodiscard]] Gfx::IntRect frame_rect() const;
    [[nodiscard]] Gfx::IntRect inflated_for_shadow(Gfx::IntRect const& frame_rect) const;

    [[nodiscard]] Gfx::IntRect leftmost_titlebar_button_rect() const;

private:

    struct OutputBuffer {
        RefPtr<Gfx::Bitmap> bitmap;
        NonnullOwnPtr<Wayland::Buffer> buffer;
        bool released { false };
    };

    Gfx::Bitmap* shadow_bitmap() const;
    void paint_frame(Gfx::Bitmap&);
    void discard_released_buffers();
    void present_if_possible();

    Window& m_window;
    Wayland::Shm& m_shm;

    RefPtr<Gfx::Bitmap> m_content_snapshot_bitmap;
    Vector<NonnullOwnPtr<OutputBuffer>> m_submitted_buffers;
    OwnPtr<Wayland::Callback> m_frame_callback;
    bool m_pending_present { false };

};

}
