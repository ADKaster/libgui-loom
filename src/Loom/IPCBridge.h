/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <LibIPC/MultiServer.h>
#include <Loom/WindowServerConnectionProxy.h>

namespace Loom {
class IPCBridge
{
    AK_MAKE_NONCOPYABLE(IPCBridge);
    AK_MAKE_NONMOVABLE(IPCBridge);
public:

    static NonnullOwnPtr<IPCBridge> create();

    ~IPCBridge();

private:
    IPCBridge(NonnullOwnPtr<IPC::MultiServer<WindowServerConnectionProxy>>);

    NonnullOwnPtr<IPC::MultiServer<WindowServerConnectionProxy>> m_window_server;
};

}
