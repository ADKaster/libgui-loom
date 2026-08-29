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
    static NonnullOwnPtr<Window> create(Display&, WindowServer::WindowType, WindowServer::WindowMode, i32 window_id, i32 process_id, bool minimizable, bool closeable, bool frameless, bool resizable, bool fullscreen, Window* parent_window);

    ~Window();

    void set_title(ByteString const& title);
    void set_content_rect(Gfx::IntRect);
    void set_content_buffer(Core::AnonymousBuffer const&, i32 pitch, Gfx::IntSize, Gfx::BitmapFormat);

    [[nodiscard]] WindowServer::WindowType type() const { return m_type; }
    [[nodiscard]] WindowServer::WindowMode mode() const { return m_mode; }
    [[nodiscard]] i32 window_id() const { return m_window_id; }
    [[nodiscard]] i32 process_id() const { return m_process_id; }

    [[nodiscard]] bool is_minimizable() const { return m_flags.minimizable; }
    [[nodiscard]] bool is_closeable() const { return m_flags.closeable; }
    [[nodiscard]] bool is_frameless() const { return m_flags.frameless; }
    [[nodiscard]] bool is_resizable() const { return m_flags.resizable; }
    [[nodiscard]] bool is_fullscreen() const { return m_flags.fullscreen; }

    [[nodiscard]] StringView title() const { return m_title; }
    [[nodiscard]] Gfx::IntRect content_rect() const { return m_content_rect; }

private:
    struct WindowFlags;

    explicit Window(NonnullOwnPtr<Protocol::XdgToplevel>, Protocol::Shm&, WindowServer::WindowType, WindowServer::WindowMode, i32 window_id, i32 process_id, WindowFlags);

    Core::AnonymousBuffer m_content_buffer;
    NonnullOwnPtr<Protocol::XdgToplevel> m_toplevel;
    Protocol::Shm& m_shm;

    WindowServer::WindowType m_type { WindowServer::WindowType::Invalid };
    WindowServer::WindowMode m_mode { WindowServer::WindowMode::Modeless };
    i32 m_window_id { -1 };
    i32 m_process_id { -1 };

    struct WindowFlags {
        bool minimizable : 1 { false };
        bool closeable : 1 { false };
        bool frameless : 1 { false };
        bool resizable : 1 { false };
        bool fullscreen : 1 { false };
    } m_flags;

    ByteString m_title;
    Gfx::IntRect m_content_rect;

    // FIXME: Handle parent/child relationships
};

}
