# AGENT_PLAN.md — CrabsTK/Web REST API (rewritten 2026-10-23)

Everything before this line is historical. This is the single source of truth.

## Goal

Build a working REST interface with CrabsTK + ASCIICrabs. One app, working
well, that serves a basic page, then user sessions over HTTP, then HTTPS.
This is the foundation the JSX engine and SubsecondDb build on.

## What we have (read and verified)

### ASCIICrabs (abstract Chinese Room stack machine)
- `Room.hpp` — TRoom<CHS,CHT,ISZ,ISY,DT,HSH> template: inline ASCII List
  Floor (1024 bytes, TList<ISR,ISQ,DTB>), up to 5 TWall (dynamic memory via
  AddWall, RAMFactory), derives Operand (has Star()). The Floor is the
  machine's data stack: type-value tuples pushed on, read off.
- `List.hpp` — TList<TS,TS2,TS3>: the ASCII List (stack of type-value
  tuples). TListInsert/TListType/TListValue/TListClear/TListLength/TListIndex.
- `Wall.hpp` — TWall: dynamic memory block (heap above the Floor).
- `Door.hpp` — TDoor: wraps TBIn/TBOut byte rings for inter-Room / network I/O.
- `BIn.hpp/BOut.hpp` — byte ring buffer: BInRead/BOutWrite/BInIsReadable/
  BInAttach. The physical pipe at the Door.
- `Op.hpp/Operand.h` — the Op vtable: Star(index, Crabs*) is the reducer.
  Concrete apps override Star to dispatch operations against the Floor.
- `Crabs.hpp` — the interpreter: drives Star() over Operands.
- Types: CHA (char8), CHR (char), ISN/ISC/ISW (ints), IUD (uint64),
  FPD (double64), BOL (bool), TMD (timestamp).
- Uniprinter (TSPrinter) — number-to-string formatting (no sprintf).
- File/TextFile — Linux file stream system (fopen/fread/fwrite).

### CrabsTK/Who (sessions + auth)
- `TSession.hpp` — TSession: cache-line-aligned session (uid, handle, state,
  role, color, room, sent, last_ts, name[64]). FNV-1a handle hash.
- `TSessionTable.hpp` — ASessionTable: fixed hash table of sessions.
- `TMessage.hpp` + AMessageRing — message ring for per-session outbound.
- `TRoom.hpp` — Who's "Room" = a channel (member list + replay ring).
  NOTE: this is NOT the ASCIICrabs TRoom. Two different things, same name.
  The ASCIICrabs TRoom is the Chinese Room state machine (Floor/Walls/Star).
  The Who TRoom is a chat channel. The Web API uses the ASCIICrabs TRoom.
- `TRoster.hpp` — ARoomTable + session table + room table.
- `Password.hpp` — PBKDF2-SHA256 KDF, getrandom salt, constant-time compare.
  (TPasswordSha256/TPasswordHex2 one-shot drivers: added, 25/25 pass.)
- `TUIDAssigner.hpp` — unique id assignment, Save/Load to file.

### CrabsTK/Web (current state: broken prototypes)
- `HttpRequest.hpp` — does NOT compile (duplicate returns, undefined types,
  C-lib calls). Must be rewritten.
- `Routing.hpp` — does NOT compile (wrong member names, dangling code).
  Must be rewritten.
- No JSON, no response, no server, no parser, no app facade. All missing.

### Crow (design reference only — we do NOT link it)
Read in full from `~/3P/Crow/`. Key patterns we port (adapted to no-stdlib):
- `app.h` — App<Middleware...> facade: .port().run(), CROW_ROUTE macro,
  get_middleware<T>(), route_dynamic, catchall.
- `http_request.h` — request: method, raw_url, url, url_params, headers, body.
- `http_response.h` — response: code, body, headers, write()/end()/redirect().
- `routing.h` — Router: rule table, {param} pattern matching, method-aware
  dispatch, 405 vs 404, typed params (int/uint/double/string/path).
- `json.h` — value/wvalue: object/array/string/number/bool/null, dump(),
  load() (parse), operator[] access, type system (type::Object/List/String/
  Number/True/False/Null, num_type::Integer/Unsigned/Floating_point).
- `mustache.h` — template_t: compile(), render(ctx), ActionType (Tag,
  UnescapeTag, OpenBlock, CloseBlock, ElseBlock, Partial, Ignore),
  load/load_text, set_base, set_loader.
- `http_parser_merged.h` — C state-machine HTTP parser (http_parser_execute,
  callbacks: on_message_begin/method/url/header_field/header_value/
  headers_complete/body/message_complete).
- `ci_map.h` — case-insensitive header map.
- `query_string.h` — url_params: get(key), get_list(key), operator string.
- `common.h` — HTTPMethod enum, method_name(), routing_params.
- `returnable.h` — abstract return type (content_type + dump()).
- `middlewares/cookie_parser.h` — CookieParser: parse Cookie header,
  set_cookie with attributes (path, max_age, httponly, secure, same_site).
- `middlewares/session.h` — SessionMiddleware: token-based sessions,
  multi_value (variant: bool/int64/double/string), get/set/remove/apply,
  InMemoryStore/FileStore.
- `middlewares/cors.h` — CORSHandler: global/prefix rules, origin/methods/
  headers/expose/max_age/allow_credentials.
- `utility.h` — base64encode/decode, trim, split, string_equals,
  lexical_cast, random_alphanum, sanitize_filename, join_path.
- `logging.h` — logger: LogLevel, ILogHandler, CerrLogHandler, CROW_LOG_*.
- `compression.h` — zlib compress_string/decompress_string (optional).
- `socket_acceptors.h` — TCPAcceptor (asio::ip::tcp), UnixSocketAcceptor.
  WE REPLACE asio with POSIX sockets (socket/bind/listen/accept/poll).
- `http_server.h` — the accept loop (asio-based). WE REPLACE with a
  single-threaded poll(2) event loop over POSIX sockets.

## What we need to build (the function inventory)

This is the complete list of functions/features to implement in CrabsTK/Web,
mapped from Crow. Organized by file. All no-stdlib, Chimera+, CHA buffers,
TSPrinter for numbers, single event thread.

### 1. `Web/HttpTypes.hpp` (NEW) — shared types and enums
- `HttpMethod` enum: GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, TRACE.
- `MethodStr(method)` → const CHA* (method name string).
- `MethodFromStr(const CHA*, ISZ)` → HttpMethod (case-insensitive).
- `StatusCode` table: 100-599, `StatusReason(code)` → const CHA*.
- `HttpVersion` enum: V0_9, V1_0, V1_1.

### 2. `Web/HttpRequest.hpp` (REWRITE)
- `struct THttpRequest`:
  - `method_` (HttpMethod), `raw_url_` (CHA[2048], len), `url_` (CHA[2048], len)
  - `version_` (HttpVersion)
  - `headers_` — fixed table: 32 entries, each {key CHA[64], value CHA[512],
    key_len, value_len}. Case-insensitive lookup.
  - `body_` — pointer + length (set by parser after Content-Length read).
  - `url_params_` — TUrlParams (query string parse, see below).
  - Methods: `Method()`, `Url()`, `RawUrl()`, `Version()`,
    `GetHeader(const CHA*)` → const CHA* (case-insensitive),
    `HasHeader(const CHA*)` → BOL,
    `HeaderCount(const CHA*)` → IUC,
    `GetHeaderValue(const CHA*)` → const CHA* (first value),
    `Body()`, `BodyLength()`,
    `UrlParam(const CHA*)` → const CHA* (first value or null),
    `UrlParams()` → TUrlParams&,
    `Cookie(const CHA*)` → const CHA* (from Cookie header),
    `ContentLength()` → IUD (from Content-Length header, 0 if absent).
- `struct TUrlParams` (query string):
  - Parse `key=value&key2=value2` into a fixed table (16 entries, each
    {key CHA[128], value CHA[256], key_len, value_len}).
  - `Get(const CHA*)` → const CHA* (first value or null).
  - `GetList(const CHA*)` → (const CHA* values[], count) (all values for key).
  - `Count()` → IUC.
  - URL-decode %XX and + → space.

### 3. `Web/HttpResponse.hpp` (NEW)
- `struct THttpResponse`:
  - `code_` (ISC, default 200)
  - `body_` (CHA buffer, 64KB max, `body_len_`)
  - `headers_` — fixed table: 16 entries, {key CHA[64], value CHA[512]}.
  - `content_type_` (CHA[128], default "text/html")
  - Methods:
    - `Code()` / `CodeSet(ISC)`
    - `Body()` → const CHA*, `BodyLength()` → IUD
    - `Write(const CHA*, IUD)` — append to body
    - `End()` — mark complete (sets Content-Length, done flag)
    - `Done()` → BOL
    - `SetHeader(const CHA*, const CHA*)`
    - `GetHeader(const CHA*)` → const CHA*
    - `ContentType(const CHA*)` / `ContentType()` → const CHA*
    - `SetCookie(const CHA* name, const CHA* value, ISZ max_age, const CHA* path, BOL httponly, BOL secure)`
    - `Redirect(const CHA* location, ISC code=302)`
    - `Json(const CHA* json_body)` — set content_type application/json, write body
    - `JsonStatus(ISC code, const CHA* json_body)` — error JSON
    - `Dump()` → (const CHA* full_response, IUD length) — serialize
      "HTTP/1.1 200 OK\r\n" + headers + "\r\n" + body.

### 4. `Web/THttpParser.hpp` (NEW)
- `struct THttpParser`:
  - State machine (simplified from Crow's http_parser_merged.h):
    - States: Start, Method, SpacesBeforeUrl, Path, HttpStart, HttpMajor,
      HttpDot, HttpMinor, LineAlmostDone, HeaderFieldStart, HeaderField,
      HeaderValue, HeadersAlmostDone, HeadersDone, Body, MessageDone.
  - Fields: `state_`, `method_`, `raw_url_` (buffer+cursor), `http_major_`,
    `http_minor_`, `content_length_` (IUD, -1 if absent), `body_cursor_`,
    `body_expected_` (IUD), `flags_` (keep-alive, chunked, close).
  - Callbacks (function pointers, not lambdas — no stdlib):
    - `on_url_(THttpRequest*, const CHA*, IUD)`
    - `on_header_field_(THttpRequest*, const CHA*, IUD)`
    - `on_header_value_(THttpRequest*, const CHA*, IUD)`
    - `on_headers_complete_(THttpRequest*)` → IUA (0=ok, 1=no body)
    - `on_body_(THttpRequest*, const CHA*, IUD)`
    - `on_message_complete_(THttpRequest*)`
  - `THttpParserInit()` — reset state.
  - `THttpParserExecute(THttpParser*, THttpRequest*, const CHA* data, IUD len)` → IUD (bytes consumed).
  - HTTP/1.1 keep-alive; Content-Length body; reject >1MB body (413).
  - No chunked encoding in v1 (reject with 411).

### 5. `Web/Router.hpp` (REWRITE)
- `struct TRule`:
  - `pattern_` (CHA[512]) — e.g. "/v1/users/{id}"
  - `method_` (HttpMethod)
  - `handler_` — function pointer: `void (*)(const THttpRequest*, THttpResponse*)`
  - `param_names_` (CHA[16][32]) — extracted {param} names
  - `param_count_` (IUC)
  - `name_` (CHA[64]) — route name (for debugging)
- `struct TRouter`:
  - `rules_` — fixed array of 64 TRule
  - `rule_count_` (IUC)
  - Methods:
    - `AddRoute(HttpMethod, const CHA* pattern, Handler fn, const CHA* name)`
    - `Dispatch(const THttpRequest*, THttpResponse*)` → IUD (0=matched, -1=404, -2=405)
      - Walk rules; match pattern against request URL (segment by segment,
        {param} segments extract into params).
      - Method match: if path matches but method differs → 405 with Allow header.
      - If no path match → 404.
      - On match: set params, call handler.
    - `CatchAll(Handler fn)` — set a catch-all handler for 404/405.
- Pattern matching: segment-by-segment, {param} captures any non-slash segment
  into the typed params. Static segments must match exactly (case-sensitive).

### 6. `Web/TJson.hpp` (NEW) — minimal JSON
- `enum TJsonType { JT_Null, JT_Bool, JT_Int, JT_Uint, JT_Double, JT_String, JT_Array, JT_Object };`
- `struct TJsonValue`:
  - `type_` (TJsonType)
  - Union: `bool_` (BOL), `int_` (ISN), `uint_` (IUD), `double_` (FPD),
    `str_` (CHA[1024] + str_len_), `array_` (TJsonValue* items, ISZ count, ISZ cap),
    `object_` (TJsonEntry* entries, ISZ count, ISZ cap) where TJsonEntry = {key CHA[256], key_len, TJsonValue value}.
  - Constructor from: const CHA* (string), const CHA[64] (char array),
    ISC/ISN (int), IUD (uint), FPD (double), BOL (bool), TJsonType::Null.
  - `operator[](const CHA* key)` → TJsonValue& (object member, create if absent)
  - `operator[](ISZ index)` → TJsonValue& (array element)
  - `Type()` → TJsonType
  - `AsBool()`, `AsInt()`, `AsUint()`, `AsDouble()`, `AsString()` → const CHA*
  - `Size()` → ISZ (array length or object member count)
  - `Key()` → const CHA* (object member key)
  - `Contains(const CHA* key)` → BOL (object)
  - `Dump(CHA* out, ISZ out_cap, ISZ* out_len)` → IUD (0=ok, -1=overflow)
    - Serialize: null, true/false, int, uint, double (via TSPrinter),
      string (with escaping: \" \\ \n \t \r \b \f, \u00XX for control),
      array [v,v,...], object {"k":v,...}. Compact (no pretty).
  - `static Load(const CHA* in, ISZ in_len, TJsonValue& out)` → IUD (0=ok, -1=error)
    - Recursive-descent parser: value → object/array/string/number/true/false/null.
    - Number: integer (ISN or IUD), unsigned (IUD), floating (FPD).
    - String: with escape sequences.
    - Nested objects/arrays (depth limit 32).
- `TJsonDump(TJsonValue*, CHA*, ISZ, ISZ*)` — free function wrapper.
- `TJsonLoad(const CHA*, ISZ, TJsonValue*)` — free function wrapper.

### 7. `Web/WebApp.hpp` (NEW) — the facade (Crow's App<...> shape)
- `struct TWebApp`:
  - `router_` (TRouter)
  - `port_` (IUD, default 8080)
  - `bindaddr_` (CHA[64], default "0.0.0.0")
  - `server_name_` (CHA[64], default "CrabsTK")
  - `catchall_` (Handler, null = none)
  - `log_level_` (IUC, default 1=Info)
  - Methods:
    - `Route(HttpMethod, const CHA* pattern)` → TRule& (fluent: .handler(fn))
    - `Get(const CHA* pattern, Handler)` / `Post(...)` / `Put(...)` / `Delete(...)` — sugar
    - `CatchAll(Handler)`
    - `Port(IUD)` → TWebApp& (fluent)
    - `BindAddr(const CHA*)` → TWebApp&
    - `ServerName(const CHA*)` → TWebApp&
    - `LogLevel(IUC)` → TWebApp&
    - `Run()` → IUD (0=ok, -1=socket error) — blocking: socket/bind/listen/poll loop
    - `Stop()` — set stop flag (called from signal handler)
    - `GetRouter()` → TRouter&
- `WEB_ROUTE(app, METHOD, pattern)` macro — expands to app.Route(METHOD, pattern).handler(...)
  (the CROW_ROUTE equivalent).
- The Run() loop:
  1. socket(AF_INET, SOCK_STREAM, 0)
  2. setsockopt(SO_REUSEADDR)
  3. bind(port)
  4. listen(64)
  5. Loop: poll(accept_fd) → accept() → read request (THttpParser) →
     router.Dispatch() → write response (THttpResponse::Dump) → close/keep-alive.
  6. Single thread, no threads, no boost. POSIX sockets only.
  7. SIGINT handler sets stop flag; loop exits cleanly.

### 8. `Web/TTemplate.hpp` (NEW) — mustache subset (the JSX engine seam)
- `struct TTemplate`:
  - `body_` (CHA[16384], body_len_) — template source
  - `actions_` — fixed array of 128 TAction (see below)
  - `action_count_` (IUC)
  - `fragments_` — fixed array of 128 TFragment {start, end}
  - Methods:
    - `Compile(const CHA* body, ISZ body_len)` → IUD (0=ok, -1=error)
      - Parse {{ }} tags into actions + fragments (like Crow's mustache.h).
      - Tag types: Tag ({{var}}), UnescapeTag ({{{var}}} or {{&var}}),
        OpenBlock ({{#var}}), CloseBlock ({{/var}}), ElseBlock ({{^var}}),
        Comment ({{!...}}), Partial ({{>name}} — skip in v1).
      - Validate matching {{#}}/{{/}} pairs.
    - `Render(TJsonValue* ctx, CHA* out, ISZ out_cap, ISZ* out_len)` → IUD
      - Walk actions, splice in context values from TJsonValue.
      - Tag: look up key in ctx (dot notation: "a.b.c" walks nested objects).
      - OpenBlock: if value is array, iterate (push each item as sub-context);
        if truthy, push as sub-context; if falsy, skip to CloseBlock.
      - ElseBlock: if value is falsy/empty, render; else skip.
      - CloseBlock: pop context.
      - UnescapeTag: raw (no HTML escaping).
      - Tag: HTML-escaped (& < > " ' / ` = → entities).
    - `static Load(const CHA* filename, TTemplate& out)` → IUD — read from file.
    - `static LoadText(const CHA* body, ISZ len, TTemplate& out)` → IUD.
  - `rendered_` — output buffer (CHA[16384]).
- HTML escaping: &, <, >, ", ', /, `, = → &amp; &lt; &gt; &quot; &#39; &#x2F; &#x60; &#x3D;

### 9. `Web/TWebServer.hpp` (NEW) — POSIX socket transport
- `struct TWebServer`:
  - `listen_fd_` (ISC)
  - `port_` (IUD)
  - `stop_` (volatile BOL)
  - `app_` (TWebApp*)
  - Methods:
    - `Init(TWebApp*, IUD port, const CHA* bindaddr)` → IUD (socket/bind/listen)
    - `Run()` → IUD — poll loop: accept, read, parse, dispatch, write, close.
      Per-connection: 16KB read buffer, read until "\r\n\r\n" (headers done),
      then read Content-Length body bytes.
      Non-blocking read with poll (1000ms timeout) for graceful shutdown.
    - `Stop()` — set stop_ = true.
    - `Close()` — close listen_fd_.
  - No threads. Single event loop. SIGINT → Stop().

### 10. `Web/HttpSession.hpp` (NEW) — cookie-based session bridge
- `struct THttpSession`:
  - `cookie_name_` (CHA[64], default "subsecond_session")
  - `secret_` (IUD, from TUIDAssigner or fixed at startup)
  - Methods:
    - `TokenFromUid(IUD uid)` → CHA[64] (format: "<uid>.<fnv1a(uid|secret)>")
    - `UidFromToken(const CHA* token)` → IUD (parse + verify hash, 0 if invalid)
    - `GetSession(const THttpRequest*)` → IUD (read cookie, verify, return uid or 0)
    - `SetSession(THttpResponse*, IUD uid, ISZ max_age)` — Set-Cookie header
    - `ClearSession(THttpResponse*)` — Set-Cookie with max_age=0
- Token format: `<IUD uid>.<IUD hash>` where hash = FNV-1a32(uid | secret).
  On verify: parse uid, recompute hash, compare. Constant-time compare.

### 11. `Web/TMime.hpp` (NEW) — MIME type table
- `MimeFromExt(const CHA* ext)` → const CHA* (e.g. "html"→"text/html",
  "js"→"application/javascript", "json"→"application/json", etc.)
- `MimeFromFilename(const CHA* filename)` → const CHA* (extract .ext, lookup).
- ~60 common types (from Crow's mime_types.h, trimmed to what we need).

### 12. `Web/_Config.h` (NEW) + seam wiring
- `CRABSTK_WEB_CORE` seam number (63, after CRABSTK_RELEASE 62).
- Add to `CrabsTK/_Seams.h`.

## Build order (get one app working)

### Step 1: Basic page (no sessions)
1. `HttpTypes.hpp` — methods, status codes.
2. `THttpParser.hpp` — parse request line + headers + body.
3. `HttpRequest.hpp` — request struct (method, url, headers, body, url_params).
4. `HttpResponse.hpp` — response struct (code, headers, body, Dump()).
5. `TWebServer.hpp` — POSIX socket, poll loop, accept/read/write.
6. `WebApp.hpp` — Route/Run facade.
7. `TMime.hpp` — content types.
8. **Test app**: serve a static HTML page at GET /, serve /health (JSON
   {"status":"ok"}), 404 for everything else. Build, run, curl.
   **Acceptance**: `curl http://localhost:8080/` returns the HTML page;
   `curl http://localhost:8080/health` returns JSON.

### Step 2: JSON + routing params
1. `TJson.hpp` — value, dump, load (nested object/array/string/number/bool/null).
2. `Router.hpp` — {param} matching, typed params, 405/404.
3. **Test app**: GET /add/{a}/{b} (int params, JSON response),
   POST /echo (JSON body in, JSON echo out).
   **Acceptance**: `curl localhost:8080/add/2/3` → {"result":5};
   `curl -d '{"x":1}' localhost:8080/echo` → {"x":1}.

### Step 3: Sessions (HTTP)
1. `HttpSession.hpp` — token cookie, uid↔token, FNV-1a verify.
2. Wire Who: POST /users/login (handle+password → TSession, cookie),
   GET /users/me (session → user JSON), DELETE /users/me (logout).
3. **Test app**: create user, login (cookie jar), /users/me, logout.
   **Acceptance**: curl -c jar -b jar walkthrough works end-to-end.

### Step 4: HTTPS
1. OpenSSL: SSL_CTX_new, SSL_CTX_use_certificate_file, SSL_CTX_use_PrivateKey_file.
2. TWebServer: optional SSL (if ssl_file set, wrap accepted fd in SSL*).
3. **Test app**: same as Step 3 but over https.
   **Acceptance**: `curl -k https://localhost:8443/health` returns JSON.

### Step 5: Templates (JSX engine foundation)
1. `TTemplate.hpp` — mustache compile + render over TJsonValue context.
2. **Test app**: GET /page renders a mustache template with user context.
   **Acceptance**: `curl localhost:8080/page` returns rendered HTML.

## Conventions (enforced)

- Chimera+ naming. No C++ stdlib types in product code.
- CHA fixed buffers with explicit len. No strlen in hot path.
- TSPrinter for all number-to-string. No sprintf in the framework path.
- Single event thread. No threads, no boost, no asio.
- POSIX sockets only: socket/bind/listen/accept/poll/read/write/close.
- File I/O: fopen/fread/fwrite (Linux file stream system).
- Build gate: `g++ -std=gnu++23 -Wall -Wextra` clean on all new files.
- 2-space indent, K&R braces, CamelCase functions, lower_snake members.
- Include order: system → Crabs library → PostgreSQL → project.
- `#pragma once` + `#ifndef CRABSTK_WEB_*` guard + `#if SEAM >= CRABSTK_WEB_CORE`.

## Files to create/rewrite

- `CrabsTK/Web/HttpTypes.hpp` (NEW)
- `CrabsTK/Web/HttpRequest.hpp` (REWRITE — replace broken prototype)
- `CrabsTK/Web/HttpResponse.hpp` (NEW)
- `CrabsTK/Web/THttpParser.hpp` (NEW)
- `CrabsTK/Web/Router.hpp` (REWRITE — replace broken Routing.hpp)
- `CrabsTK/Web/TJson.hpp` (NEW)
- `CrabsTK/Web/WebApp.hpp` (NEW)
- `CrabsTK/Web/TWebServer.hpp` (NEW)
- `CrabsTK/Web/HttpSession.hpp` (NEW)
- `CrabsTK/Web/TMime.hpp` (NEW)
- `CrabsTK/Web/TTemplate.hpp` (NEW)
- `CrabsTK/Web/_Config.h` (NEW)
- `CrabsTK/_Seams.h` (add CRABSTK_WEB_CORE 63)
- `CrabsTK/_Seams/Web/01.Parser.hxx` (NEW — parser state machine tests)
- `CrabsTK/_Seams/Web/02.Json.hxx` (NEW — JSON round-trip tests)
- `CrabsTK/_Seams/Web/03.Router.hxx` (NEW — router param matching tests)
- `CrabsTK/_Seams/Web/04.Template.hxx` (NEW — template render tests)

## Acceptance gates

- [ ] Step 1: `curl localhost:8080/` returns HTML; `/health` returns JSON.
- [ ] Step 2: `/add/2/3` → {"result":5}; POST /echo round-trips JSON.
- [ ] Step 3: curl cookie-jar walkthrough: login → /users/me → logout.
- [ ] Step 4: `curl -k https://localhost:8443/health` returns JSON.
- [ ] Step 5: `curl localhost:8080/page` returns rendered HTML from template.
- [ ] All seam tests (parser/json/router/template) pass.
- [ ] `g++ -std=gnu++23 -Wall -Wextra` clean on all Web/ files.
