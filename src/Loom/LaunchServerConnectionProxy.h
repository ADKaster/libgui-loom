/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/HashMap.h>
#include <LibIPC/ConnectionFromClient.h>
#include <LaunchServer/LaunchClientEndpoint.h>
#include <LaunchServer/LaunchServerEndpoint.h>

namespace Loom {

class LaunchServerConnectionProxy final
    : public IPC::ConnectionFromClient<LaunchClientEndpoint, LaunchServerEndpoint> {
    C_OBJECT(LaunchServerConnectionProxy);

public:
    virtual ~LaunchServerConnectionProxy() override;

private:
    LaunchServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket>, int client_id);

    virtual void die() override;
    virtual Messages::LaunchServer::OpenUrlResponse open_url(URL::URL const&, ByteString const&) override;
    virtual Messages::LaunchServer::GetHandlersForUrlResponse get_handlers_for_url(URL::URL const&) override;
    virtual Messages::LaunchServer::GetHandlersWithDetailsForUrlResponse get_handlers_with_details_for_url(URL::URL const&) override;
    virtual void add_allowed_url(URL::URL const&) override;
    virtual void add_allowed_handler_with_any_url(ByteString const&) override;
    virtual void add_allowed_handler_with_only_specific_urls(ByteString const&, Vector<URL::URL> const&) override;
    virtual void seal_allowlist() override;
};

}
