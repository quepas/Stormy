#pragma once

#include "common/net_http_server.hpp"
#include "net_get_requests.hpp"
#include "net_request_factory.hpp"

namespace stormy {
  namespace net {

typedef HttpServer<DatabaseContext, StorageRequestFactory> StorageHttpServer;

}}
// ~~ stormy::net::HttpServer
