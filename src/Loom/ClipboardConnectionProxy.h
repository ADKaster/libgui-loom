/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/HashMap.h>
#include <LibCore/AnonymousBuffer.h>
#include <LibIPC/ConnectionFromClient.h>
#include <Clipboard/ClipboardClientEndpoint.h>
#include <Clipboard/ClipboardServerEndpoint.h>

namespace Loom {

class ClipboardConnectionProxy final
    : public IPC::ConnectionFromClient<ClipboardClientEndpoint, ClipboardServerEndpoint> {
    C_OBJECT(ClipboardConnectionProxy);

public:
    virtual ~ClipboardConnectionProxy() override;

    static void for_each_client(Function<void(ClipboardConnectionProxy&)>);

    void notify_about_clipboard_change();

private:
    ClipboardConnectionProxy(NonnullOwnPtr<Core::LocalSocket>, int client_id);

    virtual void die() override;
    virtual Messages::ClipboardServer::GetClipboardDataResponse get_clipboard_data() override;
    virtual void set_clipboard_data(Core::AnonymousBuffer const& data, ByteString const& mime_type, HashMap<ByteString, ByteString> const& metadata) override;
};

}
