// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWebHttpRequestHPP
#define _CrabsTKWebHttpRequestHPP
#include <_Config.h>
namespace CTK {
namespace Web {
// C++23 HTTP Request representation for ASCIICrabs Web Framework
// Uses ASCIICrabs types: BOL (BOL), ISZ (size types), CHA (strings)
// Single-translation-unit (STU) compatible

/// HTTP Method enumeration
enum class HttpMethod {
  GET, POST, PUT, DELETE_, PATCH, HEAD, OPTIONS, UNKNOWN
};

/// HTTP Status Code
enum class HttpStatusCode {
  Continue = 100, SwitchingProtocols = 101,
  OK = 200, Created = 201, Accepted = 202,
  NoContent = 204, PartialContent = 206,
  MovedPermanently = 301, Found = 302, NotModified = 304,
  BadRequest = 400, Unauthorized = 401, Forbidden = 403,
  NotFound = 404, MethodNotAllowed = 405,
  InternalServerError = 500, NotSupported = 501,
  BadGateway = 502, ServiceUnavailable = 503, GatewayTimeout = 504
};

/// HTTP Header - uses ASCIICrabs CHA string
struct HttpHeader {
  CHA key[64];
  CHA value[256];
  ISZ key_len;
  ISZ value_len;
};

/// HTTP Request structure - uses ASCIICrabs types
struct HttpRequest {
  HttpMethod method;
  CHA target[512];
  ISZ target_len;
  HttpHeader headers[8];
  ISZ header_count;
  CHA body[1024];
  ISZ body_len;
  CHA remote_addr[128];
  unsigned short remote_port;

  HttpRequest() : method(HttpMethod::GET), target_len(0),
    header_count(0), body_len(0), remote_port(0) {}
};

/// HTTP Response structure - uses ASCIICrabs types
struct HttpResponse {
  HttpStatusCode status;
  CHA reason[64];
  ISZ reason_len;
  HttpHeader headers[8];
  ISZ header_count;
  CHA body[4096];
  ISZ body_len;
  unsigned short status_code;

  HttpResponse() : status(HttpStatusCode::OK), reason_len(0),
    header_count(0), body_len(0),
    status_code(static_cast<unsigned short>(200)) {}
};

/// Route parameter match result
struct RouteMatch {
  BOL matched;
  CHA path[512];
  ISZ path_len;
  CHA params_keys[8][64];
  CHA params_values[8][256];
  ISZ param_count;

  RouteMatch() : matched(false), path_len(0), param_count(0) {}
};

}  // namespace Web
}  // namespace CTK
#endif  //< _CrabsTKWebHttpRequestHPP
