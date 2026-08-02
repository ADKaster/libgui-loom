/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Backend.h"

#if LOOM_BACKEND(APPKIT)
#    include <Loom/AppKit/WindowServerConnectionProxy.h>
#else
#    error "Unknown LOOM_BACKEND"
#endif
