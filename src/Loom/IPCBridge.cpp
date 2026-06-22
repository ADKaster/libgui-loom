/*
* Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "IPCBridge.h"
#include <LibCore/Socket.h>
#include <LibCore/Directory.h>

namespace Loom
{

IPCBridge::IPCBridge(NonnullOwnPtr<IPC::MultiServer<WindowServerConnectionProxy>> window_server, NonnullOwnPtr<IPC::MultiServer<ClipboardConnectionProxy>> clipboard_server)
    : m_window_server(move(window_server))
    , m_clipboard_server(move(clipboard_server))
{
}

IPCBridge::~IPCBridge() = default;

// This is heavily based on how SystemServer's Service creates its socket.
// This method is also copied from Core::IPCProcess
static ErrorOr<int> create_ipc_socket(ByteString const& socket_path)
{
    if (!Core::System::stat(socket_path).is_error())
        TRY(Core::System::unlink(socket_path));

#ifdef SOCK_NONBLOCK
    auto socket_fd = TRY(Core::System::socket(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0));
#else
    auto socket_fd = TRY(Core::System::socket(AF_LOCAL, SOCK_STREAM, 0));

    int option = 1;
    TRY(Core::System::ioctl(socket_fd, FIONBIO, &option));
    TRY(Core::System::fcntl(socket_fd, F_SETFD, FD_CLOEXEC));
#endif

#if !defined(AK_OS_BSD_GENERIC) && !defined(AK_OS_GNU_HURD)
    TRY(Core::System::fchmod(socket_fd, 0600));
#endif

    auto socket_address = Core::SocketAddress::local(socket_path);
    auto socket_address_un = socket_address.to_sockaddr_un().release_value();

    TRY(Core::System::bind(socket_fd, reinterpret_cast<sockaddr*>(&socket_address_un), sizeof(socket_address_un)));
    TRY(Core::System::listen(socket_fd, 16));

    return socket_fd;
}

NonnullOwnPtr<IPCBridge> IPCBridge::create()
{
    MUST(Core::Directory::create("/tmp/portal"sv, Core::Directory::CreateDirectories::Yes));
    MUST(Core::Directory::create("/tmp/session/0/portal"sv, Core::Directory::CreateDirectories::Yes));

    auto window_server_server = MUST(Core::LocalServer::try_create());
    auto const window_server_socket = MUST(create_ipc_socket("/tmp/portal/window"sv));
    MUST(window_server_server->take_over_fd(window_server_socket));
    auto window_server = MUST(IPC::MultiServer<WindowServerConnectionProxy>::try_create(move(window_server_server)));

    auto clipboard_server_server = MUST(Core::LocalServer::try_create());
    auto const clipboard_server_socket = MUST(create_ipc_socket("/tmp/session/0/portal/clipboard"sv));
    MUST(clipboard_server_server->take_over_fd(clipboard_server_socket));
    auto clipboard_server = MUST(IPC::MultiServer<ClipboardConnectionProxy>::try_create(move(clipboard_server_server)));

    return adopt_own(*new IPCBridge(move(window_server), move(clipboard_server)));
}

}
