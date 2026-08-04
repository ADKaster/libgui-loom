/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowServerConnectionProxy.h"

#include <LibGfx/Font/FontDatabase.h>
#include <LibGfx/SystemTheme.h>

namespace Loom {

#define WINDOW_SERVER_IPC_DEBUG 1

static HashMap<int, NonnullRefPtr<WindowServerConnectionProxy>>* s_connections;

struct WindowServerConnectionProxy::Impl {};

WindowServerConnectionProxy::~WindowServerConnectionProxy() = default;

WindowServerConnectionProxy::WindowServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket> socket, int client_id)
    : IPC::ConnectionFromClient<WindowClientEndpoint, WindowServerEndpoint>(*this, move(socket), client_id)
    , m_impl(make<Impl>())
{
    if (!s_connections)
        s_connections = new HashMap<int, NonnullRefPtr<WindowServerConnectionProxy>>;
    s_connections->set(client_id, *this);

    Vector<Gfx::IntRect, 1> const screen_rects = { { 0, 0, 1024, 768 } };
    auto system_effects = Vector { true, true, true, true, true, true, true, true, true, true };
    async_fast_greet(screen_rects, 0, 1, 1, Gfx::current_system_theme_buffer(), Gfx::FontDatabase::default_font_query(), Gfx::FontDatabase::fixed_width_font_query(), Gfx::FontDatabase::window_title_font_query(), system_effects, client_id);
}

void WindowServerConnectionProxy::die()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: die()");
    s_connections->remove(client_id());
}

void WindowServerConnectionProxy::create_menu(i32 menu_id, String const& name, i32 minimum_width)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: create_menu()");
    (void)menu_id;
    (void)name;
    (void)minimum_width;
}

void WindowServerConnectionProxy::set_menu_name(i32 menu_id, String const& name)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_menu_name()");
    (void)menu_id;
    (void)name;
}

void WindowServerConnectionProxy::set_menu_minimum_width(i32 menu_id, i32 minimum_width)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_menu_minimum_width()");
    (void)menu_id;
    (void)minimum_width;
}

void WindowServerConnectionProxy::destroy_menu(i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: destroy_menu()");
    (void)menu_id;
}

void WindowServerConnectionProxy::add_menu(i32 window_id, i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu()");
    (void)window_id;
    (void)menu_id;
}

void WindowServerConnectionProxy::add_menu_item(i32 menu_id, i32 identifier, i32 submenu_id, ByteString const& text, bool enabled, bool visible, bool checkable, bool checked, bool is_default, ByteString const& shortcut, Gfx::ShareableBitmap const& icon, bool exclusive)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu_item()");
    (void)menu_id;
    (void)identifier;
    (void)submenu_id;
    (void)text;
    (void)enabled;
    (void)visible;
    (void)checkable;
    (void)checked;
    (void)is_default;
    (void)shortcut;
    (void)icon;
    (void)exclusive;
}

void WindowServerConnectionProxy::add_menu_separator(i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu_separator()");
    (void)menu_id;
}

void WindowServerConnectionProxy::update_menu_item(i32 menu_id, i32 identifier, i32 submenu_id, ByteString const& text, bool enabled, bool visible, bool checkable, bool checked, bool is_default, ByteString const& shortcut, Gfx::ShareableBitmap const& icon)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: update_menu_item()");
    (void)menu_id;
    (void)identifier;
    (void)submenu_id;
    (void)text;
    (void)enabled;
    (void)visible;
    (void)checkable;
    (void)checked;
    (void)is_default;
    (void)shortcut;
    (void)icon;
}

void WindowServerConnectionProxy::remove_menu_item(i32 menu_id, i32 identifier)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_menu_item()");
    (void)menu_id;
    (void)identifier;
}

void WindowServerConnectionProxy::flash_menubar_menu(i32 window_id, i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: flash_menubar_menu()");
    (void)window_id;
    (void)menu_id;
}

void WindowServerConnectionProxy::create_window(i32 window_id, i32 process_id, Gfx::IntRect const& rect, bool auto_position, bool has_alpha_channel, bool minimizable, bool closeable, bool resizable, bool fullscreen, bool frameless, bool forced_shadow, float alpha_hit_threshold, Gfx::IntSize base_size, Gfx::IntSize size_increment, Gfx::IntSize minimum_size, Optional<Gfx::IntSize> const& resize_aspect_ratio, i32 type, i32 mode, ByteString const& title, i32 parent_window_id, Gfx::IntRect const& launch_origin_rect)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: create_window()");
    (void)window_id;
    (void)process_id;
    (void)rect;
    (void)auto_position;
    (void)has_alpha_channel;
    (void)minimizable;
    (void)closeable;
    (void)resizable;
    (void)fullscreen;
    (void)frameless;
    (void)forced_shadow;
    (void)alpha_hit_threshold;
    (void)base_size;
    (void)size_increment;
    (void)minimum_size;
    (void)resize_aspect_ratio;
    (void)type;
    (void)mode;
    (void)title;
    (void)parent_window_id;
    (void)launch_origin_rect;
}

Messages::WindowServer::DestroyWindowResponse WindowServerConnectionProxy::destroy_window(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: destroy_window()");
    (void)window_id;
    return nullptr;
}

void WindowServerConnectionProxy::set_window_title(i32 window_id, ByteString const& title)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_title()");
    (void)window_id;
    (void)title;
}

Messages::WindowServer::GetWindowTitleResponse WindowServerConnectionProxy::get_window_title(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_title()");
    (void)window_id;
    return nullptr;
}

Messages::WindowServer::IsMaximizedResponse WindowServerConnectionProxy::is_maximized(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_maximized()");
    (void)window_id;
    return nullptr;
}

void WindowServerConnectionProxy::set_maximized(i32 window_id, bool maximized)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_maximized()");
    (void)window_id;
    (void)maximized;
}

Messages::WindowServer::IsMinimizedResponse WindowServerConnectionProxy::is_minimized(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_minimized()");
    (void)window_id;
    return nullptr;
}

void WindowServerConnectionProxy::set_minimized(i32 window_id, bool minimized)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_minimized()");
    (void)window_id;
    (void)minimized;
}

void WindowServerConnectionProxy::start_window_resize(i32 window_id, i32 resize_direction)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: start_window_resize()");
    (void)window_id;
    (void)resize_direction;
}

Messages::WindowServer::SetWindowRectResponse WindowServerConnectionProxy::set_window_rect(i32 window_id, Gfx::IntRect const& rect)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_rect()");
    (void)window_id;
    (void)rect;
    return nullptr;
}

Messages::WindowServer::GetWindowRectResponse WindowServerConnectionProxy::get_window_rect(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_rect()");
    (void)window_id;
    return nullptr;
}

Messages::WindowServer::GetWindowFloatingRectResponse WindowServerConnectionProxy::get_window_floating_rect(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_floating_rect()");
    (void)window_id;
    return nullptr;
}

void WindowServerConnectionProxy::set_window_minimum_size(i32 window_id, Gfx::IntSize size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_minimum_size()");
    (void)window_id;
    (void)size;
}

Messages::WindowServer::GetWindowMinimumSizeResponse WindowServerConnectionProxy::get_window_minimum_size(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_minimum_size()");
    (void)window_id;
    return nullptr;
}

Messages::WindowServer::GetAppletRectOnScreenResponse WindowServerConnectionProxy::get_applet_rect_on_screen(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_applet_rect_on_screen()");
    (void)window_id;
    return nullptr;
}

void WindowServerConnectionProxy::invalidate_rect(i32 window_id, Vector<Gfx::IntRect> const& rects, bool ignore_occlusion)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: invalidate_rect()");
    (void)window_id;
    (void)rects;
    (void)ignore_occlusion;
}

void WindowServerConnectionProxy::did_finish_painting(i32 window_id, Vector<Gfx::IntRect> const& rects)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: did_finish_painting()");
    (void)window_id;
    (void)rects;
}

void WindowServerConnectionProxy::set_global_mouse_tracking(bool enabled)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_global_mouse_tracking()");
    (void)enabled;
}

void WindowServerConnectionProxy::set_window_backing_store(i32 window_id, i32 bpp, i32 pitch, IPC::File const& anon_file, i32 serial, bool has_alpha_channel, Gfx::IntSize size, Gfx::IntSize visible_size, bool flush_immediately)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_backing_store()");
    (void)window_id;
    (void)bpp;
    (void)pitch;
    (void)anon_file;
    (void)serial;
    (void)has_alpha_channel;
    (void)size;
    (void)visible_size;
    (void)flush_immediately;
}

void WindowServerConnectionProxy::set_window_has_alpha_channel(i32 window_id, bool has_alpha_channel)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_has_alpha_channel()");
    (void)window_id;
    (void)has_alpha_channel;
}

void WindowServerConnectionProxy::set_window_alpha_hit_threshold(i32 window_id, float threshold)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_alpha_hit_threshold()");
    (void)window_id;
    (void)threshold;
}

void WindowServerConnectionProxy::move_window_to_front(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: move_window_to_front()");
    (void)window_id;
}

void WindowServerConnectionProxy::set_fullscreen(i32 window_id, bool fullscreen)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_fullscreen()");
    (void)window_id;
    (void)fullscreen;
}

void WindowServerConnectionProxy::set_frameless(i32 window_id, bool frameless)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_frameless()");
    (void)window_id;
    (void)frameless;
}

void WindowServerConnectionProxy::set_forced_shadow(i32 window_id, bool shadow)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_forced_shadow()");
    (void)window_id;
    (void)shadow;
}

Messages::WindowServer::SetWallpaperResponse WindowServerConnectionProxy::set_wallpaper(Gfx::ShareableBitmap const& bitmap)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_wallpaper()");
    (void)bitmap;
    return nullptr;
}

void WindowServerConnectionProxy::set_background_color(ByteString const& background_color)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_background_color()");
    (void)background_color;
}

void WindowServerConnectionProxy::set_wallpaper_mode(ByteString const& wallpaper_mode)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_wallpaper_mode()");
    (void)wallpaper_mode;
}

Messages::WindowServer::GetWallpaperResponse WindowServerConnectionProxy::get_wallpaper()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_wallpaper()");
    return nullptr;
}

Messages::WindowServer::SetScreenLayoutResponse WindowServerConnectionProxy::set_screen_layout(WindowServer::ScreenLayout const& screen_layout, bool save)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_screen_layout()");
    (void)screen_layout;
    (void)save;
    return nullptr;
}

Messages::WindowServer::GetScreenLayoutResponse WindowServerConnectionProxy::get_screen_layout()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_layout()");
    return nullptr;
}

Messages::WindowServer::SaveScreenLayoutResponse WindowServerConnectionProxy::save_screen_layout()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: save_screen_layout()");
    return nullptr;
}

Messages::WindowServer::ApplyWorkspaceSettingsResponse WindowServerConnectionProxy::apply_workspace_settings(u32 rows, u32 columns, bool save)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: apply_workspace_settings()");
    (void)rows;
    (void)columns;
    (void)save;
    return nullptr;
}

Messages::WindowServer::GetWorkspaceSettingsResponse WindowServerConnectionProxy::get_workspace_settings()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_workspace_settings()");
    return nullptr;
}

void WindowServerConnectionProxy::show_screen_numbers(bool show)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: show_screen_numbers()");
    (void)show;
}

void WindowServerConnectionProxy::set_window_cursor(i32 window_id, i32 cursor_type)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_cursor()");
    (void)window_id;
    (void)cursor_type;
}

void WindowServerConnectionProxy::set_window_custom_cursor(i32 window_id, Gfx::ShareableBitmap const& cursor)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_custom_cursor()");
    (void)window_id;
    (void)cursor;
}

void WindowServerConnectionProxy::popup_menu(i32 menu_id, Gfx::IntPoint screen_position, Gfx::IntRect const& button_rect)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: popup_menu()");
    (void)menu_id;
    (void)screen_position;
    (void)button_rect;
}

void WindowServerConnectionProxy::dismiss_menu(i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: dismiss_menu()");
    (void)menu_id;
}

void WindowServerConnectionProxy::set_window_icon_bitmap(i32 window_id, Gfx::ShareableBitmap const& icon)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_icon_bitmap()");
    (void)window_id;
    (void)icon;
}

Messages::WindowServer::StartDragResponse WindowServerConnectionProxy::start_drag(ByteString const& text, HashMap<String, ByteBuffer> const& mime_data, Gfx::ShareableBitmap const& drag_bitmap)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: start_drag()");
    (void)text;
    (void)mime_data;
    (void)drag_bitmap;
    return nullptr;
}

void WindowServerConnectionProxy::set_accepts_drag(bool accepts)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_accepts_drag()");
    (void)accepts;
}

Messages::WindowServer::SetSystemThemeResponse WindowServerConnectionProxy::set_system_theme(ByteString const& theme_path, ByteString const& theme_name, bool keep_desktop_background, Optional<ByteString> const& color_scheme_path)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_theme()");
    (void)theme_path;
    (void)theme_name;
    (void)keep_desktop_background;
    (void)color_scheme_path;
    return nullptr;
}

Messages::WindowServer::GetSystemThemeResponse WindowServerConnectionProxy::get_system_theme()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_system_theme()");
    return nullptr;
}

Messages::WindowServer::SetSystemThemeOverrideResponse WindowServerConnectionProxy::set_system_theme_override(Core::AnonymousBuffer const& buffer)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_theme_override()");
    (void)buffer;
    return nullptr;
}

Messages::WindowServer::GetSystemThemeOverrideResponse WindowServerConnectionProxy::get_system_theme_override()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_system_theme_override()");
    return nullptr;
}

void WindowServerConnectionProxy::clear_system_theme_override()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: clear_system_theme_override()");
}

Messages::WindowServer::IsSystemThemeOverriddenResponse WindowServerConnectionProxy::is_system_theme_overridden()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_system_theme_overridden()");
    return nullptr;
}

Messages::WindowServer::GetPreferredColorSchemeResponse WindowServerConnectionProxy::get_preferred_color_scheme()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_preferred_color_scheme()");
    return nullptr;
}

void WindowServerConnectionProxy::apply_cursor_theme(ByteString const& name)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: apply_cursor_theme()");
    (void)name;
}

void WindowServerConnectionProxy::set_cursor_highlight_radius(int radius)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_cursor_highlight_radius()");
    (void)radius;
}

Messages::WindowServer::GetCursorHighlightRadiusResponse WindowServerConnectionProxy::get_cursor_highlight_radius()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_cursor_highlight_radius()");
    return nullptr;
}

void WindowServerConnectionProxy::set_cursor_highlight_color(Gfx::Color color)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_cursor_highlight_color()");
    (void)color;
}

Messages::WindowServer::GetCursorHighlightColorResponse WindowServerConnectionProxy::get_cursor_highlight_color()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_cursor_highlight_color()");
    return nullptr;
}

Messages::WindowServer::GetCursorThemeResponse WindowServerConnectionProxy::get_cursor_theme()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_cursor_theme()");
    return nullptr;
}

Messages::WindowServer::SetSystemFontsResponse WindowServerConnectionProxy::set_system_fonts(ByteString const& default_font_query, ByteString const& fixed_width_font_query, ByteString const& window_title_font_query)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_fonts()");
    (void)default_font_query;
    (void)fixed_width_font_query;
    (void)window_title_font_query;
    return nullptr;
}

void WindowServerConnectionProxy::set_system_effects(Vector<bool> const& effects, u8 geometry, u8 tile_window)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_effects()");
    (void)effects;
    (void)geometry;
    (void)tile_window;
}

void WindowServerConnectionProxy::set_window_base_size_and_size_increment(i32 window_id, Gfx::IntSize base_size, Gfx::IntSize size_increment)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_base_size_and_size_increment()");
    (void)window_id;
    (void)base_size;
    (void)size_increment;
}

void WindowServerConnectionProxy::set_window_resize_aspect_ratio(i32 window_id, Optional<Gfx::IntSize> const& resize_aspect_ratio)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_resize_aspect_ratio()");
    (void)window_id;
    (void)resize_aspect_ratio;
}

void WindowServerConnectionProxy::enable_display_link()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: enable_display_link()");
}

void WindowServerConnectionProxy::disable_display_link()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: disable_display_link()");
}

void WindowServerConnectionProxy::set_window_progress(i32 window_id, Optional<i32> const& progress)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_progress()");
    (void)window_id;
    (void)progress;
}

void WindowServerConnectionProxy::refresh_system_theme()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: refresh_system_theme()");
}

void WindowServerConnectionProxy::pong()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: pong()");
}

void WindowServerConnectionProxy::set_global_cursor_position(Gfx::IntPoint position)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_global_cursor_position()");
    (void)position;
}

Messages::WindowServer::GetGlobalCursorPositionResponse WindowServerConnectionProxy::get_global_cursor_position()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_global_cursor_position()");
    return nullptr;
}

void WindowServerConnectionProxy::set_mouse_acceleration(float factor)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_mouse_acceleration()");
    (void)factor;
}

Messages::WindowServer::GetMouseAccelerationResponse WindowServerConnectionProxy::get_mouse_acceleration()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_mouse_acceleration()");
    return nullptr;
}

void WindowServerConnectionProxy::set_scroll_step_size(u32 step_size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_scroll_step_size()");
    (void)step_size;
}

Messages::WindowServer::GetScrollStepSizeResponse WindowServerConnectionProxy::get_scroll_step_size()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_scroll_step_size()");
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapResponse WindowServerConnectionProxy::get_screen_bitmap(Optional<Gfx::IntRect> const& rect, Optional<u32> const& screen_index)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap()");
    (void)rect;
    (void)screen_index;
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapAroundCursorResponse WindowServerConnectionProxy::get_screen_bitmap_around_cursor(Gfx::IntSize size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap_around_cursor()");
    (void)size;
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapAroundLocationResponse WindowServerConnectionProxy::get_screen_bitmap_around_location(Gfx::IntSize size, Gfx::IntPoint location)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap_around_location()");
    (void)size;
    (void)location;
    return nullptr;
}

void WindowServerConnectionProxy::set_double_click_speed(i32 speed)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_double_click_speed()");
    (void)speed;
}

Messages::WindowServer::GetDoubleClickSpeedResponse WindowServerConnectionProxy::get_double_click_speed()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_double_click_speed()");
    return nullptr;
}

void WindowServerConnectionProxy::set_mouse_buttons_switched(bool switched)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_mouse_buttons_switched()");
    (void)switched;
}

Messages::WindowServer::AreMouseButtonsSwitchedResponse WindowServerConnectionProxy::are_mouse_buttons_switched()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: are_mouse_buttons_switched()");
    return nullptr;
}

void WindowServerConnectionProxy::set_natural_scroll(bool inverted)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_natural_scroll()");
    (void)inverted;
}

Messages::WindowServer::IsNaturalScrollResponse WindowServerConnectionProxy::is_natural_scroll()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_natural_scroll()");
    return nullptr;
}

void WindowServerConnectionProxy::set_window_modified(i32 window_id, bool modified)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_modified()");
    (void)window_id;
    (void)modified;
}

Messages::WindowServer::IsWindowModifiedResponse WindowServerConnectionProxy::is_window_modified(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_window_modified()");
    (void)window_id;
    return nullptr;
}

Messages::WindowServer::GetDesktopDisplayScaleResponse WindowServerConnectionProxy::get_desktop_display_scale(u32 screen_index)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_desktop_display_scale()");
    (void)screen_index;
    return nullptr;
}

void WindowServerConnectionProxy::set_flash_flush(bool enabled)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_flash_flush()");
    (void)enabled;
}

void WindowServerConnectionProxy::set_window_parent_from_client(i32 client_id, i32 parent_id, i32 child_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_parent_from_client()");
    (void)client_id;
    (void)parent_id;
    (void)child_id;
}

Messages::WindowServer::GetWindowRectFromClientResponse WindowServerConnectionProxy::get_window_rect_from_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_rect_from_client()");
    (void)client_id;
    (void)window_id;
    return nullptr;
}

void WindowServerConnectionProxy::add_window_stealing_for_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_window_stealing_for_client()");
    (void)client_id;
    (void)window_id;
}

void WindowServerConnectionProxy::remove_window_stealing_for_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_window_stealing_for_client()");
    (void)client_id;
    (void)window_id;
}

void WindowServerConnectionProxy::remove_window_stealing(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_window_stealing()");
    (void)window_id;
}

void WindowServerConnectionProxy::set_always_on_top(i32 window_id, bool always_on_top)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_always_on_top()");
    (void)window_id;
    (void)always_on_top;
}

Messages::WindowServer::GetColorUnderCursorResponse WindowServerConnectionProxy::get_color_under_cursor()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_color_under_cursor()");
    return nullptr;
}


}
