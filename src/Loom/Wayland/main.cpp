/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Error.h>
#include <AK/Format.h>
#include <LibMain/Main.h>
#include <Loom/Wayland/Application.h>

ErrorOr<int> serenity_main(Main::Arguments arguments)
{
    AK::set_rich_debug_enabled(true);

    if (!Loom::Application::request_single_instance())
        return 0;

    Loom::Application app;

    TRY(app.initialize(arguments));

   return app.exec();
}
