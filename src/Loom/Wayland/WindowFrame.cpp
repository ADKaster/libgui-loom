/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/Application.h>
#include <Loom/Wayland/Button.h>
#include <Loom/Wayland/Window.h>
#include <Loom/Wayland/WindowFrame.h>
#include <LibGfx/WindowTheme.h>
#include <WindowServer/WindowType.h>
#include <WindowServer/WindowMode.h>

namespace Loom::Wayland {

static Button::Icon s_minimize_icon;
static Button::Icon s_maximize_icon;
static Button::Icon s_restore_icon;
static Button::Icon s_close_icon;
static Button::Icon s_close_modified_icon;

static RefPtr<Gfx::Bitmap> s_active_window_shadow;
static RefPtr<Gfx::Bitmap> s_inactive_window_shadow;
static RefPtr<Gfx::Bitmap> s_menu_shadow;
static RefPtr<Gfx::Bitmap> s_taskbar_shadow;
static RefPtr<Gfx::Bitmap> s_tooltip_shadow;

static Gfx::WindowTheme& current_window_theme()
{
    auto const& app = Application::the();
    return app.palette().window_theme();
}

static ByteString to_resource_path(StringView path)
{
    if (path.starts_with("/res/"sv))
        return path.replace("/res/"sv, "resource://"sv, ReplaceMode::FirstOnly);
    return path;
}

static RefPtr<Gfx::Bitmap> load_bitmap(StringView path, StringView name, StringView default_path)
{
    StringBuilder full_path;
    full_path.append(path);
    full_path.append(name);

    auto bitmap_or_error = Gfx::Bitmap::load_from_uri(full_path.string_view());
    if (!bitmap_or_error.is_error())
        return bitmap_or_error.release_value();

    if (!default_path.is_empty()) {
        auto default_bitmap_or_error = Gfx::Bitmap::load_from_uri(default_path);
        if (!default_bitmap_or_error.is_error())
            return default_bitmap_or_error.release_value();
    }

    dbgln("Failed to load bitmap from {} or default path {}", full_path.string_view(), default_path);
    return {};
}

static void load_icon(Button::Icon& icon, StringView icons_path, StringView name, StringView default_path = ""sv)
{
    icon.bitmap = load_bitmap(icons_path, ByteString::formatted("{}.png", name), default_path);
    icon.hover_bitmap = load_bitmap(icons_path, ByteString::formatted("{}-hover.png", name), ""sv);
}

static RefPtr<Gfx::Bitmap> load_shadow(StringView path)
{
    if (path.is_empty())
        return nullptr;

    auto bitmap_or_error = Gfx::Bitmap::load_from_uri(path);
    if (!bitmap_or_error.is_error())
        return bitmap_or_error.release_value();

    return nullptr;
}

static Gfx::WindowTheme::WindowType to_theme_window_type(WindowServer::WindowType type)
{
    switch (type) {
    case WindowServer::WindowType::Normal:
        return Gfx::WindowTheme::WindowType::Normal;
    case WindowServer::WindowType::Notification:
        return Gfx::WindowTheme::WindowType::Notification;
    default:
        return Gfx::WindowTheme::WindowType::Other;
    }
}

static Gfx::WindowTheme::WindowMode to_theme_window_mode(WindowServer::WindowMode mode)
{
    switch (mode) {
    case WindowServer::WindowMode::RenderAbove:
        return Gfx::WindowTheme::WindowMode::RenderAbove;
    default:
        return Gfx::WindowTheme::WindowMode::Other;
    }
}

void WindowFrame::load_theme_config()
{
    ByteString const icons_path = to_resource_path(Application::the().palette().title_button_icons_path());

    // FIXME: Handle bitmaps at different scale factors
    load_icon(s_minimize_icon, icons_path, "window-minimize"sv, "resource://icons/16x16/downward-triangle.png"sv);
    load_icon(s_maximize_icon, icons_path, "window-maximize"sv, "resource://icons/16x16/upward-triangle.png"sv);
    load_icon(s_restore_icon, icons_path, "window-restore"sv, "resource://icons/16x16/window-restore.png"sv);
    load_icon(s_close_icon, icons_path, "window-close"sv, "resource://icons/16x16/window-close.png"sv);
    load_icon(s_close_modified_icon, icons_path, "window-close-modified"sv, "resource://icons/16x16/window-close-modified.png"sv);

    auto const palette = Application::the().palette();

    s_active_window_shadow = load_shadow(to_resource_path(palette.active_window_shadow_path()));
    s_inactive_window_shadow = load_shadow(to_resource_path(palette.inactive_window_shadow_path()));
    s_menu_shadow = load_shadow(to_resource_path(palette.menu_shadow_path()));
    s_taskbar_shadow = load_shadow(to_resource_path(palette.taskbar_shadow_path()));
    s_tooltip_shadow = load_shadow(to_resource_path(palette.tooltip_shadow_path()));
}

WindowFrame::WindowFrame(Window& window)
    : m_window(window)
{
}

WindowFrame::~WindowFrame() = default;

Gfx::IntRect WindowFrame::menubar_rect() const
{
    return {};
}

Gfx::IntRect WindowFrame::titlebar_rect() const
{
    return current_window_theme().titlebar_rect(to_theme_window_type(m_window.type()), to_theme_window_mode(m_window.mode()), m_window.content_rect(), Application::the().palette());
}

Gfx::IntRect WindowFrame::titlebar_icon_rect() const
{
    return current_window_theme().titlebar_icon_rect(to_theme_window_type(m_window.type()), to_theme_window_mode(m_window.mode()), m_window.content_rect(), Application::the().palette());
}

Gfx::IntRect WindowFrame::titlebar_text_rect() const
{
    return current_window_theme().titlebar_text_rect(to_theme_window_type(m_window.type()), to_theme_window_mode(m_window.mode()), m_window.content_rect(), Application::the().palette());
}

Gfx::IntRect WindowFrame::frame_rect() const
{
    if (m_window.is_frameless())
        return m_window.content_rect();

    // FIXME: Account for menus
    auto const menu_row_count = 0;
    return current_window_theme().frame_rect_for_window(to_theme_window_type(m_window.type()), to_theme_window_mode(m_window.mode()), m_window.content_rect(), Application::the().palette(), menu_row_count);
}

Gfx::IntRect WindowFrame::inflated_for_shadow(Gfx::IntRect const& frame_rect) const
{
    // FIXME: Actually paint the shadow
    return frame_rect;
}

}
