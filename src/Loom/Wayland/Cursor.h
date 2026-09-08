/*
 * Copyright (c) 2018-2023, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullOwnPtr.h>
#include <AK/RefCounted.h>
#include <AK/RefPtr.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/CursorParams.h>
#include <LibGfx/StandardCursor.h>
#include <LibWayland/Forward.h>

namespace Loom {

class Cursor : public RefCounted<Cursor> {
public:
    static RefPtr<Cursor const> create(StringView, StringView, int);
    static NonnullRefPtr<Cursor const> create(NonnullRefPtr<Gfx::Bitmap>&&, Optional<Gfx::CursorParams> = {});
    static RefPtr<Cursor const> create(Gfx::StandardCursor);

    ~Cursor();

    Gfx::CursorParams const& params() const { return m_params; }

    void set_frame(i32 frame);

    Gfx::IntRect rect() const { return m_rect; }
    Gfx::IntSize size() const { return m_rect.size(); }

    Wayland::Surface& surface() const;

private:
    Cursor(NonnullRefPtr<Gfx::Bitmap const>&&, Gfx::CursorParams const&, NonnullOwnPtr<Wayland::Surface>, NonnullOwnPtr<Wayland::Buffer>, NonnullOwnPtr<Wayland::Viewport>);

    void update_rect_if_animated();

    NonnullRefPtr<Gfx::Bitmap const> m_bitmap;
    Gfx::CursorParams m_params;
    Gfx::IntRect m_rect;

    NonnullOwnPtr<Wayland::Surface> m_surface;
    NonnullOwnPtr<Wayland::Buffer> m_buffer;
    NonnullOwnPtr<Wayland::Viewport> m_viewport;
};

}
