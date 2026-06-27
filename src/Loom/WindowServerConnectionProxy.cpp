/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowServerConnectionProxy.h"
#include <LibGfx/Font/FontDatabase.h>
#include <LibGfx/SystemTheme.h>

#define WINDOW_SERVER_IPC_DEBUG 1

namespace Loom {

HashMap<int, NonnullRefPtr<WindowServerConnectionProxy>>* s_connections;

WindowServerConnectionProxy::WindowServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket> socket, int client_id)
    : IPC::ConnectionFromClient<WindowClientEndpoint, WindowServerEndpoint>(*this, move(socket), client_id)
{
    if (!s_connections)
        s_connections = new HashMap<int, NonnullRefPtr<WindowServerConnectionProxy>>;
    s_connections->set(client_id, *this);

    // FIXME: Actually get info about # of screens from backend
    Vector<Gfx::IntRect, 1> const screen_rects = { { 0, 0, 1024, 768 } };
    async_fast_greet(screen_rects, 0, 1, 1, Gfx::current_system_theme_buffer(), Gfx::FontDatabase::default_font_query(), Gfx::FontDatabase::fixed_width_font_query(), Gfx::FontDatabase::window_title_font_query(), {}, client_id);
}

void WindowServerConnectionProxy::set_callbacks(NonnullRefPtr<WindowServerCallbacks> callbacks)
{
    m_callbacks = move(callbacks);
    m_callbacks->on_misbehave = [this](char const* message) {
        if (message)
            this->did_misbehave(message);
        else
            this->did_misbehave();
    };
}

WindowServerConnectionProxy::~WindowServerConnectionProxy() = default;

void WindowServerConnectionProxy::die()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: die()");
}

void WindowServerConnectionProxy::may_have_become_unresponsive()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: may_have_become_unresponsive()");
}

void WindowServerConnectionProxy::did_become_responsive()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: did_become_responsive()");
}

void WindowServerConnectionProxy::create_menu(i32 menu_id, String const& name, i32 minimum_width)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: create_menu(menu_id={}, name={}, minimum_width={})", menu_id, name, minimum_width);
    m_callbacks->create_menu(menu_id, name, minimum_width);
}

void WindowServerConnectionProxy::set_menu_name(i32 menu_id, String const& name)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_menu_name(menu_id={}, name={})", menu_id, name);
    m_callbacks->set_menu_name(menu_id, name);
}

void WindowServerConnectionProxy::set_menu_minimum_width(i32 menu_id, i32 minimum_width)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_menu_minimum_width(menu_id={}, minimum_width={})", menu_id, minimum_width);
    m_callbacks->set_menu_minimum_width(menu_id, minimum_width);
}

void WindowServerConnectionProxy::destroy_menu(i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: destroy_menu(menu_id={})", menu_id);
    m_callbacks->destroy_menu(menu_id);
}

void WindowServerConnectionProxy::add_menu(i32 window_id, i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu(window_id={}, menu_id={})", window_id, menu_id);
    m_callbacks->add_menu(window_id, menu_id);
}

void WindowServerConnectionProxy::add_menu_item(i32 menu_id, i32 identifier, i32 submenu_id, ByteString const& text, bool enabled, bool visible, bool checkable, bool checked, bool is_default, ByteString const& shortcut, Gfx::ShareableBitmap const& icon, bool exclusive)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu_item(menu_id={}, identifier={}, submenu_id={}, text={}, enabled={}, visible={}, checkable={}, checked={}, is_default={}, shortcut={}, icon_is_valid={}, exclusive={})", menu_id, identifier, submenu_id, text, enabled, visible, checkable, checked, is_default, shortcut, icon.is_valid(), exclusive);
    m_callbacks->add_menu_item(menu_id, identifier, submenu_id, text, enabled, visible, checkable, checked, is_default, shortcut, icon, exclusive);
}

void WindowServerConnectionProxy::add_menu_separator(i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu_separator(menu_id={})", menu_id);
    m_callbacks->add_menu_separator(menu_id);
}

void WindowServerConnectionProxy::update_menu_item(i32 menu_id, i32 identifier, i32 submenu_id, ByteString const& text, bool enabled, bool visible, bool checkable, bool checked, bool is_default, ByteString const& shortcut, Gfx::ShareableBitmap const& icon)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: update_menu_item(menu_id={}, identifier={}, submenu_id={}, text={}, enabled={}, visible={}, checkable={}, checked={}, is_default={}, shortcut={}, icon_is_valid={})", menu_id, identifier, submenu_id, text, enabled, visible, checkable, checked, is_default, shortcut, icon.is_valid());
    m_callbacks->update_menu_item(menu_id, identifier, submenu_id, text, enabled, visible, checkable, checked, is_default, shortcut, icon);
}

void WindowServerConnectionProxy::remove_menu_item(i32 menu_id, i32 identifier)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_menu_item(menu_id={}, identifier={})", menu_id, identifier);
    m_callbacks->remove_menu_item(menu_id, identifier);
}

void WindowServerConnectionProxy::flash_menubar_menu(i32 window_id, i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: flash_menubar_menu(window_id={}, menu_id={})", window_id, menu_id);
    m_callbacks->flash_menubar_menu(window_id, menu_id);
}

void WindowServerConnectionProxy::create_window(i32 window_id, i32 process_id, Gfx::IntRect const& rect, bool auto_position, bool has_alpha_channel, bool minimizable, bool closeable, bool resizable, bool fullscreen, bool frameless, bool forced_shadow, float alpha_hit_threshold, Gfx::IntSize base_size, Gfx::IntSize size_increment, Gfx::IntSize minimum_size, Optional<Gfx::IntSize> const& resize_aspect_ratio, i32 type, i32 mode, ByteString const& title, i32 parent_window_id, Gfx::IntRect const& launch_origin_rect)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: create_window(window_id={}, process_id={}, rect={}, auto_position={}, has_alpha_channel={}, minimizable={}, closeable={}, resizable={}, fullscreen={}, frameless={}, forced_shadow={}, alpha_hit_threshold={}, base_size={}, size_increment={}, minimum_size={}, resize_aspect_ratio={}, type={}, mode={}, title={}, parent_window_id={}, launch_origin_rect={})", window_id, process_id, rect, auto_position, has_alpha_channel, minimizable, closeable, resizable, fullscreen, frameless, forced_shadow, alpha_hit_threshold, base_size, size_increment, minimum_size, resize_aspect_ratio, type, mode, title, parent_window_id, launch_origin_rect);
    m_callbacks->create_window(window_id, process_id, rect, auto_position, has_alpha_channel, minimizable, closeable, resizable, fullscreen, frameless, forced_shadow, alpha_hit_threshold, base_size, size_increment, minimum_size, resize_aspect_ratio, type, mode, title, parent_window_id, launch_origin_rect);
}

Messages::WindowServer::DestroyWindowResponse WindowServerConnectionProxy::destroy_window(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: destroy_window(window_id={})", window_id);
    return m_callbacks->destroy_window(window_id);
}

void WindowServerConnectionProxy::set_window_title(i32 window_id, ByteString const& title)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_title(window_id={}, title={})", window_id, title);
    m_callbacks->set_window_title(window_id, title);
}

Messages::WindowServer::GetWindowTitleResponse WindowServerConnectionProxy::get_window_title(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_title(window_id={})", window_id);
    return m_callbacks->get_window_title(window_id);
}

Messages::WindowServer::IsMaximizedResponse WindowServerConnectionProxy::is_maximized(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_maximized(window_id={})", window_id);
    return m_callbacks->is_maximized(window_id);
}

void WindowServerConnectionProxy::set_maximized(i32 window_id, bool maximized)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_maximized(window_id={}, maximized={})", window_id, maximized);
    m_callbacks->set_maximized(window_id, maximized);
}

Messages::WindowServer::IsMinimizedResponse WindowServerConnectionProxy::is_minimized(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_minimized(window_id={})", window_id);
    return m_callbacks->is_minimized(window_id);
}

void WindowServerConnectionProxy::set_minimized(i32 window_id, bool minimized)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_minimized(window_id={}, minimized={})", window_id, minimized);
    m_callbacks->set_minimized(window_id, minimized);
}

void WindowServerConnectionProxy::start_window_resize(i32 window_id, i32 resize_direction)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: start_window_resize(window_id={}, resize_direction={})", window_id, resize_direction);
    m_callbacks->start_window_resize(window_id, resize_direction);
}

Messages::WindowServer::SetWindowRectResponse WindowServerConnectionProxy::set_window_rect(i32 window_id, Gfx::IntRect const& rect)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_rect(window_id={}, rect={})", window_id, rect);
    return m_callbacks->set_window_rect(window_id, rect);
}

Messages::WindowServer::GetWindowRectResponse WindowServerConnectionProxy::get_window_rect(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_rect(window_id={})", window_id);
    return m_callbacks->get_window_rect(window_id);
}

Messages::WindowServer::GetWindowFloatingRectResponse WindowServerConnectionProxy::get_window_floating_rect(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_floating_rect(window_id={})", window_id);
    return m_callbacks->get_window_floating_rect(window_id);
}

void WindowServerConnectionProxy::set_window_minimum_size(i32 window_id, Gfx::IntSize size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_minimum_size(window_id={}, size={})", window_id, size);
    m_callbacks->set_window_minimum_size(window_id, size);
}

Messages::WindowServer::GetWindowMinimumSizeResponse WindowServerConnectionProxy::get_window_minimum_size(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_minimum_size(window_id={})", window_id);
    return m_callbacks->get_window_minimum_size(window_id);
}

Messages::WindowServer::GetAppletRectOnScreenResponse WindowServerConnectionProxy::get_applet_rect_on_screen(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_applet_rect_on_screen(window_id={})", window_id);
    return m_callbacks->get_applet_rect_on_screen(window_id);
}

void WindowServerConnectionProxy::invalidate_rect(i32 window_id, Vector<Gfx::IntRect> const& rects, bool ignore_occlusion)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: invalidate_rect(window_id={}, rects={}, ignore_occlusion={})", window_id, rects, ignore_occlusion);
    m_callbacks->invalidate_rect(window_id, rects, ignore_occlusion);
}

void WindowServerConnectionProxy::did_finish_painting(i32 window_id, Vector<Gfx::IntRect> const& rects)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: did_finish_painting(window_id={}, rects={})", window_id, rects);
    m_callbacks->did_finish_painting(window_id, rects);
}

void WindowServerConnectionProxy::set_global_mouse_tracking(bool enabled)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_global_mouse_tracking(enabled={})", enabled);
    m_callbacks->set_global_mouse_tracking(enabled);
}

void WindowServerConnectionProxy::set_window_backing_store(i32 window_id, i32 bpp, i32 pitch, IPC::File const& anon_file, i32 serial, bool has_alpha_channel, Gfx::IntSize size, Gfx::IntSize visible_size, bool flush_immediately)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_backing_store(window_id={}, bpp={}, pitch={}, anon_file_fd={}, serial={}, has_alpha_channel={}, size={}, visible_size={}, flush_immediately={})", window_id, bpp, pitch, anon_file.fd(), serial, has_alpha_channel, size, visible_size, flush_immediately);
    m_callbacks->set_window_backing_store(window_id, bpp, pitch, anon_file, serial, has_alpha_channel, size, visible_size, flush_immediately);
}

void WindowServerConnectionProxy::set_window_has_alpha_channel(i32 window_id, bool has_alpha_channel)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_has_alpha_channel(window_id={}, has_alpha_channel={})", window_id, has_alpha_channel);
    m_callbacks->set_window_has_alpha_channel(window_id, has_alpha_channel);
}

void WindowServerConnectionProxy::set_window_alpha_hit_threshold(i32 window_id, float threshold)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_alpha_hit_threshold(window_id={}, threshold={})", window_id, threshold);
    m_callbacks->set_window_alpha_hit_threshold(window_id, threshold);
}

void WindowServerConnectionProxy::move_window_to_front(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: move_window_to_front(window_id={})", window_id);
    m_callbacks->move_window_to_front(window_id);
}

void WindowServerConnectionProxy::set_fullscreen(i32 window_id, bool fullscreen)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_fullscreen(window_id={}, fullscreen={})", window_id, fullscreen);
    m_callbacks->set_fullscreen(window_id, fullscreen);
}

void WindowServerConnectionProxy::set_frameless(i32 window_id, bool frameless)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_frameless(window_id={}, frameless={})", window_id, frameless);
    m_callbacks->set_frameless(window_id, frameless);
}

void WindowServerConnectionProxy::set_forced_shadow(i32 window_id, bool shadow)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_forced_shadow(window_id={}, shadow={})", window_id, shadow);
    m_callbacks->set_forced_shadow(window_id, shadow);
}

Messages::WindowServer::SetWallpaperResponse WindowServerConnectionProxy::set_wallpaper(Gfx::ShareableBitmap const& wallpaper_bitmap)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_wallpaper(wallpaper_bitmap_is_valid={})", wallpaper_bitmap.is_valid());
    return m_callbacks->set_wallpaper(wallpaper_bitmap);
}

void WindowServerConnectionProxy::set_background_color(ByteString const& background_color)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_background_color(background_color={})", background_color);
    m_callbacks->set_background_color(background_color);
}

void WindowServerConnectionProxy::set_wallpaper_mode(ByteString const& mode)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_wallpaper_mode(mode={})", mode);
    m_callbacks->set_wallpaper_mode(mode);
}

Messages::WindowServer::GetWallpaperResponse WindowServerConnectionProxy::get_wallpaper()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_wallpaper()");
    return m_callbacks->get_wallpaper();
}

Messages::WindowServer::SetScreenLayoutResponse WindowServerConnectionProxy::set_screen_layout(WindowServer::ScreenLayout const& screen_layout, bool save)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_screen_layout(screen_layout_screens={}, screen_layout_main_screen_index={}, save={})", screen_layout.screens.size(), screen_layout.main_screen_index, save);
    return m_callbacks->set_screen_layout(screen_layout, save);
}

Messages::WindowServer::GetScreenLayoutResponse WindowServerConnectionProxy::get_screen_layout()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_layout()");
    return m_callbacks->get_screen_layout();
}

Messages::WindowServer::SaveScreenLayoutResponse WindowServerConnectionProxy::save_screen_layout()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: save_screen_layout()");
    return m_callbacks->save_screen_layout();
}

Messages::WindowServer::ApplyWorkspaceSettingsResponse WindowServerConnectionProxy::apply_workspace_settings(u32 rows, u32 columns, bool save)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: apply_workspace_settings(rows={}, columns={}, save={})", rows, columns, save);
    return m_callbacks->apply_workspace_settings(rows, columns, save);
}

Messages::WindowServer::GetWorkspaceSettingsResponse WindowServerConnectionProxy::get_workspace_settings()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_workspace_settings()");
    return m_callbacks->get_workspace_settings();
}

void WindowServerConnectionProxy::show_screen_numbers(bool show)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: show_screen_numbers(show={})", show);
    m_callbacks->show_screen_numbers(show);
}

void WindowServerConnectionProxy::set_window_cursor(i32 window_id, i32 cursor_type)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_cursor(window_id={}, cursor_type={})", window_id, cursor_type);
    m_callbacks->set_window_cursor(window_id, cursor_type);
}

void WindowServerConnectionProxy::set_window_custom_cursor(i32 window_id, Gfx::ShareableBitmap const& cursor)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_custom_cursor(window_id={}, cursor_is_valid={})", window_id, cursor.is_valid());
    m_callbacks->set_window_custom_cursor(window_id, cursor);
}

void WindowServerConnectionProxy::popup_menu(i32 menu_id, Gfx::IntPoint screen_position, Gfx::IntRect const& button_rect)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: popup_menu(menu_id={}, screen_position={}, button_rect={})", menu_id, screen_position, button_rect);
    m_callbacks->popup_menu(menu_id, screen_position, button_rect);
}

void WindowServerConnectionProxy::dismiss_menu(i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: dismiss_menu(menu_id={})", menu_id);
    m_callbacks->dismiss_menu(menu_id);
}

void WindowServerConnectionProxy::set_window_icon_bitmap(i32 window_id, Gfx::ShareableBitmap const& icon)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_icon_bitmap(window_id={}, icon_is_valid={})", window_id, icon.is_valid());
    m_callbacks->set_window_icon_bitmap(window_id, icon);
}

Messages::WindowServer::StartDragResponse WindowServerConnectionProxy::start_drag(ByteString const& text, HashMap<String, ByteBuffer> const& mime_data, Gfx::ShareableBitmap const& drag_bitmap)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: start_drag(text={}, mime_data_count={}, drag_bitmap_is_valid={})", text, mime_data.size(), drag_bitmap.is_valid());
    return m_callbacks->start_drag(text, mime_data, drag_bitmap);
}

void WindowServerConnectionProxy::set_accepts_drag(bool accepts)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_accepts_drag(accepts={})", accepts);
    m_callbacks->set_accepts_drag(accepts);
}

Messages::WindowServer::SetSystemThemeResponse WindowServerConnectionProxy::set_system_theme(ByteString const& theme_path, ByteString const& theme_name, bool keep_desktop_background, Optional<ByteString> const& color_scheme_path)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_theme(theme_path={}, theme_name={}, keep_desktop_background={}, color_scheme_path={})", theme_path, theme_name, keep_desktop_background, color_scheme_path);
    return m_callbacks->set_system_theme(theme_path, theme_name, keep_desktop_background, color_scheme_path);
}

Messages::WindowServer::GetSystemThemeResponse WindowServerConnectionProxy::get_system_theme()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_system_theme()");
    return m_callbacks->get_system_theme();
}

Messages::WindowServer::SetSystemThemeOverrideResponse WindowServerConnectionProxy::set_system_theme_override(Core::AnonymousBuffer const& buffer)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_theme_override(buffer_is_valid={}, buffer_size={}, buffer_fd={})", buffer.is_valid(), buffer.size(), buffer.fd());
    return m_callbacks->set_system_theme_override(buffer);
}

Messages::WindowServer::GetSystemThemeOverrideResponse WindowServerConnectionProxy::get_system_theme_override()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_system_theme_override()");
    return m_callbacks->get_system_theme_override();
}

void WindowServerConnectionProxy::clear_system_theme_override()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: clear_system_theme_override()");
    m_callbacks->clear_system_theme_override();
}

Messages::WindowServer::IsSystemThemeOverriddenResponse WindowServerConnectionProxy::is_system_theme_overridden()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_system_theme_overridden()");
    return m_callbacks->is_system_theme_overridden();
}

Messages::WindowServer::GetPreferredColorSchemeResponse WindowServerConnectionProxy::get_preferred_color_scheme()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_preferred_color_scheme()");
    return m_callbacks->get_preferred_color_scheme();
}

void WindowServerConnectionProxy::apply_cursor_theme(ByteString const& name)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: apply_cursor_theme(name={})", name);
    m_callbacks->apply_cursor_theme(name);
}

void WindowServerConnectionProxy::set_cursor_highlight_radius(int radius)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_cursor_highlight_radius(radius={})", radius);
    m_callbacks->set_cursor_highlight_radius(radius);
}

Messages::WindowServer::GetCursorHighlightRadiusResponse WindowServerConnectionProxy::get_cursor_highlight_radius()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_cursor_highlight_radius()");
    return m_callbacks->get_cursor_highlight_radius();
}

void WindowServerConnectionProxy::set_cursor_highlight_color(Gfx::Color color)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_cursor_highlight_color(color={})", color);
    m_callbacks->set_cursor_highlight_color(color);
}

Messages::WindowServer::GetCursorHighlightColorResponse WindowServerConnectionProxy::get_cursor_highlight_color()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_cursor_highlight_color()");
    return m_callbacks->get_cursor_highlight_color();
}

Messages::WindowServer::GetCursorThemeResponse WindowServerConnectionProxy::get_cursor_theme()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_cursor_theme()");
    return m_callbacks->get_cursor_theme();
}

Messages::WindowServer::SetSystemFontsResponse WindowServerConnectionProxy::set_system_fonts(ByteString const& default_font_query, ByteString const& fixed_width_font_query, ByteString const& window_title_font_query)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_fonts(default_font_query={}, fixed_width_font_query={}, window_title_font_query={})", default_font_query, fixed_width_font_query, window_title_font_query);
    return m_callbacks->set_system_fonts(default_font_query, fixed_width_font_query, window_title_font_query);
}

void WindowServerConnectionProxy::set_system_effects(Vector<bool> const& effects, u8 geometry, u8 tile_window)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_effects(effects={}, geometry={}, tile_window={})", effects, geometry, tile_window);
    m_callbacks->set_system_effects(effects, geometry, tile_window);
}

void WindowServerConnectionProxy::set_window_base_size_and_size_increment(i32 window_id, Gfx::IntSize base_size, Gfx::IntSize size_increment)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_base_size_and_size_increment(window_id={}, base_size={}, size_increment={})", window_id, base_size, size_increment);
    m_callbacks->set_window_base_size_and_size_increment(window_id, base_size, size_increment);
}

void WindowServerConnectionProxy::set_window_resize_aspect_ratio(i32 window_id, Optional<Gfx::IntSize> const& resize_aspect_ratio)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_resize_aspect_ratio(window_id={}, resize_aspect_ratio={})", window_id, resize_aspect_ratio);
    m_callbacks->set_window_resize_aspect_ratio(window_id, resize_aspect_ratio);
}

void WindowServerConnectionProxy::enable_display_link()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: enable_display_link()");
    m_callbacks->enable_display_link();
}

void WindowServerConnectionProxy::disable_display_link()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: disable_display_link()");
    m_callbacks->disable_display_link();
}

void WindowServerConnectionProxy::set_window_progress(i32 window_id, Optional<i32> const& progress)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_progress(window_id={}, progress={})", window_id, progress);
    m_callbacks->set_window_progress(window_id, progress);
}

void WindowServerConnectionProxy::refresh_system_theme()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: refresh_system_theme()");
    m_callbacks->refresh_system_theme();
}

void WindowServerConnectionProxy::pong()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: pong()");
    m_callbacks->pong();
}

void WindowServerConnectionProxy::set_global_cursor_position(Gfx::IntPoint position)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_global_cursor_position(position={})", position);
    m_callbacks->set_global_cursor_position(position);
}

Messages::WindowServer::GetGlobalCursorPositionResponse WindowServerConnectionProxy::get_global_cursor_position()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_global_cursor_position()");
    return m_callbacks->get_global_cursor_position();
}

void WindowServerConnectionProxy::set_mouse_acceleration(float factor)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_mouse_acceleration(factor={})", factor);
    m_callbacks->set_mouse_acceleration(factor);
}

Messages::WindowServer::GetMouseAccelerationResponse WindowServerConnectionProxy::get_mouse_acceleration()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_mouse_acceleration()");
    return m_callbacks->get_mouse_acceleration();
}

void WindowServerConnectionProxy::set_scroll_step_size(u32 step_size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_scroll_step_size(step_size={})", step_size);
    m_callbacks->set_scroll_step_size(step_size);
}

Messages::WindowServer::GetScrollStepSizeResponse WindowServerConnectionProxy::get_scroll_step_size()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_scroll_step_size()");
    return m_callbacks->get_scroll_step_size();
}

Messages::WindowServer::GetScreenBitmapResponse WindowServerConnectionProxy::get_screen_bitmap(Optional<Gfx::IntRect> const& rect, Optional<u32> const& screen_index)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap(rect={}, screen_index={})", rect, screen_index);
    return m_callbacks->get_screen_bitmap(rect, screen_index);
}

Messages::WindowServer::GetScreenBitmapAroundCursorResponse WindowServerConnectionProxy::get_screen_bitmap_around_cursor(Gfx::IntSize size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap_around_cursor(size={})", size);
    return m_callbacks->get_screen_bitmap_around_cursor(size);
}

Messages::WindowServer::GetScreenBitmapAroundLocationResponse WindowServerConnectionProxy::get_screen_bitmap_around_location(Gfx::IntSize size, Gfx::IntPoint location)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap_around_location(size={}, location={})", size, location);
    return m_callbacks->get_screen_bitmap_around_location(size, location);
}

void WindowServerConnectionProxy::set_double_click_speed(i32 speed)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_double_click_speed(speed={})", speed);
    m_callbacks->set_double_click_speed(speed);
}

Messages::WindowServer::GetDoubleClickSpeedResponse WindowServerConnectionProxy::get_double_click_speed()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_double_click_speed()");
    return m_callbacks->get_double_click_speed();
}

void WindowServerConnectionProxy::set_mouse_buttons_switched(bool switched)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_mouse_buttons_switched(switched={})", switched);
    m_callbacks->set_mouse_buttons_switched(switched);
}

Messages::WindowServer::AreMouseButtonsSwitchedResponse WindowServerConnectionProxy::are_mouse_buttons_switched()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: are_mouse_buttons_switched()");
    return m_callbacks->are_mouse_buttons_switched();
}

void WindowServerConnectionProxy::set_natural_scroll(bool inverted)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_natural_scroll(inverted={})", inverted);
    m_callbacks->set_natural_scroll(inverted);
}

Messages::WindowServer::IsNaturalScrollResponse WindowServerConnectionProxy::is_natural_scroll()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_natural_scroll()");
    return m_callbacks->is_natural_scroll();
}

void WindowServerConnectionProxy::set_window_modified(i32 window_id, bool modified)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_modified(window_id={}, modified={})", window_id, modified);
    m_callbacks->set_window_modified(window_id, modified);
}

Messages::WindowServer::IsWindowModifiedResponse WindowServerConnectionProxy::is_window_modified(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_window_modified(window_id={})", window_id);
    return m_callbacks->is_window_modified(window_id);
}

Messages::WindowServer::GetDesktopDisplayScaleResponse WindowServerConnectionProxy::get_desktop_display_scale(u32 screen_index)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_desktop_display_scale(screen_index={})", screen_index);
    return m_callbacks->get_desktop_display_scale(screen_index);
}

void WindowServerConnectionProxy::set_flash_flush(bool enabled)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_flash_flush(enabled={})", enabled);
    m_callbacks->set_flash_flush(enabled);
}

void WindowServerConnectionProxy::set_window_parent_from_client(i32 client_id, i32 parent_id, i32 child_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_parent_from_client(client_id={}, parent_id={}, child_id={})", client_id, parent_id, child_id);
    m_callbacks->set_window_parent_from_client(client_id, parent_id, child_id);
}

Messages::WindowServer::GetWindowRectFromClientResponse WindowServerConnectionProxy::get_window_rect_from_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_rect_from_client(client_id={}, window_id={})", client_id, window_id);
    return m_callbacks->get_window_rect_from_client(client_id, window_id);
}

void WindowServerConnectionProxy::add_window_stealing_for_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_window_stealing_for_client(client_id={}, window_id={})", client_id, window_id);
    m_callbacks->add_window_stealing_for_client(client_id, window_id);
}

void WindowServerConnectionProxy::remove_window_stealing_for_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_window_stealing_for_client(client_id={}, window_id={})", client_id, window_id);
    m_callbacks->remove_window_stealing_for_client(client_id, window_id);
}

void WindowServerConnectionProxy::remove_window_stealing(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_window_stealing(window_id={})", window_id);
    m_callbacks->remove_window_stealing(window_id);
}

void WindowServerConnectionProxy::set_always_on_top(i32 window_id, bool always_on_top)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_always_on_top(window_id={}, always_on_top={})", window_id, always_on_top);
    m_callbacks->set_always_on_top(window_id, always_on_top);
}

Messages::WindowServer::GetColorUnderCursorResponse WindowServerConnectionProxy::get_color_under_cursor()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_color_under_cursor()");
    return m_callbacks->get_color_under_cursor();
}

}
