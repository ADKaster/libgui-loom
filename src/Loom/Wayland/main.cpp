/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/IPCBridge.h>

#include <LibCore/EventLoop.h>

int main(int argc, char const* argv[])
{
    Core::EventLoop event_loop;

    auto ipc_bridge = Loom::IPCBridge::create();

    (void)argc;
    (void)argv;

    return event_loop.exec();
}
