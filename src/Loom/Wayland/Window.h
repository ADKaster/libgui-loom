/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <LibCore/AnonymousBuffer.h>
#include <LibGfx/Rect.h>
#include <Loom/WindowServerConnectionProxy.h>
#include <Loom/Wayland/Protocol/XdgToplevel.h>
#include <WindowServer/WindowMode.h>
#include <WindowServer/WindowType.h>

namespace Loom::Wayland {

namespace Protocol {
class Shm;
}

class Display;

class Window {
    AK_MAKE_NONCOPYABLE(Window);
    AK_MAKE_NONMOVABLE(Window);
public:
    struct WindowFlags {
        bool minimizable : 1 { false };
        bool closeable : 1 { false };
        bool frameless : 1 { false };
        bool resizable : 1 { false };
        bool fullscreen : 1 { false };
        bool forced_shadow : 1 { false };
    };

    static NonnullOwnPtr<Window> create(WindowServerConnectionProxy&, Display&, WindowServer::WindowType, WindowServer::WindowMode, i32 window_id, i32 process_id, WindowFlags, Window* parent_window);

    ~Window();

    void set_title(ByteString const& title);
    void set_content_rect(Gfx::IntRect);
    void set_content_buffer(Core::AnonymousBuffer const&, i32 pitch, Gfx::IntSize, Gfx::BitmapFormat);

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

    [[nodiscard]] bool is_modal() const { return m_mode != WindowServer::WindowMode::Modeless; }
    [[nodiscard]] bool is_passive() const { return m_mode == WindowServer::WindowMode::Passive; }
    [[nodiscard]] bool is_rendering_above() const { return m_mode == WindowServer::WindowMode::RenderAbove; }
    [[nodiscard]] bool is_blocking() const { return m_mode == WindowServer::WindowMode::Blocking; }

    [[nodiscard]] bool is_moveable() const { return m_type == WindowServer::WindowType::Normal; }

    [[nodiscard]] StringView title() const { return m_title; }
    [[nodiscard]] Gfx::IntRect content_rect() const { return m_content_rect; }
    [[nodiscard]] WindowServerConnectionProxy& client() const { return m_client; }

private:
    Window(WindowServerConnectionProxy&, NonnullOwnPtr<Protocol::XdgToplevel>, Protocol::Shm&, WindowServer::WindowType, WindowServer::WindowMode, i32 window_id, i32 process_id, WindowFlags);

    Core::AnonymousBuffer m_content_buffer;

    WindowServerConnectionProxy& m_client;
    NonnullOwnPtr<Protocol::XdgToplevel> m_toplevel;
    Protocol::Shm& m_shm;

    WindowServer::WindowType m_type { WindowServer::WindowType::Invalid };
    WindowServer::WindowMode m_mode { WindowServer::WindowMode::Modeless };
    i32 m_window_id { -1 };
    i32 m_process_id { -1 };

    WindowFlags m_flags;

    ByteString m_title;
    Gfx::IntRect m_content_rect;

    // FIXME: Handle parent/child relationships
};

}
