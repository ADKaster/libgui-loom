/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWayland/Compositor.h>
#include <LibWayland/Display.h>
#include <LibWayland/Registry.h>
#include <LibWayland/Surface.h>
#include <LibWayland/XdgSurface.h>
#include <LibWayland/XdgToplevel.h>
#include <LibWayland/XdgWmBase.h>
#include <Loom/Wayland/Application.h>
#include <Loom/Wayland/Window.h>
#include <Loom/Wayland/WindowFrame.h>

namespace Loom {

static Gfx::Bitmap const& default_window_icon()
{
    static RefPtr<Gfx::Bitmap const> s_icon;
    if (!s_icon)
        s_icon = MUST(Gfx::Bitmap::load_from_uri("resource://icons/16x16/window.png"sv));
    return *s_icon;
}

Window::Window(WindowServerConnectionProxy& client, NonnullOwnPtr<Wayland::XdgToplevel> toplevel, Wayland::Shm& shm, WindowServer::WindowType type, WindowServer::WindowMode mode, i32 window_id, i32 process_id, WindowFlags flags)
    : m_client(client)
    , m_frame(*this, shm)
    , m_toplevel(move(toplevel))
    , m_type(type)
    , m_mode(mode)
    , m_window_id(window_id)
    , m_process_id(process_id)
    , m_flags(flags)
    , m_icon(default_window_icon())
{
    m_toplevel->on_close = [this] {
        m_client.async_window_close_request(m_window_id);
    };
}

Window::~Window() = default;

NonnullOwnPtr<Window> Window::create(WindowServerConnectionProxy& client, Wayland::Display& display, WindowServer::WindowType window_type, WindowServer::WindowMode window_mode, i32 window_id, i32 process_id, WindowFlags flags, Window* parent_window)
{
    auto& registry = display.registry();
    auto& shm = registry.shm();

    auto surface = registry.compositor().create_surface();
    auto xdg_surface = registry.wm_base().get_xdg_surface(move(surface));
    auto xdg_toplevel = Wayland::XdgSurface::get_xdg_toplevel(move(xdg_surface));

    xdg_toplevel->set_app_id(Application::the().app_id());

    (void)parent_window;

    return adopt_own(*new Window(client, move(xdg_toplevel), shm, window_type, window_mode, window_id, process_id, flags));
}

void Window::set_title(ByteString const& title)
{
    if (title == m_title)
        return;

    m_title = title;
    m_toplevel->set_title(title);
}

void Window::set_content_rect(Gfx::IntRect rect)
{
    if (m_content_rect == rect)
        return;

    m_content_rect = rect;

    // FIXME: When, exactly, am I supposed to do the initial commit? and why?
    m_toplevel->surface().surface().commit();

    // TODO: Do something to lay out frame
}

void Window::set_content(NonnullRefPtr<Gfx::Bitmap> bitmap)
{
    m_content_bitmap = move(bitmap);

    m_frame.window_content_changed({});
}

void Window::set_default_icon()
{
    m_icon = default_window_icon();
}

Wayland::XdgSurface& Window::xdg_surface() const
{
    return m_toplevel->surface();
}

}
