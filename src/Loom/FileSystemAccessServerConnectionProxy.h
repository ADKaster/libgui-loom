/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <FileSystemAccessServer/FileSystemAccessClientEndpoint.h>
#include <FileSystemAccessServer/FileSystemAccessServerEndpoint.h>
#include <LibGUI/FileTypeFilter.h>
#include <LibIPC/ConnectionFromClient.h>

namespace Loom {

class FileSystemAccessServerConnectionProxy final
    : public IPC::ConnectionFromClient<FileSystemAccessClientEndpoint, FileSystemAccessServerEndpoint> {
    C_OBJECT(FileSystemAccessServerConnectionProxy);

public:
    ~FileSystemAccessServerConnectionProxy() override = default;

    virtual void die() override;

private:
    explicit FileSystemAccessServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket>, int client_id);

    virtual void request_file_read_only_approved(i32, ByteString const&) override;
    virtual void request_file(i32, i32, i32, ByteString const&, Core::File::OpenMode) override;
    virtual void prompt_open_file(i32, i32, i32, ByteString const&, ByteString const&, Core::File::OpenMode, Optional<Vector<GUI::FileTypeFilter>> const&) override;
    virtual void prompt_save_file(i32, i32, i32, ByteString const&, ByteString const&, ByteString const&, Core::File::OpenMode) override;
    virtual Messages::FileSystemAccessServer::ExposeWindowServerClientIdResponse expose_window_server_client_id() override;
};

}
