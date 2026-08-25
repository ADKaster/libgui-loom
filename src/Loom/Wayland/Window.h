/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <Loom/Wayland/Protocol/XdgToplevel.h>

namespace Loom::Wayland {

class Display;

class Window {
    AK_MAKE_NONCOPYABLE(Window);
    AK_MAKE_DEFAULT_MOVABLE(Window);
public:
    static NonnullOwnPtr<Window> create(Display&);

    ~Window();

private:
    explicit Window(NonnullOwnPtr<Protocol::XdgToplevel>);

    NonnullOwnPtr<Protocol::XdgToplevel> m_toplevel;
};

}
