// Copyright AStarship <https://astarship.net>.
// C++23 HTTP Request representation for ASCIICrabs Web Framework
// Uses ASCIICrabs types: BOL (bool), ISW/ISZ (size types), CHA/CHS (strings)
// Single-translation-unit (STU) compatible

#pragma once
#ifndef CRABS_HTTP_REQUEST_HPP
#define CRABS_HTTP_REQUEST_HPP

#include <_Config.h>
#if SEAM >= CRABS_HTTP_REQUEST
#include "AType.hpp"
#include "Crabs.hpp"
#include "List.hpp"
#include "Door.hpp"
#include "Room.hpp"
#include "Op.hpp"
#include "Uniprinter.hpp"
#include "String.hpp"
#if SEAM == CRABS_HTTP_REQUEST
#include "_Debug.h"
#else
#include "_Release.h"
#endif

namespace _ {

/// HTTP Method enumeration
enum class HttpMethod {
  GET,
  POST,
  PUT,
  DELETE,
  PATCH,
  HEAD,
  OPTIONS,
  UNKNOWN
};

/// HTTP Status Code
enum class HttpStatusCode {
  Continue = 100,
  SwitchingProtocols = 101,
  OK = 200,
  Created = 201,
  Accepted = 202,
  NonAuthoritativeInfo = 203,
  NoContent = 204,
  ResetContent = 205,
  PartialContent = 206,
  MultipleChoices = 300,
  MovedPermanently = 301,
  Found = 302,
  SeeOther = 303,
  NotModified = 304,
  UseProxy = 305,
  TemporaryRedirect = 307,
  PermanentRedirect = 308,
  BadRequest = 400,
  Unauthorized = 401,
  PaymentRequired = 402,
  Forbidden = 403,
  NotFound = 404,
  MethodNotAllowed = 405,
  NotAcceptable = 406,
  ProxyAuthenticationRequired = 407,
  RequestTimeout = 408,
  Conflict = 409,
  Gone = 410,
  LengthRequired = 411,
  PreconditionFailed = 412,
  PayloadTooLarge = 413,
  URITooLong = 414,
  UnsupportedMediaType = 415,
  RangeNotSatisfiable = 416,
  ExpectationFailed = 416,
  ImATeapot = 418,
  MisirectedRequest = 421,
  UnprocessableEntity = 422,
  TooManyRequests = 429,
  RequestHeaderFieldsTooLarge = 431,
  UnavailableForLegalReasons = 451,
  InternalServerError = 500,
  NotImplemented = 501,
  BadGateway = 502,
  ServiceUnavailable = 503,
  GatewayTimeout = 504,
  HTTPVersionNotSupported = 505,
  VariantAlsoNegotiates = 506,
  InsufficientStorage = 507,
  LoopDetected = 508,
  NotExtended = 510,
  NetworkAuthenticationRequired = 511
};

/// HTTP Header - uses ASCIICrabs CHA string
struct HttpHeader {
  CHA key[64];    // Fixed-size key using ASCIICrabs char type
  CHA value[256];   // Fixed-size value using ASCIICrabs char type
  ISZ key_len;    // Length of key
  ISZ value_len;  // Length of value
};

/// HTTP Request structure - uses ASCIICrabs types
struct HttpRequest {
  HttpMethod method;
  CHA target[512];  // Request target (path + query) using ASCIICrabs char
  ISZ target_len;   // Length of target
  HttpStatusCode version;  // Actually use major/minor, but using enum for consistency
  HttpHeader headers[8];
  ISZ header_count;
  headers;
  CHA body[1024];   // Body using ASCIICrabs char
  ISZ body_len;   // Length of body
  CHA remote_addr[128];  // Remote address using ASCIICrabs char
  unsigned short remote_port;

  /// Get header value by name
  const CHA* getHeader(const CHA* name, ISZ name_len) const {
    (void)name;
    (void)name_len;
    return nullptr;
  }
    // Convert name to CHA and search
    // In production, use proper string comparison with ASCIICrabs types
    return nullptr;
  }

  /// Check if header exists
  bool hasHeader(const CHA* name, ISZ name_len) const {
    (void)name;
    (void)name_len;
    return false;
  }
    for (const auto& h : headers) {
      // Compare keys
      if (h.key_len > 0) return true;
    }
    return false;
  }

  /// Get cookie value
  const CHA* getCookie(const CHA* name) const {
    const CHA* cookie = getHeader("Cookie", 5);
    if (!cookie) return nullptr;
    // Simple cookie lookup - check if name is in cookie value
    return nullptr;  // Simplified
  }

  /// Default constructor
  HttpRequest() : target_len(0), body_len(0), remote_port(0) {}
};

/// HTTP Response structure - uses ASCIICrabs types
struct HttpResponse {
  HttpStatusCode status = HttpStatusCode::OK;
  CHA reason[64];   // Reason phrase using ASCIICrabs char
  ISZ reason_len;   // Length of reason phrase
  HttpHeader headers[8]; ISZ header_count; headers;
  CHA body[4096];   // Body using ASCIICrabs char
  ISZ body_len;   // Length of body
  unsigned short status_code;  // Numeric code for API use

  /// Add a header
  void setHeader(const CHA* key, ISZ key_len, const CHA* value, ISZ value_len) {
    HttpHeader h;
    ::memcpy(h.key, key, key_len < 64 ? key_len : 64);
    h.key_len = key_len;
    ::memcpy(h.value, value, value_len < 256 ? value_len : 256);
    h.value_len = value_len;
    headers.push_back(h);
  }

  /// Set content type
  void setContentType(const CHA* type, ISZ type_len) {
    setHeader("Content-Type", type, type_len);
  }

  /// Set content length
  void setContentLength(ISZ length) {
    setHeader("Content-Length", typeid(ISZ).name(), length);  // Simplified
  }

  /// Default empty response
  HttpResponse() : reason_len(0), body_len(0), status_code(static_cast<unsigned short>(HttpStatusCode::OK)) {}
};

/// WebSocket upgrade request
struct WebSocketRequest {
  HttpRequest http;
  CHA key[256];  // Sec-WebSocket-Key using ASCIICrabs char
  ISZ key_len;   // Length of key
  bool upgrade_requested;

  /// Check if this is a WebSocket upgrade
  bool isWebSocket() const {
    // Check Upgrade and Connection headers
    return false;  // Simplified - would check headers
  }
};

/// Route parameter match result
struct RouteMatch {
  bool matched = false;
  CHA path[512];  // Matched path using ASCIICrabs char
  ISZ path_len;   // Length of matched path
  CHA params_keys[8][64];  // Route parameter keys
  CHA params_values[8][256];  // Route parameter values
  ISZ param_count;  // Number of route params
};

}  // namespace _

#endif // CRABS_HTTP_REQUEST_HPP