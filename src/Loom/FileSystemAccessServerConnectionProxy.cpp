/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Loom/FileSystemAccessServerConnectionProxy.h>

namespace Loom {

#define FSAS_IPC_DEBUG 1

static HashMap<int, NonnullRefPtr<FileSystemAccessServerConnectionProxy>>* s_connections;

FileSystemAccessServerConnectionProxy::FileSystemAccessServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket> socket, int client_id)
    : IPC::ConnectionFromClient<FileSystemAccessClientEndpoint, FileSystemAccessServerEndpoint>(*this, move(socket), client_id)
{
    if (!s_connections)
        s_connections = new HashMap<int, NonnullRefPtr<FileSystemAccessServerConnectionProxy>>;
    s_connections->set(client_id, *this);
}

void FileSystemAccessServerConnectionProxy::die()
{
    dbgln_if(FSAS_IPC_DEBUG, "FileSystemAccessServer IPC: die()");
    s_connections->remove(this->client_id());
}

void FileSystemAccessServerConnectionProxy::request_file_read_only_approved(i32 request_id, ByteString const& path)
{
    dbgln_if(FSAS_IPC_DEBUG, "FileSystemAccessServer IPC: request_file_read_only_approved(request_id={}, path={})", request_id, path);

    auto fd = Core::File::open(path, Core::File::OpenMode::Read);

    if (fd.is_error())
        return async_handle_prompt_end(request_id, fd.error().code(), {}, {});

    return async_handle_prompt_end(request_id, 0, IPC::File::adopt_file(fd.release_value()), path);
}

void FileSystemAccessServerConnectionProxy::request_file(i32 request_id, i32 window_server_client_id, i32 parent_window_id, ByteString const& path, Core::File::OpenMode requested_access)
{
    dbgln_if(FSAS_IPC_DEBUG, "FileSystemAccessServer IPC: request_file(request_id={}, window_server_client_id={}, parent_window_id={}, path={}, requested_access={})", request_id, window_server_client_id, parent_window_id, path, (int)requested_access);
    (void)window_server_client_id;
    (void)parent_window_id;

    auto file = Core::File::open(path, requested_access);

    if (file.is_error())
        return async_handle_prompt_end(request_id, file.error().code(), {}, {});

    return async_handle_prompt_end(request_id, 0, IPC::File::adopt_file(file.release_value()), path);
}

void FileSystemAccessServerConnectionProxy::prompt_open_file(i32 request_id, i32, i32, ByteString const&, ByteString const&, Core::File::OpenMode, Optional<Vector<GUI::FileTypeFilter>> const&)
{
    dbgln_if(FSAS_IPC_DEBUG, "FileSystemAccessServer IPC: prompt_open_file(request_id={})", request_id);
    return async_handle_prompt_end(request_id, EPERM, {}, {});
}

void FileSystemAccessServerConnectionProxy::prompt_save_file(i32 request_id, i32, i32, ByteString const&, ByteString const&, ByteString const&, Core::File::OpenMode)
{
    dbgln_if(FSAS_IPC_DEBUG, "FileSystemAccessServer IPC: prompt_save_file(request_id={})", request_id);
    return async_handle_prompt_end(request_id, EPERM, {}, {});
}

Messages::FileSystemAccessServer::ExposeWindowServerClientIdResponse FileSystemAccessServerConnectionProxy::expose_window_server_client_id()
{
    dbgln_if(FSAS_IPC_DEBUG, "FileSystemAccessServer IPC: expose_window_server_client_id()");
    return -1;
}

}
