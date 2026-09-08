/*
 * Copyright (c) 2018-2023, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibCore/AnonymousBuffer.h>
#include <LibGfx/ShareableBitmap.h>
#include <LibWayland/Buffer.h>
#include <LibWayland/Compositor.h>
#include <LibWayland/Display.h>
#include <LibWayland/Registry.h>
#include <LibWayland/Shm.h>
#include <LibWayland/ShmPool.h>
#include <LibWayland/Surface.h>
#include <LibWayland/Viewport.h>
#include <LibWayland/Viewporter.h>
#include <Loom/Wayland/Application.h>
#include <Loom/Wayland/Conversions.h>
#include <Loom/Wayland/Cursor.h>

namespace Loom {

Cursor::Cursor(NonnullRefPtr<Gfx::Bitmap const>&& bitmap, Gfx::CursorParams const& cursor_params, NonnullOwnPtr<Wayland::Surface> surface, NonnullOwnPtr<Wayland::Buffer> buffer, NonnullOwnPtr<Wayland::Viewport> viewport)
    : m_bitmap(move(bitmap))
    , m_params(cursor_params.constrained(*m_bitmap))
    , m_rect(m_bitmap->rect())
    , m_surface(move(surface))
    , m_buffer(move(buffer))
    , m_viewport(move(viewport))
{
    update_rect_if_animated();
}

Cursor::~Cursor() = default;

Wayland::Surface& Cursor::surface() const
{
    return *m_surface;
}

void Cursor::update_rect_if_animated()
{
    if (m_params.frames() > 1) {
        VERIFY(m_rect.width() % m_params.frames() == 0);
        m_rect.set_width(m_rect.width() / m_params.frames());
    }
}

void Cursor::set_frame(i32 frame)
{
    VERIFY(frame >= 0 && frame < static_cast<i32>(m_params.frames()));

    auto source_rect = m_rect.translated(frame * m_rect.width(), 0);

    m_viewport->set_destination(m_rect.size());
    m_viewport->set_source(source_rect);
}

NonnullRefPtr<Cursor const> Cursor::create(NonnullRefPtr<Gfx::Bitmap>&& bitmap, Optional<Gfx::CursorParams> cursor_params)
{
    if (!cursor_params.has_value()) {
        auto const hotspot = bitmap->rect().center();
        cursor_params = Gfx::CursorParams(hotspot);
    }

    premultiply_alpha_channel(*bitmap);

    auto& registry = Application::the().display().registry();
    auto& compositor = registry.compositor();
    auto& shm = registry.shm();

    auto pool = shm.create_pool(bitmap->anonymous_buffer());
    auto surface = compositor.create_surface();
    auto buffer = pool->create_buffer(bitmap->size(), bitmap->pitch(), bitmap->format());
    surface->attach(*buffer, 0, 0);
    surface->commit();
    auto viewport = registry.viewporter().get_viewport(surface.ptr());

    return adopt_ref(*new Cursor(move(bitmap), *cursor_params, move(surface), move(buffer), move(viewport)));
}

RefPtr<Cursor const> Cursor::create(StringView uri, StringView default_uri, int scale_factor)
{
    auto load_bitmap = [scale_factor](StringView path) -> RefPtr<Gfx::Bitmap> {
        auto bitmap_or_error = Gfx::Bitmap::load_from_uri(path, scale_factor);
        if (bitmap_or_error.is_error())
            return {};
        return bitmap_or_error.release_value();
    };

    auto bitmap = load_bitmap(uri);
    if (!bitmap) {
        bitmap = load_bitmap(default_uri);
    }

    if (bitmap) {
        bitmap = MUST(bitmap->to_bitmap_backed_by_anonymous_buffer());
        auto cursor_params = Gfx::CursorParams::parse_from_filename(uri, bitmap->rect().center()).constrained(*bitmap);
        return create(*bitmap, cursor_params);
    }
    return {};
}

RefPtr<Cursor const> Cursor::create(Gfx::StandardCursor standard_cursor)
{
    switch (standard_cursor) {
    case Gfx::StandardCursor::None:
        return nullptr;
    case Gfx::StandardCursor::Hidden:
        return Application::the().hidden_cursor();
    case Gfx::StandardCursor::Arrow:
        return Application::the().arrow_cursor();
    case Gfx::StandardCursor::Crosshair:
        return Application::the().crosshair_cursor();
    case Gfx::StandardCursor::IBeam:
        return Application::the().i_beam_cursor();
    case Gfx::StandardCursor::ResizeHorizontal:
        return Application::the().resize_horizontally_cursor();
    case Gfx::StandardCursor::ResizeVertical:
        return Application::the().resize_vertically_cursor();
    case Gfx::StandardCursor::ResizeDiagonalTLBR:
        return Application::the().resize_diagonally_tlbr_cursor();
    case Gfx::StandardCursor::ResizeDiagonalBLTR:
        return Application::the().resize_diagonally_bltr_cursor();
    case Gfx::StandardCursor::ResizeColumn:
        return Application::the().resize_column_cursor();
    case Gfx::StandardCursor::ResizeRow:
        return Application::the().resize_row_cursor();
    case Gfx::StandardCursor::Hand:
        return Application::the().hand_cursor();
    case Gfx::StandardCursor::Help:
        return Application::the().help_cursor();
    case Gfx::StandardCursor::Drag:
        return Application::the().drag_cursor();
    case Gfx::StandardCursor::DragCopy:
        return Application::the().drag_copy_cursor();
    case Gfx::StandardCursor::Move:
        return Application::the().move_cursor();
    case Gfx::StandardCursor::Wait:
        return Application::the().wait_cursor();
    case Gfx::StandardCursor::Disallowed:
        return Application::the().disallowed_cursor();
    case Gfx::StandardCursor::Eyedropper:
        return Application::the().eyedropper_cursor();
    case Gfx::StandardCursor::Zoom:
        return Application::the().zoom_cursor();
    default:
        VERIFY_NOT_REACHED();
    }
}

}
