/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullRefPtr.h>
#include <AK/OwnPtr.h>
#include <AK/RefPtr.h>
#include <LibCore/Forward.h>
#include <LibGfx/Palette.h>
#include <LibWayland/Forward.h>
#include <Loom/Wayland/Cursor.h>
#include <LibMain/Main.h>

namespace DBus {
class ObjectRegistration;
}

namespace WindowServer {
class ScreenLayout;
class SystemEffects;
}

namespace Loom {

class IPCBridge;
class SeatDelegate;

class Application {
public:
    Application();
    ~Application();

    static Application& the();

    [[nodiscard]] static bool request_single_instance();

    ErrorOr<void> initialize(Main::Arguments);
    int exec();

    [[nodiscard]] Wayland::Display& display() { return *m_display; }
    [[nodiscard]] SeatDelegate& seat_delegate();
    [[nodiscard]] StringView app_id() const;

    [[nodiscard]] Gfx::Palette palette() const { return Palette(*m_palette_impl); }
    [[nodiscard]] WindowServer::ScreenLayout screen_layout() const;
    [[nodiscard]] WindowServer::SystemEffects& system_effects() const;

    ErrorOr<void> load_cursor_theme(StringView theme_name);

    [[nodiscard]] Cursor const& hidden_cursor() const { return *m_hidden_cursor; }
    [[nodiscard]] Cursor const& arrow_cursor() const { return *m_arrow_cursor; }
    [[nodiscard]] Cursor const& crosshair_cursor() const { return *m_crosshair_cursor; }
    [[nodiscard]] Cursor const& hand_cursor() const { return *m_hand_cursor; }
    [[nodiscard]] Cursor const& help_cursor() const { return *m_help_cursor; }
    [[nodiscard]] Cursor const& resize_horizontally_cursor() const { return *m_resize_horizontally_cursor; }
    [[nodiscard]] Cursor const& resize_vertically_cursor() const { return *m_resize_vertically_cursor; }
    [[nodiscard]] Cursor const& resize_diagonally_tlbr_cursor() const { return *m_resize_diagonally_tlbr_cursor; }
    [[nodiscard]] Cursor const& resize_diagonally_bltr_cursor() const { return *m_resize_diagonally_bltr_cursor; }
    [[nodiscard]] Cursor const& resize_column_cursor() const { return *m_resize_column_cursor; }
    [[nodiscard]] Cursor const& resize_row_cursor() const { return *m_resize_row_cursor; }
    [[nodiscard]] Cursor const& i_beam_cursor() const { return *m_i_beam_cursor; }
    [[nodiscard]] Cursor const& disallowed_cursor() const { return *m_disallowed_cursor; }
    [[nodiscard]] Cursor const& move_cursor() const { return *m_move_cursor; }
    [[nodiscard]] Cursor const& drag_cursor() const { return *m_drag_cursor; }
    [[nodiscard]] Cursor const& drag_copy_cursor() const { return *m_drag_copy_cursor; }
    [[nodiscard]] Cursor const& wait_cursor() const { return *m_wait_cursor; }
    [[nodiscard]] Cursor const& eyedropper_cursor() const { return *m_eyedropper_cursor; }
    [[nodiscard]] Cursor const& zoom_cursor() const { return *m_zoom_cursor; }

private:
    void register_dbus_handlers();

    RefPtr<Gfx::PaletteImpl> m_palette_impl;
    OwnPtr<Wayland::Display> m_display;
    OwnPtr<SeatDelegate> m_seat_delegate;
    OwnPtr<IPCBridge> m_ipc_bridge;
    OwnPtr<Core::EventLoop> m_event_loop;
    OwnPtr<DBus::ObjectRegistration> m_dbus_interface_registration;

    RefPtr<Cursor const> m_hidden_cursor;
    RefPtr<Cursor const> m_arrow_cursor;
    RefPtr<Cursor const> m_hand_cursor;
    RefPtr<Cursor const> m_help_cursor;
    RefPtr<Cursor const> m_resize_horizontally_cursor;
    RefPtr<Cursor const> m_resize_vertically_cursor;
    RefPtr<Cursor const> m_resize_diagonally_tlbr_cursor;
    RefPtr<Cursor const> m_resize_diagonally_bltr_cursor;
    RefPtr<Cursor const> m_resize_column_cursor;
    RefPtr<Cursor const> m_resize_row_cursor;
    RefPtr<Cursor const> m_i_beam_cursor;
    RefPtr<Cursor const> m_disallowed_cursor;
    RefPtr<Cursor const> m_move_cursor;
    RefPtr<Cursor const> m_drag_cursor;
    RefPtr<Cursor const> m_drag_copy_cursor;
    RefPtr<Cursor const> m_wait_cursor;
    RefPtr<Cursor const> m_crosshair_cursor;
    RefPtr<Cursor const> m_eyedropper_cursor;
    RefPtr<Cursor const> m_zoom_cursor;
};

}
