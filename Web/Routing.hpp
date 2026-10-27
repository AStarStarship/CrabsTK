// Copyright AStarship <https://astarship.net>.
// Routing for ASCIICrabs Web Framework
// Uses ASCIICrabs types: BOL (bool), ISW/ISZ (size types), CHA/CHS (strings)
// Supports parameterized routes and pattern matching
// Single-translation-unit (STU) compatible

#pragma once
#ifndef CRABS_ROUTING_HPP
#define CRABS_ROUTING_HPP

#include <_Config.h>
#if SEAM >= CRABS_ROUTING
#include "AType.hpp"
#include "Crabs.hpp"
#include "List.hpp"
#include "Door.hpp"
#include "Room.hpp"
#include "Op.hpp"
#include "Uniprinter.hpp"
#include "String.hpp"
#if SEAM == CRABS_ROUTING
#include "_Debug.h"
#else
#include "_Release.h"
#endif

// Use Array.h memcpy for memory operations
#include "Array.h"

// Route pattern with optional parameters
// Example: "/users/{id}" or "/api/v1/{version}/resource/{id}"
struct RoutePattern {
  CHA pattern[256];    // The route pattern using ASCIICrabs char
  ISZ pattern_len;     // Length of pattern
  CHA method[16];  // HTTP method (GET, POST, etc.) or empty for any
  RouteHandler handler;  // Handler function
  CHA params[8][64];  // Parameter names in order of appearance
  ISZ param_count;  // Number of route params

  RoutePattern(const CHA* pat = NILP, ISZ pat_len = 0, const CHA* meth = NILP, ISZ meth_len = 0) {
    pattern_len = 0;
    param_count = 0;
    if (pat && pat_len > 0) {
      ArrayCopy(pattern, pat, pat_len < 256 ? pat_len : 256);
      pattern_len = pattern_len < 256 ? pat_len : 256;
    }
    if (meth && meth_len > 0) {
      ArrayCopy(method, meth, meth_len < 16 ? meth_len : 16);
    }
  }

  /// Match a request path against this pattern
  /// @param path  The request path to match (as CHA array)
  /// @param params  Output map of parameter names to values
  /// @return true if the path matches the pattern
  bool match(const CHA* path, ISZ path_len, ISZ& out_param_count) const {
    out_param_count = 0;
    ArrayFill(params, sizeof(params), 0);
    
    // Convert path to our working buffer
    CHA pat_buf[256];
    ArrayCopy(pat_buf, path, path_len < 256 ? path_len : 256);
    
    ISZ pi = 0, qi = 0;
    bool success = true;
    
    while (pi < 256 && qi < 256 && pat_buf[pi] && path[qi]) {
      if (pat_buf[pi] == '{') {
        // Find closing brace
        ISZ end_brace = (ISZ)-1;
        for (ISZ j = pi; j < 256 && pat_buf[j]; ++j) {
          if (pat_buf[j] == '}') {
            end_brace = j;
            break;
          }
        }
        
        if (end_brace == (ISZ)-1) {
          success = false;
          break;
        }
        
        // Extract parameter name
        ISZ pn_len = end_brace - pi - 1;
        if (pn_len >= 64) pn_len = 63;
        ArrayCopy(params[out_param_count], pat_buf + pi + 1, pn_len);
        out_param_count++;
        
        // Replace {param} with wildcard
        for (ISZ k = pi; k <= end_brace; ++k) pat_buf[k] = 0;
        
        // Find the next '/' or end in path
        ISZ next_slash = (ISZ)-1;
        for (ISZ j = qi; j < 256 && pat_buf[j]; ++j) {
          if (pat_buf[j] == '/') {
            next_slash = j;
            break;
          }
        }
        
        ISZ pv_len = (next_slash == (ISZ)-1) ? 256 - qi : next_slash - qi;
        if (pv_len >= 256) pv_len = 255;
        ArrayCopy(params_values[out_param_count - 1], pat_buf + qi, pv_len);
        
        if (next_slash == (ISZ)-1) {
          qi = 256;
        } else {
          qi = next_slash + 1;
        }
      } else if (pat_buf[pi] == path[qi]) {
        pi++;
        qi++;
      } else {
        success = false;
        break;
      }
    }
    
    // Handle remaining pattern
    while (pi < 256) {
      if (pat_buf[pi] == '{') {
        ISZ end_brace = (ISZ)-1;
        for (ISZ j = pi; j < 256 && pat_buf[j]; ++j) {
          if (pat_buf[j] == '}') {
            end_brace = j;
            break;
          }
        }
        if (end_brace == (ISZ)-1) { success = false; break; }
        ISZ pn_len = end_brace - pi - 1;
        if (pn_len >= 64) pn_len = 63;
        ArrayCopy(params[out_param_count], pat_buf + pi + 1, pn_len);
        out_param_count++;
        break;
      } else if (pat_buf[pi] != '/') {
        success = false;
        break;
      }
      pi++;
    }
    
    // Both must be exhausted for success
    bool pat_done = (pi >= 256) || (pat_buf[pi] == 0);
    bool path_done = (qi >= 256) || (path_len - qi == 1 && path[qi] == '/') || (path_len > 0 && path[qi] == 0);
    
    if (success && pat_done && path_done) {
      ArrayFill(params_keys[out_param_count], sizeof(params_keys[0]), 0);  // Clear any remaining
      return true;
    }
    return false;
  }
};

// Router manages registered routes
class Router {
public:
  Router() : route_count(0) {}

  /// Add a route
  /// @param pattern  Route pattern (e.g., "/api/users/{id}")
  /// @param method   HTTP method string or empty for all
  /// @param handler  Route handler function
  void addRoute(const CHA* pattern, const CHA* method, RouteHandler handler) {
    if (route_count < 32) {
      ArrayCopy(routes_[route_count].pattern, pattern, ::strlen(pattern) < 256 ? ::strlen(pattern) : 256);
      routes_[route_count].pattern_len = ::strlen(pattern) < 256 ? ::strlen(pattern) : 256;
      if (method) {
        ArrayCopy(routes_[route_count].method, method, ::strlen(method) < 16 ? ::strlen(method) : 16);
      } else {
        routes_[route_count].method[0] = 0;
      }
      routes_[route_count].handler = handler;
      routes_[route_count].param_count = 0;
      route_count++;
    }
  }

  /// Match and dispatch a request
  /// @param req  Incoming HTTP request
  /// @param resp HTTP response (output)
  /// @return true if a route was matched and handled
  bool dispatch(const HttpRequest& req, HttpResponse& resp) const {
    ISZ matched_params = 0;
    
    for (ISZ i = 0; i < route_count; ++i) {
      ISZ pcount = 0;
      if (route_patterns_[i].match(req.target, req.target_len, pcount)) {
        // Call the handler
        HttpRequest copy = req;
        return routes_[i].handler(copy, resp);
      }
    }
    return false;
  }

  /// Get all matched parameters
    return params_;
  }

private:
  RoutePattern routes_[32];  // Maximum 32 routes
  ISZ route_count;
  CHA params_keys[8][64];
  CHA params_values[8][256];
  ISZ param_count;
};

// Using ASCIICrabs fixed arrays instead
