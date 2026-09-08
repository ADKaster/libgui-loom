/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/Wayland/Cursor.h>
#include <Loom/Wayland/Events.h>
#include <Loom/Wayland/SeatDelegate.h>
#include <Loom/Wayland/Window.h>
#include <Loom/Wayland/WindowServerConnectionProxy.h>
#include <Kernel/API/KeyCode.h>
#include <LibGUI/Event.h>
#include <LibWayland/Pointer.h>
#include <xkbcommon/xkbcommon.h>

namespace Loom {

static bool is_keypad_key(xkb_keysym_t key)
{
    switch (key) {
    case XKB_KEY_KP_0:
    case XKB_KEY_KP_1:
    case XKB_KEY_KP_2:
    case XKB_KEY_KP_3:
    case XKB_KEY_KP_4:
    case XKB_KEY_KP_5:
    case XKB_KEY_KP_6:
    case XKB_KEY_KP_7:
    case XKB_KEY_KP_8:
    case XKB_KEY_KP_9:
    case XKB_KEY_KP_Begin:
    case XKB_KEY_KP_Decimal:
    case XKB_KEY_KP_Separator:
    case XKB_KEY_KP_Divide:
    case XKB_KEY_KP_Enter:
    case XKB_KEY_KP_Equal:
    case XKB_KEY_KP_Subtract:
    case XKB_KEY_KP_Multiply:
    case XKB_KEY_KP_Add:
    case XKB_KEY_KP_Home:
    case XKB_KEY_KP_Up:
    case XKB_KEY_KP_Page_Up:
    case XKB_KEY_KP_Left:
    case XKB_KEY_KP_Right:
    case XKB_KEY_KP_End:
    case XKB_KEY_KP_Down:
    case XKB_KEY_KP_Page_Down:
    case XKB_KEY_KP_Insert:
    case XKB_KEY_KP_Delete:
    case XKB_KEY_KP_Space:
    case XKB_KEY_KP_Tab:
        return true;
    default:
        return false;
    }
}

static KeyCode translate_key_code(u32 key_sym, KeyModifier& modifiers)
{
    auto xkb_key = static_cast<xkb_keysym_t>(key_sym);
    if (is_keypad_key(xkb_key))
        modifiers |= Mod_Keypad;

    switch (xkb_key) {
    case XKB_KEY_Escape:
        return Key_Escape;
    case XKB_KEY_Tab:
        return Key_Tab;
    case XKB_KEY_BackSpace:
        return Key_Backspace;
    case XKB_KEY_Return:
        return Key_Return;
    case XKB_KEY_Insert:
        return Key_Insert;
    case XKB_KEY_Delete:
        return Key_Delete;
    case XKB_KEY_Print:
        return Key_PrintScreen;
    case XKB_KEY_Pause:
        return Key_PauseBreak;
    case XKB_KEY_Sys_Req:
        return Key_SysRq;
    case XKB_KEY_Home:
        return Key_Home;
    case XKB_KEY_End:
        return Key_End;
    case XKB_KEY_Left:
        return Key_Left;
    case XKB_KEY_Up:
        return Key_Up;
    case XKB_KEY_Right:
        return Key_Right;
    case XKB_KEY_Down:
        return Key_Down;
    case XKB_KEY_Page_Up:
        return Key_PageUp;
    case XKB_KEY_Page_Down:
        return Key_PageDown;
    case XKB_KEY_Shift_L:
        return Key_LeftShift;
    case XKB_KEY_Shift_R:
        return Key_RightShift;
    case XKB_KEY_Control_L:
        return Key_LeftControl;
    case XKB_KEY_Control_R:
        return Key_RightControl;
    case XKB_KEY_Alt_L:
        return Key_LeftAlt;
    case XKB_KEY_Alt_R:
        return Key_RightAlt;
    case XKB_KEY_ISO_Level3_Shift:
        return Key_AltGr;
    case XKB_KEY_Caps_Lock:
        return Key_CapsLock;
    case XKB_KEY_Num_Lock:
        return Key_NumLock;
    case XKB_KEY_Scroll_Lock:
        return Key_ScrollLock;
    case XKB_KEY_F1:
        return Key_F1;
    case XKB_KEY_F2:
        return Key_F2;
    case XKB_KEY_F3:
        return Key_F3;
    case XKB_KEY_F4:
        return Key_F4;
    case XKB_KEY_F5:
        return Key_F5;
    case XKB_KEY_F6:
        return Key_F6;
    case XKB_KEY_F7:
        return Key_F7;
    case XKB_KEY_F8:
        return Key_F8;
    case XKB_KEY_F9:
        return Key_F9;
    case XKB_KEY_F10:
        return Key_F10;
    case XKB_KEY_F11:
        return Key_F11;
    case XKB_KEY_F12:
        return Key_F12;
    case XKB_KEY_Super_L:
        return Key_LeftSuper;
    case XKB_KEY_Super_R:
        return Key_RightSuper;
    case XKB_KEY_Menu:
    case XKB_KEY_XF86MenuKB:
        return Key_Menu;
    case XKB_KEY_XF86Search:
        return Key_BrowserSearch;
    case XKB_KEY_XF86Favorites:
        return Key_BrowserFavorites;
    case XKB_KEY_XF86HomePage:
        return Key_BrowserHome;
    case XKB_KEY_XF86AudioPrev:
        return Key_PreviousTrack;
    case XKB_KEY_XF86Back:
        return Key_BrowserBack;
    case XKB_KEY_XF86Forward:
        return Key_BrowserForward;
    case XKB_KEY_XF86Refresh:
        return Key_BrowserRefresh;
    case XKB_KEY_XF86Stop:
        return Key_BrowserStop;
    case XKB_KEY_XF86AudioLowerVolume:
        return Key_VolumeDown;
    case XKB_KEY_XF86AudioRaiseVolume:
        return Key_VolumeUp;
    case XKB_KEY_XF86WakeUp:
        return Key_Wake;
    case XKB_KEY_XF86Sleep:
        return Key_Sleep;
    case XKB_KEY_XF86AudioNext:
        return Key_NextTrack;
    case XKB_KEY_XF86AudioMedia:
        return Key_MediaSelect;
    case XKB_KEY_XF86Mail:
        return Key_Email;
    case XKB_KEY_XF86MyComputer:
        return Key_MyComputer;
    case XKB_KEY_XF86PowerOff:
        return Key_Power;
    case XKB_KEY_XF86AudioStop:
        return Key_Stop;
    case XKB_KEY_XF86AudioMute:
        return Key_Mute;
    case XKB_KEY_XF86Calculator:
        return Key_Calculator;
    case XKB_KEY_XF86AudioPlay:
    case XKB_KEY_XF86AudioPause:
        return Key_PlayPause;
    case XKB_KEY_KP_0:
        return Key_0;
    case XKB_KEY_KP_End:
        return Key_End;
    case XKB_KEY_KP_Down:
        return Key_Down;
    case XKB_KEY_KP_Page_Down:
        return Key_PageDown;
    case XKB_KEY_KP_Left:
        return Key_Left;
    case XKB_KEY_KP_Right:
        return Key_Right;
    case XKB_KEY_KP_Home:
        return Key_Home;
    case XKB_KEY_KP_Up:
        return Key_Up;
    case XKB_KEY_KP_Page_Up:
        return Key_PageUp;
    case XKB_KEY_KP_1:
        return Key_1;
    case XKB_KEY_KP_2:
        return Key_2;
    case XKB_KEY_KP_3:
        return Key_3;
    case XKB_KEY_KP_4:
        return Key_4;
    case XKB_KEY_KP_5:
        return Key_5;
    case XKB_KEY_KP_6:
        return Key_6;
    case XKB_KEY_KP_7:
        return Key_7;
    case XKB_KEY_KP_8:
        return Key_8;
    case XKB_KEY_KP_9:
        return Key_9;
    case XKB_KEY_KP_Delete:
        return Key_Delete;
    case XKB_KEY_KP_Separator:
        return Key_Comma;
    case XKB_KEY_KP_Divide:
        return Key_Slash;
    case XKB_KEY_KP_Enter:
        return Key_Return;
    case XKB_KEY_KP_Equal:
        return Key_Equal;
    case XKB_KEY_KP_Subtract:
        return Key_Minus;
    case XKB_KEY_KP_Multiply:
        return Key_Asterisk;
    case XKB_KEY_KP_Add:
        return Key_Plus;
    case XKB_KEY_KP_Insert:
        return Key_Insert;
    case XKB_KEY_KP_Begin:
        return Key_Invalid;
    case XKB_KEY_KP_Space:
        return Key_Space;
    case XKB_KEY_KP_Tab:
        return Key_Tab;
    default:
        return code_point_to_key_code(key_sym);
    }
}

static KeyModifier translate_key_modifiers(Wayland::KeyboardModifier modifiers)
{
    KeyModifier key_modifiers = Mod_None;
    if (has_flag(modifiers, Wayland::KeyboardModifier::Alt))
        key_modifiers |= Mod_Alt;
    if (has_flag(modifiers, Wayland::KeyboardModifier::Ctrl))
        key_modifiers |= Mod_Ctrl;
    if (has_flag(modifiers, Wayland::KeyboardModifier::Shift))
        key_modifiers |= Mod_Shift;
    if (has_flag(modifiers, Wayland::KeyboardModifier::Super))
        key_modifiers |= Mod_Super;
    if (has_flag(modifiers, Wayland::KeyboardModifier::AltGr))
        key_modifiers |= Mod_AltGr;
    return key_modifiers;
}

static GUI::MouseButton translate_mouse_button(Wayland::RawMouseButton button)
{
    // FIXME: Query OS about whether the primary/secondary mouse buttons are swapped, and return the correct button here.
    switch (button) {
    case Wayland::RawMouseButton::Left:
        return GUI::MouseButton::Primary;
    case Wayland::RawMouseButton::Right:
        return GUI::MouseButton::Secondary;
    case Wayland::RawMouseButton::Middle:
        return GUI::MouseButton::Middle;
    case Wayland::RawMouseButton::Backward:
        return GUI::MouseButton::Backward;
    case Wayland::RawMouseButton::Forward:
        return GUI::MouseButton::Forward;
    default:
        return GUI::MouseButton::None;
    }
}

void SeatDelegate::register_surface_owner(Wayland::Surface& surface, Window& window)
{
    m_surface_owners.set(&surface, &window);
}

void SeatDelegate::unregister_surface_owner(Wayland::Surface& surface)
{
    m_surface_owners.remove(&surface);
}

Window* SeatDelegate::get_window(Wayland::Surface* surface)
{
    if (!surface)
        return nullptr;
    auto window = m_surface_owners.get(surface);
    if (!window.has_value())
        return nullptr;
    return *window;
}

SeatDelegate::SeatDelegate()
{
    on_pointer_enter = [this](Wayland::Pointer&, Wayland::Surface* surface, u32 serial, Gfx::IntPoint position) {
        m_last_pointer_serial = serial;
        m_pointer_position = position;
        if (auto* window = get_window(surface))
            window->client().async_window_entered(window->window_id());
    };

    on_pointer_leave = [this](Wayland::Pointer&, Wayland::Surface* surface, u32 serial) {
        m_last_pointer_serial = serial;
        if (auto* window = get_window(surface))
            window->client().async_window_left(window->window_id());
    };

    on_pointer_button = [this](Wayland::Pointer& pointer, Wayland::Surface* surface, u32 serial, Duration, Wayland::RawMouseButton button, Wayland::MouseButtonState button_state) {
        m_last_pointer_serial = serial;
        auto* window = get_window(surface);
        if (!window)
            return;

        auto gui_button = translate_mouse_button(button);
        MouseEvent::Type type;

        if (button_state == Wayland::MouseButtonState::Pressed) {
            m_mouse_buttons |= to_underlying(gui_button);
            type = MouseEvent::Type::MouseDown;
        }
        else {
            VERIFY(button_state == Wayland::MouseButtonState::Released);
            m_mouse_buttons &= ~to_underlying(gui_button);
            type = MouseEvent::Type::MouseUp;
        }

        if (auto cursor = window->handle_mouse_event(MouseEvent(type, m_pointer_position, to_underlying(gui_button), m_mouse_buttons, m_key_modifiers)))
            pointer.set_cursor(serial, &cursor->surface(), cursor->params().hotspot());
        else
            pointer.set_cursor(serial, nullptr, {});
    };

    on_pointer_motion = [this](Wayland::Pointer& pointer, Wayland::Surface* surface, Duration, Gfx::IntPoint position) {
        m_pointer_position = position;
        auto* window = get_window(surface);
        if (!window)
            return;

        if (auto cursor = window->handle_mouse_event(MouseEvent(MouseEvent::Type::MouseMove, m_pointer_position, GUI::MouseButton::None, m_mouse_buttons, m_key_modifiers)))
            pointer.set_cursor(m_last_pointer_serial, &cursor->surface(), cursor->params().hotspot());
        else
            pointer.set_cursor(m_last_pointer_serial, nullptr, {});
    };

    on_pointer_axis = [](Wayland::Pointer&) {
      // FIXME: Do the scroll dance
    };

    on_keyboard_enter = [](Wayland::Keyboard&, Wayland::Surface*, u32) {
        // No-op
    };

    on_keyboard_leave = [](Wayland::Keyboard&, Wayland::Surface*, u32) {
        // No-op
    };

    on_keyboard_key = [this](Wayland::Keyboard&, Wayland::Surface* surface, u32, Duration, u32 raw_key, Wayland::KeyState key_state, u32 key_sym, u32 code_point, Wayland::KeyboardModifier modifiers) {
        auto* window = get_window(surface);
        if (!window)
            return;

        auto key_modifiers = translate_key_modifiers(modifiers);
        auto key_code = translate_key_code(key_sym, key_modifiers);

        if (key_state == Wayland::KeyState::Pressed || key_state == Wayland::KeyState::Repeated)
            window->client().async_key_down(window->window_id(), code_point, key_code, 0xFF, key_modifiers, raw_key);
        else if (key_state == Wayland::KeyState::Released)
            window->client().async_key_up(window->window_id(), code_point, key_code, 0xFF, key_modifiers, raw_key);
    };
}

}
