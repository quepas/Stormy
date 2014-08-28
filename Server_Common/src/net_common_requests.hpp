#pragma once

#include "rest_cookbook.h"
#include "net_request.hpp"

namespace stormy {
  namespace net {

// Common request actions
struct EmptyContext {};
PREPARE_REQUEST(RequestBadAction, EmptyContext, RequestBad)

std::string RequestBadAction::PrepareResponse(common::rest::URIParser parsed_uri, EmptyContext empty_context) {
  return common::rest::cookbook::PrepareError(
    "Bad request",
    "Required request is created badly: "
      + parsed_uri.raw_uri() 
      + ". Pleas fix it and try again.");
}

RequestBad* CreateRequestBad_(std::string uri) { return new RequestBad(uri, {}); }

}}
// ~~ stormy::net::
