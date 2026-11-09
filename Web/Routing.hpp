// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKWebRoutingHPP
#define _CrabsTKWebRoutingHPP
#include "HttpRequest.hpp"
namespace CTK {

// Routing for ASCIICrabs Web Framework
// Supports parameterized routes and pattern matching
// Single-translation-unit (STU) compatible

/// Route handler callback type
using RouteHandler = BOL (*)(const HttpRequest& req, HttpResponse& resp);

/// Route pattern with optional parameters
struct RoutePattern {
  CHA pattern[256];
  ISZ pattern_len;
  CHA method[16];
  RouteHandler handler;
  CHA params[8][64];
  ISZ param_count;

  RoutePattern(const CHA* pat = _NilP, ISZ pat_len = 0,
               const CHA* meth = _NilP, ISZ meth_len = 0,
               RouteHandler h = _NilP)
    : pattern_len(0), handler(h), param_count(0) {
    if (pat && pat_len > 0) {
      ISZ n = pat_len < 256 ? pat_len : 256;
      ::memcpy(pattern, pat, n);
      pattern[n] = 0;
      pattern_len = n;
    }
    if (meth && meth_len > 0) {
      ISZ n = meth_len < 16 ? meth_len : 16;
      ::memcpy(method, meth, n);
      method[n] = 0;
    } else {
      method[0] = 0;
    }
  }

  /// Match a request path against this pattern
  BOL match(const CHA* path, ISZ path_len, ISZ& out_param_count) const {
    out_param_count = 0;
    ISZ pi = 0, qi = 0;
    while (pi < pattern_len && qi < path_len) {
      if (pattern[pi] == '{') {
        ISZ end_brace = (ISZ)-1;
        for (ISZ j = pi; j < pattern_len; ++j) {
          if (pattern[j] == '}') { end_brace = j; break; }
        }
        if (end_brace == (ISZ)-1) return false;
        ISZ pn_len = end_brace - pi - 1;
        if (pn_len >= 64) pn_len = 63;
        if (out_param_count < 8) {
          ::memcpy(params[out_param_count], pattern + pi + 1, pn_len);
          params[out_param_count][pn_len] = 0;
          out_param_count++;
        }
        ISZ next_slash = (ISZ)-1;
        for (ISZ j = qi; j < path_len; ++j) {
          if (path[j] == '/') { next_slash = j; break; }
        }
        qi = (next_slash == (ISZ)-1) ? path_len : next_slash + 1;
        pi = end_brace + 1;
      } else if (pattern[pi] == path[qi]) {
        pi++; qi++;
      } else {
        return false;
      }
    }
    return (pi == pattern_len) && (qi == path_len);
  }
};

/// Router manages registered routes
class Router {
public:
  Router() : route_count(0) {}

  void addRoute(const CHA* pattern, const CHA* method, RouteHandler handler) {
    if (route_count < 32) {
      routes_[route_count] = RoutePattern(pattern,
        (ISZ)::strlen(pattern), method,
        method ? (ISZ)::strlen(method) : 0, handler);
      route_count++;
    }
  }

  BOL dispatch(const HttpRequest& req, HttpResponse& resp) const {
    for (ISZ i = 0; i < route_count; ++i) {
      ISZ pcount = 0;
      if (routes_[i].match(req.target, req.target_len, pcount)) {
        return routes_[i].handler(req, resp);
      }
    }
    return false;
  }

private:
  RoutePattern routes_[32];
  ISZ route_count;
};

}  // namespace Web
}  // namespace CTK
#endif  //< _CrabsTKWebRoutingHPP
