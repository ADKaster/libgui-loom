/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#ifdef LOOM_BACKEND_APPKIT
#    define LOOM_BACKEND_IS_APPKIT() 1
#else
#    define LOOM_BACKEND_IS_APPKIT() 0
#endif

#define LOOM_BACKEND(IMPL) (LOOM_BACKEND_IS_##IMPL())
