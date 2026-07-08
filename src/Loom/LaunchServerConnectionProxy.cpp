/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "LaunchServerConnectionProxy.h"

#define LAUNCHSERVER_IPC_DEBUG 0

namespace Loom {

static HashMap<int, NonnullRefPtr<LaunchServerConnectionProxy>>* s_connections;

LaunchServerConnectionProxy::LaunchServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket> socket, int client_id)
    : IPC::ConnectionFromClient<LaunchClientEndpoint, LaunchServerEndpoint>(*this, move(socket), client_id)
{
    if (!s_connections)
        s_connections = new HashMap<int, NonnullRefPtr<LaunchServerConnectionProxy>>;
    s_connections->set(client_id, *this);
}

LaunchServerConnectionProxy::~LaunchServerConnectionProxy() = default;

void LaunchServerConnectionProxy::die()
{
    dbgln_if(LAUNCHSERVER_IPC_DEBUG, "LaunchServer IPC: die()");
    s_connections->remove(this->client_id());
}

Messages::LaunchServer::OpenUrlResponse LaunchServerConnectionProxy::open_url(URL::URL const& url, ByteString const& handler_name)
{
    dbgln_if(LAUNCHSERVER_IPC_DEBUG, "LaunchServer IPC: open_url(url={}, handler_name={})", url, handler_name);
    return false;
}

Messages::LaunchServer::GetHandlersForUrlResponse LaunchServerConnectionProxy::get_handlers_for_url(URL::URL const& url)
{
    dbgln_if(LAUNCHSERVER_IPC_DEBUG, "LaunchServer IPC: get_handlers_for_url(url={})", url);
    return Vector<ByteString> {};
}

Messages::LaunchServer::GetHandlersWithDetailsForUrlResponse LaunchServerConnectionProxy::get_handlers_with_details_for_url(URL::URL const& url)
{
    dbgln_if(LAUNCHSERVER_IPC_DEBUG, "LaunchServer IPC: get_handlers_with_details_for_url(url={})", url);
    return Vector<ByteString> {};
}

void LaunchServerConnectionProxy::add_allowed_url(URL::URL const& url)
{
    dbgln_if(LAUNCHSERVER_IPC_DEBUG, "LaunchServer IPC: add_allowed_url(url={})", url);
}

void LaunchServerConnectionProxy::add_allowed_handler_with_any_url(ByteString const& handler_name)
{
    dbgln_if(LAUNCHSERVER_IPC_DEBUG, "LaunchServer IPC: add_allowed_handler_with_any_url(handler_name={})", handler_name);
}

void LaunchServerConnectionProxy::add_allowed_handler_with_only_specific_urls(ByteString const& handler_name, Vector<URL::URL> const& urls)
{
    dbgln_if(LAUNCHSERVER_IPC_DEBUG, "LaunchServer IPC: add_allowed_handler_with_only_specific_urls(handler_name={}, urls={})", handler_name, urls);
}

void LaunchServerConnectionProxy::seal_allowlist()
{
    dbgln_if(LAUNCHSERVER_IPC_DEBUG, "LaunchServer IPC: seal_allowlist()");
}

}
