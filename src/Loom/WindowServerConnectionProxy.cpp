/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowServerConnectionProxy.h"

namespace Loom {

HashMap<int, NonnullRefPtr<WindowServerConnectionProxy>>* s_connections;

WindowServerConnectionProxy::WindowServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket> socket, int client_id)
    : IPC::ConnectionFromClient<WindowClientEndpoint, WindowServerEndpoint>(*this, move(socket), client_id)
{
    if (!s_connections)
        s_connections = new HashMap<int, NonnullRefPtr<WindowServerConnectionProxy>>;
    s_connections->set(client_id, *this);

    // FIXME: async_fast_greet()
}

WindowServerConnectionProxy::~WindowServerConnectionProxy() = default;

void WindowServerConnectionProxy::die()
{
}

void WindowServerConnectionProxy::may_have_become_unresponsive()
{
}

void WindowServerConnectionProxy::did_become_responsive()
{
}

void WindowServerConnectionProxy::create_menu(i32, String const&, i32)
{
}

void WindowServerConnectionProxy::set_menu_name(i32, String const&)
{
}

void WindowServerConnectionProxy::set_menu_minimum_width(i32, i32)
{
}

void WindowServerConnectionProxy::destroy_menu(i32)
{
}

void WindowServerConnectionProxy::add_menu(i32, i32)
{
}

void WindowServerConnectionProxy::add_menu_item(i32, i32, i32, ByteString const&, bool, bool, bool, bool, bool, ByteString const&, Gfx::ShareableBitmap const&, bool)
{
}

void WindowServerConnectionProxy::add_menu_separator(i32)
{
}

void WindowServerConnectionProxy::update_menu_item(i32, i32, i32, ByteString const&, bool, bool, bool, bool, bool, ByteString const&, Gfx::ShareableBitmap const&)
{
}

void WindowServerConnectionProxy::remove_menu_item(i32, i32)
{
}

void WindowServerConnectionProxy::flash_menubar_menu(i32, i32)
{
}

void WindowServerConnectionProxy::create_window(i32, i32, Gfx::IntRect const&, bool, bool, bool, bool, bool, bool, bool, bool, float, Gfx::IntSize, Gfx::IntSize, Gfx::IntSize, Optional<Gfx::IntSize> const&, i32, i32, ByteString const&, i32, Gfx::IntRect const&)
{
}

Messages::WindowServer::DestroyWindowResponse WindowServerConnectionProxy::destroy_window(i32)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_window_title(i32, ByteString const&)
{
}

Messages::WindowServer::GetWindowTitleResponse WindowServerConnectionProxy::get_window_title(i32)
{
    return nullptr;
}

Messages::WindowServer::IsMaximizedResponse WindowServerConnectionProxy::is_maximized(i32)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_maximized(i32, bool)
{
}

Messages::WindowServer::IsMinimizedResponse WindowServerConnectionProxy::is_minimized(i32)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_minimized(i32, bool)
{
}

void WindowServerConnectionProxy::start_window_resize(i32, i32)
{
}

Messages::WindowServer::SetWindowRectResponse WindowServerConnectionProxy::set_window_rect(i32, Gfx::IntRect const&)
{
    return nullptr;
}

Messages::WindowServer::GetWindowRectResponse WindowServerConnectionProxy::get_window_rect(i32)
{
    return nullptr;
}

Messages::WindowServer::GetWindowFloatingRectResponse WindowServerConnectionProxy::get_window_floating_rect(i32)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_window_minimum_size(i32, Gfx::IntSize)
{
}

Messages::WindowServer::GetWindowMinimumSizeResponse WindowServerConnectionProxy::get_window_minimum_size(i32)
{
    return nullptr;
}

Messages::WindowServer::GetAppletRectOnScreenResponse WindowServerConnectionProxy::get_applet_rect_on_screen(i32)
{
    return nullptr;
}

void WindowServerConnectionProxy::invalidate_rect(i32, Vector<Gfx::IntRect> const&, bool)
{
}

void WindowServerConnectionProxy::did_finish_painting(i32, Vector<Gfx::IntRect> const&)
{
}

void WindowServerConnectionProxy::set_global_mouse_tracking(bool)
{
}

void WindowServerConnectionProxy::set_window_backing_store(i32, i32, i32, IPC::File const&, i32, bool, Gfx::IntSize, Gfx::IntSize, bool)
{
}

void WindowServerConnectionProxy::set_window_has_alpha_channel(i32, bool)
{
}

void WindowServerConnectionProxy::set_window_alpha_hit_threshold(i32, float)
{
}

void WindowServerConnectionProxy::move_window_to_front(i32)
{
}

void WindowServerConnectionProxy::set_fullscreen(i32, bool)
{
}

void WindowServerConnectionProxy::set_frameless(i32, bool)
{
}

void WindowServerConnectionProxy::set_forced_shadow(i32, bool)
{
}

Messages::WindowServer::SetWallpaperResponse WindowServerConnectionProxy::set_wallpaper(Gfx::ShareableBitmap const&)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_background_color(ByteString const&)
{
}

void WindowServerConnectionProxy::set_wallpaper_mode(ByteString const&)
{
}

Messages::WindowServer::GetWallpaperResponse WindowServerConnectionProxy::get_wallpaper()
{
    return nullptr;
}

Messages::WindowServer::SetScreenLayoutResponse WindowServerConnectionProxy::set_screen_layout(WindowServer::ScreenLayout const&, bool)
{
    return nullptr;
}

Messages::WindowServer::GetScreenLayoutResponse WindowServerConnectionProxy::get_screen_layout()
{
    return nullptr;
}

Messages::WindowServer::SaveScreenLayoutResponse WindowServerConnectionProxy::save_screen_layout()
{
    return nullptr;
}

Messages::WindowServer::ApplyWorkspaceSettingsResponse WindowServerConnectionProxy::apply_workspace_settings(u32, u32, bool)
{
    return nullptr;
}

Messages::WindowServer::GetWorkspaceSettingsResponse WindowServerConnectionProxy::get_workspace_settings()
{
    return nullptr;
}

void WindowServerConnectionProxy::show_screen_numbers(bool)
{
}

void WindowServerConnectionProxy::set_window_cursor(i32, i32)
{
}

void WindowServerConnectionProxy::set_window_custom_cursor(i32, Gfx::ShareableBitmap const&)
{
}

void WindowServerConnectionProxy::popup_menu(i32, Gfx::IntPoint, Gfx::IntRect const&)
{
}

void WindowServerConnectionProxy::dismiss_menu(i32)
{
}

void WindowServerConnectionProxy::set_window_icon_bitmap(i32, Gfx::ShareableBitmap const&)
{
}

Messages::WindowServer::StartDragResponse WindowServerConnectionProxy::start_drag(ByteString const&, HashMap<String, ByteBuffer> const&, Gfx::ShareableBitmap const&)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_accepts_drag(bool)
{
}

Messages::WindowServer::SetSystemThemeResponse WindowServerConnectionProxy::set_system_theme(ByteString const&, ByteString const&, bool, Optional<ByteString> const&)
{
    return nullptr;
}

Messages::WindowServer::GetSystemThemeResponse WindowServerConnectionProxy::get_system_theme()
{
    return nullptr;
}

Messages::WindowServer::SetSystemThemeOverrideResponse WindowServerConnectionProxy::set_system_theme_override(Core::AnonymousBuffer const&)
{
    return nullptr;
}

Messages::WindowServer::GetSystemThemeOverrideResponse WindowServerConnectionProxy::get_system_theme_override()
{
    return nullptr;
}

void WindowServerConnectionProxy::clear_system_theme_override()
{
}

Messages::WindowServer::IsSystemThemeOverriddenResponse WindowServerConnectionProxy::is_system_theme_overridden()
{
    return nullptr;
}

Messages::WindowServer::GetPreferredColorSchemeResponse WindowServerConnectionProxy::get_preferred_color_scheme()
{
    return nullptr;
}

void WindowServerConnectionProxy::apply_cursor_theme(ByteString const&)
{
}

void WindowServerConnectionProxy::set_cursor_highlight_radius(int)
{
}

Messages::WindowServer::GetCursorHighlightRadiusResponse WindowServerConnectionProxy::get_cursor_highlight_radius()
{
    return nullptr;
}

void WindowServerConnectionProxy::set_cursor_highlight_color(Gfx::Color)
{
}

Messages::WindowServer::GetCursorHighlightColorResponse WindowServerConnectionProxy::get_cursor_highlight_color()
{
    return nullptr;
}

Messages::WindowServer::GetCursorThemeResponse WindowServerConnectionProxy::get_cursor_theme()
{
    return nullptr;
}

Messages::WindowServer::SetSystemFontsResponse WindowServerConnectionProxy::set_system_fonts(ByteString const&, ByteString const&, ByteString const&)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_system_effects(Vector<bool> const&, u8, u8)
{
}

void WindowServerConnectionProxy::set_window_base_size_and_size_increment(i32, Gfx::IntSize, Gfx::IntSize)
{
}

void WindowServerConnectionProxy::set_window_resize_aspect_ratio(i32, Optional<Gfx::IntSize> const&)
{
}

void WindowServerConnectionProxy::enable_display_link()
{
}

void WindowServerConnectionProxy::disable_display_link()
{
}

void WindowServerConnectionProxy::set_window_progress(i32, Optional<i32> const&)
{
}

void WindowServerConnectionProxy::refresh_system_theme()
{
}

void WindowServerConnectionProxy::pong()
{
}

void WindowServerConnectionProxy::set_global_cursor_position(Gfx::IntPoint)
{
}

Messages::WindowServer::GetGlobalCursorPositionResponse WindowServerConnectionProxy::get_global_cursor_position()
{
    return nullptr;
}

void WindowServerConnectionProxy::set_mouse_acceleration(float)
{
}

Messages::WindowServer::GetMouseAccelerationResponse WindowServerConnectionProxy::get_mouse_acceleration()
{
    return nullptr;
}

void WindowServerConnectionProxy::set_scroll_step_size(u32)
{
}

Messages::WindowServer::GetScrollStepSizeResponse WindowServerConnectionProxy::get_scroll_step_size()
{
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapResponse WindowServerConnectionProxy::get_screen_bitmap(Optional<Gfx::IntRect> const&, Optional<u32> const&)
{
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapAroundCursorResponse WindowServerConnectionProxy::get_screen_bitmap_around_cursor(Gfx::IntSize)
{
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapAroundLocationResponse WindowServerConnectionProxy::get_screen_bitmap_around_location(Gfx::IntSize, Gfx::IntPoint)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_double_click_speed(i32)
{
}

Messages::WindowServer::GetDoubleClickSpeedResponse WindowServerConnectionProxy::get_double_click_speed()
{
    return nullptr;
}

void WindowServerConnectionProxy::set_mouse_buttons_switched(bool)
{
}

Messages::WindowServer::AreMouseButtonsSwitchedResponse WindowServerConnectionProxy::are_mouse_buttons_switched()
{
    return nullptr;
}

void WindowServerConnectionProxy::set_natural_scroll(bool)
{
}

Messages::WindowServer::IsNaturalScrollResponse WindowServerConnectionProxy::is_natural_scroll()
{
    return nullptr;
}

void WindowServerConnectionProxy::set_window_modified(i32, bool)
{
}

Messages::WindowServer::IsWindowModifiedResponse WindowServerConnectionProxy::is_window_modified(i32)
{
    return nullptr;
}

Messages::WindowServer::GetDesktopDisplayScaleResponse WindowServerConnectionProxy::get_desktop_display_scale(u32)
{
    return nullptr;
}

void WindowServerConnectionProxy::set_flash_flush(bool)
{
}

void WindowServerConnectionProxy::set_window_parent_from_client(i32, i32, i32)
{
}

Messages::WindowServer::GetWindowRectFromClientResponse WindowServerConnectionProxy::get_window_rect_from_client(i32, i32)
{
    return nullptr;
}

void WindowServerConnectionProxy::add_window_stealing_for_client(i32, i32)
{
}

void WindowServerConnectionProxy::remove_window_stealing_for_client(i32, i32)
{
}

void WindowServerConnectionProxy::remove_window_stealing(i32)
{
}

void WindowServerConnectionProxy::set_always_on_top(i32, bool)
{
}

Messages::WindowServer::GetColorUnderCursorResponse WindowServerConnectionProxy::get_color_under_cursor()
{
    return nullptr;
}

}
