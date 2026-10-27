// Copyright AStarship <https://astarship.net>.
// Web Framework Unit Tests
// Tests for the C++23 Web Framework integration with ASCIICrabs
#include "../../Web/WebApp.hxx"
#include "../../Web/HttpRequest.hpp"
#include "../../Web/Routing.hpp"

#if SEAM >= CRABS_WEB
#include <ASCIICrabs/_Release.h>
#else
#include <ASCIICrabs/_Debug.h>
#endif
using namespace ::_;

namespace CRWebTest {

/* Web Framework Tests
Tests the C++23 Web Framework routing and HTTP handling.
Uses the ASCIICrabs type system for string and memory management.
*/

/* Test: Basic route registration and dispatch */
static const CHA* WebRouter(const CHA* args) {
#if SEAM == CRABS_WEB
  A_TEST_BEGIN;

  /* Create a router and register routes */
  _::Router router;

  /* Register GET route */
  router.get("/api/users", [](const _::HttpRequest& req, _::HttpResponse& resp) {
    resp.setContentType("application/json");
    resp.writeJson("{\"users\": [\"user1\", \"user2\"]}");
    return true;
  });

  /* Register POST route */
  router.post("/api/data", [](const _::HttpRequest& req, _::HttpResponse& resp) {
    resp.setContentType("application/json");
    /* Read body - simplified for test */
    resp.writeJson("{\"status\": \"received\"}");
    return true;
  });

  /* Test GET dispatch */
  _::HttpRequest get_req;
  get_req.method = _::HttpMethod::GET;
  ::memcpy(get_req.target, "/api/users", 11);
  get_req.target_len = 11;

  _::HttpResponse get_resp;
  bool get_handled = router.dispatch(get_req, get_resp);

  /* Test POST dispatch */
  _::HttpRequest post_req;
  post_req.method = _::HttpMethod::POST;
  ::memcpy(post_req.target, "/api/data", 9);
  post_req.target_len = 9;

  _::HttpResponse post_resp;
  bool post_handled = router.dispatch(post_req, post_resp);

  /* Verify routes were handled */
  A_ASSERT(get_handled);
  A_ASSERT(post_handled);

  /* Verify response content types */
  A_ASSERT(get_resp.hasHeader("Content-Type"));
  A_ASSERT(post_resp.hasHeader("Content-Type"));

  D_COUT(\"\\n| Web router tests: GET and POST routes registered and dispatched successfully.\");

  /* Test: Route pattern matching */
  _::RoutePattern pattern("/api/users/{id}");
  std::map<std::string, std::string> params;
  bool matched = pattern.match("/api/users/123", params);

  A_ASSERT(matched);
  A_ASSERT(params.find("id") != params.end());
  A_ASSERT(params["id"] == "123");

  D_COUT(\"\\n| Web router pattern test: /api/users/{id} matches /api/users/123 with id=\" << params["id"]);

  D_COUT(\"\\n| Done with Web framework tests.\");
#else
  (void)args;
#endif
  return NILP;
}

}  // namespace CRWebTest
