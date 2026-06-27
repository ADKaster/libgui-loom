/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ClipboardConnectionProxy.h"

#define CLIPBOARD_IPC_DEBUG 0

namespace Loom {

static HashMap<int, NonnullRefPtr<ClipboardConnectionProxy>>* s_connections;

ClipboardConnectionProxy::ClipboardConnectionProxy(NonnullOwnPtr<Core::LocalSocket> socket, int client_id)
    : IPC::ConnectionFromClient<ClipboardClientEndpoint, ClipboardServerEndpoint>(*this, move(socket), client_id)
{
    if (!s_connections)
        s_connections = new HashMap<int, NonnullRefPtr<ClipboardConnectionProxy>>;
    s_connections->set(client_id, *this);
}

ClipboardConnectionProxy::~ClipboardConnectionProxy()
{
    s_connections->remove(this->client_id());
}

void ClipboardConnectionProxy::die()
{
    dbgln_if(CLIPBOARD_IPC_DEBUG, "Clipboard IPC: die()");
}

Messages::ClipboardServer::GetClipboardDataResponse ClipboardConnectionProxy::get_clipboard_data()
{
    dbgln_if(CLIPBOARD_IPC_DEBUG, "Clipboard IPC: get_clipboard_data()");
    return nullptr;
}

void ClipboardConnectionProxy::set_clipboard_data(Core::AnonymousBuffer const& data, ByteString const& mime_type, HashMap<ByteString, ByteString> const& metadata)
{
    dbgln_if(CLIPBOARD_IPC_DEBUG, "Clipboard IPC: set_clipboard_data(data_is_valid={}, data_size={}, mime_type={}, metadata_count={})", data.is_valid(), data.size(), mime_type, metadata.size());
}

}
