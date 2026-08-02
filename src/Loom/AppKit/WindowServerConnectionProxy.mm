/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "CocoaWrapper.h"

#include "Conversions.h"
#include "ContentView.h"
#include "Window.h"
#include "WindowView.h"
#include "WindowServerConnectionProxy.h"
#include "WindowController.h"

#include <LibCore/AnonymousBuffer.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/Font/FontDatabase.h>
#include <LibGfx/Palette.h>
#include <LibGfx/ShareableBitmap.h>
#include <LibGfx/SystemTheme.h>

namespace Loom {

#define WINDOW_SERVER_IPC_DEBUG 1

static HashMap<int, NonnullRefPtr<WindowServerConnectionProxy>>* s_connections;

struct WindowServerConnectionProxy::Impl
{
    NSMutableDictionary* windows;

    [[nodiscard]] WindowController* window_for_id(i32 window_id) const
    {
        return (WindowController*)[windows objectForKey:[NSNumber numberWithInt:window_id]];
    }
};

static ContentView* content_view_for_window(WindowController* window)
{
    auto* window_view = window.window.contentView;
    if (![window_view isKindOfClass:[WindowView class]])
        return nil;
    return [(WindowView*)window_view contentView];
}

static NSRect full_frame_rect_for_content_rect(NSRect content_rect)
{
    auto palette = Gfx::Palette(Gfx::PaletteImpl::create_with_anonymous_buffer(Gfx::current_system_theme_buffer()));
    auto const border_thickness = static_cast<CGFloat>(palette.window_border_thickness());
    auto const titlebar_height = static_cast<CGFloat>(palette.window_title_height());

    return NSMakeRect(0, 0,
        content_rect.size.width + border_thickness * 2.0,
        content_rect.size.height + border_thickness * 2.0 + titlebar_height);
}

WindowServerConnectionProxy::~WindowServerConnectionProxy() = default;

WindowServerConnectionProxy::WindowServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket> socket, int client_id)
    : IPC::ConnectionFromClient<WindowClientEndpoint, WindowServerEndpoint>(*this, move(socket), client_id)
    , m_impl(make<Impl>())
{
    if (!s_connections)
        s_connections = new HashMap<int, NonnullRefPtr<WindowServerConnectionProxy>>;
    s_connections->set(client_id, *this);
    m_impl->windows = [[NSMutableDictionary alloc] init];

    Vector<Gfx::IntRect, 1> const screen_rects = { { 0, 0, 1024, 768 } };
    auto system_effects = Vector { true, true, true, true, true, true, true, true, true, true };
    async_fast_greet(screen_rects, 0, 1, 1, Gfx::current_system_theme_buffer(), Gfx::FontDatabase::default_font_query(), Gfx::FontDatabase::fixed_width_font_query(), Gfx::FontDatabase::window_title_font_query(), system_effects, client_id);
}

void WindowServerConnectionProxy::die()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: die()");

    [m_impl->windows enumerateKeysAndObjectsUsingBlock:^(id, id obj, BOOL*) {
        auto* window = (WindowController*)obj;
        [window close];
    }];
    s_connections->remove(client_id());
}

void WindowServerConnectionProxy::create_menu(i32 menu_id, String const& name, i32 minimum_width)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: create_menu(menu_id={}, name={}, minimum_width={})", menu_id, name, minimum_width);
    (void)menu_id;
    (void)name;
    (void)minimum_width;
}

void WindowServerConnectionProxy::set_menu_name(i32 menu_id, String const& name)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_menu_name(menu_id={}, name={})", menu_id, name);
    (void)menu_id;
    (void)name;
}

void WindowServerConnectionProxy::set_menu_minimum_width(i32 menu_id, i32 minimum_width)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_menu_minimum_width(menu_id={}, minimum_width={})", menu_id, minimum_width);
    (void)menu_id;
    (void)minimum_width;
}

void WindowServerConnectionProxy::destroy_menu(i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: destroy_menu(menu_id={})", menu_id);
    (void)menu_id;
}

void WindowServerConnectionProxy::add_menu(i32 window_id, i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu(window_id={}, menu_id={})", window_id, menu_id);
    (void)window_id;
    (void)menu_id;
}

void WindowServerConnectionProxy::add_menu_item(i32 menu_id, i32 identifier, i32 submenu_id, ByteString const& text, bool enabled, bool visible, bool checkable, bool checked, bool is_default, ByteString const& shortcut, Gfx::ShareableBitmap const& icon, bool exclusive)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu_item(menu_id={}, identifier={}, submenu_id={}, text={}, enabled={}, visible={}, checkable={}, checked={}, is_default={}, shortcut={}, icon.is_valid()={}, exclusive={})", menu_id, identifier, submenu_id, text, enabled, visible, checkable, checked, is_default, shortcut, icon.is_valid(), exclusive);
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_menu_separator(menu_id={})", menu_id);
    (void)menu_id;
}

void WindowServerConnectionProxy::update_menu_item(i32 menu_id, i32 identifier, i32 submenu_id, ByteString const& text, bool enabled, bool visible, bool checkable, bool checked, bool is_default, ByteString const& shortcut, Gfx::ShareableBitmap const& icon)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: update_menu_item(menu_id={}, identifier={}, submenu_id={}, text={}, enabled={}, visible={}, checkable={}, checked={}, is_default={}, shortcut={}, icon.is_valid()={})", menu_id, identifier, submenu_id, text, enabled, visible, checkable, checked, is_default, shortcut, icon.is_valid());
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_menu_item(menu_id={}, identifier={})", menu_id, identifier);
    (void)menu_id;
    (void)identifier;
}

void WindowServerConnectionProxy::flash_menubar_menu(i32 window_id, i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: flash_menubar_menu(window_id={}, menu_id={})", window_id, menu_id);
    (void)window_id;
    (void)menu_id;
}

void WindowServerConnectionProxy::create_window(i32 window_id, i32 process_id, Gfx::IntRect const& rect, bool auto_position, bool has_alpha_channel, bool minimizable, bool closeable, bool resizable, bool fullscreen, bool frameless, bool forced_shadow, float alpha_hit_threshold, Gfx::IntSize base_size, Gfx::IntSize size_increment, Gfx::IntSize minimum_size, Optional<Gfx::IntSize> const& resize_aspect_ratio, i32 type, i32 mode, ByteString const& title, i32 parent_window_id, Gfx::IntRect const& launch_origin_rect)
{
    (void)process_id;
    (void)fullscreen;
    (void)frameless;
    (void)forced_shadow;
    (void)alpha_hit_threshold;
    (void)base_size;
    (void)size_increment;
    (void)resize_aspect_ratio;
    (void)mode;
    (void)launch_origin_rect;

    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: create_window(window_id={}, process_id={}, rect={}, auto_position={}, has_alpha_channel={}, minimizable={}, closeable={}, resizable={}, fullscreen={}, frameless={}, forced_shadow={}, alpha_hit_threshold={}, base_size={}, size_increment={}, minimum_size={}, resize_aspect_ratio={}, type={}, mode={}, title=\"{}\", parent_window_id={}, launch_origin_rect={})",
        window_id, process_id, rect, auto_position, has_alpha_channel, minimizable, closeable, resizable, fullscreen, frameless, forced_shadow, alpha_hit_threshold, base_size, size_increment, minimum_size, resize_aspect_ratio, type, mode, title, parent_window_id, launch_origin_rect);


    // FIXME: Do math to reconcile 'rect_when_windowless' with 'system recommended window size'
    //        Ref: WindowServer's CreateWindow callback
    auto* new_window = [[WindowController alloc] initWithContentRect:gfx_rect_to_ns_rect(rect)
                                                            windowID:window_id
                                                            connection:*this];

    [m_impl->windows setObject:new_window
                        forKey:[NSNumber numberWithInt:window_id]];

    if (auto_position) {
        [[new_window window] center];
    }

    [[new_window window] setTitle:[NSString stringWithUTF8String:title.characters()]];
    [[new_window window] setOpaque:!has_alpha_channel];
    [[new_window window] setAlphaValue:has_alpha_channel ? 0.5 : 1.0];
    [[new_window window] setContentMinSize:gfx_size_to_ns_size(minimum_size)];

    NSWindowStyleMask style_mask = [[new_window window] styleMask];
    
    if (resizable)
        style_mask |= NSWindowStyleMaskResizable;
    else
        style_mask &= ~NSWindowStyleMaskResizable;
    
    if (closeable)
        style_mask |= NSWindowStyleMaskClosable;
    else
        style_mask &= ~NSWindowStyleMaskClosable;
    
    if (minimizable)
        style_mask |= NSWindowStyleMaskMiniaturizable;
    else
        style_mask &= ~NSWindowStyleMaskMiniaturizable;
    
    auto* ns_window = [new_window window];
    [ns_window setStyleMask:style_mask];

    constexpr i32 window_type_normal = 1;
    constexpr i32 window_type_desktop = 8;
    bool can_become_active = type == window_type_normal || type == window_type_desktop;
    if (can_become_active) {
        if (parent_window_id == 0)
            [NSApp activateIgnoringOtherApps:YES];
        [ns_window makeKeyAndOrderFront:nil];
    } else {
        [ns_window orderFront:nil];
    }
}

Messages::WindowServer::DestroyWindowResponse WindowServerConnectionProxy::destroy_window(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: destroy_window(window_id={})", window_id);

    auto* window = m_impl->window_for_id(window_id);
    if (!window) {
        did_misbehave("DeleteWindow: Bad Window ID");
        return nullptr;
    }

    [window serenityCloseFromServer];
    [m_impl->windows removeObjectForKey:[NSNumber numberWithInt:window_id]];
    Vector<i32> destroyed_window_ids;
    destroyed_window_ids.append(window_id);
    return destroyed_window_ids;
}

void WindowServerConnectionProxy::set_window_title(i32 window_id, ByteString const& title)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_title(window_id={}, title=\"{}\")", window_id, title);

    if (auto* window = m_impl->window_for_id(window_id)) {
        [[window window] setTitle:string_to_ns_string(title)];
    } else {
        did_misbehave("SetWindowTitle: Bad Window ID");
    }
}

Messages::WindowServer::GetWindowTitleResponse WindowServerConnectionProxy::get_window_title(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_title(window_id={})", window_id);

    if (auto* window = m_impl->window_for_id(window_id)) {
        NSString* title = [[window window] title];
        return ns_string_to_byte_string(title);
    }
    did_misbehave("GetWindowTitle: Bad Window ID");
    return nullptr;
}

Messages::WindowServer::IsMaximizedResponse WindowServerConnectionProxy::is_maximized(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_maximized(window_id={})", window_id);

    if (auto* window = m_impl->window_for_id(window_id))
        return [(Window*)[window window] serenityIsZoomed];

    did_misbehave("IsMaximized: Bad Window ID");
    return nullptr;
}

void WindowServerConnectionProxy::set_maximized(i32 window_id, bool maximized)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_maximized(window_id={}, maximized={})", window_id, maximized);

    if (auto* window = m_impl->window_for_id(window_id))
        [(Window*)[window window] serenitySetZoomed:maximized];
    else
        did_misbehave("SetMaximized: Bad Window ID");
}

Messages::WindowServer::IsMinimizedResponse WindowServerConnectionProxy::is_minimized(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_minimized(window_id={})", window_id);

    if (auto* window = m_impl->window_for_id(window_id))
        return [(Window*)[window window] serenityIsMiniaturized];

    did_misbehave("IsMinimized: Bad Window ID");
    return nullptr;
}

void WindowServerConnectionProxy::set_minimized(i32 window_id, bool minimized)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_minimized(window_id={}, minimized={})", window_id, minimized);

    if (auto* window = m_impl->window_for_id(window_id))
        [(Window*)[window window] serenitySetMiniaturized:minimized];
    else
        did_misbehave("SetMinimized: Bad Window ID");
}

void WindowServerConnectionProxy::start_window_resize(i32 window_id, i32 resize_direction)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: start_window_resize(window_id={}, resize_direction={})", window_id, resize_direction);
    (void)window_id;
    (void)resize_direction;
}

Messages::WindowServer::SetWindowRectResponse WindowServerConnectionProxy::set_window_rect(i32 window_id, Gfx::IntRect const& rect)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_rect(window_id={}, rect={})", window_id, rect);

    if (auto* window = m_impl->window_for_id(window_id)) {
        auto* ns_window = [window window];
        auto content_rect = gfx_rect_to_ns_rect(rect);
        auto frame_rect = full_frame_rect_for_content_rect(content_rect);
        window.serenityContentSize = content_rect.size;
        [ns_window setFrame:frame_rect display:YES animate:NO];
        return rect;
    }
    did_misbehave("SetWindowRect: Bad Window ID");
    return nullptr;
}

Messages::WindowServer::GetWindowRectResponse WindowServerConnectionProxy::get_window_rect(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_rect(window_id={})", window_id);

    if (auto* window = m_impl->window_for_id(window_id)) {
        return ns_rect_to_gfx_rect(NSMakeRect(0, 0, window.serenityContentSize.width, window.serenityContentSize.height));
    }
    did_misbehave("GetWindowRect: Bad Window ID");
    return nullptr;
}

Messages::WindowServer::GetWindowFloatingRectResponse WindowServerConnectionProxy::get_window_floating_rect(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_floating_rect(window_id={})", window_id);

    if (auto* window = m_impl->window_for_id(window_id)) {
        auto* ns_window = [window window];
        auto content_rect = [ns_window contentRectForFrameRect:ns_window.frame];
        return ns_rect_to_gfx_rect(content_rect);
    }
    did_misbehave("GetWindowFloatingRect: Bad Window ID");
    return nullptr;
}

void WindowServerConnectionProxy::set_window_minimum_size(i32 window_id, Gfx::IntSize size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_minimum_size(window_id={}, size={})", window_id, size);

    if (auto* window = m_impl->window_for_id(window_id)) {
        [[window window] setContentMinSize:gfx_size_to_ns_size(size)];
        return;
    }
    did_misbehave("SetWindowMinimumSize: Bad Window ID");
}

Messages::WindowServer::GetWindowMinimumSizeResponse WindowServerConnectionProxy::get_window_minimum_size(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_minimum_size(window_id={})", window_id);

    if (auto* window = m_impl->window_for_id(window_id))
        return ns_size_to_gfx_size([[window window] contentMinSize]);
    did_misbehave("GetWindowMinimumSize: Bad Window ID");
    return nullptr;
}

Messages::WindowServer::GetAppletRectOnScreenResponse WindowServerConnectionProxy::get_applet_rect_on_screen(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_applet_rect_on_screen(window_id={})", window_id);
    (void)window_id;
    return nullptr;
}

void WindowServerConnectionProxy::invalidate_rect(i32 window_id, Vector<Gfx::IntRect> const& rects, bool ignore_occlusion)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: invalidate_rect(window_id={}, rects={}, ignore_occlusion={})", window_id, rects, ignore_occlusion);

    if (auto* window = m_impl->window_for_id(window_id)) {
        auto* content_view = content_view_for_window(window);
        if (!content_view) {
            did_misbehave("InvalidateRect: Missing ContentView");
            return;
        }
        for (auto const& rect : rects)
            [content_view setNeedsDisplayInRect:gfx_rect_to_ns_rect(rect)];
        if (auto window_rect = get_window_rect(window_id); window_rect.valid())
            async_paint(window_id, window_rect.rect().size(), rects);
        return;
    }

    did_misbehave("InvalidateRect: Bad Window ID");
}

void WindowServerConnectionProxy::did_finish_painting(i32 window_id, Vector<Gfx::IntRect> const& rects)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: did_finish_painting(window_id={}, rects={})", window_id, rects);

    if (auto* window = m_impl->window_for_id(window_id)) {
        auto* content_view = content_view_for_window(window);
        if (!content_view) {
            did_misbehave("DidFinishPainting: Missing ContentView");
            return;
        }
        for (auto const& rect : rects)
            [content_view setNeedsDisplayInRect:gfx_rect_to_ns_rect(rect)];
        [content_view displayIfNeeded];
        return;
    }

    did_misbehave("DidFinishPainting: Bad Window ID");
}

void WindowServerConnectionProxy::set_global_mouse_tracking(bool enabled)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_global_mouse_tracking(enabled={})", enabled);
    (void)enabled;
}

void WindowServerConnectionProxy::set_window_backing_store(i32 window_id, i32 bpp, i32 pitch, IPC::File const& anon_file, i32 serial, bool has_alpha_channel, Gfx::IntSize size, Gfx::IntSize visible_size, bool flush_immediately)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_backing_store(window_id={}, bpp={}, pitch={}, anon_file.fd={}, serial={}, has_alpha_channel={}, size={}, visible_size={}, flush_immediately={})", window_id, bpp, pitch, anon_file.fd(), serial, has_alpha_channel, size, visible_size, flush_immediately);

    if (auto* window = m_impl->window_for_id(window_id)) {
        window.backingStoreHasAlpha = has_alpha_channel;
        window.backingStoreVisibleSize = gfx_size_to_ns_size(visible_size);

        auto buffer_or_error = Core::AnonymousBuffer::create_from_anon_fd(anon_file.take_fd(), pitch * size.height());
        if (buffer_or_error.is_error()) {
            did_misbehave("SetWindowBackingStore: Failed to create anonymous buffer");
            return;
        }

        auto bitmap_or_error = Gfx::Bitmap::create_with_anonymous_buffer(
            has_alpha_channel ? Gfx::BitmapFormat::BGRA8888 : Gfx::BitmapFormat::BGRx8888,
            buffer_or_error.release_value(),
            size,
            1);
        if (bitmap_or_error.is_error()) {
            did_misbehave("SetWindowBackingStore: Failed to create bitmap");
            return;
        }

        [window setBackingStoreBitmap:bitmap_or_error.release_value()];

        auto* content_view = content_view_for_window(window);
        if (!content_view) {
            did_misbehave("SetWindowBackingStore: Missing ContentView");
            return;
        }
        if (flush_immediately)
            [content_view setNeedsDisplay:YES];
        else
            [content_view setNeedsDisplayInRect:content_view.bounds];
        // AppKit otherwise tends to coalesce these swaps until later in the run loop,
        // which drops short-lived press-state frames from the client.
        [content_view displayIfNeeded];
        return;
    }

    did_misbehave("SetWindowBackingStore: Bad Window ID");
}

void WindowServerConnectionProxy::set_window_has_alpha_channel(i32 window_id, bool has_alpha_channel)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_has_alpha_channel(window_id={}, has_alpha_channel={})", window_id, has_alpha_channel);
    (void)window_id;
    (void)has_alpha_channel;
}

void WindowServerConnectionProxy::set_window_alpha_hit_threshold(i32 window_id, float threshold)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_alpha_hit_threshold(window_id={}, threshold={})", window_id, threshold);
    (void)window_id;
    (void)threshold;
}

void WindowServerConnectionProxy::move_window_to_front(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: move_window_to_front(window_id={})", window_id);
    (void)window_id;
}

void WindowServerConnectionProxy::set_fullscreen(i32 window_id, bool fullscreen)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_fullscreen(window_id={}, fullscreen={})", window_id, fullscreen);
    (void)window_id;
    (void)fullscreen;
}

void WindowServerConnectionProxy::set_frameless(i32 window_id, bool frameless)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_frameless(window_id={}, frameless={})", window_id, frameless);
    (void)window_id;
    (void)frameless;
}

void WindowServerConnectionProxy::set_forced_shadow(i32 window_id, bool shadow)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_forced_shadow(window_id={}, shadow={})", window_id, shadow);
    (void)window_id;
    (void)shadow;
}

Messages::WindowServer::SetWallpaperResponse WindowServerConnectionProxy::set_wallpaper(Gfx::ShareableBitmap const& bitmap)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_wallpaper(bitmap.is_valid()={})", bitmap.is_valid());
    (void)bitmap;
    return nullptr;
}

void WindowServerConnectionProxy::set_background_color(ByteString const& background_color)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_background_color(background_color={})", background_color);
    (void)background_color;
}

void WindowServerConnectionProxy::set_wallpaper_mode(ByteString const& wallpaper_mode)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_wallpaper_mode(wallpaper_mode={})", wallpaper_mode);
    (void)wallpaper_mode;
}

Messages::WindowServer::GetWallpaperResponse WindowServerConnectionProxy::get_wallpaper()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_wallpaper()");
    return nullptr;
}

Messages::WindowServer::SetScreenLayoutResponse WindowServerConnectionProxy::set_screen_layout(WindowServer::ScreenLayout const& screen_layout, bool save)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_screen_layout(screen_layout.screens.size()={}, save={})", screen_layout.screens.size(), save);
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: apply_workspace_settings(rows={}, columns={}, save={})", rows, columns, save);
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: show_screen_numbers(show={})", show);
    (void)show;
}

void WindowServerConnectionProxy::set_window_cursor(i32 window_id, i32 cursor_type)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_cursor(window_id={}, cursor_type={})", window_id, cursor_type);
    (void)window_id;
    (void)cursor_type;
}

void WindowServerConnectionProxy::set_window_custom_cursor(i32 window_id, Gfx::ShareableBitmap const& cursor)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_custom_cursor(window_id={}, cursor_is_valid={})", window_id, cursor.is_valid());
    (void)window_id;
    (void)cursor;
}

void WindowServerConnectionProxy::popup_menu(i32 menu_id, Gfx::IntPoint screen_position, Gfx::IntRect const& button_rect)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: popup_menu(menu_id={}, screen_position={}, button_rect={})", menu_id, screen_position, button_rect);
    (void)menu_id;
    (void)screen_position;
    (void)button_rect;
}

void WindowServerConnectionProxy::dismiss_menu(i32 menu_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: dismiss_menu(menu_id={})", menu_id);
    (void)menu_id;
}

void WindowServerConnectionProxy::set_window_icon_bitmap(i32 window_id, Gfx::ShareableBitmap const& icon)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_icon_bitmap(window_id={}, icon_is_valid={})", window_id, icon.is_valid());

    if (auto* window = m_impl->window_for_id(window_id)) {
        NSImage* icon_image = nil;
        if (icon.is_valid())
            icon_image = gfx_bitmap_to_ns_image(*icon.bitmap());

        window.windowIconImage = icon_image;
        if ([window.window.contentView isKindOfClass:[WindowView class]])
            [(WindowView*)window.window.contentView setWindowIconImage:icon_image];
    }
}

Messages::WindowServer::StartDragResponse WindowServerConnectionProxy::start_drag(ByteString const& text, HashMap<String, ByteBuffer> const& mime_data, Gfx::ShareableBitmap const& drag_bitmap)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: start_drag(text={}, mime_data_count={}, drag_bitmap_is_valid={})", text, mime_data.size(), drag_bitmap.is_valid());
    (void)text;
    (void)mime_data;
    (void)drag_bitmap;
    return nullptr;
}

void WindowServerConnectionProxy::set_accepts_drag(bool accepts)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_accepts_drag(accepts={})", accepts);
    (void)accepts;
}

Messages::WindowServer::SetSystemThemeResponse WindowServerConnectionProxy::set_system_theme(ByteString const& theme_path, ByteString const& theme_name, bool keep_desktop_background, Optional<ByteString> const& color_scheme_path)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_theme(theme_path={}, theme_name={}, keep_desktop_background={}, color_scheme_path={})", theme_path, theme_name, keep_desktop_background, color_scheme_path);
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_theme_override(buffer_is_valid={}, buffer_size={}, buffer_fd={})", buffer.is_valid(), buffer.size(), buffer.fd());
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: apply_cursor_theme(name={})", name);
    (void)name;
}

void WindowServerConnectionProxy::set_cursor_highlight_radius(int radius)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_cursor_highlight_radius(radius={})", radius);
    (void)radius;
}

Messages::WindowServer::GetCursorHighlightRadiusResponse WindowServerConnectionProxy::get_cursor_highlight_radius()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_cursor_highlight_radius()");
    return nullptr;
}

void WindowServerConnectionProxy::set_cursor_highlight_color(Gfx::Color color)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_cursor_highlight_color(color={})", color);
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_fonts(default_font_query={}, fixed_width_font_query={}, window_title_font_query={})", default_font_query, fixed_width_font_query, window_title_font_query);
    (void)default_font_query;
    (void)fixed_width_font_query;
    (void)window_title_font_query;
    return nullptr;
}

void WindowServerConnectionProxy::set_system_effects(Vector<bool> const& effects, u8 geometry, u8 tile_window)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_system_effects(effects={}, geometry={}, tile_window={})", effects, geometry, tile_window);
    (void)effects;
    (void)geometry;
    (void)tile_window;
}

void WindowServerConnectionProxy::set_window_base_size_and_size_increment(i32 window_id, Gfx::IntSize base_size, Gfx::IntSize size_increment)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_base_size_and_size_increment(window_id={}, base_size={}, size_increment={})", window_id, base_size, size_increment);
    (void)window_id;
    (void)base_size;
    (void)size_increment;
}

void WindowServerConnectionProxy::set_window_resize_aspect_ratio(i32 window_id, Optional<Gfx::IntSize> const& resize_aspect_ratio)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_resize_aspect_ratio(window_id={}, resize_aspect_ratio={})", window_id, resize_aspect_ratio);
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_progress(window_id={}, progress={})", window_id, progress);
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
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_global_cursor_position(position={})", position);
    (void)position;
}

Messages::WindowServer::GetGlobalCursorPositionResponse WindowServerConnectionProxy::get_global_cursor_position()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_global_cursor_position()");
    return nullptr;
}

void WindowServerConnectionProxy::set_mouse_acceleration(float factor)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_mouse_acceleration(factor={})", factor);
    (void)factor;
}

Messages::WindowServer::GetMouseAccelerationResponse WindowServerConnectionProxy::get_mouse_acceleration()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_mouse_acceleration()");
    return nullptr;
}

void WindowServerConnectionProxy::set_scroll_step_size(u32 step_size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_scroll_step_size(step_size={})", step_size);
    (void)step_size;
}

Messages::WindowServer::GetScrollStepSizeResponse WindowServerConnectionProxy::get_scroll_step_size()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_scroll_step_size()");
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapResponse WindowServerConnectionProxy::get_screen_bitmap(Optional<Gfx::IntRect> const& rect, Optional<u32> const& screen_index)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap(rect={}, screen_index={})", rect, screen_index);
    (void)rect;
    (void)screen_index;
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapAroundCursorResponse WindowServerConnectionProxy::get_screen_bitmap_around_cursor(Gfx::IntSize size)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap_around_cursor(size={})", size);
    (void)size;
    return nullptr;
}

Messages::WindowServer::GetScreenBitmapAroundLocationResponse WindowServerConnectionProxy::get_screen_bitmap_around_location(Gfx::IntSize size, Gfx::IntPoint location)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_screen_bitmap_around_location(size={}, location={})", size, location);
    (void)size;
    (void)location;
    return nullptr;
}

void WindowServerConnectionProxy::set_double_click_speed(i32 speed)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_double_click_speed(speed={})", speed);
    (void)speed;
}

Messages::WindowServer::GetDoubleClickSpeedResponse WindowServerConnectionProxy::get_double_click_speed()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_double_click_speed()");
    return nullptr;
}

void WindowServerConnectionProxy::set_mouse_buttons_switched(bool switched)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_mouse_buttons_switched(switched={})", switched);
    (void)switched;
}

Messages::WindowServer::AreMouseButtonsSwitchedResponse WindowServerConnectionProxy::are_mouse_buttons_switched()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: are_mouse_buttons_switched()");
    return nullptr;
}

void WindowServerConnectionProxy::set_natural_scroll(bool inverted)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_natural_scroll(inverted={})", inverted);
    (void)inverted;
}

Messages::WindowServer::IsNaturalScrollResponse WindowServerConnectionProxy::is_natural_scroll()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_natural_scroll()");
    return nullptr;
}

void WindowServerConnectionProxy::set_window_modified(i32 window_id, bool modified)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_modified(window_id={}, modified={})", window_id, modified);
    (void)window_id;
    (void)modified;
}

Messages::WindowServer::IsWindowModifiedResponse WindowServerConnectionProxy::is_window_modified(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: is_window_modified(window_id={})", window_id);
    (void)window_id;
    return nullptr;
}

Messages::WindowServer::GetDesktopDisplayScaleResponse WindowServerConnectionProxy::get_desktop_display_scale(u32 screen_index)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_desktop_display_scale(screen_index={})", screen_index);
    (void)screen_index;
    return nullptr;
}

void WindowServerConnectionProxy::set_flash_flush(bool enabled)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_flash_flush(enabled={})", enabled);
    (void)enabled;
}

void WindowServerConnectionProxy::set_window_parent_from_client(i32 client_id, i32 parent_id, i32 child_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_window_parent_from_client(client_id={}, parent_id={}, child_id={})", client_id, parent_id, child_id);
    (void)client_id;
    (void)parent_id;
    (void)child_id;
}

Messages::WindowServer::GetWindowRectFromClientResponse WindowServerConnectionProxy::get_window_rect_from_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_window_rect_from_client(client_id={}, window_id={})", client_id, window_id);
    (void)client_id;

    if (auto* window = m_impl->window_for_id(window_id)) {
        auto* ns_window = [window window];
        NSRect content_rect = [ns_window contentRectForFrameRect:[ns_window frame]];
        return ns_rect_to_gfx_rect(content_rect);
    }
    did_misbehave("GetWindowRectFromClient: Bad Window ID");
    return nullptr;
}

void WindowServerConnectionProxy::add_window_stealing_for_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: add_window_stealing_for_client(client_id={}, window_id={})", client_id, window_id);
    (void)client_id;
    (void)window_id;
}

void WindowServerConnectionProxy::remove_window_stealing_for_client(i32 client_id, i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_window_stealing_for_client(client_id={}, window_id={})", client_id, window_id);
    (void)client_id;
    (void)window_id;
}

void WindowServerConnectionProxy::remove_window_stealing(i32 window_id)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: remove_window_stealing(window_id={})", window_id);
    (void)window_id;
}

void WindowServerConnectionProxy::set_always_on_top(i32 window_id, bool always_on_top)
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: set_always_on_top(window_id={}, always_on_top={})", window_id, always_on_top);
    (void)window_id;
    (void)always_on_top;
}

Messages::WindowServer::GetColorUnderCursorResponse WindowServerConnectionProxy::get_color_under_cursor()
{
    dbgln_if(WINDOW_SERVER_IPC_DEBUG, "WindowServer IPC: get_color_under_cursor()");
    return nullptr;
}

}
