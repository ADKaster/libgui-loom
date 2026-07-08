/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "WindowController.h"
#include "ContentView.h"
#include "CarbonWrapper.h"
#include "Window.h"
#include "WindowView.h"
#include "WindowViewController.h"
#include "Conversions.h"
#include <Loom/WindowServerConnectionProxy.h>

#include <AK/TypeCasts.h>
#include <AK/StringView.h>
#include <AK/Utf8View.h>
#include <Kernel/API/KeyCode.h>
#include <cstring>

using namespace WindowServer;

static u8 serenity_modifiers_from_event(NSEventModifierFlags modifier_flags)
{
    u8 modifiers = Mod_None;

    if ((modifier_flags & NSEventModifierFlagShift) != 0)
        modifiers |= Mod_Shift;
    if ((modifier_flags & NSEventModifierFlagControl) != 0)
        modifiers |= Mod_Ctrl;
    if ((modifier_flags & NSEventModifierFlagOption) != 0)
        modifiers |= Mod_Alt;
    if ((modifier_flags & NSEventModifierFlagCommand) != 0)
        modifiers |= Mod_Super;

    return modifiers;
}

static int serenity_key_from_mac_key_code(unsigned short key_code)
{
    switch (key_code) {
    case kVK_ANSI_0: return Key_0;
    case kVK_ANSI_1: return Key_1;
    case kVK_ANSI_2: return Key_2;
    case kVK_ANSI_3: return Key_3;
    case kVK_ANSI_4: return Key_4;
    case kVK_ANSI_5: return Key_5;
    case kVK_ANSI_6: return Key_6;
    case kVK_ANSI_7: return Key_7;
    case kVK_ANSI_8: return Key_8;
    case kVK_ANSI_9: return Key_9;
    case kVK_ANSI_A: return Key_A;
    case kVK_ANSI_B: return Key_B;
    case kVK_ANSI_C: return Key_C;
    case kVK_ANSI_D: return Key_D;
    case kVK_ANSI_E: return Key_E;
    case kVK_ANSI_F: return Key_F;
    case kVK_ANSI_G: return Key_G;
    case kVK_ANSI_H: return Key_H;
    case kVK_ANSI_I: return Key_I;
    case kVK_ANSI_J: return Key_J;
    case kVK_ANSI_K: return Key_K;
    case kVK_ANSI_L: return Key_L;
    case kVK_ANSI_M: return Key_M;
    case kVK_ANSI_N: return Key_N;
    case kVK_ANSI_O: return Key_O;
    case kVK_ANSI_P: return Key_P;
    case kVK_ANSI_Q: return Key_Q;
    case kVK_ANSI_R: return Key_R;
    case kVK_ANSI_S: return Key_S;
    case kVK_ANSI_T: return Key_T;
    case kVK_ANSI_U: return Key_U;
    case kVK_ANSI_V: return Key_V;
    case kVK_ANSI_W: return Key_W;
    case kVK_ANSI_X: return Key_X;
    case kVK_ANSI_Y: return Key_Y;
    case kVK_ANSI_Z: return Key_Z;
    case kVK_ANSI_Backslash: return Key_Backslash;
    case kVK_ANSI_Comma: return Key_Comma;
    case kVK_ANSI_Equal: return Key_Equal;
    case kVK_ANSI_Grave: return Key_Backtick;
    case kVK_ANSI_Keypad0: return Key_0;
    case kVK_ANSI_Keypad1: return Key_1;
    case kVK_ANSI_Keypad2: return Key_2;
    case kVK_ANSI_Keypad3: return Key_3;
    case kVK_ANSI_Keypad4: return Key_4;
    case kVK_ANSI_Keypad5: return Key_5;
    case kVK_ANSI_Keypad6: return Key_6;
    case kVK_ANSI_Keypad7: return Key_7;
    case kVK_ANSI_Keypad8: return Key_8;
    case kVK_ANSI_Keypad9: return Key_9;
    case kVK_ANSI_KeypadClear: return Key_Delete;
    case kVK_ANSI_KeypadDecimal: return Key_Period;
    case kVK_ANSI_KeypadDivide: return Key_Slash;
    case kVK_ANSI_KeypadEnter: return Key_Return;
    case kVK_ANSI_KeypadEquals: return Key_Equal;
    case kVK_ANSI_KeypadMinus: return Key_Minus;
    case kVK_ANSI_KeypadMultiply: return Key_Asterisk;
    case kVK_ANSI_KeypadPlus: return Key_Plus;
    case kVK_ANSI_LeftBracket: return Key_LeftBracket;
    case kVK_ANSI_Minus: return Key_Minus;
    case kVK_ANSI_Period: return Key_Period;
    case kVK_ANSI_Quote: return Key_Apostrophe;
    case kVK_ANSI_RightBracket: return Key_RightBracket;
    case kVK_ANSI_Semicolon: return Key_Semicolon;
    case kVK_ANSI_Slash: return Key_Slash;
    case kVK_CapsLock: return Key_CapsLock;
    case kVK_Command: return Key_LeftSuper;
    case kVK_Control: return Key_LeftControl;
    case kVK_Delete: return Key_Backspace;
    case kVK_DownArrow: return Key_Down;
    case kVK_End: return Key_End;
    case kVK_Escape: return Key_Escape;
    case kVK_F1: return Key_F1;
    case kVK_F2: return Key_F2;
    case kVK_F3: return Key_F3;
    case kVK_F4: return Key_F4;
    case kVK_F5: return Key_F5;
    case kVK_F6: return Key_F6;
    case kVK_F7: return Key_F7;
    case kVK_F8: return Key_F8;
    case kVK_F9: return Key_F9;
    case kVK_F10: return Key_F10;
    case kVK_F11: return Key_F11;
    case kVK_F12: return Key_F12;
    case kVK_ForwardDelete: return Key_Delete;
    case kVK_Home: return Key_Home;
    case kVK_LeftArrow: return Key_Left;
    case kVK_Option: return Key_LeftAlt;
    case kVK_PageDown: return Key_PageDown;
    case kVK_PageUp: return Key_PageUp;
    case kVK_Return: return Key_Return;
    case kVK_RightArrow: return Key_Right;
    case kVK_RightCommand: return Key_RightSuper;
    case kVK_RightControl: return Key_RightControl;
    case kVK_RightOption: return Key_RightAlt;
    case kVK_RightShift: return Key_RightShift;
    case kVK_Shift: return Key_LeftShift;
    case kVK_Space: return Key_Space;
    case kVK_Tab: return Key_Tab;
    case kVK_UpArrow: return Key_Up;
    default:
        return Key_Invalid;
    }
}

static unsigned serenity_button_from_event(NSEvent* event)
{
    switch (event.buttonNumber) {
    case 0: return 1u;
    case 1: return 2u;
    case 2: return 4u;
    case 3: return 8u;
    case 4: return 16u;
    default: return 0u;
    }
}

static void normalize_mouse_event(NSEvent* event, unsigned& button, unsigned& buttons, unsigned& modifiers)
{
    modifiers = serenity_modifiers_from_event(event.modifierFlags);
    buttons = static_cast<unsigned>([NSEvent pressedMouseButtons]);

    if (button == 1u && (event.modifierFlags & NSEventModifierFlagControl) != 0) {
        button = 2u;
        buttons &= ~1u;
        buttons |= 2u;
        modifiers &= ~Mod_Ctrl;
    }
}

@implementation WindowController {
    RefPtr<Gfx::Bitmap> _backing_store_bitmap;
}

- (instancetype)initWithContentRect:(NSRect)contentRect windowID:(int)id connection:(Loom::WindowServerConnectionProxy*)connection
{
    if (self = [super init]) {
        self.windowID = id;
        self.connection = connection;
        self.serenityContentSize = contentRect.size;

        auto* viewController = [[WindowViewController alloc] initWithFrame:contentRect];

        self.window = [Window windowWithContentViewController:viewController];
        [self.window setDelegate:self];
        [self.window setTitlebarAppearsTransparent:YES];
        [self.window setTitleVisibility:NSWindowTitleHidden];
        self.window.styleMask = NSWindowStyleMaskBorderless;
        self.window.styleMask |= NSWindowStyleMaskResizable;
        [self.window setMovableByWindowBackground:YES];

        [[self.window standardWindowButton:NSWindowCloseButton] setHidden:YES];
        [[self.window standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
        [[self.window standardWindowButton:NSWindowZoomButton] setHidden:YES];

        auto* windowView = (WindowView*)viewController.view;
        auto* contentView = windowView.contentView;
        contentView.eventDelegate = self;
        [windowView configureTitlebarButtons];
        [windowView setNeedsLayout:YES];
        [self.window makeFirstResponder:contentView];
    }
    return self;
}

- (void)setBackingStoreBitmap:(RefPtr<Gfx::Bitmap>)bitmap
{
    _backing_store_bitmap = move(bitmap);
}

- (Loom::WindowServerConnectionProxy*)windowServerConnection
{
    return self.connection;
}

- (NSImage*)backingStoreImage
{
    if (!_backing_store_bitmap)
        return nil;
    return Loom::gfx_bitmap_to_ns_image(*_backing_store_bitmap);
}

- (void)contentView:(ContentView*)contentView didReceiveEvent:(NSEvent*)event
{
    (void)contentView;

    auto* connection = [self windowServerConnection];
    if (!connection)
        return;

    auto* windowView = (WindowView*)self.window.contentView;
    auto* content = windowView.contentView;
    auto const location = [content convertPoint:event.locationInWindow fromView:nil];
    auto const position = Gfx::IntPoint {
        static_cast<int>(location.x),
        static_cast<int>(content.bounds.size.height - location.y),
    };

    switch (event.type) {
    case NSEventTypeMouseMoved:
        connection->async_mouse_move(self.windowID, position, 0, static_cast<unsigned>([NSEvent pressedMouseButtons]), serenity_modifiers_from_event(event.modifierFlags), 0, 0, 0, 0);
        break;
    case NSEventTypeLeftMouseDown:
    case NSEventTypeRightMouseDown:
    case NSEventTypeOtherMouseDown: {
        auto button = serenity_button_from_event(event);
        unsigned buttons = 0;
        unsigned modifiers = 0;
        normalize_mouse_event(event, button, buttons, modifiers);
        if (button == 0u)
            break;

        connection->async_mouse_down(self.windowID, position, button, buttons, modifiers, 0, 0, 0, 0);
        if (event.clickCount >= 2)
            connection->async_mouse_double_click(self.windowID, position, button, buttons, modifiers, 0, 0, 0, 0);
        break;
    }
    case NSEventTypeLeftMouseDragged:
    case NSEventTypeRightMouseDragged:
    case NSEventTypeOtherMouseDragged: {
        auto button = serenity_button_from_event(event);
        unsigned buttons = 0;
        unsigned modifiers = 0;
        normalize_mouse_event(event, button, buttons, modifiers);
        if (button == 0u)
            break;
        connection->async_mouse_move(self.windowID, position, button, buttons, modifiers, 0, 0, 0, 0);
        break;
    }
    case NSEventTypeLeftMouseUp:
    case NSEventTypeRightMouseUp:
    case NSEventTypeOtherMouseUp: {
        auto button = serenity_button_from_event(event);
        unsigned buttons = static_cast<unsigned>([NSEvent pressedMouseButtons]);
        unsigned modifiers = serenity_modifiers_from_event(event.modifierFlags);
        if (button == 1u && (event.modifierFlags & NSEventModifierFlagControl) != 0)
            button = 2u;
        if (button == 0u)
            break;
        connection->async_mouse_up(self.windowID, position, button, buttons, modifiers, 0, 0, 0, 0);
        break;
    }
    case NSEventTypeScrollWheel:
        connection->async_mouse_wheel(self.windowID, position, 0, static_cast<unsigned>([NSEvent pressedMouseButtons]), serenity_modifiers_from_event(event.modifierFlags), static_cast<int>(-event.scrollingDeltaX), static_cast<int>(-event.scrollingDeltaY), static_cast<int>(-event.scrollingDeltaX), static_cast<int>(-event.scrollingDeltaY));
        break;
    case NSEventTypeKeyDown: {
        auto key = serenity_key_from_mac_key_code(event.keyCode);
        auto modifiers = serenity_modifiers_from_event(event.modifierFlags);
        u32 code_point = 0;
        auto const* utf8 = [event.charactersIgnoringModifiers UTF8String];
        if (utf8 != nullptr) {
            Utf8View utf8_view { StringView { utf8, strlen(utf8) } };
            if (!utf8_view.is_empty())
                code_point = *utf8_view.begin();
        }
        connection->async_key_down(self.windowID, code_point, static_cast<u32>(key), 0xff, modifiers, static_cast<u32>(event.keyCode));
        break;
    }
    case NSEventTypeKeyUp: {
        auto key = serenity_key_from_mac_key_code(event.keyCode);
        auto modifiers = serenity_modifiers_from_event(event.modifierFlags);
        u32 code_point = 0;
        auto const* utf8 = [event.charactersIgnoringModifiers UTF8String];
        if (utf8 != nullptr) {
            Utf8View utf8_view { StringView { utf8, strlen(utf8) } };
            if (!utf8_view.is_empty())
                code_point = *utf8_view.begin();
        }
        connection->async_key_up(self.windowID, code_point, static_cast<u32>(key), 0xff, modifiers, static_cast<u32>(event.keyCode));
        break;
    }
    case NSEventTypeMouseEntered:
        connection->async_window_entered(self.windowID);
        break;
    case NSEventTypeMouseExited:
        connection->async_window_left(self.windowID);
        break;
    default:
        break;
    }
}

- (void)windowDidBecomeKey:(NSNotification*)notification
{
    (void)notification;
    if (auto* connection = [self windowServerConnection])
        connection->async_window_activated(self.windowID);
}

- (void)windowDidResignKey:(NSNotification*)notification
{
    (void)notification;
    if (auto* connection = [self windowServerConnection])
        connection->async_window_deactivated(self.windowID);
}

- (void)windowDidResize:(NSNotification*)notification
{
    (void)notification;
    if (auto* connection = [self windowServerConnection]) {
        auto* window_view = (WindowView*)self.window.contentView;
        connection->async_window_resized(self.windowID, Loom::ns_rect_to_gfx_rect([window_view contentRect]));
    }
}

- (void)windowDidMove:(NSNotification*)notification
{
    (void)notification;
    if (auto* connection = [self windowServerConnection]) {
        auto content_rect = [self.window contentRectForFrameRect:self.window.frame];
        connection->async_window_moved(self.windowID, Loom::ns_rect_to_gfx_rect(content_rect));
    }
}

@end
