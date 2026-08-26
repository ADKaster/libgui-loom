/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibMain/Main.h>
#include <Loom/Wayland/Application.h>
#include <Loom/Wayland/Display.h>
#include <Loom/Wayland/Protocol/Buffer.h>
#include <Loom/Wayland/Protocol/Compositor.h>
#include <Loom/Wayland/Protocol/Registry.h>
#include <Loom/Wayland/Protocol/Shm.h>
#include <Loom/Wayland/Protocol/ShmPool.h>
#include <Loom/Wayland/Protocol/Surface.h>
#include <Loom/Wayland/Protocol/XdgSurface.h>
#include <Loom/Wayland/Protocol/XdgToplevel.h>
#include <Loom/Wayland/Protocol/XdgWmBase.h>

using namespace Loom::Wayland;

ErrorOr<int> serenity_main(Main::Arguments arguments)
{
    AK::set_rich_debug_enabled(true);

    if (!Loom::Application::request_single_instance())
        return 0;

    Loom::Application app;

    TRY(app.initialize(arguments));

    auto& registry = app.display().registry();

    auto surface = registry.compositor().create_surface();
    auto xdg_surface = registry.wm_base().get_xdg_surface(move(surface));
    auto xdg_toplevel = Protocol::XdgSurface::get_xdg_toplevel(move(xdg_surface));

    auto window_size = Gfx::IntSize { 200, 200 };
    auto buf = MUST(Core::AnonymousBuffer::create_with_size(window_size.area() * 4));
    auto shm_pool = registry.shm().create_pool(buf);
    auto buffer = shm_pool->create_buffer(window_size, Gfx::BitmapFormat::BGRA8888);

    auto bitmap = MUST(Gfx::Bitmap::create_with_anonymous_buffer(Gfx::BitmapFormat::BGRA8888, move(buf), window_size, 1));

    bitmap->fill(Color::Magenta);

    xdg_toplevel->surface().surface().commit();

    xdg_toplevel->surface().on_configure = [&]() {
        xdg_toplevel->surface().surface().attach(move(buffer), 0, 0);
        xdg_toplevel->surface().surface().commit();
        xdg_toplevel->surface().on_configure = nullptr;
    };

   return app.exec();
}
