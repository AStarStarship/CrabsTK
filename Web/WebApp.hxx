// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_CROW_HXX
#define CRABS_CROW_HXX
#include <_Config.h>
#if defined(_CrabsTKWebCore) && _Seam >= _CrabsTKWebCore
#include "Jsx.hxx"
#include "JsxTransform.hxx"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
namespace CTK {
namespace Web {

// CrabsTK Web Framework — the C++ JSX web framework.
// A minimal, stdlib-free HTTP server + JSX rendering pipeline.
// Follows the Crow/Flask pattern:
//   TCrowApp app;
//   app.Route("/", [](...) { ... });
//   app.Run(18080);
// The server uses POSIX sockets (socket/bind/listen/accept/recv/send).
// Handlers receive an HTTP request and return a JSX string or plain text.
// JSX strings are transformed to C++ (via JsxToCpp) at compile time, or
// rendered directly via the TJsx engine at runtime.
//
// Stdlib-free, single-threaded, no allocation (caller-owned buffers).

// HTTP request (minimal).
struct THttpRequest {
  CHA method[16];     //< GET, POST, etc.
  CHA target[512];    //< Path + query.
  ISN method_len;
  ISN target_len;
  CHA headers[4096];  //< Raw headers.
  ISN headers_len;
  CHA body[1024];     //< Request body.
  ISN body_len;

  // Parse the raw HTTP request buffer.
  ISN Parse(const CHA* raw, ISN raw_len) {
    method_len = 0; target_len = 0; headers_len = 0; body_len = 0;
    if (!raw || raw_len < 4) return -1;
    // Method: first token.
    ISN i = 0;
    while (i < raw_len && raw[i] != ' ') {
      if (method_len < 15) method[method_len++] = raw[i];
      i++;
    }
    method[method_len] = 0;
    // Target: second token.
    while (i < raw_len && raw[i] == ' ') i++;
    while (i < raw_len && raw[i] != ' ') {
      if (target_len < 511) target[target_len++] = raw[i];
      i++;
    }
    target[target_len] = 0;
    // Skip HTTP version.
    while (i < raw_len && raw[i] != '\r') i++;
    // Headers: until blank line.
    while (i < raw_len) {
      if (raw[i] == '\r' && i + 1 < raw_len && raw[i+1] == '\n' &&
          i + 2 < raw_len && raw[i+2] == '\r' && i + 3 < raw_len && raw[i+3] == '\n') {
        i += 4;
        break;
      }
      if (headers_len < 4095) headers[headers_len++] = raw[i];
      i++;
    }
    headers[headers_len] = 0;
    // Body: remainder.
    while (i < raw_len && body_len < 1023) {
      body[body_len++] = raw[i++];
    }
    body[body_len] = 0;
    return 0;
  }
};

// HTTP response (minimal).
struct THttpResponse {
  ISN status;         //< Status code (200, 404, etc.)
  CHA content_type[64];
  ISN content_type_len;
  CHA location[256];  //< Redirect target (emitted as a Location header when set).
  ISN location_len;
  CHA body[8192];     //< Response body.
  ISN body_len;

  THttpResponse() : status(200), content_type_len(0), location_len(0),
                    body_len(0) {
    content_type[0] = 0;
    location[0] = 0;
    body[0] = 0;
  }

  // Set a redirect target (Location header). Pair with status 302/301/303.
  void SetRedirect(const CHA* url) {
    ISN i = 0;
    while (url[i] && location_len < 255) location[location_len++] = url[i++];
    location[location_len] = 0;
  }

  void SetText(const CHA* text) {
    ISN i = 0;
    while (text[i] && body_len < 8191) body[body_len++] = text[i++];
    body[body_len] = 0;
    SetContentType("text/plain");
  }

  void SetHtml(const CHA* html) {
    ISN i = 0;
    while (html[i] && body_len < 8191) body[body_len++] = html[i++];
    body[body_len] = 0;
    SetContentType("text/html");
  }

  void SetContentType(const CHA* type) {
    ISN i = 0;
    while (type[i] && content_type_len < 63) content_type[content_type_len++] = type[i++];
    content_type[content_type_len] = 0;
  }

  // Serialize to a raw HTTP response buffer.
  ISN Serialize(CHA* out, ISN out_cap) const {
    ISN n = 0;
    auto emit = [&](const CHA* s) { while (*s && n < out_cap - 1) out[n++] = *s++; };
    auto emitc = [&](CHA c) { if (n < out_cap - 1) out[n++] = c; };
    emit("HTTP/1.1 ");
    if (status == 200) emit("200 OK");
    else if (status == 301) emit("301 Moved Permanently");
    else if (status == 302) emit("302 Found");
    else if (status == 303) emit("303 See Other");
    else if (status == 400) emit("400 Bad Request");
    else if (status == 404) emit("404 Not Found");
    else if (status == 405) emit("405 Method Not Allowed");
    else if (status == 500) emit("500 Internal Server Error");
    else if (status == 501) emit("501 Not Implemented");
    else if (status == 503) emit("503 Service Unavailable");
    else if (status == 507) emit("507 Insufficient Storage");
    else emit("500 Error");
    emitc('\r'); emitc('\n');
    // Location header (redirect target), if set.
    if (location_len > 0) {
      emit("Location: ");
      emit(location);
      emitc('\r'); emitc('\n');
    }
    emit("Content-Type: ");
    emit(content_type);
    emitc('\r'); emitc('\n');
    emit("Content-Length: ");
    // Write body length as digits.
    {
      char buf[16]; ISN m = 0; ISN v = body_len;
      if (v == 0) buf[m++] = '0';
      else { char tb[16]; ISN tn = 0; while (v > 0) { tb[tn++] = (char)('0' + (v % 10)); v /= 10; } for (ISN k = tn; k > 0; k--) buf[m++] = tb[k-1]; }
      emit(buf);
    }
    emitc('\r'); emitc('\n');
    emit("Connection: close");
    emitc('\r'); emitc('\n');
    emitc('\r'); emitc('\n');
    for (ISN i = 0; i < body_len && n < out_cap - 1; i++) out[n++] = body[i];
    out[n] = 0;
    return n;
  }
};

// Route handler: takes a request, fills a response.
using TRouteHandler = void (*)(THttpRequest& req, THttpResponse& resp);

// A registered route.
struct TRoute {
  CHA path[256];
  ISN path_len;
  TRouteHandler handler;
};

// The web app (Crow pattern).
struct TCrowApp {
  TRoute routes[32];
  ISN route_count;
  ISN port;
  in_addr_t bind_addr;  //< Bind address (default INADDR_LOOPBACK).
  // JSX engine (shared across requests).
  TJsx<ISC, ISC, DTB, CHR, CHR, IUN>* jsx;
  ISC ctx;  //< Shared context.

  TCrowApp() : route_count(0), port(18080), bind_addr(INADDR_LOOPBACK),
               jsx(_NilP), ctx(0) {}

  // Set the bind address (e.g. INADDR_ANY for LAN). Loopback by default.
  void Bind(in_addr_t addr) { bind_addr = addr; }

  // Register a route.
  void Route(const CHA* path, TRouteHandler handler) {
    if (route_count >= 32) return;
    TRoute* r = &routes[route_count];
    r->path[0] = 0;
    r->path_len = 0;
    r->handler = handler;
    ISN i = 0;
    while (path[i] != 0 && i < 255) {
      r->path[i] = path[i];
      i++;
    }
    r->path[i] = 0;
    r->path_len = i;
    route_count++;
  }

  // Set the port.
  TCrowApp& Port(ISN p) { port = p; return *this; }

  // Set the JSX engine + context.
  TCrowApp& Jsx(TJsx<ISC, ISC, DTB, CHR, CHR, IUN>* j, ISC c) {
    jsx = j; ctx = c; return *this;
  }

  // Match a request path to a route handler.
  TRouteHandler Match(const CHA* path, ISN path_len) const {
    for (ISN i = 0; i < route_count; i++) {
      if (routes[i].path_len == path_len) {
        ISN k = 0;
        BOL match = true;
        while (k < path_len) {
          if (routes[i].path[k] != path[k]) { match = false; break; }
          k++;
        }
        if (match) return routes[i].handler;
      }
    }
    return _NilP;
  }

  // Handle a single request (parse, match, respond).
  ISN Handle(CHA* raw, ISN raw_len, CHA* out, ISN out_cap) {
    THttpRequest req;
    req.Parse(raw, raw_len);
    TRouteHandler handler = Match(req.target, req.target_len);
    THttpResponse resp;
    if (handler) {
      handler(req, resp);
    } else {
      resp.status = 404;
      resp.SetText("404 Not Found");
    }
    return resp.Serialize(out, out_cap);
  }

  // Run the HTTP server (blocking).
  ISN Run() {
    ISN sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(bind_addr);
    addr.sin_port = htons((unsigned short)port);
    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
      close(sock);
      return -1;
    }
    if (listen(sock, 8) < 0) {
      close(sock);
      return -1;
    }
    // Serve requests (single-threaded, blocking). Robust loop:
    //  * accept() retrying on transient errors (EINTR/EAGAIN) instead of
    //    breaking the whole server on one hiccup.
    //  * per-client SO_RCVTIMEO so a stalled/half-open connection (a browser
    //    that opens a socket and never sends, or a partial request) can't
    //    wedge the server in recv() forever.
    //  * recv loop until a full request or timeout (handles partial sends).
    while (true) {
      ISN client = accept(sock, _NilP, _NilP);
      if (client < 0) {
        if (errno == EINTR || errno == EAGAIN) continue;  // transient, retry
        break;  // genuine error (socket dead) — stop.
      }
      // Bound how long we wait for this client's request.
      struct timeval tv;
      tv.tv_sec = 10;
      tv.tv_usec = 0;
      setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
      CHA raw[8192];
      ISN n = 0;
      // Read until we have a full request (blank-line terminator) or time out.
      // A single recv can return a partial request; the browser may also send
      // the body in a second packet. Stop at the first "\r\n\r\n" (end of
      // headers) — the body, if any, follows in the same or next read.
      for (;;) {
        ISN r = recv(client, raw + n, 8191 - n, 0);
        if (r < 0) {
          if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            // Timeout (or transient). Break the recv loop; the `n <= 0` check
            // below closes the client and returns to accept. Do NOT `continue`
            // here — that would re-enter recv() on the same stalled fd forever
            // and wedge the server (the 2nd-stall bug).
            break;
          }
          break;  // genuine recv error
        }
        if (r == 0) break;  // client closed
        n += r;
        raw[n] = 0;
        // Done once we've seen the end of the header block.
        if (n > 4 && n < 8191) {
          BOL hdr_done = false;
          for (ISN i = 0; i + 3 < n; i++) {
            if (raw[i] == '\r' && raw[i + 1] == '\n' &&
                raw[i + 2] == '\r' && raw[i + 3] == '\n') {
              hdr_done = true;
              break;
            }
          }
          if (hdr_done) break;
        }
        if (n >= 8191) break;  // buffer full
      }
      if (n <= 0) { close(client); continue; }
      CHA out[16384];
      ISN out_len = Handle(raw, n, out, 16384);
      if (out_len > 0) send(client, out, out_len, 0);
      close(client);
    }
    close(sock);
    return 0;
  }
};

}  // namespace Web
}  // namespace CTK
#endif  //< _CrabsTKWebCore && _Seam
#endif  //< CRABS_CROW_HXX
