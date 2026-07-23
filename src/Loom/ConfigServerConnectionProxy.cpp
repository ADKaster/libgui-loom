/*
 * Copyright (c) 2026, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ConfigServerConnectionProxy.h"

#include <AK/HashMap.h>

#define CONFIGSERVER_IPC_DEBUG 0

namespace Loom {

static HashMap<int, NonnullRefPtr<ConfigServerConnectionProxy>>* s_connections;

ConfigServerConnectionProxy::ConfigServerConnectionProxy(NonnullOwnPtr<Core::LocalSocket> socket, int client_id)
    : IPC::ConnectionFromClient<ConfigClientEndpoint, ConfigServerEndpoint>(*this, move(socket), client_id)
{
    if (!s_connections)
        s_connections = new HashMap<int, NonnullRefPtr<ConfigServerConnectionProxy>>;
    s_connections->set(client_id, *this);
}

void ConfigServerConnectionProxy::die()
{
    s_connections->remove(client_id());
}

void ConfigServerConnectionProxy::pledge_domains(Vector<ByteString> const& domains)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: pledge_domains(domains={})", domains);
}

void ConfigServerConnectionProxy::enable_permissive_mode()
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: enable_permissive_mode()");
}

void ConfigServerConnectionProxy::monitor_domain(ByteString const& domain)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: monitor_domain(domain={})", domain);
}

Messages::ConfigServer::ListConfigGroupsResponse ConfigServerConnectionProxy::list_config_groups(ByteString const& domain)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: list_config_groups(domain={})", domain);
    return { Vector<ByteString> {} };
}

Messages::ConfigServer::ListConfigKeysResponse ConfigServerConnectionProxy::list_config_keys(ByteString const& domain, ByteString const& group)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: list_config_keys(domain={}, group={})", domain, group);
    return { Vector<ByteString> {} };
}

Messages::ConfigServer::ReadStringValueResponse ConfigServerConnectionProxy::read_string_value(ByteString const& domain, ByteString const& group, ByteString const& key)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: read_string_value(domain={}, group={}, key={})", domain, group, key);
    return OptionalNone{};
}

Messages::ConfigServer::ReadI32ValueResponse ConfigServerConnectionProxy::read_i32_value(ByteString const& domain, ByteString const& group, ByteString const& key)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: read_i32_value(domain={}, group={}, key={})", domain, group, key);
    return OptionalNone{};
}

Messages::ConfigServer::ReadU32ValueResponse ConfigServerConnectionProxy::read_u32_value(ByteString const& domain, ByteString const& group, ByteString const& key)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: read_u32_value(domain={}, group={}, key={})", domain, group, key);
    return OptionalNone{};
}

Messages::ConfigServer::ReadBoolValueResponse ConfigServerConnectionProxy::read_bool_value(ByteString const& domain, ByteString const& group, ByteString const& key)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: read_bool_value(domain={}, group={}, key={})", domain, group, key);
    return OptionalNone{};
}

void ConfigServerConnectionProxy::write_string_value(ByteString const& domain, ByteString const& group, ByteString const& key, ByteString const& value)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: write_string_value(domain={}, group={}, key={}, value={})", domain, group, key, value);
}

void ConfigServerConnectionProxy::write_i32_value(ByteString const& domain, ByteString const& group, ByteString const& key, i32 value)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: write_i32_value(domain={}, group={}, key={}, value={})", domain, group, key, value);
}

void ConfigServerConnectionProxy::write_u32_value(ByteString const& domain, ByteString const& group, ByteString const& key, u32 value)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: write_u32_value(domain={}, group={}, key={}, value={})", domain, group, key, value);
}

void ConfigServerConnectionProxy::write_bool_value(ByteString const& domain, ByteString const& group, ByteString const& key, bool value)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: write_bool_value(domain={}, group={}, key={}, value={})", domain, group, key, value);
}

void ConfigServerConnectionProxy::remove_key_entry(ByteString const& domain, ByteString const& group, ByteString const& key)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: remove_key_entry(domain={}, group={}, key={})", domain, group, key);
}

void ConfigServerConnectionProxy::remove_group_entry(ByteString const& domain, ByteString const& group)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: remove_group_entry(domain={}, group={})", domain, group);
}

void ConfigServerConnectionProxy::add_group_entry(ByteString const& domain, ByteString const& group)
{
    dbgln_if(CONFIGSERVER_IPC_DEBUG, "ConfigServer IPC: add_group_entry(domain={}, group={})", domain, group);
}

}
