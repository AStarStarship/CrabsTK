// TPassword seam test: FIPS SHA-256 vectors + salted password lifecycle.
// Uses the real ASCIICrabs config chain + Random (IUDRandom for salt).
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include "_Config.h"
#include "Random.hxx"
#include "../Password.hpp"

using namespace ::_;

static int failures = 0;
#define CHECK(label, cond) do { \
  BOL ok = (cond); \
  printf("%-44s %s\n", label, ok ? "PASS" : "FAIL"); \
  if (!ok) ++failures; \
} while (0)

int main() {
  srand((unsigned)time(nullptr));  // Time-based seed for unique salts.
  // --- FIPS 180-2 vectors (TPasswordSha256 one-shot path) ---
  {
    IUA digest[32];
    IUA prefix[3] = {'a', 'b', 'c'};
    TPasswordSha256(prefix, 3, digest);
    char hex[70] = {0};
    CHA* out = hex;
    for (ISN i = 0; i < 32; ++i) TPasswordHex2(digest[i], &out);
    CHECK("sha256(\"abc\") full vector",
        strcmp(hex, "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD") == 0);
  }
  {
    IUA digest[32];
    TPasswordSha256((const IUA*)"", 0, digest);
    char hex[70] = {0};
    CHA* out = hex;
    for (ISN i = 0; i < 32; ++i) TPasswordHex2(digest[i], &out);
    CHECK("sha256(\"\") full vector",
        strcmp(hex, "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855") == 0);
  }

  // --- TPassword lifecycle (nil authenticator: no validation rules) ---
  {
    TPassword pw(NILP, "s3cret-pa55");
    CHECK("SetKey succeeds", pw.IsSalted() == true);
    CHECK("GetKey starts with PBK2$", strncmp(pw.GetKey(), "PBK2$", 5) == 0);
    CHECK("stored length is 5+32+1+64", TPasswordLen(pw.GetKey()) == 102);
    CHECK("Verify correct password", pw.Equals("s3cret-pa55") == true);
    CHECK("Reject wrong password", pw.Equals("wrong-password") == false);
    CHECK("Reject empty password", pw.Equals("") == false);
    CHECK("Reject nil password", pw.Equals(NILP) == false);

    // Two passwords of the same value get DIFFERENT salts (fresh random each).
    TPassword pw2(NILP, "s3cret-pa55");
    CHECK("same password, different salt",
        strcmp(pw.GetKey(), pw2.GetKey()) != 0);
    CHECK("both verify against their own salt",
        pw.Equals("s3cret-pa55") && pw2.Equals("s3cret-pa55"));

    // Salt accessor round-trip.
    IUA salt_a[PasswordSaltBytes], salt_b[PasswordSaltBytes];
    CHECK("GetSalt(pw)", pw.GetSalt(salt_a) == true);
    CHECK("GetSalt(pw2)", pw2.GetSalt(salt_b) == true);
    CHECK("salts differ",
        memcmp(salt_a, salt_b, PasswordSaltBytes) != 0);

    // Deterministic derivation: same salt + password => same key.
    IUA key1[PasswordKeyBytes], key2[PasswordKeyBytes];
    TPasswordDeriveKey(salt_a, PasswordSaltBytes, "s3cret-pa55", key1);
    TPasswordDeriveKey(salt_a, PasswordSaltBytes, "s3cret-pa55", key2);
    CHECK("deterministic derivation",
        memcmp(key1, key2, PasswordKeyBytes) == 0);

    // Different salt => different key (same password).
    IUA key3[PasswordKeyBytes];
    TPasswordDeriveKey(salt_b, PasswordSaltBytes, "s3cret-pa55", key3);
    CHECK("different salt changes key",
        memcmp(key1, key3, PasswordKeyBytes) != 0);

    // Different password => different key (same salt).
    IUA key4[PasswordKeyBytes];
    TPasswordDeriveKey(salt_a, PasswordSaltBytes, "other-password", key4);
    CHECK("different password changes key",
        memcmp(key1, key4, PasswordKeyBytes) == 0 ? false : true);
  }

  // --- Serialize / Parse round-trip ---
  {
    IUA salt[PasswordSaltBytes], key[PasswordKeyBytes];
    for (ISN i = 0; i < PasswordSaltBytes; ++i) salt[i] = (IUA)(i * 7);
    for (ISN i = 0; i < PasswordKeyBytes; ++i) key[i] = (IUA)(0xAB - i);
    CHA stored[PasswordStoredLengthMin] = {0};
    TPasswordSerialize(salt, key, stored);
    IUA salt2[PasswordSaltBytes], key2[PasswordKeyBytes];
    CHECK("Parse round-trip", TPasswordParse(stored, salt2, key2) == true);
    CHECK("salt round-trip", memcmp(salt, salt2, PasswordSaltBytes) == 0);
    CHECK("key round-trip", memcmp(key, key2, PasswordKeyBytes) == 0);
    CHECK("reject malformed (no PBK2$)",
        TPasswordParse("plaintext", salt2, key2) == false);
    CHECK("reject malformed (short)",
        TPasswordParse("PBK2$ABCD", salt2, key2) == false);
    CHECK("reject malformed (bad hex)",
        TPasswordParse("PBK2$ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ$AB", salt2, key2) == false);
  }

  // --- Constant-time comparison ---
  {
    IUA a[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    IUA b[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    IUA c[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17};
    CHECK("CT equals (match)", TPasswordConstantTimeEquals(a, b, 16) == true);
    CHECK("CT equals (mismatch last byte)",
        TPasswordConstantTimeEquals(a, c, 16) == false);
  }

  printf("\n%s (%d failures)\n", failures ? "FAIL" : "ALL PASS", failures);
  return failures;
}
