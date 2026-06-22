/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibGfx/Font/FontDatabase.h>
#include <LibGfx/SystemTheme.h>
#include <LibGfx/Palette.h>
#include <LibCore/Environment.h>
#include <LibCore/EventLoop.h>
#include <LibCore/ResourceImplementation.h>
#include <LibCore/ResourceImplementationFile.h>
#include "ApplicationDelegate.h"
#include "CocoaWrapper.h"
#include "EventLoopImplementation.h"

int main(int argc, const char* argv[])
{
    Core::EventLoopManager::install(*new Mac::CFEventLoopManager);
    Core::EventLoop event_loop;

    NSApplication* app = [NSApplication sharedApplication];
    LoomAppDelegate* delegate = [[LoomAppDelegate alloc] init];
    [app setActivationPolicy:NSApplicationActivationPolicyRegular];
    [app setDelegate:delegate];

    (void)argc;
    (void)argv;

    auto serenity_source_dir = Core::Environment::get("SERENITY_SOURCE_DIR"sv);
    if (!serenity_source_dir.has_value()) {
        warnln("SERENITY_SOURCE_DIR environment variable is not set");
        return 1;
    }

    auto resource_dir = MUST(String::formatted("{}/Base/res", serenity_source_dir.value()));

    Core::ResourceImplementation::install(make<Core::ResourceImplementationFile>(resource_dir));

    auto theme_name = "Default"sv;

    auto theme = MUST(Gfx::load_system_theme(ByteString::formatted("{}/themes/{}.ini", resource_dir, theme_name), {}));
    Gfx::set_system_theme(theme);
    // FIXME: auto palette = Gfx::PaletteImpl::create_with_anonymous_buffer(theme);

    auto default_font_query = "Katica 10 400 0"sv;
    auto fixed_width_font_query = "Csilla 10 400 0"sv;
    auto window_title_font_query = "Katica 10 700 0"sv;

    Gfx::FontDatabase::set_default_font_query(default_font_query);
    Gfx::FontDatabase::set_fixed_width_font_query(fixed_width_font_query);
    Gfx::FontDatabase::set_window_title_font_query(window_title_font_query);

    return event_loop.exec();
}
