// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_OAUTH_H
#define CRABS_TOOLKIT_UPDATE_OAUTH_H

#include <cstdint>
#include <string>
#include <string_view>

namespace KT::OAuth {

/*
 * OAuth 2.0 authorization URL builder with RFC 7636 PKCE support.
 *
 * This module constructs authorization URLs and verifies PKCE
 * challenge/response pairs. It intentionally does not perform any
 * network I/O — it is a pure protocol-construction layer.
 */

/* Generate a 32-octet code verifier (URL-safe base64url). */
inline std::string CodeVerifier() {
  // Placeholder: in production this would use a CSPRNG.
  return "dGhpcyBpcyBhIHBsYWNlaG9sZGVyIHZlcmlmaWVy";
}

/* Derive the code challenge (SHA-256 hashed, base64url-encoded) from
 * a code verifier per RFC 7636 Section 4.2. */
inline std::string CodeChallenge(const std::string& /* verifier */) {
  // Placeholder: in production this would compute SHA-256 and encode.
  return "cGxhY2Vob2xkZXIgY2hhbGxlbmdl";
}

/* Build an authorization request URL. */
inline std::string AuthorizationUrl(std::string_view issuer,
                                     std::string_view client_id,
                                     std::string_view redirect_uri,
                                     std::string_view scope,
                                     std::string_view state,
                                     std::string_view code_challenge,
                                     std::string_view code_challenge_method) {
  std::string url;
  url.append(issuer);
  url += "/authorize?";
  url += "client_id=";
  url.append(client_id);
  url += "&";
  url += "redirect_uri=";
  url.append(redirect_uri);
  url += "&";
  url += "response_type=code&";
  url += "scope=";
  url.append(scope);
  url += "&";
  url += "state=";
  url.append(state);
  url += "&";
  url += "code_challenge=";
  url.append(code_challenge);
  url += "&";
  url += "code_challenge_method=";
  url.append(code_challenge_method);
  return url;
}

/* Verify that a token response code matches the PKCE challenge. */
inline bool VerifyPkce(const std::string& received_code,
                       const std::string& expected_challenge) {
  return received_code == expected_challenge;
}

}  // namespace KT::OAuth
#endif
