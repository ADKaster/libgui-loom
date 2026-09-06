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

#include "SeatDelegate.h"

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
    m_toplevel->on_configure = [this](i32 width, i32 height, Span<u32> states) {
        if (width > 0 && height > 0)
            m_pending_toplevel_size = Gfx::IntSize { width, height };

        bool active = false;
        for (auto state : states) {
            // FIXME: handle other states
            if (state == XDG_TOPLEVEL_STATE_ACTIVATED) {
                active = true;
            }
        }
        this->set_active(active);
    };

    m_toplevel->surface().on_configure = [this](u32 serial) {
        if (m_pending_toplevel_size.has_value()) {
            auto const frame_size = m_pending_toplevel_size.value();
            auto const current_frame_rect = m_frame.frame_rect();
            auto const content_size = Gfx::IntSize {
                frame_size.width() - (current_frame_rect.width() - m_content_rect.width()),
                frame_size.height() - (current_frame_rect.height() - m_content_rect.height())
            };
            if (!content_size.is_empty() && content_size != m_content_rect.size()) {
                m_content_rect.set_size(content_size);
                m_frame.content_rect_changed({});
                m_client.async_window_resized(m_window_id, m_content_rect);
            }
        }
        m_pending_toplevel_size = {};

        m_toplevel->surface().ack_configure(serial);
        m_surface_state = SurfaceState::Configured;
        m_frame.surface_configured({});
    };

    m_toplevel->on_close = [this] {
        m_client.async_window_close_request(m_window_id);
    };

    Application::the().seat_delegate().register_surface_owner(m_toplevel->surface().surface(), *this);
}

Window::~Window()
{
    Application::the().seat_delegate().unregister_surface_owner(m_toplevel->surface().surface());
}

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
    m_frame.invalidate_decorations({});
}

void Window::set_content_rect(Gfx::IntRect rect)
{
    if (m_content_rect == rect)
        return;

    m_content_rect = rect;

    m_frame.content_rect_changed({});

    if (m_surface_state == SurfaceState::AwaitingInitialCommit) {
        // xdg-shell requires this bufferless commit before the first buffer attach.
        m_toplevel->surface().surface().commit();
        m_surface_state = SurfaceState::AwaitingConfigure;
    }
}

void Window::set_content_backing_store(NonnullRefPtr<Gfx::Bitmap> bitmap, i32 serial)
{
    m_last_content_backing_store = move(m_content_backing_store);
    m_last_content_backing_store_serial = m_content_backing_store_serial;

    m_content_backing_store = move(bitmap);
    m_content_backing_store_serial = serial;
}

void Window::swap_content_backing_stores()
{
    swap(m_content_backing_store, m_last_content_backing_store);
    swap(m_content_backing_store_serial, m_last_content_backing_store_serial);
}

void Window::invalidate(Vector<Gfx::IntRect> const& rects)
{
    m_pending_paint_rects.extend(rects);
    request_paint_if_needed();
}

void Window::request_paint_if_needed()
{
    if (m_paint_in_flight || m_pending_paint_rects.is_empty())
        return;

    m_paint_in_flight = true;
    m_client.async_paint(m_window_id, m_content_rect.size(), move(m_pending_paint_rects));
}

void Window::did_finish_painting(Vector<Gfx::IntRect> const& rects)
{
    m_frame.content_paint_finished({}, rects);
    m_paint_in_flight = false;
    request_paint_if_needed();
}

void Window::set_default_icon()
{
    m_icon = default_window_icon();
    m_frame.invalidate_decorations({});
}

void Window::set_icon(NonnullRefPtr<Gfx::Bitmap> icon)
{
    m_icon = move(icon);
    m_frame.invalidate_decorations({});
}

void Window::set_active(bool active)
{
    if (m_flags.active == active)
        return;

    m_flags.active = active;
    if (m_flags.active)
        m_client.async_window_activated(m_window_id);
    else
        m_client.async_window_deactivated(m_window_id);

    m_frame.invalidate_decorations({});
}

void Window::set_modified(bool modified)
{
    if (m_modified == modified)
        return;

    m_modified = modified;
    m_frame.invalidate_decorations({});
}

Wayland::XdgSurface& Window::xdg_surface() const
{
    return m_toplevel->surface();
}

}
