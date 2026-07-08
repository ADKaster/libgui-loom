/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <LibIPC/MultiServer.h>
#include <Loom/ClipboardConnectionProxy.h>
#include <Loom/LaunchServerConnectionProxy.h>
#include <Loom/WindowServerConnectionProxy.h>

namespace Loom {
class IPCBridge
{
    AK_MAKE_NONCOPYABLE(IPCBridge);
    AK_MAKE_NONMOVABLE(IPCBridge);
public:

    static NonnullOwnPtr<IPCBridge> create(NonnullRefPtr<WindowServerCallbacks>);

    ~IPCBridge();

private:
    IPCBridge(NonnullRefPtr<WindowServerCallbacks>, NonnullOwnPtr<IPC::MultiServer<WindowServerConnectionProxy>>, NonnullOwnPtr<IPC::MultiServer<ClipboardConnectionProxy>>, NonnullOwnPtr<IPC::MultiServer<LaunchServerConnectionProxy>>);

    NonnullRefPtr<WindowServerCallbacks> m_window_server_callbacks;
    NonnullOwnPtr<IPC::MultiServer<WindowServerConnectionProxy>> m_window_server;
    NonnullOwnPtr<IPC::MultiServer<ClipboardConnectionProxy>> m_clipboard_server;
    NonnullOwnPtr<IPC::MultiServer<LaunchServerConnectionProxy>> m_launch_server;
};

}
