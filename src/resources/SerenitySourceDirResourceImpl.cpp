/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/String.h>
#include <LibCore/Environment.h>
#include <LibCore/ResourceImplementationFile.h>

[[gnu::used]] static bool s_resources_initialized = []{
    auto serenity_source_dir = Core::Environment::get("SERENITY_SOURCE_DIR"sv);
    if (!serenity_source_dir.has_value()) {
        warnln("SERENITY_SOURCE_DIR environment variable is not set");
        return false;
    }
    auto resource_dir = MUST(String::formatted("{}/Base/res", serenity_source_dir.value()));
    Core::ResourceImplementation::install(make<Core::ResourceImplementationFile>(resource_dir));
    return true;
}();
