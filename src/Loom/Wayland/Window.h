/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <LibGfx/Rect.h>
#include <LibWayland/Forward.h>
#include <Loom/Wayland/WindowFrame.h>
#include <Loom/WindowServerConnectionProxy.h>
#include <WindowServer/WindowMode.h>
#include <WindowServer/WindowType.h>

namespace Loom {

class Cursor;

class Window {
    AK_MAKE_NONCOPYABLE(Window);
    AK_MAKE_NONMOVABLE(Window);
private:
    enum class SurfaceState {
        AwaitingInitialCommit,
        AwaitingConfigure,
        Configured,
    };

public:
    struct WindowFlags {
        bool minimizable : 1 { false };
        bool closeable : 1 { false };
        bool frameless : 1 { false };
        bool resizable : 1 { false };
        bool fullscreen : 1 { false };
        bool forced_shadow : 1 { false };
        bool active : 1 { false };
    };

    static NonnullOwnPtr<Window> create(WindowServerConnectionProxy&, Wayland::Display&, WindowServer::WindowType, WindowServer::WindowMode, i32 window_id, i32 process_id, WindowFlags, Window* parent_window);

    ~Window();

    void set_title(ByteString const& title);
    void set_content_rect(Gfx::IntRect);
    void invalidate(Vector<Gfx::IntRect> const&);
    void did_finish_painting(Vector<Gfx::IntRect> const&);

    [[nodiscard]] WindowServer::WindowType type() const { return m_type; }
    [[nodiscard]] WindowServer::WindowMode mode() const { return m_mode; }
    [[nodiscard]] i32 window_id() const { return m_window_id; }
    [[nodiscard]] i32 process_id() const { return m_process_id; }

    [[nodiscard]] bool is_minimizable() const { return m_type == WindowServer::WindowType::Normal && m_flags.minimizable; }
    [[nodiscard]] bool is_closeable() const { return m_flags.closeable; }
    [[nodiscard]] bool is_frameless() const { return m_flags.frameless; }
    [[nodiscard]] bool is_resizable() const { return m_type != WindowServer::WindowType::Popup && m_flags.resizable && !m_flags.fullscreen; }
    [[nodiscard]] bool is_fullscreen() const { return m_flags.fullscreen; }
    [[nodiscard]] bool has_forced_shadow() const { return m_flags.forced_shadow; }
    [[nodiscard]] bool is_active() const { return m_flags.active; }
    [[nodiscard]] bool is_modified() const { return m_modified; }

    void set_active(bool);
    void set_modified(bool);

    [[nodiscard]] bool is_modal() const { return m_mode != WindowServer::WindowMode::Modeless; }
    [[nodiscard]] bool is_passive() const { return m_mode == WindowServer::WindowMode::Passive; }
    [[nodiscard]] bool is_rendering_above() const { return m_mode == WindowServer::WindowMode::RenderAbove; }
    [[nodiscard]] bool is_blocking() const { return m_mode == WindowServer::WindowMode::Blocking; }

    [[nodiscard]] bool is_moveable() const { return m_type == WindowServer::WindowType::Normal; }

    [[nodiscard]] StringView title() const { return m_title; }
    [[nodiscard]] Gfx::Bitmap const& icon() const { return *m_icon; }
    void set_icon(NonnullRefPtr<Gfx::Bitmap>);
    void set_default_icon();

    [[nodiscard]] RefPtr<Cursor const> cursor() const;
    void set_cursor(RefPtr<Cursor const>);

    [[nodiscard]] Gfx::IntRect content_rect() const { return m_content_rect; }
    [[nodiscard]] RefPtr<Gfx::Bitmap> content() const { return m_content_backing_store; }
    [[nodiscard]] RefPtr<Gfx::Bitmap> last_content_backing_store() const { return m_last_content_backing_store; }
    [[nodiscard]] i32 last_content_backing_store_serial() const { return m_last_content_backing_store_serial; }
    [[nodiscard]] bool is_configured() const { return m_surface_state == SurfaceState::Configured; }

    void swap_content_backing_stores();
    void set_content_backing_store(NonnullRefPtr<Gfx::Bitmap>, i32 serial);

    [[nodiscard]] Wayland::XdgSurface& xdg_surface() const;
    [[nodiscard]] WindowServerConnectionProxy& client() const { return m_client; }
    [[nodiscard]] WindowFrame& frame() { return m_frame; }

private:
    Window(WindowServerConnectionProxy&, NonnullOwnPtr<Wayland::XdgToplevel>, Wayland::Shm&, WindowServer::WindowType, WindowServer::WindowMode, i32 window_id, i32 process_id, WindowFlags);

    void request_paint_if_needed();

    WindowServerConnectionProxy& m_client;
    WindowFrame m_frame;
    NonnullOwnPtr<Wayland::XdgToplevel> m_toplevel;

    WindowServer::WindowType m_type { WindowServer::WindowType::Invalid };
    WindowServer::WindowMode m_mode { WindowServer::WindowMode::Modeless };
    i32 m_window_id { -1 };
    i32 m_process_id { -1 };

    WindowFlags m_flags;
    bool m_modified { false };

    ByteString m_title;
    NonnullRefPtr<Gfx::Bitmap const> m_icon;
    Gfx::IntRect m_content_rect;
    Optional<Gfx::IntSize> m_pending_toplevel_size;
    Vector<Gfx::IntRect> m_pending_paint_rects;
    SurfaceState m_surface_state { SurfaceState::AwaitingInitialCommit };
    bool m_paint_in_flight { false };

    RefPtr<Gfx::Bitmap> m_content_backing_store;
    RefPtr<Gfx::Bitmap> m_last_content_backing_store;

    i32 m_content_backing_store_serial { -1 };
    i32 m_last_content_backing_store_serial { -1 };

    // FIXME: Handle parent/child relationships

    RefPtr<Cursor const> m_cursor;
};

}
