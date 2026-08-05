/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Platform.h>
#include <xdg-shell-client.h>

namespace Loom::Wayland::Protocol {

class XdgWmBase {
    AK_MAKE_NONCOPYABLE(XdgWmBase);
    AK_MAKE_NONMOVABLE(XdgWmBase);
public:
    explicit XdgWmBase(xdg_wm_base* wm_base);
    ~XdgWmBase();

    RETURNS_NONNULL [[nodiscard]] xdg_wm_base* ptr() const { return m_wm_base; }

private:
    xdg_wm_base* m_wm_base;
};

}

