/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "CocoaWrapper.h"

#include <LibGfx/Font/FontDatabase.h>
#include <LibGfx/SystemTheme.h>
#include <LibGfx/Palette.h>
#include <LibCore/Environment.h>
#include <LibCore/EventLoop.h>
#include <LibCore/ResourceImplementation.h>
#include <LibCore/ResourceImplementationFile.h>
#include "Application.h"
#include "ApplicationDelegate.h"
#include "EventLoopImplementation.h"

int main(int argc, const char* argv[])
{
    Core::EventLoopManager::install(*new Mac::CFEventLoopManager);
    Core::EventLoop event_loop;

    NSApplication* app = [::Application sharedApplication];
    LoomAppDelegate* delegate = [[LoomAppDelegate alloc] init];
    [app setActivationPolicy:NSApplicationActivationPolicyRegular];
    [app setDelegate:delegate];

    (void)argc;
    (void)argv;

    auto theme_name = "Default"sv;
    auto theme = MUST(Gfx::load_system_theme(ByteString::formatted("resource://themes/{}.ini", theme_name)));
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
