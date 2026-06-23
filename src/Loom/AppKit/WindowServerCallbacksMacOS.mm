/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowServerCallbacksMacOS.h"

namespace Loom {

void WindowServerCallbacksMacOS::create_menu(i32, String const&, i32)
{
}

void WindowServerCallbacksMacOS::set_menu_name(i32, String const&)
{
}

void WindowServerCallbacksMacOS::set_menu_minimum_width(i32, i32)
{
}

void WindowServerCallbacksMacOS::destroy_menu(i32)
{
}

void WindowServerCallbacksMacOS::add_menu(i32, i32)
{
}

void WindowServerCallbacksMacOS::add_menu_item(i32, i32, i32, ByteString const&, bool, bool, bool, bool, bool, ByteString const&, Gfx::ShareableBitmap const&, bool)
{
}

void WindowServerCallbacksMacOS::add_menu_separator(i32)
{
}

void WindowServerCallbacksMacOS::update_menu_item(i32, i32, i32, ByteString const&, bool, bool, bool, bool, bool, ByteString const&, Gfx::ShareableBitmap const&)
{
}

void WindowServerCallbacksMacOS::remove_menu_item(i32, i32)
{
}

void WindowServerCallbacksMacOS::flash_menubar_menu(i32, i32)
{
}

void WindowServerCallbacksMacOS::create_window(i32 window_id, i32 process_id, Gfx::IntRect const& rect, bool auto_position, bool has_alpha_channel, bool minimizable, bool closeable, bool resizable, bool fullscreen, bool frameless, bool forced_shadow, float alpha_hit_threshold, Gfx::IntSize base_size, Gfx::IntSize size_increment, Gfx::IntSize minimum_size, Optional<Gfx::IntSize> const& resize_aspect_ratio, i32 type, i32 mode, ByteString const& title, i32 parent_window_id, Gfx::IntRect const& launch_origin_rect)
{
}

Messages::WindowServer::DestroyWindowResponse WindowServerCallbacksMacOS::destroy_window(i32)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_window_title(i32, ByteString const&)
{
}

Messages::WindowServer::GetWindowTitleResponse WindowServerCallbacksMacOS::get_window_title(i32)
{
    return nullptr;
}

Messages::WindowServer::IsMaximizedResponse WindowServerCallbacksMacOS::is_maximized(i32)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_maximized(i32, bool)
{
}

Messages::WindowServer::IsMinimizedResponse WindowServerCallbacksMacOS::is_minimized(i32)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_minimized(i32, bool)
{
}

void WindowServerCallbacksMacOS::start_window_resize(i32, i32)
{
}

Messages::WindowServer::SetWindowRectResponse WindowServerCallbacksMacOS::set_window_rect(i32, Gfx::IntRect const&)
{
    return nullptr;
}

Messages::WindowServer::GetWindowRectResponse WindowServerCallbacksMacOS::get_window_rect(i32)
{
    return nullptr;
}

Messages::WindowServer::GetWindowFloatingRectResponse WindowServerCallbacksMacOS::get_window_floating_rect(i32)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_window_minimum_size(i32, Gfx::IntSize)
{
}

Messages::WindowServer::GetWindowMinimumSizeResponse WindowServerCallbacksMacOS::get_window_minimum_size(i32)
{
    return nullptr;
}

Messages::WindowServer::GetAppletRectOnScreenResponse WindowServerCallbacksMacOS::get_applet_rect_on_screen(i32)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::invalidate_rect(i32, Vector<Gfx::IntRect> const&, bool)
{
}

void WindowServerCallbacksMacOS::did_finish_painting(i32, Vector<Gfx::IntRect> const&)
{
}

void WindowServerCallbacksMacOS::set_global_mouse_tracking(bool)
{
}

void WindowServerCallbacksMacOS::set_window_backing_store(i32, i32, i32, IPC::File const&, i32, bool, Gfx::IntSize, Gfx::IntSize, bool)
{
}

void WindowServerCallbacksMacOS::set_window_has_alpha_channel(i32, bool)
{
}

void WindowServerCallbacksMacOS::set_window_alpha_hit_threshold(i32, float)
{
}

void WindowServerCallbacksMacOS::move_window_to_front(i32)
{
}

void WindowServerCallbacksMacOS::set_fullscreen(i32, bool)
{
}

void WindowServerCallbacksMacOS::set_frameless(i32, bool)
{
}

void WindowServerCallbacksMacOS::set_forced_shadow(i32, bool)
{
}

Messages::WindowServer::SetWallpaperResponse WindowServerCallbacksMacOS::set_wallpaper(Gfx::ShareableBitmap const&)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_background_color(ByteString const&)
{
}

void WindowServerCallbacksMacOS::set_wallpaper_mode(ByteString const&)
{
}

Messages::WindowServer::GetWallpaperResponse WindowServerCallbacksMacOS::get_wallpaper()
{
    return nullptr;
}

Messages::WindowServer::SetScreenLayoutResponse WindowServerCallbacksMacOS::set_screen_layout(WindowServer::ScreenLayout const&, bool)
{
    return nullptr;
}

Messages::WindowServer::GetScreenLayoutResponse WindowServerCallbacksMacOS::get_screen_layout()
{
    return nullptr;
}

Messages::WindowServer::SaveScreenLayoutResponse WindowServerCallbacksMacOS::save_screen_layout()
{
    return nullptr;
}

Messages::WindowServer::ApplyWorkspaceSettingsResponse WindowServerCallbacksMacOS::apply_workspace_settings(u32, u32, bool)
{
    return nullptr;
}

Messages::WindowServer::GetWorkspaceSettingsResponse WindowServerCallbacksMacOS::get_workspace_settings()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::show_screen_numbers(bool)
{
}

void WindowServerCallbacksMacOS::set_window_cursor(i32, i32)
{
}

void WindowServerCallbacksMacOS::set_window_custom_cursor(i32, Gfx::ShareableBitmap const&)
{
}

void WindowServerCallbacksMacOS::popup_menu(i32, Gfx::IntPoint, Gfx::IntRect const&)
{
}

void WindowServerCallbacksMacOS::dismiss_menu(i32)
{
}

void WindowServerCallbacksMacOS::set_window_icon_bitmap(i32, Gfx::ShareableBitmap const&)
{
}

Messages::WindowServer::StartDragResponse WindowServerCallbacksMacOS::start_drag(ByteString const&, HashMap<String, ByteBuffer> const&, Gfx::ShareableBitmap const&)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_accepts_drag(bool)
{
}

Messages::WindowServer::SetSystemThemeResponse WindowServerCallbacksMacOS::set_system_theme(ByteString const&, ByteString const&, bool, Optional<ByteString> const&)
{
    return nullptr;
}

Messages::WindowServer::GetSystemThemeResponse WindowServerCallbacksMacOS::get_system_theme()
{
    return nullptr;
}

Messages::WindowServer::SetSystemThemeOverrideResponse WindowServerCallbacksMacOS::set_system_theme_override(Core::AnonymousBuffer const&)
{
    return nullptr;
}

Messages::WindowServer::GetSystemThemeOverrideResponse WindowServerCallbacksMacOS::get_system_theme_override()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::clear_system_theme_override()
{
}

Messages::WindowServer::IsSystemThemeOverriddenResponse WindowServerCallbacksMacOS::is_system_theme_overridden()
{
    return nullptr;
}

Messages::WindowServer::GetPreferredColorSchemeResponse WindowServerCallbacksMacOS::get_preferred_color_scheme()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::apply_cursor_theme(ByteString const&)
{
}

void WindowServerCallbacksMacOS::set_cursor_highlight_radius(int)
{
}

Messages::WindowServer::GetCursorHighlightRadiusResponse WindowServerCallbacksMacOS::get_cursor_highlight_radius()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_cursor_highlight_color(Gfx::Color)
{
}

Messages::WindowServer::GetCursorHighlightColorResponse WindowServerCallbacksMacOS::get_cursor_highlight_color()
{
    return nullptr;
}

Messages::WindowServer::GetCursorThemeResponse WindowServerCallbacksMacOS::get_cursor_theme()
{
    return nullptr;
}

Messages::WindowServer::SetSystemFontsResponse WindowServerCallbacksMacOS::set_system_fonts(ByteString const&, ByteString const&, ByteString const&)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_system_effects(Vector<bool> const&, u8, u8)
{
}

void WindowServerCallbacksMacOS::set_window_base_size_and_size_increment(i32, Gfx::IntSize, Gfx::IntSize)
{
}

void WindowServerCallbacksMacOS::set_window_resize_aspect_ratio(i32, Optional<Gfx::IntSize> const&)
{
}

void WindowServerCallbacksMacOS::enable_display_link()
{
}

void WindowServerCallbacksMacOS::disable_display_link()
{
}

void WindowServerCallbacksMacOS::set_window_progress(i32, Optional<i32> const&)
{
}

void WindowServerCallbacksMacOS::refresh_system_theme()
{
}

void WindowServerCallbacksMacOS::pong()
{
}

void WindowServerCallbacksMacOS::set_global_cursor_position(Gfx::IntPoint)
{
}

Messages::WindowServer::GetGlobalCursorPositionResponse WindowServerCallbacksMacOS::get_global_cursor_position()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_mouse_acceleration(float)
{
}

Messages::WindowServer::GetMouseAccelerationResponse WindowServerCallbacksMacOS::get_mouse_acceleration()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_scroll_step_size(u32)
{
}

Messages::WindowServer::GetScrollStepSizeResponse WindowServerCallbacksMacOS::get_scroll_step_size()
{
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapResponse WindowServerCallbacksMacOS::get_screen_bitmap(Optional<Gfx::IntRect> const&, Optional<u32> const&)
{
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapAroundCursorResponse WindowServerCallbacksMacOS::get_screen_bitmap_around_cursor(Gfx::IntSize)
{
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapAroundLocationResponse WindowServerCallbacksMacOS::get_screen_bitmap_around_location(Gfx::IntSize, Gfx::IntPoint)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_double_click_speed(i32)
{
}

Messages::WindowServer::GetDoubleClickSpeedResponse WindowServerCallbacksMacOS::get_double_click_speed()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_mouse_buttons_switched(bool)
{
}

Messages::WindowServer::AreMouseButtonsSwitchedResponse WindowServerCallbacksMacOS::are_mouse_buttons_switched()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_natural_scroll(bool)
{
}

Messages::WindowServer::IsNaturalScrollResponse WindowServerCallbacksMacOS::is_natural_scroll()
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_window_modified(i32, bool)
{
}

Messages::WindowServer::IsWindowModifiedResponse WindowServerCallbacksMacOS::is_window_modified(i32)
{
    return nullptr;
}

Messages::WindowServer::GetDesktopDisplayScaleResponse WindowServerCallbacksMacOS::get_desktop_display_scale(u32)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::set_flash_flush(bool)
{
}

void WindowServerCallbacksMacOS::set_window_parent_from_client(i32, i32, i32)
{
}

Messages::WindowServer::GetWindowRectFromClientResponse WindowServerCallbacksMacOS::get_window_rect_from_client(i32, i32)
{
    return nullptr;
}

void WindowServerCallbacksMacOS::add_window_stealing_for_client(i32, i32)
{
}

void WindowServerCallbacksMacOS::remove_window_stealing_for_client(i32, i32)
{
}

void WindowServerCallbacksMacOS::remove_window_stealing(i32)
{
}

void WindowServerCallbacksMacOS::set_always_on_top(i32, bool)
{
}

Messages::WindowServer::GetColorUnderCursorResponse WindowServerCallbacksMacOS::get_color_under_cursor()
{
    return nullptr;
}

}
