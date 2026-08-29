/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Platform.h>
#include <Loom/Wayland/Application.h>
#include <Loom/Wayland/Window.h>
#include <Loom/Wayland/Display.h>
#include <Loom/Wayland/Protocol/Buffer.h>
#include <Loom/Wayland/Protocol/Registry.h>
#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Shm.h>
#include <Loom/Wayland/Protocol/ShmPool.h>
#include <Loom/Wayland/Protocol/Surface.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>
#include <Loom/Wayland/Protocol/XdgToplevel.h>

#if defined(AK_OS_LINUX)
#    include <sys/syscall.h>
#    include <linux/kcmp.h>
#endif

namespace Loom::Wayland {

Window::Window(WindowServerConnectionProxy& client, NonnullOwnPtr<Protocol::XdgToplevel> toplevel, Protocol::Shm& shm, WindowServer::WindowType type, WindowServer::WindowMode mode, i32 window_id, i32 process_id, WindowFlags flags)
    : m_client(client)
    , m_toplevel(move(toplevel))
    , m_shm(shm)
    , m_type(type)
    , m_mode(mode)
    , m_window_id(window_id)
    , m_process_id(process_id)
    , m_flags(flags)
{
    m_toplevel->on_close = [this] {
        m_client.async_window_close_request(m_window_id);
    };
}

Window::~Window() = default;

NonnullOwnPtr<Window> Window::create(WindowServerConnectionProxy& client, Display& display, WindowServer::WindowType window_type, WindowServer::WindowMode window_mode, i32 window_id, i32 process_id, bool minimizable, bool closeable, bool frameless, bool resizable, bool fullscreen, Window* parent_window)
{
    auto& registry = display.registry();
    auto& shm = registry.shm();

    auto surface = registry.compositor().create_surface();
    auto xdg_surface = registry.wm_base().get_xdg_surface(move(surface));
    auto xdg_toplevel = Protocol::XdgSurface::get_xdg_toplevel(move(xdg_surface));

    xdg_toplevel->set_app_id(Application::the().app_id());

    WindowFlags flags {
        .minimizable = minimizable,
        .closeable = closeable,
        .frameless = frameless,
        .resizable = resizable,
        .fullscreen = fullscreen
    };

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

void Window::set_content_buffer(Core::AnonymousBuffer const& buffer, i32 pitch, Gfx::IntSize size, Gfx::BitmapFormat format)
{
    auto buf_fd = buffer.fd();
    auto my_fd = m_content_buffer.fd();

    if (buf_fd == my_fd)
        return;

#if defined(AK_OS_LINUX)
    auto const pid = getpid();
    if (syscall(SYS_kcmp, pid, pid, KCMP_FILE, buf_fd, my_fd) == 0) {
        // The two fds refer to the same underlying file description, so we don't need to do anything.
        return;
    }
#endif

    m_content_buffer = buffer;

    // FIXME: Use window frame
    auto shm_pool = m_shm.create_pool(m_content_buffer);
    auto shm_buffer = shm_pool->create_buffer(size, pitch, format);

    // FIXME: xdg surface set_window_geometry
    m_toplevel->surface().surface().attach(move(shm_buffer), m_content_rect.x(), m_content_rect.y());
    m_toplevel->surface().surface().commit();
}

}
